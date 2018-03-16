// ErrLog.cpp: implementation of the CErrLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ErrLog.h"
#include <sys/timeb.h>
#ifdef _WINDOWS
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
//#include <unistd.h>
#endif
CErrLog ERRLOGOBJ;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void RVBLog(long rvbt, const char* cust_no, const char* rvbc)
{
#ifndef ORACLE
	return;
#endif
	char sSql[1024];
	int iRet;
	int iRec;
	CSqlData2 sqldata;
	sprintf(sSql," insert into RealVariateBook(seq_no, cust_no, rvbt,rvbc) "
				 " values(rvb_seq_no.nextval, '%s', %d, '%s') ", 
				 cust_no, rvbt, rvbc);
	SQLTRACE(sSql)
	 iRet = sqldb->Execute(sSql);
	if (iRet != 0)
	{
		//jianjie.sun 2012-5-10 提交和回滚均由上层控制
		//sqldb->RollBack();
		ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录实时变化流水时出错！");
		return;
	}
	iRec = sqldb->GetRows();
	if(0 == iRec)
	{
		//不成功
		//jianjie.sun 2012-5-10 提交和回滚均由上层控制
		//sqldb->RollBack();
		ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录实时变化流水时出错！");
		return;
	}
}

void OperLog(int func_no, char* func_name, char* parse, char* content, char* oper, int update_ctrl_no)
{

   //1.获得功能号，功能名称，日志内容解析，更新控制号，日志内容，操作员；
   //2.根据功能号和更新控制号查询"功能函数"表，确定是否插入或更新该功能号标识的数据并根据需要执行插入或更新操作；
   char sSql[1024];
   int iRet;
   int iRec;
   int iSeq;
   CSqlData2 sqldata;
   int db_ctrl_no;
   sprintf(sSql,"select ctrl_no from OperLogFunc where func_no=%d ", func_no);
   sqldata = sqldb->LoadResultSql2(sSql);
   iRec = sqldata.GetSqlCount();
   if (iRec < 0)
   {
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }
   else if (0 == iRec)
   {
      //3.2.如果没有查询条件为功能号和更新控制号的数据，并且没有查询条件为功能号的数据，则插入数据到"功能函数"表；
      sprintf(sSql," insert into OperLogFunc(func_no, name, parse, ctrl_no) \
                     values(%d, '%s', '%s', %d) ", \
                     func_no, func_name, parse, update_ctrl_no);
      SQLTRACE(sSql)
         iRet = sqldb->Execute(sSql);
      if (iRet != 0)
      {
         sqldb->RollBack();
          ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
          return;
      }
      iRec = sqldb->GetRows();
      if(0 == iRec)
      {
          //不成功
         sqldb->RollBack();
          ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
          return;
      }
   }
   else if (iRec > 0)
   {
      sqldata.GetAt(0, 0, db_ctrl_no);
      if (db_ctrl_no == update_ctrl_no)
      {
         //3.1.如果存在查询条件为功能号和更新控制号的数据则执行下一步
         
      }
      else
      {
         //3.3.如果没有查询条件为功能号和更新控制号的数据，并且存在查询条件为功能号的数据，则更新修改"功能函数"表中数据；
         sprintf(sSql," update OperLogFunc set ctrl_no=%d where func_no=%d ", update_ctrl_no, func_no);
         SQLTRACE(sSql)
            iRet = sqldb->Execute(sSql);
         if (iRet != 0)
         {
            sqldb->RollBack();
            ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
            return;
         }
         iRec = sqldb->GetRows();
         if(0 == iRec)
         {
            //不成功
            sqldb->RollBack();
            ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
            return;
         }
      }
   }
 
   char date[9];
   char time[9];
   //4.得到当前日期和时间；
   getfmttime(NULL, 3, date);
   getfmttime(NULL, 0, time);
   //5.更新和得到操作日志流水号；
   sprintf(sSql," update OperLogSeq set seq=seq + 1 where tx_date='%s' ", date);
   SQLTRACE(sSql)
   iRet = sqldb->Execute(sSql);
   if (iRet != 0)
   {
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }
   iRec = sqldb->GetRows();
   if(0 == iRec)
   {
      //没有数据
      sprintf(sSql," insert into OperLogSeq(tx_date, seq) \
         values('%s', 1) ", \
         date);
      SQLTRACE(sSql)
         iRet = sqldb->Execute(sSql);
      if (iRet != 0)
      {
         sqldb->RollBack();
         ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
         return;
      }
      iRec = sqldb->GetRows();
      if(0 == iRec)
      {
         //不成功
         sqldb->RollBack();
         ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
         return;
      }
   }
   sprintf(sSql," select seq from OperLogSeq where tx_date='%s' ",date);
   SQLTRACE(sSql)
      sqldata= sqldb->LoadResultSql2(sSql);
   iRec = sqldata.GetSqlCount();
   if (iRec<0)//错误
   {
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }
   if (0==iRec)//没有数据
   {
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }
   sqldata.GetAt(0, 0, iSeq);    
   //6.插入到"操作日志"表；  
   /*==============================================================*/
   /* Table: OperLog                                               */
   /*==============================================================*/
   sprintf(sSql," insert into OperLog(tx_date, seq, tx_time, oper, func_no, content) \
      values('%s', %d, '%s', '%s', %d, '%s') ", \
      date, iSeq, time, oper, func_no, content);
   SQLTRACE(sSql)
      iRet = sqldb->Execute(sSql);
   if (iRet != 0)
   {
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }
   iRec = sqldb->GetRows();
   if(0 == iRec)
   {
      //不成功
      sqldb->RollBack();
      ERRLOGOBJ.WriteErrLog(__FILE__,__LINE__, 0, 0, 0,"记录操作日志时出错！");
      return;
   }


}


CErrLog::CErrLog()
{
	//suxf2006-3-8,以前使用，目前不需要
/*	struct _timeb timebuffer;
    _ftime( &timebuffer );
	sprintf(strId,"%s.%d",getfmttime(NULL, 0, NULL),timebuffer.millitm  );
*/
	//end of suxf2006-3-8
	m_strLog = NULL;
	m_strSqlLog = NULL;
	m_strLog = new char[102400];
	m_strSqlLog = new char[102400];

	MAKEDIR("log");

	char strFileName[258];
	sprintf(strFileName,"log/ErrLog-%s.log",getfmttime(NULL, 3, NULL));
	SetErrFile(strFileName);
	sprintf(strFileName,"log/SqlLog-%s.log",getfmttime(NULL, 3, NULL));
	SetSqlFile(strFileName);
	sprintf(strFileName,"log/MsgLog-%s.log",getfmttime(NULL, 3, NULL));
	SetMsgFile(strFileName);
	sprintf(strFileName,"log/AllLog-%s.log",getfmttime(NULL, 3, NULL));
	SetAllFile(strFileName);
	OnDebug();
	OnErrLog();
	OnMsgLog();
	OnSqlLog();
	OnAllLog();

	bIsFlush = false;

}

CErrLog::~CErrLog()
{
	if (NULL != m_strLog)
	{
		delete m_strLog;
		m_strLog = NULL;
	}
	if (NULL != m_strSqlLog)
	{
		delete m_strSqlLog;
		m_strSqlLog = NULL;
	}

}

int CErrLog::WriteLog(const char* strFileName,logRecord::LOGTYPE log_type,const char* strLog)
{
	if (bIsFlush == false)
	{
		char sFileName[258];
		sprintf(sFileName,"log/ErrLog-%s.log",getfmttime(NULL, 3, NULL));
		SetErrFile(sFileName);
		sprintf(sFileName,"log/SqlLog-%s.log",getfmttime(NULL, 3, NULL));
		SetSqlFile(sFileName);
		sprintf(sFileName,"log/MsgLog-%s.log",getfmttime(NULL, 3, NULL));
		SetMsgFile(sFileName);

		FILE *fp;
		if(strlen(strFileName)==0)
			return 0;
		fp = fopen(strFileName,"a");
		if(fp==NULL)
			return 0;
		fprintf(fp," [%s] %s\n\n",getfmttime(NULL, 0, NULL), strLog);
		fclose(fp);
		return 1;
	}
	else
	{
		if(strlen(strFileName)==0)
			return 0;
		logRecord record;
		record.str_filename = std::string(strFileName);
		sprintf(m_strLog," [%s] %s\n\n",getfmttime(NULL, 0, NULL), strLog);
		record.str_log = std::string (m_strLog);
		record.log_type = log_type;
		m_logRecord.push_back (record);
		return 1;
	}

}

int CErrLog::LookUpErrInfo(long mainFuncNo, long errCode, int* pIndex)
{
	int i;
	for(i=0;i<MAX_ERR_INFO_NO;i++)
	{
		if(errInfoMap[i].mainFuncNo==mainFuncNo&&errInfoMap[i].errCode==errCode)
		{
			*pIndex=i;
			return 1;
		}
	}
	return 0;
}
int CErrLog::GetErrInfo(long mainFuncNo, long errCode, char* errMsg)
{
	int nIndex;
	if(!LookUpErrInfo(mainFuncNo, errCode, &nIndex))
		return 0;
	strcpy(errMsg,errInfoMap[nIndex].errMessage);
	return 1;
}
int CErrLog::InitErrInfoMap()
{
	char sSql[800];
	int count=0;
	gErrInfoNum = 0;
	sprintf(sSql,"select request_no, err_code, err_msg	 from   err_message ");
	CSqlData2 sqldata = sqldb->LoadResultSql2(sSql);
	gErrInfoNum = sqldata.GetSqlCount();
	if (gErrInfoNum<1)
	{
		return 0;
	}
	for(count=0;count<gErrInfoNum;count++)
	{
		memset(&errInfoMap[count], 0, sizeof(errInfoRecord));
		sqldata.GetAt(count,0,errInfoMap[count].mainFuncNo);//请求号
		sqldata.GetAt(count,1,errInfoMap[count].errCode);//出错信息编码
		sqldata.GetAt(count,2,errInfoMap[count].errMessage);//出错信息
	}
	return 1;
}

void CErrLog::OnDebug()
{
	bIsDebug=true;
}
void CErrLog::OffDebug()
{
	bIsDebug=false;
}
void CErrLog::OnErrLog()
{
	bIsErrLog=true;
}
void CErrLog::OnSqlLog()
{
	bIsSqlLog=true;
}
void CErrLog::OnMsgLog()
{
	bIsMsgLog=true;
}
void CErrLog::OnAllLog ()
{
	bIsAllLog=true;
}
void CErrLog::OffErrLog()
{
	bIsErrLog=false;
}
void CErrLog::OffSqlLog()
{
	bIsSqlLog=false;
}
void CErrLog::OffMsgLog()
{
	bIsMsgLog=false;
}
void CErrLog::OffAllLog()
{
	bIsAllLog=false;
}

void CErrLog::SetErrFile(const char* pstrFileName)
{
	strcpy(m_strErrFileName,pstrFileName);
	mytrim(m_strErrFileName);
}
void CErrLog::SetSqlFile(const char* pstrFileName)
{
	strcpy(m_strSqlFileName,pstrFileName);
	mytrim(m_strSqlFileName);
}
void CErrLog::SetMsgFile(const char* pstrFileName)
{
	strcpy(m_strMsgFileName,pstrFileName);
	mytrim(m_strMsgFileName);
}
void CErrLog::SetAllFile(const char* pstrFileName)
{
	strcpy(m_strAllFileName,pstrFileName);
	mytrim(m_strAllFileName);
}

int CErrLog::ProcessErr(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,int iSqlCode,char* strMsg)
{
	if(bIsDebug)	//1.如果打开调试开关则记录日志文件
	{
		if(GetErrInfo(iReqNo, iErrCode, strMsg))	//1.1找到错误信息
		{	
			return WriteErrLog(strCodeFileName,iCodeLineNo,iReqNo,iErrCode,iSqlCode,strMsg);
		}
		else										//1.2没有错误信息
		{
			sprintf(strMsg,"此错误信息([%d,%d])没有定义！",iReqNo,iErrCode);
			return WriteErrLog(strCodeFileName,iCodeLineNo,iReqNo,iErrCode,iSqlCode,strMsg);
		}
	}
	else			//2.如果关闭调试，不记录日志文件
	{
		if(GetErrInfo(iReqNo, iErrCode, strMsg))	//2.1找到错误信息
		{	
			return 1;
		}
		else										//2.2没有错误信息
		{
			sprintf(strMsg,"此错误信息([%d,%d])没有定义！",iReqNo,iErrCode);
			return 1;
		}
	}
}
int CErrLog::ProcessSql(const char* strCodeFileName,int iCodeLineNo,int iSqlSize,const char* strSql)
{
	if(bIsDebug)	//1.如果打开调试开关则记录日志文件
	{
		return WriteSqlLog(strCodeFileName,iCodeLineNo,iSqlSize,strSql);
	}
	return 1;		//2.如果关闭调试，不记录日志文件
}
int CErrLog::ProcessMsg(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,char* strMsg)
{
	if(bIsDebug)	//1.如果打开调试开关则记录日志文件
	{
		if(GetErrInfo(iReqNo, iErrCode, strMsg))	//1.1找到错误信息
		{	
			return WriteMsgLog(strCodeFileName,iCodeLineNo,iReqNo,iErrCode,strMsg);
		}
		else										//1.2没有错误信息
		{
			sprintf(strMsg,"此信息([%d,%d])没有定义！",iReqNo,iErrCode);
			return WriteMsgLog(strCodeFileName,iCodeLineNo,iReqNo,iErrCode,strMsg);
		}
	}
	else			//2.如果关闭调试，不记录日志文件
	{
		if(GetErrInfo(iReqNo, iErrCode, strMsg))	//2.1找到错误信息
		{	
			return 1;
		}
		else										//2.2没有错误信息
		{
			sprintf(strMsg,"此信息([%d,%d])没有定义！",iReqNo,iErrCode);
			return 1;
		}
	}
}

int CErrLog::WriteErrLog(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,int iSqlCode,const char* strMsg)
{
	if(bIsErrLog)	//1.如果打开错误信息的日志记录则记录日志
	{
		char	strLog[1024];
		sprintf(strLog," File=[%s],LineNo=[%d],ReqNo=[%d],ErrCode=[%d],SQLCODE=[%d],ErrInfo=[%s] ",strCodeFileName,iCodeLineNo,iReqNo,iErrCode,iSqlCode,strMsg);
      return WriteLog(m_strErrFileName, logRecord::ERR, strLog);
	}
	return 1;		//2.否则忽略
}
int CErrLog::WriteSqlLog(const char* strCodeFileName,int iCodeLineNo,int iSqlSize,const char* strSql)
{
	if(bIsSqlLog)	//1.如果打开Sql语句信息的日志记录则记录日志
	{
		sprintf(m_strSqlLog," File=[%s],LineNo=[%d],SqlAllocateSize=[%d],SqlRealSize=[%d],SQL=[%s] ",strCodeFileName,iCodeLineNo,iSqlSize,strlen(strSql),strSql);
      return WriteLog(m_strSqlFileName,  logRecord::SQL, m_strSqlLog);
	}
	return 1;		//2.否则忽略
}
int CErrLog::WriteMsgLog(const char* strCodeFileName,int iCodeLineNo,int iReqNo,int iErrCode,const char* strMsg)
{
	if(bIsMsgLog)	//1.如果打开一般信息的日志记录则记录日志
	{
		char	strLog[1024];
		sprintf(strLog," File=[%s],LineNo=[%d],ReqNo=[%d],ErrCode=[%d],ErrInfo=[%s] ",strCodeFileName,iCodeLineNo,iReqNo,iErrCode,strMsg);
      return WriteLog(m_strMsgFileName,  logRecord::NOR, strLog);
	}
	return 1;		//2.否则忽略
}
int CErrLog::flush (int FuncNo)
{
	if (bIsFlush == false)
	{
		return 1;
	}

	char strFileName[258];
	sprintf(strFileName,"log/ErrLog-%s.log",getfmttime(NULL, 3, NULL));
	SetErrFile(strFileName);
	sprintf(strFileName,"log/SqlLog-%s.log",getfmttime(NULL, 3, NULL));
	SetSqlFile(strFileName);
	sprintf(strFileName,"log/MsgLog-%s.log",getfmttime(NULL, 3, NULL));
	SetMsgFile(strFileName);
	sprintf(strFileName,"log/AllLog-%s.log",getfmttime(NULL, 3, NULL));
	SetAllFile(strFileName);

	FILE *fp;
	if(bIsAllLog)
	{
		fp = fopen(m_strAllFileName,"a");
		if(fp==NULL)
			return 0;
		fprintf(fp,"{{{开始BU[%d],功能号%d的日志{{{{{{{{{{{{{{{{{{{{{{{{{\n\n",iThisProcID,FuncNo);
		std::vector<logRecord>::iterator  it;
		for(it=m_logRecord.begin ();it!=m_logRecord.end ();it++ )
		{
			fprintf(fp,"type=%s, %s",(*it).str_filename.c_str(),(*it).str_log.c_str ()  );
		}
		fprintf(fp,"}}}结束BU[%d],功能号%d的日志}}}}}}}}}}}}}}}}}}}}}}}}}\n\n",iThisProcID,FuncNo);
		fclose(fp);
	}
//   m_logRecord.clear();
//   return 1;
	//char strFileName[258];
//	while(!m_logRecord.empty())
   int nType;
   for(nType = 1; nType <= 3; nType++)
	{
		std::vector<logRecord>::iterator  it;
		it=m_logRecord.begin ();
      while((it!=m_logRecord.end ()) && ((*it).log_type != nType))
         it++;
      if(it!=m_logRecord.end ())
      {
         strcpy(strFileName,(*it).str_filename.c_str () );
         fp = fopen(strFileName,"a");
         if(fp==NULL)
         return 0;
         fprintf(fp,"{{{开始BU[%d],功能号%d的日志{{{{{{{{{{{{{{{{{{{{{{{{{\n\n",iThisProcID,FuncNo);

         while (it!=m_logRecord.end ())
         {
            if (nType == (*it).log_type)//0 == ((*it).str_filename.compare (strFileName)) )
            {
               fprintf(fp,"%s",(*it).str_log.c_str ()  );
               //it=m_logRecord.erase (it);
               //it++;
            }
            else
            {
               //it++;
            }
            it++;
		   }
         fprintf(fp,"}}}结束BU[%d],功能号%d的日志}}}}}}}}}}}}}}}}}}}}}}}}}\n\n",iThisProcID,FuncNo);
         fclose(fp);
         fp = NULL;
      }
	}
   m_logRecord.clear();
	return 1;
}