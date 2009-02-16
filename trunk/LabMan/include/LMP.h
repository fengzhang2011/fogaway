#ifndef _INCLUDE_LMP_H_
#define _INCLUDE_LMP_H_

/*
 * Command Definition
 */

// Section mask
const unsigned short LMP_SECTION_MASK		(0xF000);

// Heart Beat
const unsigned short LMP_HEARTBEAT_SECTION	(0x0000);
const unsigned short LMP_HEARTBEAT			(0x0001);
const unsigned short LMP_HEARTBEAT_ACK		(0x0002);

// Log on/off
const unsigned short LMP_LOG_ONOFF_SECTION	(0x1000);
const unsigned short LMP_LOG_ON				(0x1001);
const unsigned short LMP_LOG_ON_ACK			(0x1002);
const unsigned short LMP_LOG_ON_OK			(0x1003);
const unsigned short LMP_LOG_ON_FAIL		(0x1004);
const unsigned short LMP_LOG_OFF			(0x1005);
const unsigned short LMP_LOG_OFF_ACK		(0x1006);
const unsigned short LMP_LOG_OFF_OK			(0x1007);
const unsigned short LMP_LOG_OFF_FAIL		(0x1008);

// System
const unsigned short LMP_SYSTEM_SECTION		(0x2000);
const unsigned short LMP_UPGRADE			(0x2001);
const unsigned short LMP_UPGRADE_ACK		(0x2002);
const unsigned short LMP_UPGRADE_SUC		(0x2003);
const unsigned short LMP_UPGRADE_FAIL		(0x2004);

// Transfer
const unsigned short LMP_TRANSFER_SECTION	(0x3000);
const unsigned short LMP_DWL_SGL_BEGIN		(0x3001);
const unsigned short LMP_DWL_SGL_BEGIN_ACK	(0x3002);
const unsigned short LMP_DWL_SGL_DATA		(0x3003);
const unsigned short LMP_DWL_SGL_END		(0x3004);
const unsigned short LMP_DWL_SGL_END_ACK	(0x3005);
const unsigned short LMP_DWL_LIST_BEGIN		(0x3006);
const unsigned short LMP_DWL_LIST_BEGIN_ACK	(0x3007);
const unsigned short LMP_DWL_LIST_DATA		(0x3008);
const unsigned short LMP_DWL_LIST_END		(0x3009);
const unsigned short LMP_DWL_LIST_END_ACK	(0x300a);
const unsigned short LMP_DWL_FILE_BEGIN		(0x300b);
const unsigned short LMP_DWL_FILE_BEGIN_ACK	(0x300c);
const unsigned short LMP_DWL_FILE_DATA		(0x300d);
const unsigned short LMP_DWL_FILE_END		(0x300e);
const unsigned short LMP_DWL_FILE_END_ACK	(0x300f);

// Application
const unsigned short LMP_APPLICATION_SECTION	(0x4000);
const unsigned short LMP_SYSINFO_REQUEST		(0x4001);
const unsigned short LMP_SYSINFO_REQUEST_ACK	(0x4002);
const unsigned short LMP_SYSINFO_REPORT		(0x4003);
const unsigned short LMP_SYSINFO_REPORT_ACK	(0x4004);
const unsigned short LMP_SWINFO_REQUEST		(0x4005);
const unsigned short LMP_SWINFO_REQUEST_ACK	(0x4006);
const unsigned short LMP_SWINFO_REPORT		(0x4007);
const unsigned short LMP_SWINFO_REPORT_ACK	(0x4008);
const unsigned short LMP_SYSCMD_REQUEST		(0x4009);
const unsigned short LMP_SYSCMD_REQUEST_ACK	(0x400a);
const unsigned short LMP_SYSCMD_REPORT		(0x400b);
const unsigned short LMP_SYSCMD_REPORT_ACK	(0x400c);

/*
 * Constant Definition
 */

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

/*
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
 */

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
#endif // #ifndef _INCLUDE_LMP_H_
