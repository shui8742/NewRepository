#include "stdafx.h" 
#include "myfunction.h"


TdeqLogS m_TdeqLogS;

/* 去除左右空格   */
char* mytrim(char *s)
{
	int i,len;
	
	len=strlen(s);
	for (i=len-1;i>=0;i--)
	{
		if ((s[i] != ' ') && (s[i] != '\t'))
			break;
		else
			s[i]=0;
	}
	for (i=0; i<len; i++)
	{
		if ((s[i] != ' ') && (s[i] != '\t'))
			break;
	}
	if (i != 0)
	{
		strncpy(s,s+i,len-i);
		s[len-i]=0;
	}
	return s;
}


int WriteLog(char *obj,char *file,int line,int code,char *info)
{
    FILE *fp;
	char errfile[280];
	char date[9];
	char time[22];
	memset(date,0,sizeof(date));
	memset(time,0,sizeof(time));
	
	CTime t1 = CTime::GetCurrentTime();
	CString cstrDate = t1.Format("%Y%m%d");
	CString cstrtime = t1.Format("%H:%M:%S");
	
	CString strCurDirectory;
	
	GetCurrentDirectory(FILEPATH_LENGTH, strCurDirectory.GetBuffer(FILEPATH_LENGTH));
	
#ifdef WIN32
	if(strlen(strCurDirectory) >0 )
	{
		
		sprintf(errfile,"%s\\%s",strCurDirectory,"log");
		_mkdir(errfile);
		
		sprintf(errfile,"%s\\%s\\%s%s.log",strCurDirectory,"log",cstrDate,obj);
		
	}
	else
	{
		sprintf(errfile,"%s%s.log",cstrDate,obj);
	}
#else
	sprintf(errfile,"%s/%s%s.log",strCurDirectory,cstrDate,obj);
#endif
	
	fp=fopen(errfile,"a");
	if(fp==NULL)
		return(-1);
	
	fprintf(fp,"%s %s line=[%d] code=[%d]__%s\n",cstrtime,file,line,code,info);
    fclose(fp);
	
    return(0);
}

int myWriteFile(char *name,char *key,char *value)
{
	FILE *fp = NULL;
	char pathName[512] = {0};
	char m_errMsg[512] = {0};
	
	CString szModulePath;
	::GetModuleFileName(NULL, szModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	szModulePath.ReleaseBuffer();
	szModulePath = name;
	int nIndex = szModulePath.ReverseFind('\\');
	if (nIndex != -1){
	szModulePath = szModulePath.Left(nIndex+1);
	}
	sprintf(pathName,"%s",szModulePath);

	if (access(pathName,0)!=0)
	{
		if(mkdir(pathName)!=0)
		{
			sprintf(m_errMsg,"不能创建文件路径[%s]", pathName);
			
			printf("%s", m_errMsg);
			return -1;
		}
	}

	fp = fopen(name, "w");
	if (fp == NULL)
	{
		printf("write file false");
		return -1;
	}
	
	if (1 ==1 )
	{		
		fprintf(fp,"%s\n","[COMMON]");
		fprintf(fp,"%s=%s\n",key,value);	
		fclose(fp);
	}
	return 0;
}


int myReadFile(char *FileName,char *Key,char *Value)
{
	TIniFile tf;
	
	char szBuf[256];
	
//	printf("Begin to read ini-file:%s...\n",FileName);
	if (!tf.Open(FileName))
	{
		sprintf(szBuf,"不能打开配置文件<%s>\n",FileName);
		printf(szBuf);
		
		strcpy(Value,"");
		
		return -1;
	}
	
	tf.ReadString("COMMON",Key,"1",szBuf,sizeof(szBuf)-1);
	strcpy(Value,szBuf);
	tf.Close();
	return 0;   
}

int myReadFile2(char *FileName,char separator,map<string,string> &pMap)
{
	FILE *fp = NULL;
	int gs;
	char fileName[512] = {0};
	char m_errMsg[512] = {0};	
	char buf[501] = {0};
	CString strTemp,strKey,strValue;
	int i = 0;
	char sign[2] = {0};
	sign[0] = separator;
	sign[1]=0;

	//打开文件
	sprintf(fileName,"%s",FileName);
	fp = fopen(fileName,"r");	
	
	if (fp == NULL)
	{
		sprintf(m_errMsg,"不能打开文件[%s]", FileName);		
		printf("%s", m_errMsg);
		return -1;
	}
	int iCount = 0;
	for(gs=0;!feof(fp);)
	{
		memset(buf,0,sizeof(buf));
		if(fgets(buf,500,fp)==NULL)
		{
		//	printf("%s文件为空 \n",FileName);
			break;
		}
		mytrim(buf);
		if(strlen(buf) <= 2)
		{
		//	printf("文件有空格+空行+回车，被忽略");
			continue;
		}
		int i=strlen(buf);                  
		if(buf[i-1]=='\n') //因为最后一行没有回车符
		{
			buf[i-1]=0;
        }
		i=strlen(buf);                  
		if(buf[i-1]=='\r') //因为最后一行没有回车符
		{
			buf[i-1]=0;
        }

		strTemp = buf;
		int pos = strTemp.FindOneOf("=");
		if (pos == -1 )
		{
			continue;
		}
		strKey = strTemp.Left(pos);
		strValue = strTemp.Right(strTemp.GetLength() - pos - 1);
		pMap.insert(pair<string,string>(strKey.GetBuffer(strKey.GetLength()),strValue.GetBuffer(strValue.GetLength())));
		iCount ++;
	}	
	fclose(fp);
	if (0 == iCount)
	{
		printf("%s文件中没有符合条件的记录 \n",FileName);
	}
	return 0; 
}


int myWriteFile2(char *FileName,char separator,map<string,string> &pMap)
{
	FILE *fp = NULL;
	char szPathName[512] = {0};
	char m_errMsg[512] = {0};
	
	CString szModulePath;
	::GetModuleFileName(NULL, szModulePath.GetBuffer(MAX_PATH), MAX_PATH);
	szModulePath.ReleaseBuffer();
	szModulePath = FileName;
	int nIndex = szModulePath.ReverseFind('\\');
	if (nIndex != -1){
		szModulePath = szModulePath.Left(nIndex+1);
	}
	
	
	sprintf(szPathName,"%s",szModulePath);
	
	if (access(szPathName,0)!=0)
	{
		if(mkdir(szPathName)!=0)
		{
			sprintf(m_errMsg,"不能创建文件路径[%s]", szPathName);
			
			printf("%s", m_errMsg);
			return -1;
		}
	}

	fp = fopen(FileName, "w");
	if (fp == NULL)
	{		
		printf("write file false");
		return -1;
	}
	
	map<string,string>::iterator it_OilPrice;	
	for (it_OilPrice = pMap.begin(); it_OilPrice != pMap.end(); it_OilPrice++)
	{	
		fprintf(fp,"%s%c%s\n",it_OilPrice->first.c_str(),separator,it_OilPrice->second.c_str());	
	}
	
	fclose(fp);

	return 0;
}

double interROUND(double d,int n)
{
	double tt = 0.0;
	if (d < 0)
	{
		tt = -0.5000001;//-0.555555555;
	}
	else
	{
		tt = 0.5000001;// 0.555555555;
	}
#ifdef WIN32
	__int64 power = pow((double)10, n);
	__int64 t = (d) * pow((double)10, n) + tt;
#else
	long long power = (long long)pow((double)10, n);
	long long t = (long long)((d) * pow((double)10, n) + tt);
#endif
	return ((double)t / power);
}

double ROUND(double num, int n)          
{  
	char tmpchar[30];
	double aa;
	aa = num;
	int len;
	int splen=0;
	sprintf(tmpchar,"%lf",num);
	len = strlen(tmpchar);
	for (int j=0;j<len;j++)
	{
		if (tmpchar[j]=='.')
		{
			splen = j;
			break;
		}
	}
	if ( 0 == splen )
	{
		return num;
	}
	char ccccc = tmpchar[splen+n+1];
	if (ccccc=='5' )
	{
		double bb = (double)2/pow((double)10,n+3);
		if (tmpchar[0]=='-')
		{
			aa = aa - bb;
		}
		else
		{
			aa = aa + bb;
		}
		
	}
	aa = interROUND(aa,n);
	return aa;
}

void AddLogQueue(st_log& myst_log)
{	
	m_TdeqLogS.m_mutex.Lock();
	m_TdeqLogS.m_deqStLog.push_back(myst_log);
	m_TdeqLogS.m_mutex.UnLock();
	
}
int WriteLogSQueue()
{
	st_log m_stlog;
	memset(&m_stlog,0,sizeof(m_stlog));
	
	m_TdeqLogS.m_mutex.Lock();
	bool a = m_TdeqLogS.m_deqStLog.empty();
	if (0 == m_TdeqLogS.m_deqStLog.size())
	{		
		m_TdeqLogS.m_mutex.UnLock();
		return -1;
	}
	
	if (m_TdeqLogS.m_deqStLog.size()>3000)
	{
		int num = m_TdeqLogS.m_deqStLog.size();
		m_TdeqLogS.m_deqStLog.erase(m_TdeqLogS.m_deqStLog.begin(),m_TdeqLogS.m_deqStLog.end()-1000);		
		
	}
	
	memcpy(&m_stlog, &(m_TdeqLogS.m_deqStLog[0]), sizeof(m_stlog));
	m_TdeqLogS.m_deqStLog.pop_front();
	m_TdeqLogS.m_mutex.UnLock();		
	
	
	int ret = WriteLog(m_stlog.logName,m_stlog.fileName,m_stlog.line,m_stlog.code,m_stlog.logInfo);
	if (0 != ret)
	{	
		// 		wrlog(m_stlog,"All_log",__FILE__,__LINE__,0,"writeTable出错")
		// 		AddLogQueue(m_stlog);		
		//		WriteLog("All_log",__FILE__,__LINE__,0,"writeLog出错");
		
	}
	
	return 0;
	
}
void ClearLogSQueue()
{
	m_TdeqLogS.m_mutex.Lock();
	m_TdeqLogS.m_deqStLog.clear();
	m_TdeqLogS.m_mutex.UnLock();	
}
