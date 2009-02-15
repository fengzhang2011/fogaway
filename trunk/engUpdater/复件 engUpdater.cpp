#ifndef _WIN32_WINNT
	#define _WIN32_WINNT (0x0501)
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>

#include "pthread.h"
#include "udt.h"
#include "ZFUDTWrapper.h"

using namespace std;
using namespace UDT;

void* recvdata(void* usocket)
{
	UDTSOCKET recver = *(UDTSOCKET*)usocket;
	delete (UDTSOCKET*)usocket;

	char* data;
	int size = 100000;
	data = new char[size];

	while (true)
	{
		int rsize = 0;
		int rs;
		while (rsize < size)
		{
			if (UDTERROR == (rs = UDTrecv(recver, data + rsize, size - rsize, 0)))
			{
				cout << "recv:" << getUDTLastErrorMessage() << endl;
				break;
			}
			rsize += rs;
		}
		if (rsize < size)
			break;
	}

	delete [] data;

	UDTclose(recver);

	return NULL;
}

int testServer()
{
	// use this function to initialize the UDT library
	UDT::startup();

	addrinfo hints;
	addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_socktype = SOCK_DGRAM;

	string service("9000");

	if (0 != getaddrinfo(NULL, service.c_str(), &hints, &res))
	{
		cout << "illegal port number or port is busy.\n" << endl;
		return 0;
	}

	UDTSOCKET serv = UDTsocket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// UDT Options
	//UDT::setsockopt(serv, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(serv, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(serv, 0, UDT_RCVBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(serv, 0, UDP_RCVBUF, new int(10000000), sizeof(int));

	if(UDTERROR == UDTbind3(serv, res->ai_addr, res->ai_addrlen))
	{
		cout << "bind: " << getUDTLastErrorMessage();
		return 0;
	}

	freeaddrinfo(res);

	cout << "server is ready at port: " << service << endl;

	if (UDTERROR == UDTlisten(serv, 10))
	{
		cout << "listen: " << getUDTLastErrorMessage() << endl;
		return 0;
	}

	sockaddr_storage clientaddr;
	int addrlen = sizeof(clientaddr);

	UDTSOCKET recver;

	while (true)
	{
		if (UDTINVALID_SOCK == (recver = UDTaccept(serv, (sockaddr*)&clientaddr, &addrlen)))
		{
			cout << "accept: " << getUDTLastErrorMessage() << endl;
			return 0;
		}

		char clienthost[NI_MAXHOST];
		char clientservice[NI_MAXSERV];
		getnameinfo((sockaddr *)&clientaddr, addrlen, clienthost, sizeof(clienthost), clientservice, sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
		cout << "new connection: " << clienthost << ":" << clientservice << endl;

		pthread_t rcvthread;
		pthread_create(&rcvthread, NULL, recvdata, new UDTSOCKET(recver));
		pthread_detach(rcvthread);
	}

	UDTclose(serv);

	// use this function to release the UDT library
	UDTcleanup();

	return 1;
}

void* monitor(void* s)
{
	UDTSOCKET u = *(UDTSOCKET*)s;

	UDT::TRACEINFO perf;

	cout << "SendRate(Mb/s)\tRTT(ms)\tCWnd\tPktSndPeriod(us)\tRecvACK\tRecvNAK" << endl;

	while (true)
	{
         Sleep(1);

		 if(UDTERROR == UDTperfmon(u, &perf, true))
		 {
			 cout << "perfmon: " << getUDTLastErrorMessage() << endl;
			 break;
		 }

		 cout << perf.mbpsSendRate << "\t\t" 
			 << perf.msRTT << "\t" 
			 << perf.pktCongestionWindow << "\t" 
			 << perf.usPktSndPeriod << "\t\t\t" 
			 << perf.pktRecvACK << "\t" 
			 << perf.pktRecvNAK << endl;
	}

	return NULL;
}


int testClient()
{
	// use this function to initialize the UDT library
	UDTstartup();

	struct addrinfo hints, *local, *peer;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_socktype = SOCK_DGRAM;

	string serverIp("127.0.0.1");
	string serverPort("9000");

	if (0 != getaddrinfo(NULL, serverPort.c_str(), &hints, &local))
	{
		cout << "incorrect network address.\n" << endl;
		return 0;
	}

	UDTSOCKET client = UDTsocket(local->ai_family, local->ai_socktype, local->ai_protocol);

	// UDT Options
	//UDT::setsockopt(client, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(client, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(client, 0, UDT_SNDBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(client, 0, UDT_SNDBUF, new int(10000000), sizeof(int));

	UDTsetsockopt(client, 0, UDT_MSS, new int(1052), sizeof(int));

	// for rendezvous connection, enable the code below
/*
	UDT::setsockopt(client, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool));
	if (UDT::ERROR == UDT::bind(client, local->ai_addr, local->ai_addrlen))
	{
		cout << "bind: " << getUDTLastErrorMessage() << endl;
		return 0;
	}
*/
	freeaddrinfo(local);

	if (0 != getaddrinfo(serverIp.c_str(), serverPort.c_str(), &hints, &peer))
	{
		cout << "incorrect server/peer address. " << serverIp.c_str() << ":" << serverPort.c_str() << endl;
		return 0;
	}

	// connect to the server, implict bind
	if (UDTERROR == UDTconnect(client, peer->ai_addr, peer->ai_addrlen))
	{
		cout << "connect: " << getUDTLastErrorMessage() << endl;
		return 0;
	}

	freeaddrinfo(peer);

	// using CC method
	//CUDPBlast* cchandle = NULL;
	//int temp;
	//UDT::getsockopt(client, 0, UDT_CC, &cchandle, &temp);
	//if (NULL != cchandle)
	//	cchandle->setRate(500);

	int size = 100000;
	char* data = new char[size];

	pthread_create(new pthread_t, NULL, monitor, &client);

	for (int i = 0; i < 1000000; i ++)
	{
		int ssize = 0;
		int ss;
		while (ssize < size)
		{
			if (UDTERROR == (ss = UDTsend(client, data + ssize, size - ssize, 0)))
			{
				cout << "send:" << getUDTLastErrorMessage() << endl;
				break;
			}
			ssize += ss;
		}

		if (ssize < size)
			break;
	}

	UDTclose(client);

	delete [] data;

	// use this function to release the UDT library
	UDTcleanup();

	return 1;
}

int startZFServer(string servicePort)
{
	UDTSOCKET server;

	// use this function to initialize the UDT library
	UDTstartup();

	addrinfo hints;
	addrinfo* res;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_socktype = SOCK_DGRAM;

	if (0 != getaddrinfo(NULL, servicePort.c_str(), &hints, &res))
	{
		cout << "illegal port number or port is busy.\n" << endl;
		return 0;
	}

	server = UDTsocket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// UDT Options
	//UDT::setsockopt(server, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(server, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(server, 0, UDT_RCVBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(server, 0, UDP_RCVBUF, new int(10000000), sizeof(int));

	if(UDTERROR == UDTbind3(server, res->ai_addr, res->ai_addrlen))
	{
		cout << "bind: " << getUDTLastErrorMessage();
		return 0;
	}

	freeaddrinfo(res);

	cout << "server is ready at port: " << servicePort << endl;

	if (UDTERROR == UDTlisten(server, 10))
	{
		cout << "listen: " << getUDTLastErrorMessage() << endl;
		return 0;
	}

	return server;
}

void stopZFServer(UDTSOCKET server)
{
	UDTclose(server);

	// use this function to release the UDT library
	UDTcleanup();
}

int startZFClient(string serverIp, string serverPort)
{
	UDTSOCKET client;

	// use this function to initialize the UDT library
	UDTstartup();

	struct addrinfo hints, *local, *peer;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_socktype = SOCK_DGRAM;

	if (0 != getaddrinfo(NULL, serverPort.c_str(), &hints, &local))
	{
		cout << "incorrect network address.\n" << endl;
		return 0;
	}

	client = UDTsocket(local->ai_family, local->ai_socktype, local->ai_protocol);

	// UDT Options
	//UDT::setsockopt(client, 0, UDT_CC, new CCCFactory<CUDPBlast>, sizeof(CCCFactory<CUDPBlast>));
	//UDT::setsockopt(client, 0, UDT_MSS, new int(9000), sizeof(int));
	//UDT::setsockopt(client, 0, UDT_SNDBUF, new int(10000000), sizeof(int));
	//UDT::setsockopt(client, 0, UDT_SNDBUF, new int(10000000), sizeof(int));

	UDTsetsockopt(client, 0, UDT_MSS, new int(1052), sizeof(int));

	// for rendezvous connection, enable the code below
/*
	UDT::setsockopt(client, 0, UDT_RENDEZVOUS, new bool(true), sizeof(bool));
	if (UDT::ERROR == UDT::bind(client, local->ai_addr, local->ai_addrlen))
	{
		cout << "bind: " << getUDTLastErrorMessage() << endl;
		return 0;
	}
*/
	freeaddrinfo(local);

	if (0 != getaddrinfo(serverIp.c_str(), serverPort.c_str(), &hints, &peer))
	{
		cout << "incorrect server/peer address. " << serverIp.c_str() << ":" << serverPort.c_str() << endl;
		return 0;
	}

	// connect to the server, implict bind
	if (UDTERROR == UDTconnect(client, peer->ai_addr, peer->ai_addrlen))
	{
		cout << "connect: " << getUDTLastErrorMessage() << endl;
		return 0;
	}

	freeaddrinfo(peer);

	return client;
}

void stopZFClient(UDTSOCKET client)
{
	UDTclose(client);

	// use this function to release the UDT library
	UDTcleanup();
}

void fengTestServer()
{
	UDTSOCKET server = startZFServer("9000");

	sockaddr_in their_addr;
	int namelen = sizeof(their_addr);
	UDTSOCKET fhandle;
	if(UDTINVALID_SOCK == (fhandle = UDTaccept(server, (sockaddr*)&their_addr, &namelen)))
	{
		cout << "accept: " << getUDTLastErrorMessage() << endl;
		return;
	}

	// aquiring file name information from client
	char file[1024];
	int len;

	if (UDTERROR == UDTrecv(fhandle, (char*)&len, sizeof(int), 0))
	{
		cout << "recv: " << getUDTLastErrorMessage() << endl;
		return;
	}

	if (UDTERROR == UDTrecv(fhandle, file, len, 0))
	{
		cout << "recv: " << getUDTLastErrorMessage() << endl;
		return;
	}
	file[len] = '\0';

	// open the file
	ifstream ifs(file, ios::in | ios::binary);

	printf("%d:%s\n", len, file);
	ifs.seekg(0, ios::end);
	streampos size = ifs.tellg();
	ifs.seekg(0, ios::beg);
//	printf("1size=%d\n", size);

	// send file size information
	if (UDTERROR == UDTsend(fhandle, (char*)&size, sizeof(int64_t), 0))
	{
		cout << "send: " << getUDTLastErrorMessage() << endl;
		return;
	}

	UDT::TRACEINFO trace;
	UDTperfmon(fhandle, &trace, true);

	int64_t offset = 0;
	// send the file
	if (UDTERROR == UDTsendfile(fhandle, ifs, offset, size, 1024))
	{
		cout << "sendfile: " << getUDTLastErrorMessage() << endl;
	ifs.seekg(0, ios::beg);
//	printf("2size=%d\n", size);
		return;
	}

	UDTperfmon(fhandle, &trace, true);
	cout << "speed = " << trace.mbpsSendRate << "Mbits/sec" << endl;

	ifs.close();

	UDTclose(fhandle);

	stopZFServer(server);
}

void fengTestClient()
{
	UDTSOCKET client = startZFClient("127.0.0.1", "9000");

	const char* filename = "fengTest.cpp";

	// send name information of the requested file
	int len = strlen(filename);

	if (UDTERROR == UDTsend(client, (char*)&len, sizeof(int), 0))
	{
		cout << "send: " << getUDTLastErrorMessage() << endl;
		return;
	}

	if (UDTERROR == UDTsend(client, filename, len, 0))
	{
		cout << "send: " << getUDTLastErrorMessage() << endl;
		return;
	}

	// get size information
	int64_t size;

	if (UDTERROR == UDTrecv(client, (char*)&size, sizeof(int64_t), 0))
	{
		cout << "send: " << getUDTLastErrorMessage() << endl;
		return;
	}

	printf("file size=%d\n", size);

	// receive the file
	ofstream ofs("recvFengTest.cpp", ios::out | ios::binary | ios::trunc);
	int64_t recvsize;
	int64_t offset = 0;

	if (UDTERROR == (recvsize = UDTrecvfile(client, ofs, offset, size, 7280000)))
	{
		cout << "recvfile: " << getUDTLastErrorMessage() << endl;
		return;
	}

	ofs.close();

	stopZFClient(client);
}

int main(int argc, char* argv[])
{
	initializeUDTFunctions();

	if(argc!=2)
	{
		printf("Usage: engUpdater.exe option\nwhere option should be 's' or 'c'\n");
		return 0;
	}

//	printf("argv0: %s\n", argv[0]);
//	printf("argv1: %s\n", argv[1]);
	if(argv[1][0]=='s')
	{
//		testServer();
		fengTestServer();
	}
	else if(argv[1][0]=='c')
	{
//		testClient();
		fengTestClient();
	}
	else
	{
		printf("Usage: engUpdater.exe option\nwhere option should be 's' or 'c'\n");
		return 0;
	}

	uninitializeUDTFunctions();

	return 1;
}
