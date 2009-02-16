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

/*
 * Constant Definition

const unsigned short SW_OPERA			(0xFF01)
const unsigned short SW_WIPS			(0xFF02)
const unsigned short SW_WAUT			(0xFF03)
const unsigned short SW_WIPSTEMPLATE	(0xFF04)
const unsigned short SW_QXDM			(0xFF05)
const unsigned short SW_NETHAWK			(0xFF06)
const unsigned short SW_CDRAGON			(0xFF07)
const unsigned short SW_RFO				(0xFF08)
const unsigned short SW_ATMSPY			(0xFF09)
const unsigned short SW_SECURECRT		(0xFF0a)
const unsigned short SW_ULTRAEDIT		(0xFF0b)
const unsigned short SW_WSFTP			(0xFF0c)
const unsigned short SW_FOXITREADER		(0xFF0d)
const unsigned short SW_WIRESHARK		(0xFF0e)
const unsigned short SW_TIGHTVNC		(0xFF0f)
const unsigned short SW_VNCVIEWER		(0xFF10)

 * Header Definition
 *
=======================
Source ID;		Source subID
Destination ID;	Destination subID
Command ID
=======================
if command:
	Command ID
	Command Parameter Len
	Command Parameter List ...
=======================
if data:
	Data Len
	Data ...

typedef struct{
	unsigned short sourceID;
	unsigned short sourceSubID;
	unsigned short destinationID;
	unsigned short destinationSubID;
	unsigned short commandID;
	unsigned short reserved;
} LMP_HEADER;

typedef struct{
	LMP_HEADER header;
	unsigned int paramLen;
	unsigned int params[0];
} LMP_COMMAND;

typedef struct{
	LMP_HEADER header;
	unsigned int dataLen;
	unsigned char data[0];
} LMP_DATA;
*/

// Heart Beat
void LMPHandleHeartBeat(int commandID, char* message);
void LMPHeartBeat(int commandID, char* message);
void LMPHeartBeatAck(int commandID, char* message);

// Log on/off
void LMPHandleLogOnOff(int commandID, char* message);
void LMPLogOn(int commandID, char* message);
void LMPLogOnAck(int commandID, char* message);
void LMPLogOnOk(int commandID, char* message);
void LMPLogOnFail(int commandID, char* message);
void LMPLogOff(int commandID, char* message);
void LMPLogOffAck(int commandID, char* message);
void LMPLogOffOk(int commandID, char* message);
void LMPLogOffFail(int commandID, char* message);

// System
void LMPHandleSystem(int commandID, char* message);
void LMPUpgrade(int commandID, char* message);
void LMPUpgradeAck(int commandID, char* message);
void LMPUpgradeSuc(int commandID, char* message);
void LMPUpgradeFail(int commandID, char* message);

// Transfer
void LMPHandleTransfer(int commandID, char* message);
void LMPDownloadSingleBegin(int commandID, char* message);
void LMPDownloadSingleBeginAck(int commandID, char* message);
void LMPDownloadSingleData(int commandID, char* message);
void LMPDownloadSingleEnd(int commandID, char* message);
void LMPDownloadSingleEndAck(int commandID, char* message);
void LMPDownloadListBegin(int commandID, char* message);
void LMPDownloadListBeginAck(int commandID, char* message);
void LMPDownloadListData(int commandID, char* message);
void LMPDownloadListEnd(int commandID, char* message);
void LMPDownloadListEndAck(int commandID, char* message);
void LMPDownloadFileBegin(int commandID, char* message);
void LMPDownloadFileBeginAck(int commandID, char* message);
void LMPDownloadFileData(int commandID, char* message);
void LMPDownloadFileEnd(int commandID, char* message);
void LMPDownloadFileEndAck(int commandID, char* message);

// Application
void LMPHandleApplication(int commandID, char* message);
void LMPSysInfoRequest(int commandID, char* message);
void LMPSysInfoRequestAck(int commandID, char* message);
void LMPSysInfoReport(int commandID, char* message);
void LMPSysInfoReportAck(int commandID, char* message);
void LMPSwInfoRequest(int commandID, char* message);
void LMPSwInfoRequestAck(int commandID, char* message);
void LMPSwInfoReport(int commandID, char* message);
void LMPSwInfoReportAck(int commandID, char* message);
void LMPSysCmdRequest(int commandID, char* message);
void LMPSysCmdRequestAck(int commandID, char* message);
void LMPSysCmdReport(int commandID, char* message);
void LMPSysCmdReportAck(int commandID, char* message);

void LMPHandleWrongSection(int commandID, char* message);
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void LMPHandleHeartBeat(int commandID, char* message)
{
	if(commandID==LMP_HEARTBEAT)
	{
		LMPHeartBeat(commandID, message);
	}
	else if(commandID==LMP_HEARTBEAT_ACK)
	{
		LMPHeartBeatAck(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}

void LMPHandleLogOnOff(int commandID, char* message)
{
	if(commandID==LMP_LOG_ON)
	{
		LMPLogOn(commandID, message);
	}
	else if(commandID==LMP_LOG_ON_ACK)
	{
		LMPLogOnAck(commandID, message);
	}
	else if(commandID==LMP_LOG_ON_OK)
	{
		LMPLogOnOk(commandID, message);
	}
	else if(commandID==LMP_LOG_ON_FAIL)
	{
		LMPLogOnFail(commandID, message);
	}
	else if(commandID==LMP_LOG_OFF)
	{
		LMPLogOff(commandID, message);
	}
	else if(commandID==LMP_LOG_OFF_ACK)
	{
		LMPLogOffAck(commandID, message);
	}
	else if(commandID==LMP_LOG_OFF_OK)
	{
		LMPLogOffOk(commandID, message);
	}
	else if(commandID==LMP_LOG_OFF_FAIL)
	{
		LMPLogOffFail(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}

void LMPHandleSystem(int commandID, char* message)
{
	if(commandID==LMP_UPGRADE)
	{
		LMPUpgrade(commandID, message);
	}
	else if(commandID==LMP_UPGRADE_ACK)
	{
		LMPUpgradeAck(commandID, message);
	}
	else if(commandID==LMP_UPGRADE_SUC)
	{
		LMPUpgradeSuc(commandID, message);
	}
	else if(commandID==LMP_UPGRADE_FAIL)
	{
		LMPUpgradeFail(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}

void LMPHandleTransfer(int commandID, char* message)
{
	if(commandID==LMP_DWL_SGL_BEGIN)
	{
		LMPDownloadSingleBegin(commandID, message);
	}
	else if(commandID==LMP_DWL_SGL_BEGIN_ACK)
	{
		LMPDownloadSingleBeginAck(commandID, message);
	}
	else if(commandID==LMP_DWL_SGL_DATA)
	{
		LMPDownloadSingleData(commandID, message);
	}
	else if(commandID==LMP_DWL_SGL_END)
	{
		LMPDownloadSingleEnd(commandID, message);
	}
	else if(commandID==LMP_DWL_SGL_END_ACK)
	{
		LMPDownloadSingleEndAck(commandID, message);
	}
	else if(commandID==LMP_DWL_LIST_BEGIN)
	{
		LMPDownloadListBegin(commandID, message);
	}
	else if(commandID==LMP_DWL_LIST_BEGIN_ACK)
	{
		LMPDownloadListBeginAck(commandID, message);
	}
	else if(commandID==LMP_DWL_LIST_DATA)
	{
		LMPDownloadListData(commandID, message);
	}
	else if(commandID==LMP_DWL_LIST_END)
	{
		LMPDownloadListEnd(commandID, message);
	}
	else if(commandID==LMP_DWL_LIST_END_ACK)
	{
		LMPDownloadListEndAck(commandID, message);
	}
	else if(commandID==LMP_DWL_FILE_BEGIN)
	{
		LMPDownloadFileBegin(commandID, message);
	}
	else if(commandID==LMP_DWL_FILE_BEGIN_ACK)
	{
		LMPDownloadFileBeginAck(commandID, message);
	}
	else if(commandID==LMP_DWL_FILE_DATA)
	{
		LMPDownloadFileData(commandID, message);
	}
	else if(commandID==LMP_DWL_FILE_END)
	{
		LMPDownloadFileEnd(commandID, message);
	}
	else if(commandID==LMP_DWL_FILE_END_ACK)
	{
		LMPDownloadFileEndAck(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}

void LMPHandleApplication(int commandID, char* message)
{
	if(commandID==LMP_SYSINFO_REQUEST)
	{
		LMPSysInfoRequest(commandID, message);
	}
	else if(commandID==LMP_SYSINFO_REQUEST_ACK)
	{
		LMPSysInfoRequestAck(commandID, message);
	}
	else if(commandID==LMP_SYSINFO_REPORT)
	{
		LMPSysInfoReport(commandID, message);
	}
	else if(commandID==LMP_SYSINFO_REPORT_ACK)
	{
		LMPSysInfoReportAck(commandID, message);
	}
	else if(commandID==LMP_SWINFO_REQUEST)
	{
		LMPSwInfoRequest(commandID, message);
	}
	else if(commandID==LMP_SWINFO_REQUEST_ACK)
	{
		LMPSwInfoRequestAck(commandID, message);
	}
	else if(commandID==LMP_SWINFO_REPORT)
	{
		LMPSwInfoReport(commandID, message);
	}
	else if(commandID==LMP_SWINFO_REPORT_ACK)
	{
		LMPSwInfoReportAck(commandID, message);
	}
	else if(commandID==LMP_SYSCMD_REQUEST)
	{
		LMPSysCmdRequest(commandID, message);
	}
	else if(commandID==LMP_SYSCMD_REQUEST_ACK)
	{
		LMPSysCmdRequestAck(commandID, message);
	}
	else if(commandID==LMP_SYSCMD_REPORT)
	{
		LMPSysCmdReport(commandID, message);
	}
	else if(commandID==LMP_SYSCMD_REPORT_ACK)
	{
		LMPSysCmdReportAck(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}

void LMPHandleWrongSection(int commandID, char* message)
{
}

void handleMessage(int commandID, char* message)
{
	unsigned short sectionID = commandID&&LMP_SECTION_MASK;
	if(sectionID==LMP_HEARTBEAT_SECTION)
	{
		LMPHandleHeartBeat(commandID, message);
	}
	else if(sectionID==LMP_LOG_ONOFF_SECTION)
	{
		LMPHandleLogOnOff(commandID, message);
	}
	else if(sectionID==LMP_SYSTEM_SECTION)
	{
		LMPHandleSystem(commandID, message);
	}
	else if(sectionID==LMP_TRANSFER_SECTION)
	{
		LMPHandleTransfer(commandID, message);
	}
	else if(sectionID==LMP_APPLICATION_SECTION)
	{
		LMPHandleApplication(commandID, message);
	}
	else
	{
		LMPHandleWrongSection(commandID, message);
	}
}
