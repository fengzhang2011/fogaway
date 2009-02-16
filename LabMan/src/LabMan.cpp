#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h" 
// Server
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
// Client
#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"

#define SIZE_DATA 18
#define NO_ITERATIONS 1
#define SERVER_SIZE_BUF 1024
#define CLIENT_SIZE_BUF 128

class Server
{
public:
	Server(int port): server_addr_(port), peer_acceptor_(server_addr_)
	{
		data_buf_= new char[SERVER_SIZE_BUF];
	}

	int handle_connection();
	int accept_connections();
private:
	char *data_buf_;

	ACE_INET_Addr server_addr_;
	ACE_INET_Addr client_addr_;
	ACE_SOCK_Acceptor peer_acceptor_;
	ACE_SOCK_Stream new_stream_;
};

int Server::handle_connection()
{
	for(int i=0;i<1;i++)
	{
		int byte_count=0;

		if( (byte_count=new_stream_.recv_n (data_buf_, SIZE_DATA))==-1)
		{
			ACE_ERROR ((LM_ERROR, "%pn", "Error in recv"));
		}
		else
		{
			data_buf_[byte_count]=0;
			ACE_DEBUG((LM_DEBUG,"Server received: %sn",data_buf_));
		}
	}
	int n_bytes = 0;
	if( ( n_bytes=new_stream_.send_n( "Hi, World from Server", 21))==-1)
	{
		ACE_ERROR ((LM_ERROR, "%pn", "Error in send"));
	}
	else
	{
		ACE_DEBUG((LM_DEBUG,"Server send okn"));
	}
	if (new_stream_.close () == -1)
	{
		ACE_ERROR ((LM_ERROR, "%pn", "close"));
	}
	return 0;
}

int Server::accept_connections ()
{
	if (peer_acceptor_.get_local_addr (server_addr_) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,"%pn","Error in get_local_addr"),1);
//	ACE_DEBUG ((LM_DEBUG,"Starting server at port %dn", server_addr_.get_port_number ()));
	while(1)
	{
		ACE_Time_Value timeout (20);
		if (peer_acceptor_.accept (new_stream_, &client_addr_, &timeout)== -1)
		{
			ACE_ERROR ((LM_ERROR, "n%pn", "accept"));
			continue;
		}
		else
		{
//			ACE_DEBUG((LM_DEBUG, "nConnection established with remote %s:%dn", client_addr_.get_host_name(),client_addr_.get_port_number()));
			handle_connection();
		}
	}
}


class Client
{
public:
	Client(char *hostname, int port):remote_addr_(port,hostname)
	{
		data_buf_="Hello from Client";
	}
	int connect_to_server();
	int send_to_server();
	int close();
private:
	ACE_SOCK_Stream client_stream_;
	ACE_INET_Addr remote_addr_;
	ACE_SOCK_Connector connector_;
	char *data_buf_;
};

int Client::connect_to_server()
{
//	ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting connect to %s:%dn", remote_addr_.get_host_name(),remote_addr_.get_port_number()));
	if(connector_.connect (client_stream_, remote_addr_) == -1)
	{
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","connection failed"),-1);
	}
	else
	{
		ACE_DEBUG ((LM_DEBUG,"(%P|%t) connected to %sn", remote_addr_.get_host_name ()));
	}
	return 0;
}

int Client::send_to_server()
{
	for(int i=0; i<10; i++)
	{
		int n = 0;
		n = client_stream_.send_n (data_buf_, ACE_OS::strlen(data_buf_)+1);
		if ( n == -1)
		{
			ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","send_n"),0);
			break;
		}
		// recv
		char recv_buff[ 32 ] = { 0 };
		n=client_stream_.recv_n (recv_buff, 32 );
		if( n ==-1)
		{
			ACE_ERROR ((LM_ERROR, "%pn", "Error in recv"));
		}
		else
		{
//			data_buf_[byte_count]=0;
			ACE_DEBUG((LM_DEBUG,"Server received: %sn",recv_buff));
		}
	}
	return 0;
}

int Client::close() 
{ 
	if (client_stream_.close () == -1)
	{
		ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %pn","close"),-1);
	}
	else
	{
		return 0;
	}
}

static const char* SERVER_HOST = "127.0.0.1";
static u_short SERVER_PORT = 1009;

int main (int argc, char *argv[])
{
//	ACE_DEBUG((LM_DEBUG,"Hello, ACE! "));
//	ACE_OS::exit(1);

//	cout<<"adf"<<endl;

	if(*argv[1]=='s')
	{
		ACE_DEBUG((LM_DEBUG,"Server"));
		static const u_short port=SERVER_PORT;
		Server server(port);
		server.accept_connections();
	}
	else if(*argv[1]=='c')
	{
		ACE_DEBUG((LM_DEBUG,"Client"));
		Client client((char*)SERVER_HOST,SERVER_PORT);
		client.connect_to_server();
		client.send_to_server();
		client.close();
	}
	else
	{
		ACE_DEBUG((LM_DEBUG,"Wrong Parameter"));
	}

	return 0;
}
