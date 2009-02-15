#include "ZFUDTWrapper.h"

static HMODULE hUDTDll = NULL;

UDTstartup_F UDTstartup = NULL;
UDTcleanup_F UDTcleanup = NULL;
UDTsocket_F UDTsocket = NULL;
UDTbind3_F UDTbind3 = NULL;
UDTbind2_F UDTbind2 = NULL;
UDTlisten_F UDTlisten = NULL;
UDTaccept_F UDTaccept = NULL;
UDTconnect_F UDTconnect = NULL;
UDTclose_F UDTclose = NULL;
UDTgetpeername_F UDTgetpeername = NULL;
UDTgetsockname_F UDTgetsockname = NULL;
UDTgetsockopt_F UDTgetsockopt = NULL;
UDTsetsockopt_F UDTsetsockopt = NULL;
UDTsend_F UDTsend = NULL;
UDTrecv_F UDTrecv = NULL;
UDTsendmsg_F UDTsendmsg = NULL;
UDTrecvmsg_F UDTrecvmsg = NULL;
UDTsendfile_F UDTsendfile = NULL;
UDTrecvfile_F UDTrecvfile = NULL;
UDTselect_F UDTselect = NULL;
UDTgetlasterror_F UDTgetlasterror = NULL;
UDTperfmon_F UDTperfmon = NULL;

CUDTExceptiongetErrorMessage_F CUDTExceptiongetErrorMessage = NULL;
CUDTExceptiongetErrorCode_F CUDTExceptiongetErrorCode = NULL;
CUDTExceptionclear_F CUDTExceptionclear = NULL;

//
void initializeUDTFunctions()
{
	if(hUDTDll!=NULL) return;

	hUDTDll = ::LoadLibrary ("udt.dll");
	
	UDTstartup = (UDTstartup_F)::GetProcAddress(hUDTDll, "?startup@UDT@@YAHXZ");
	UDTcleanup = (UDTcleanup_F)::GetProcAddress(hUDTDll, "?cleanup@UDT@@YAHXZ");
	UDTsocket = (UDTsocket_F)::GetProcAddress(hUDTDll, "?socket@UDT@@YAHHHH@Z");
	UDTbind3 = (UDTbind3_F)::GetProcAddress(hUDTDll, "?bind@UDT@@YAHHPBUsockaddr@@H@Z");
	UDTbind2 = (UDTbind2_F)::GetProcAddress(hUDTDll, "?bind@UDT@@YAHHI@Z");
	UDTlisten = (UDTlisten_F)::GetProcAddress(hUDTDll, "?listen@UDT@@YAHHH@Z");
	UDTaccept = (UDTaccept_F)::GetProcAddress(hUDTDll, "?accept@UDT@@YAHHPAUsockaddr@@PAH@Z");
	UDTconnect = (UDTconnect_F)::GetProcAddress(hUDTDll, "?connect@UDT@@YAHHPBUsockaddr@@H@Z");
	UDTclose = (UDTclose_F)::GetProcAddress(hUDTDll, "?close@UDT@@YAHH@Z");
	UDTgetpeername = (UDTgetpeername_F)::GetProcAddress(hUDTDll, "?getpeername@UDT@@YAHHPAUsockaddr@@PAH@Z");
	UDTgetsockname = (UDTgetsockname_F)::GetProcAddress(hUDTDll, "?getsockname@UDT@@YAHHPAUsockaddr@@PAH@Z");
	UDTgetsockopt = (UDTgetsockopt_F)::GetProcAddress(hUDTDll, "?getsockopt@UDT@@YAHHHW4UDTOpt@@PAXPAH@Z");
	UDTsetsockopt = (UDTsetsockopt_F)::GetProcAddress(hUDTDll, "?setsockopt@UDT@@YAHHHW4UDTOpt@@PBXH@Z");
	UDTsend = (UDTsend_F)::GetProcAddress(hUDTDll, "?send@UDT@@YAHHPBDHH@Z");
	UDTrecv = (UDTrecv_F)::GetProcAddress(hUDTDll, "?recv@UDT@@YAHHPADHH@Z");
	UDTsendmsg = (UDTsendmsg_F)::GetProcAddress(hUDTDll, "?sendmsg@UDT@@YAHHPBDHH_N@Z");
	UDTrecvmsg = (UDTrecvmsg_F)::GetProcAddress(hUDTDll, "?recvmsg@UDT@@YAHHPADH@Z");
	UDTsendfile = (UDTsendfile_F)::GetProcAddress(hUDTDll, "?sendfile@UDT@@YA_JHAAV?$basic_ifstream@DU?$char_traits@D@std@@@std@@_J1H@Z");
	UDTrecvfile = (UDTrecvfile_F)::GetProcAddress(hUDTDll, "?recvfile@UDT@@YA_JHAAV?$basic_ofstream@DU?$char_traits@D@std@@@std@@_J1H@Z");
	UDTselect = (UDTselect_F)::GetProcAddress(hUDTDll, "?select@UDT@@YAHHPAV?$set@HU?$less@H@std@@V?$allocator@H@2@@std@@00PBUtimeval@@@Z");
	UDTgetlasterror = (UDTgetlasterror_F)::GetProcAddress(hUDTDll, "?getlasterror@UDT@@YAAAVCUDTException@@XZ");
	UDTperfmon = (UDTperfmon_F)::GetProcAddress(hUDTDll, "?perfmon@UDT@@YAHHPAUCPerfMon@@_N@Z");

	CUDTExceptiongetErrorMessage = (CUDTExceptiongetErrorMessage_F)::GetProcAddress(hUDTDll, "?getErrorMessage@CUDTException@@UAEPBDXZ");
	CUDTExceptiongetErrorCode = (CUDTExceptiongetErrorCode_F)::GetProcAddress(hUDTDll, "?getErrorCode@CUDTException@@UBE?BHXZ");
	CUDTExceptionclear = (CUDTExceptionclear_F)::GetProcAddress(hUDTDll, "?clear@CUDTException@@UAEXXZ");
//	int* easyncsnd = (int*)::GetProcAddress(hUDTDll, "?EASYNCSND@CUDTException@@2HB");
//	printf("easyncsnd=%d\n", *easyncsnd);
/*
	printf("UDTstartup=%x\n", UDTstartup);
	printf("UDTcleanup=%x\n", UDTcleanup);
	printf("UDTsocket=%x\n", UDTsocket);
	printf("UDTbind3=%x\n", UDTbind3);
	printf("UDTbind2=%x\n", UDTbind2);
	printf("UDTlisten=%x\n", UDTlisten);
	printf("UDTaccept=%x\n", UDTaccept);
	printf("UDTconnect=%x\n", UDTconnect);
	printf("UDTclose=%x\n", UDTclose);
	printf("UDTgetpeername=%x\n", UDTgetpeername);
	printf("UDTgetsockname=%x\n", UDTgetsockname);
	printf("UDTgetsockopt=%x\n", UDTgetsockopt);
	printf("UDTsetsockopt=%x\n", UDTsetsockopt);
	printf("UDTsend=%x\n", UDTsend);
	printf("UDTrecv=%x\n", UDTrecv);
	printf("UDTsendmsg=%x\n", UDTsendmsg);
	printf("UDTrecvmsg=%x\n", UDTrecvmsg);
	printf("UDTsendfile=%x\n", UDTsendfile);
	printf("UDTrecvfile=%x\n", UDTrecvfile);
	printf("UDTselect=%x\n", UDTselect);
	printf("UDTgetlasterror=%x\n", UDTgetlasterror);
	printf("UDTperfmon=%x\n", UDTperfmon);
//
	printf("CUDTExceptiongetErrorMessage=%x\n", CUDTExceptiongetErrorMessage);
	printf("CUDTExceptiongetErrorCode=%x\n", CUDTExceptiongetErrorCode);
	printf("CUDTExceptionclear=%x\n", CUDTExceptionclear);
*/

}

void uninitializeUDTFunctions()
{
	if(hUDTDll==NULL) return;

	FreeLibrary(hUDTDll);
}


DWORD This = 0;

const char* getUDTLastErrorMessage()
{
	ERRORINFO& err = UDTgetlasterror();
	This = (DWORD) &err;
	asm volatile(
		"mov (_This), %ecx\n"
	   );
	return CUDTExceptiongetErrorMessage();
}

const int getUDTLastErrorCode()
{
	ERRORINFO& err = UDTgetlasterror();
	This = (DWORD) &err;
	asm volatile(
		"mov (_This), %ecx\n"
	   );
	return CUDTExceptiongetErrorCode();
}

void clearUDTError()
{
	ERRORINFO& err = UDTgetlasterror();
	This = (DWORD) &err;
	asm volatile(
		"mov (_This), %ecx\n"
	   );
	CUDTExceptionclear();
}
