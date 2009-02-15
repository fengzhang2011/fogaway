#ifndef _INCLUDE_ZFUDTWRAPPER_H_
#define _INCLUDE_ZFUDTWRAPPER_H_

#include <winsock2.h>
#include <ws2tcpip.h>

#include "udt.h"

using namespace std;
using namespace UDT;

const int UDTERROR = -1;
const UDTSOCKET UDTINVALID_SOCK = -1;
const int CUDTE_SUCCESS = 0;
const int CUDTE_ECONNSETUP = 1000;
const int CUDTE_ENOSERVER = 1001;
const int CUDTE_ECONNREJ = 1002;
const int CUDTE_ESOCKFAIL = 1003;
const int CUDTE_ESECFAIL = 1004;
const int CUDTE_ECONNFAIL = 2000;
const int CUDTE_ECONNLOST = 2001;
const int CUDTE_ENOCONN = 2002;
const int CUDTE_ERESOURCE = 3000;
const int CUDTE_ETHREAD = 3001;
const int CUDTE_ENOBUF = 3002;
const int CUDTE_EFILE = 4000;
const int CUDTE_EINVRDOFF = 4001;
const int CUDTE_ERDPERM = 4002;
const int CUDTE_EINVWROFF = 4003;
const int CUDTE_EWRPERM = 4004;
const int CUDTE_EINVOP = 5000;
const int CUDTE_EBOUNDSOCK = 5001;
const int CUDTE_ECONNSOCK = 5002;
const int CUDTE_EINVPARAM = 5003;
const int CUDTE_EINVSOCK = 5004;
const int CUDTE_EUNBOUNDSOCK = 5005;
const int CUDTE_ENOLISTEN = 5006;
const int CUDTE_ERDVNOSERV = 5007;
const int CUDTE_ERDVUNBOUND = 5008;
const int CUDTE_ESTREAMILL = 5009;
const int CUDTE_EDGRAMILL = 5010;
const int CUDTE_EDUPLISTEN = 5011;
const int CUDTE_ELARGEMSG = 5012;
const int CUDTE_EASYNCFAIL = 6000;
const int CUDTE_EASYNCSND = 6001;
const int CUDTE_EASYNCRCV = 6002;
const int CUDTE_EUNKNOWN = -1;

typedef int (* UDTstartup_F)();
typedef int (* UDTcleanup_F)();
typedef UDTSOCKET (* UDTsocket_F)(int af, int type, int protocol);
typedef int (* UDTbind3_F)(UDTSOCKET u, const struct sockaddr* name, int namelen);
typedef int (* UDTbind2_F)(UDTSOCKET u, UDPSOCKET udpsock);
typedef int (* UDTlisten_F)(UDTSOCKET u, int backlog);
typedef UDTSOCKET (* UDTaccept_F)(UDTSOCKET u, struct sockaddr* addr, int* addrlen);
typedef int (* UDTconnect_F)(UDTSOCKET u, const struct sockaddr* name, int namelen);
typedef int (* UDTclose_F)(UDTSOCKET u);
typedef int (* UDTgetpeername_F)(UDTSOCKET u, struct sockaddr* name, int* namelen);
typedef int (* UDTgetsockname_F)(UDTSOCKET u, struct sockaddr* name, int* namelen);
typedef int (* UDTgetsockopt_F)(UDTSOCKET u, int level, SOCKOPT optname, void* optval, int* optlen);
typedef int (* UDTsetsockopt_F)(UDTSOCKET u, int level, SOCKOPT optname, const void* optval, int optlen);
typedef int (* UDTsend_F)(UDTSOCKET u, const char* buf, int len, int flags);
typedef int (* UDTrecv_F)(UDTSOCKET u, char* buf, int len, int flags);
typedef int (* UDTsendmsg_F)(UDTSOCKET u, const char* buf, int len, int ttl, bool inorder);
typedef int (* UDTrecvmsg_F)(UDTSOCKET u, char* buf, int len);
typedef int64_t (* UDTsendfile_F)(UDTSOCKET u, std::ifstream& ifs, int64_t offset, int64_t size, int block);
typedef int64_t (* UDTrecvfile_F)(UDTSOCKET u, std::ofstream& ofs, int64_t offset, int64_t size, int block);
typedef int (* UDTselect_F)(int nfds, UDSET* readfds, UDSET* writefds, UDSET* exceptfds, const struct timeval* timeout);
typedef ERRORINFO& (* UDTgetlasterror_F)();
typedef int (* UDTperfmon_F)(UDTSOCKET u, TRACEINFO* perf, bool clear);

//
typedef const char* (* CUDTExceptiongetErrorMessage_F)();
typedef const int (* CUDTExceptiongetErrorCode_F)();
typedef void (* CUDTExceptionclear_F)();
//
extern UDTstartup_F UDTstartup;
extern UDTcleanup_F UDTcleanup;
extern UDTsocket_F UDTsocket;
extern UDTbind3_F UDTbind3;
extern UDTbind2_F UDTbind2;
extern UDTlisten_F UDTlisten;
extern UDTaccept_F UDTaccept;
extern UDTconnect_F UDTconnect;
extern UDTclose_F UDTclose;
extern UDTgetpeername_F UDTgetpeername;
extern UDTgetsockname_F UDTgetsockname;
extern UDTgetsockopt_F UDTgetsockopt;
extern UDTsetsockopt_F UDTsetsockopt;
extern UDTsend_F UDTsend;
extern UDTrecv_F UDTrecv;
extern UDTsendmsg_F UDTsendmsg;
extern UDTrecvmsg_F UDTrecvmsg;
extern UDTsendfile_F UDTsendfile;
extern UDTrecvfile_F UDTrecvfile;
extern UDTselect_F UDTselect;
extern UDTgetlasterror_F UDTgetlasterror;
extern UDTperfmon_F UDTperfmon;
//
extern CUDTExceptiongetErrorMessage_F CUDTExceptiongetErrorMessage;
extern CUDTExceptiongetErrorCode_F CUDTExceptiongetErrorCode;
extern CUDTExceptionclear_F CUDTExceptionclear;
const char* getUDTLastErrorMessage();
const int getUDTLastErrorCode();
void clearUDTError();
//
void initializeUDTFunctions();
void uninitializeUDTFunctions();

#endif // #ifndef _INCLUDE_ZFUDTWRAPPER_H_
