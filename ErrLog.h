// ErrLog.h: interface for the CErrLog class.
//
//////////////////////////////////////////////////////////////////////
/* --------------------------------------------------------------
 * 程序名称：ErrLog.h
 * 创建日期：2005-02-22
 * 程序维护：史大江
 * 程序作者：史大江
 * 版本信息：1.0.0.0
 * 程序功能：出错信息处理
 * 备注信息：
 * --------------------------------------------------------------
 * 修改日期：2005-03-02
 * 修改人员：史大江
 * 版本信息：1.0.0.1
 * 修改描述：增强容错性
 * 备注信息
 * --------------------------------------------------------------*/
#if !defined(AFX_ERRLOG_H__724052FD_6DC3_470C_9900_1C719FBCDC68__INCLUDED_)
#define AFX_ERRLOG_H__724052FD_6DC3_470C_9900_1C719FBCDC68__INCLUDED_

#define _IN_SQC_   

#include "stdafx.h"
//#include "bdefine.h"
//#include "logfile.h"
//#include "function.h"
//#include "svrlink.h"
#include "risksqlapi.h"
//#include "errcode.h"
//#include "pubstrtime.h"
//#include "encrypt.h"
//#include "Xpack.h"
#ifdef _WINDOWS
#else
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#endif
//extern CSvrLink *g_pSvrLink;  // 与业务调度中心的连接
//extern CLogFile g_LogFile;
extern CRiskSqlAPI *sqldb;
//extern CXPack *xpack;
//songfree 20040628 
//extern vector <ST_PACK> rpArrays_all;  //请求多包包体
//extern vector <ST_PACK> apArrays_all;  //应答多包包体
#include <math.h>
//#include "pubfunc.h"
//#include "DefineVBDT.h"

#include "vector"

#define MESSAGE_LEN 1024


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2007-12-27
 * 模块名称和功能：提供函数定义接口，用于记录变化流水数据，供风险控制系统使用
 * 使用说明： 
 *      RVBLog(变化流水类型,"客户号","变化流水内容");
 * ---------------------------------------------------------------
 * 相关数据结构参考
 *
 * create sequence rvb_seq_no
 * increment by 1
 * start with 1
 * maxvalue 999999
 * nocache
 * nocycle;
 * 
 * 实时变化流水（RealVariateBook）
 * [流水号],客户号,变化流水类型,变化流水内容
 * seq_no,cust_no,rvbt,rvbc
 * 
 * seq_no      N10,0
 * cust_no     va10
 * rvbt        N10,0
 * rvbc        va256
 * 
 * 
 * --------------------------------------------------------------*/
void RVBLog(long rvbt, const char* cust_no, const char* rvbc);


/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2006-03-16
 * 模块名称和功能：提供函数定义接口，用于记录操作日志
 * 使用说明： 
 *      OperLog(功能号,功能名称,"日志内容解析","日志内容","操作员号",更新控制号);
 *      说明：
 *      功能号-当前操作功能函数的功能号,int型；
 *      功能名称-当前操作功能函数的功能名称，小于256长度的字符串格式；
 *      日志内容解析-用于解析日志内容中记录的日志数据，小于256长度的字符串格式；
 *      日志内容-按照日志内容解析格式记录的日志内容，小于256长度的字符串格式；
 *      操作员号-进行当前操作的操作员号，小于30长度的字符串格式；
 *      更新控制号-用于唯一标识当前功能号操作日志记录的日志内容解析版本号，初始值为1，每次日志内容解析修改后将其加1，系统再记录操作日志时，如果发现更新控制号发生变化，则需修改"功能函数"表中对应的"日志内容解析"，整型格式；
 *      2.示例和说明-新增操作日志
 *      2.1.内部参数表修改的一个示例：
 *      OperLog (670101,"操作|数据对象|参数代码|参数名称|参数值","修改|内部参数表|1023|留存手续费|帐户|9998","000",1);
 *      2.2.出入金的一个示例：
 *      OperLog (670101,"会员代码|记账日期|流水号|业务类型|客户代码|账户类型|发生金额|摘要|已发电标志|回执标志|发电记账日期|发电流水序号","0064|20051226|99|2|0000000064|0|65000|(成都天鑫洋)买卖黄金保证金|4|0000||","000",1);
 *      3.示例和说明-修改操作日志的"日志内容解析"
 *      3.1.内部参数表修改的一个示例：
 *      OperLog (670101,"操作|表名|参数代码|参数名称|参数值","修改|内部参数表|1023|留存手续费|帐户|9998","000",2);
 *      这里将原来的"更新控制号"从1修改为了2。
 * 普通流程：
 *     1.获得功能号，功能名称，日志内容解析，更新控制号，日志内容，操作员；
 *     2.根据功能号和更新控制号查询"功能函数"表，确定是否插入或更新该功能号标识的数据并根据需要执行插入或更新操作；
 *     3.1.如果存在查询条件为功能号和更新控制号的数据则执行下一步
 *     4.得到当前日期和时间；
 *     5.更新和得到操作日志流水号；
 *     6.插入到"操作日志"表；  
 * 
 * 可选流程：
 *     3.2.如果没有查询条件为功能号和更新控制号的数据，并且没有查询条件为功能号的数据，则插入数据到"功能函数"表；
 *     3.3.如果没有查询条件为功能号和更新控制号的数据，并且存在查询条件为功能号的数据，则更新修改"功能函数"表中数据；
 * 
 * 
 * --------------------------------------------------------------*/
void OperLog(int func_no, char* func_name, char* parse, char* content, char* oper, int update_ctrl_no);



/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称和功能：出错信息处理的主要功能的宏定义接口
 *     PROCESS_ERR		   处理出错信息并返回到BU处理线程，出错信息从数据库中查寻得到
 *     PROCESS_ERR_Tex     处理出错信息并返回到BU处理线程，出错信息直接输入
 *     NORMAL_INFO         正常信息的日志记录，信息从数据库中查寻得到
 *     NORMAL_INFO_Text    正常信息的日志记录，信息直接输入
 *     SQLTRACE            SQL语句跟踪
 *     ERR_INFO            获取错误信息和记录日志并返回-1,出错信息从数据库中查寻得到
 *     ERR_INFO_Text       获取错误信息和记录日志并返回-1,出错信息直接输入
 *
 * 说明：
 * a   所属请求号（数据库主键之一）      只读，输入
 * b   所属错误代码（数据库主键之一）    只读，输入
 * c   SQLCODE                           只读，输入
 * d   iRetCode的值                      只读，输入
 * s   待跟踪的SQL语句                   只读，输入
 * e_des 目标文本                        只写，输出
 * e_src 源文本                          只读，输入
 * 
 * 常见使用的模版范例：
 * 1.数据格式检查出错：PROCESS_ERR(a,b,0,1)，PROCESS_ERR_Text(e,0,1)
 * 2.数据库操作出错：PROCESS_ERR(a,b,c,1)，PROCESS_ERR_Text(e_src,c,1)
 *               没有数据：PROCESS_ERR_Text(ERR_NO_RECORD,1,1)
 *               操作错误：PROCESS_ERR_Text(ERR_OPERATE,1,1),PROCESS_ERR(100,1,1,1)
 * 3.跟踪数据：NORMAL_INFO_Text(NULL,e_src)
 * 4.只记录错误信息日志：ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,e_src);
 *   只获取和记录错误信息日志：ERRLOGOBJ.ProcessErr(__FILE__,__LINE__, a, b, 0,e_des);
 * --------------------------------------------------------------*/
#define PROCESS_ERR(a,b,c,d)											\
		{																\
			ERRLOGOBJ.ProcessErr(__FILE__,__LINE__, a, b, c,szMsg);		\
			SetCol(handle, -1);											\
			*iRetCode = d;												\
			Error_Deal(*iRetCode,szMsg, (szMsg));						\
			return -1;													\
		}
#define PROCESS_ERR_Text(e_src,c,d)										\
		{																\
			strcpy((szMsg), (e_src));									\
			ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__,0,0, c, e_src);		\
			SetCol(handle, -1);											\
			*iRetCode = d;												\
			Error_Deal(*iRetCode,szMsg, (szMsg));						\
			return -1;													\
		}
#define NORMAL_INFO(a,b,e_des)											\
		{																\
			ERRLOGOBJ.ProcessMsg(__FILE__,__LINE__, a, b, e_des);		\
		}
#define NORMAL_INFO_Text(e_des,e_src)									\
		{																\
			if(e_des!=NULL)													\
			{															\
				strcpy((e_des), (e_src));								\
			}															\
			ERRLOGOBJ.WriteMsgLog(__FILE__,__LINE__, 0, 0, e_src);		\
		}
#define SQLTRACE(s)														\
		{																\
			ERRLOGOBJ.ProcessSql(__FILE__,__LINE__,sizeof(s),s);		\
		}
#define ERR_INFO(a,b,e_des)												\
		{																\
			ERRLOGOBJ.ProcessErr(__FILE__,__LINE__, a, b, SQLCODE,e_des); \
			return -1;													\
		}
#define ERR_INFO_Text(e_des,e_src)										\
		{																\
			if(e_des!=NULL)													\
			{															\
				strcpy((e_des), (e_src));								\
			}															\
			ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, SQLCODE,e_src); \
			return -1;													\
		}


#define ERRLOGOBJ ErrLog

#define MAX_ERR_INFO_NO 2000

typedef struct
{
	long mainFuncNo;
	long errCode;
	char errMessage[MESSAGE_LEN];
}errInfoRecord;

typedef struct 
{
   enum LOGTYPE{NONE = 0, ERR = 1, SQL = 2, NOR=3};
	std::string str_filename;
	std::string str_log;
   LOGTYPE log_type;
}logRecord;

class CErrLog  
{
public:
	CErrLog();
	virtual ~CErrLog();

private:
	bool bIsDebug;				//是否是debug版本
	bool bIsErrLog;				//是否记录错误信息日志
	bool bIsSqlLog;				//是否记录Sql语句信息日志
	bool bIsMsgLog;				//是否记录一般信息日志
	bool bIsAllLog;				//是否记录全部信息的日志
	char m_strErrFileName[256];	//错误信息日志文件
	char m_strSqlFileName[256];	//SQL语句执行日志文件
	char m_strMsgFileName[256];	//一般信息日志文件
	char m_strAllFileName[256]; //所有日至
	char* m_strLog;
	char* m_strSqlLog;
public:
	bool bIsFlush;
private:
//suxf2006-3-8,以前使用，现在不用了
//	char strId[32];
//end of suxf2006-3-8

	int gErrInfoNum;
	errInfoRecord errInfoMap[MAX_ERR_INFO_NO];
	std::vector <logRecord> m_logRecord;
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：WriteLog
 *
 * 功能：写日志文件
 *
 * 输入：
 * strFilename   日志文件名
 * strLog        日志信息
 *
 * 输出：
 * 返回值       如果成功查到返回1,否则返回0
 *
 * --------------------------------------------------------------*/
	int WriteLog(const char* strFileName,logRecord::LOGTYPE,const char* strLog);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2004-10-14
 * 模块名称：LookUpErrInfo
 *
 * 功能：根据请求号和出错信息编码获取出错信息在数组中的位置
 *
 * 输入：
 * mainFuncNo   所属请求号
 * errCode      出错信息编码
 *
 * 输出：
 * pIndex       返回出错信息在数组中的位置
 * 返回值       如果成功查到返回1,否则返回0
 *
 * --------------------------------------------------------------*/
	int LookUpErrInfo(long mainFuncNo, long errCode, int* pIndex);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2004-10-14
 * 模块名称：GetErrInfo
 *
 * 功能：根据请求号和出错信息编码获取出错信息
 *
 * 输入：
 * mainFuncNo   所属请求号
 * errCode      出错信息编码
 *
 * 输出：
 * errMsg       返回的出错信息
 * 返回值       如果成功查到返回1,否则返回0
 *
 * 数据格式说明：
 * errMsg所指向的字符串长度不大于255
 * --------------------------------------------------------------*/
	int GetErrInfo(long mainFuncNo, long errCode, char* errMsg);
public:
	/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-10-12
 * 模块名称：flush
 *
 * 功能：将日志写入文件
 *
 * 输入：
 * FundNo		功能号
 *
 * 输出：
 * 返回值       如果成功查到返回1,否则返回0
 *
 * --------------------------------------------------------------*/
	int flush(int FuncNo);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2004-10-14
 * 模块名称：InitErrInfoMap
 *
 * 功能：初始化出错信息数组
 *
 * 返回：1 正常
 *       0 错误
 * --------------------------------------------------------------*/
	int InitErrInfoMap();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OnDebug
 *
 * 功能：打开调试状态
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OnDebug();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OffDebug
 *
 * 功能：关闭调试状态
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OffDebug();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OnErrLog
 *
 * 功能：打开错误信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OnErrLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OnSqlLog
 *
 * 功能：打开SQL语句信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OnSqlLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OnMsgLog
 *
 * 功能：打开一般信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OnMsgLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-10-12
 * 模块名称：OnAllLog
 *
 * 功能：打开所有信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OnAllLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OffErrLog
 *
 * 功能：关闭错误信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OffErrLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OffSqlLog
 *
 * 功能：关闭SQL语句信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OffSqlLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：OffMsgLog
 *
 * 功能：关闭一般信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OffMsgLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-10-12
 * 模块名称：OffAllLog
 *
 * 功能：关闭所有信息的日志记录
 *
 * 返回：
 * --------------------------------------------------------------*/
	void OffAllLog();
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：SetErrFile
 *
 * 功能：设置错误信息日志文件名
 * 输入：错误信息日志文件名 pstrFileName
 *
 * 返回：
 * --------------------------------------------------------------*/
	void SetErrFile(const char* pstrFileName);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：SetSqlFile
 *
 * 功能：设置SQL语句信息日志文件名
 * 输入：SQL语句信息日志文件名 pstrFileName
 *
 * 返回：
 * --------------------------------------------------------------*/
	void SetSqlFile(const char* pstrFileName);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：SetMsgFile
 *
 * 功能：设置一般信息日志文件名
 * 输入：一般信息日志文件名 pstrFileName
 *
 * 返回：
 * --------------------------------------------------------------*/
	void SetMsgFile(const char* pstrFileName);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-10-12
 * 模块名称：SetAllFile
 *
 * 功能：设置全部信息日志文件名
 * 输入：全部信息日志文件名 pstrFileName
 *
 * 返回：
 * --------------------------------------------------------------*/
	void SetAllFile(const char* pstrFileName);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：ProcessErr
 *
 * 功能：查询和处理错误信息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iReqNo			 请求号（数据库主键之一）
 * iErrCode			 错误代码（数据库主键之一）
 * iSqlCode			 SQL语句执行返回码
 * strMsg			 返回和处理的错误信息
 *
 * 输出：
 * strMsg       返回的出错信息
 * 返回值       如果成功返回1,否则返回0
 *
 * 数据格式说明：
 * strMsg所指向的字符串长度不大于255
 * --------------------------------------------------------------*/
	int ProcessErr(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,int iSqlCode,char* strMsg);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：ProcessSql
 *
 * 功能：查询和处理SQL语句信息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iSqlSize			 SQL语句的存储单元的大小
 * strSql			 SQL语句
 *
 * 输出：
 * 返回值       如果成功返回1,否则返回0
 *
 * --------------------------------------------------------------*/
	int ProcessSql(const char* strCodeFileName,int iCodeLineNo,int iSqlSize,const char* strSql);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-02-22
 * 模块名称：ProcessMsg
 *
 * 功能：查询和处理一般消息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iReqNo			 请求号（数据库主键之一）
 * iErrCode			 错误代码（数据库主键之一）
 * strMsg			 返回和处理的信息
 *
 * 输出：
 * strMsg       返回的出错信息
 * 返回值       如果成功返回1,否则返回0
 *
 * 数据格式说明：
 * strMsg所指向的字符串长度不定
 * --------------------------------------------------------------*/
	int ProcessMsg(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,char* strMsg);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：WriteErrLog
 *
 * 功能：写错误信息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iReqNo			 请求号（数据库主键之一）
 * iErrCode			 错误代码（数据库主键之一）
 * iSqlCode			 SQL语句执行返回码
 * strMsg			 待写的错误信息日志
 *
 * 输出：
 * 返回值       如果成功返回1,否则返回0
 *
 * 数据格式说明：
 * strMsg所指向的字符串长度不大于255
 * --------------------------------------------------------------*/
	int WriteErrLog(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,int iSqlCode,const char* strMsg);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：WriteSqlLog
 *
 * 功能：写SQL语句信息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iSqlSize			 SQL语句的存储单元的大小
 * strSql			 SQL语句
 *
 * 输出：
 * 返回值       如果成功返回1,否则返回0
 *
 * --------------------------------------------------------------*/
	int WriteSqlLog(const char* strCodeFileName,int iCodeLineNo,int iSqlSize,const char* strSql);
/* ---------------------------------------------------------------
 * 作者：史大江
 * 日期：2005-03-02
 * 模块名称：WriteMsgLog
 *
 * 功能：写一般消息日志
 *
 * 输入：
 * strCodeFileName   代码所在文件名
 * iCodeLineNo       代码所在文件的行数
 * iReqNo			 请求号（数据库主键之一）
 * iErrCode			 错误代码（数据库主键之一）
 * strMsg			 返回和处理的信息
 *
 * 输出：
 * 返回值       如果成功返回1,否则返回0
 *
 * 数据格式说明：
 * strMsg所指向的字符串长度不定
 * --------------------------------------------------------------*/
	int WriteMsgLog(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,const char* strMsg);


};

#endif // !defined(AFX_ERRLOG_H__724052FD_6DC3_470C_9900_1C719FBCDC68__INCLUDED_)
