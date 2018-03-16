// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HttpClient.h"
#include "io.h"
#include "direct.h"
#include "risksqlapi.h"
#include <map>
#include <afxinet.h> 
#include <afx.h>  
#include <afxwin.h>
#include "myfunction.h"
#include "cJSON.h"
#include "URLCode.h"


#define wrlog(STLOG,LOGNAME,FILENAME,LINE,CODE,INFO)	\
	{													\
	strcpy(STLOG.logName,(LOGNAME));					\
	strcpy(STLOG.fileName,(FILENAME));					\
	STLOG.line=(LINE);									\
	STLOG.code=(CODE);									\
	strcpy(STLOG.logInfo,(INFO));						\
}

#define FILEPATH_LENGTH 512
CRiskSqlAPI *sqldb_teamNo;
CRiskSqlAPI *sqldb_price;
CRiskSqlAPI *sqldb_record;
CRiskSqlAPI *sqldb_RealRecord;
CRiskSqlAPI *sqldb_Type;

map<string,string> mp_OilPrice;
map<string,string> mp_OilType;
map<string,string> mp_TransOilType;

char gloInterfaceURL[32] = {0};			//���۽ӿ�URL��ַ
char gloDBPrefix[32] = {0};				//���ݿ�ǰ׺
char gloStationId[9] = {0};				//��վ���
int  gloFlowControl = 0;				//���ͼ�¼���Ϳ�����Ŀ
long gloTimerAlive = 0;					//����ʱ��������λ���룩
long gloTimerRecord = 0;				//���ͼ�¼��ѯʱ��������λ���룩
long gloTimerRealRecord = 0;			//ʵʱ���ͼ�¼��ѯʱ��������λ���룩
long gloTimerPrice = 0;					//�ͼ۲�ѯʱ��������λ���룩
long gloTimerTeamNO = 0;				//�����ѯʱ��������λ���룩
long gloTimerOilType = 0;				//��ǹ-��Ʒ��ѯʱ��������λ���룩
long gloTimerUpdate = 0;				//�Զ����²�ѯʱ��������λ���룩
bool gloRecord = true;					//���ͼ�¼�Ƿ�������

	
int DatabaseIni()
{
	int	rtn = 0;
	/*
	sqldb_teamNo = new CRiskSqlAPI("Database.cfg");	
	int	rtn = sqldb_teamNo->OpenDB();
	
	if (rtn<0)
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_teamNo->DB_ErrorCode,sqldb_teamNo->DB_ErrorInfo);
		return -1;
	}
	if (sqldb_teamNo->con->isConnected())
	{
		printf("���ݿ�������\n");	
		
	}
	else
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_teamNo->DB_ErrorCode,sqldb_teamNo->DB_ErrorInfo);
		return -1;
	}
	*/
	sqldb_price = new CRiskSqlAPI(".\\ini\\Database.cfg");	
	rtn = sqldb_price->OpenDB();
	
	if (rtn<0)
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_price->DB_ErrorCode,sqldb_price->DB_ErrorInfo);
		return -1;
	}
	if (sqldb_price->con->isConnected())
	{
		printf("���ݿ�������\n");	
		
	}
	else
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_price->DB_ErrorCode,sqldb_price->DB_ErrorInfo);
		return -1;
	}
	
	sqldb_record = new CRiskSqlAPI(".\\ini\\Database.cfg");	
	rtn = sqldb_record->OpenDB();	
	if (rtn<0)
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_record->DB_ErrorCode,sqldb_record->DB_ErrorInfo);
		return -1;
	}
	if (sqldb_record->con->isConnected())
	{
		printf("���ݿ�������\n");
		
	}
	else
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_record->DB_ErrorCode,sqldb_record->DB_ErrorInfo);
		return -1;
	}
	
	sqldb_RealRecord = new CRiskSqlAPI(".\\ini\\Database.cfg");	
	rtn = sqldb_RealRecord->OpenDB();	
	if (rtn<0)
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_RealRecord->DB_ErrorCode,sqldb_RealRecord->DB_ErrorInfo);
		return -1;
	}
	if (sqldb_RealRecord->con->isConnected())
	{
		printf("���ݿ�������\n");
		
	}
	else
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_RealRecord->DB_ErrorCode,sqldb_RealRecord->DB_ErrorInfo);
		return -1;
	}

	sqldb_Type = new CRiskSqlAPI(".\\ini\\Database.cfg");	
	rtn = sqldb_Type->OpenDB();	
	if (rtn<0)
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_Type->DB_ErrorCode,sqldb_Type->DB_ErrorInfo);
		return -1;
	}
	if (sqldb_Type->con->isConnected())
	{
		printf("���ݿ�������\n");
		
	}
	else
	{
		printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_Type->DB_ErrorCode,sqldb_Type->DB_ErrorInfo);
		return -1;
	}

	return 0;
}


int parseJson(char * pMsg,int * errCode, char * versionNo, char * downLoadUrl)
{
	int iRet = 0;
	
    if(NULL == pMsg)
    {
		return iRet;
    }
    cJSON * pJson = cJSON_Parse(pMsg);
    if(NULL == pJson)                                                                                         
    {
        // parse faild, return
		return iRet;
    }
	
    cJSON * pSub = cJSON_GetObjectItem(pJson, "error_code");
    if(NULL == pSub)
    {
		return iRet;
    }
	*errCode = pSub->valueint;

	pSub = cJSON_GetObjectItem(pJson,"result");
    if(NULL == pSub)
    {
        printf("GetObjectItem result failed\n");    
        return -1;
    }
	
	cJSON * pSubSub = cJSON_GetObjectItem(pSub,"obtainUrl");
    if(NULL == pSubSub)
    {
        printf("GetObjectItem obtainUrl failed\n");    
        return -1;
    }
	strcpy(downLoadUrl,pSubSub->valuestring);
	
	pSubSub = cJSON_GetObjectItem(pSub, "versionCode");
    if(NULL == pSubSub)
    {
        printf("GetObjectItem versionCode failed\n");    
        return -1;
    }
	strcpy(versionNo,pSubSub->valuestring);	
	
    cJSON_Delete(pJson);
//    cJSON_Delete(pSub);
//    cJSON_Delete(pSubSub);
	return iRet;

}

// ִ��ָ�������ַ���
bool Exec(char *cmd)
{
	int iRet = system(cmd);
	if (iRet != 0)
	{
		return false;
	}
	return true;
}
int reboot()
{
	TCHAR szAppName[MAX_PATH];
	:: GetModuleFileName(NULL, szAppName, MAX_PATH);
	//	std::wstring strAppFullName = szAppName;
	// �رյ�ǰ����  
	HWND hWnd = ::FindWindow(NULL,szAppName);  
	ULONG nProcessID;  
	::GetWindowThreadProcessId( hWnd, &nProcessID );  
	HANDLE hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE,nProcessID );
	::TerminateProcess( hProcessHandle, -1 );
	ExitProcess(-1);
	
	return 0; 
}

int GetFile(const CString strUrl,const CString strSavePath)  
{  
    //��鴫�����������  
    if (strUrl.IsEmpty())  
        return -5;  
    if (strSavePath.IsEmpty())  
        return -6;  
	
    unsigned short nPort;       //���ڱ���Ŀ��HTTP����˿�  
    CString strServer, strObject;   //strServer���ڱ����������ַ��strObject���ڱ����ļ���������  
    DWORD dwServiceType,dwRet;      //dwServiceType���ڱ���������ͣ�dwRet���ڱ����ύGET���󷵻ص�״̬��  
	
    //����URL����ȡ��Ϣ  
    if(!AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort))  
    {  
        //����ʧ�ܣ���Url����ȷ  
        return -1;  
    }  
    //�����������Ӷ���HTTP���Ӷ���ָ������ڸ����ӵ�HttpFile�ļ�����ָ�룬ע��delete  
//    CInternetSession intsess;  
	CInternetSession * intsess = NULL;  
    CHttpFile *pHtFile = NULL;  
    //������������  
    CHttpConnection *pHtCon = intsess->GetHttpConnection(strServer,nPort);  
    if(pHtCon == NULL)  
    {  
        //������������ʧ��  
        intsess->Close();  
        return -2;  
    }  
    //����GET����  
    pHtFile = pHtCon->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject);  
    if(pHtFile == NULL)  
    {  
        //����GET����ʧ��  
        intsess->Close();  
        delete pHtCon;pHtCon = NULL;  
        return -3;  
    }  
    //�ύ����  
    pHtFile->SendRequest();  
    //��ȡ���������ص�״̬��  
    pHtFile->QueryInfoStatusCode(dwRet);  
    if (dwRet != HTTP_STATUS_OK)  
    {  
        //����������������  
        intsess->Close();  
        delete pHtCon;pHtCon = NULL;  
        delete pHtFile;pHtFile = NULL;  
        return -4;  
    }  
    //��ȡ�ļ���С  
    UINT nFileLen = (UINT)pHtFile->GetLength();  
    DWORD dwRead = 1;       //���ڱ�ʶ���˶��٣�Ϊ1��Ϊ�˽���ѭ��  
    //����������  
    CHAR *szBuffer = new CHAR[nFileLen+1];
    TRY   
    {   
        //�����ļ�  
		CFile PicFile(strSavePath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
        while(dwRead>0)  
        {    
            //��ջ�����  
            memset(szBuffer,0,(size_t)(nFileLen+1));
            //��ȡ��������  
            dwRead = pHtFile->Read(szBuffer,nFileLen);   
            //д�뵽�ļ�  
            PicFile.Write(szBuffer,dwRead);  
        }
        //�ر��ļ�  
        PicFile.Close();
        //�ͷ��ڴ�  
        delete []szBuffer;  
        delete pHtFile;  
        delete pHtCon;  
        //�ر���������  
        intsess->Close();  
    }  
    CATCH(CFileException,e)  
    {  
        //�ͷ��ڴ�  
        delete []szBuffer;
        delete pHtFile;  
        delete pHtCon;  
        //�ر���������  
        intsess->Close();  
        return -7;          //��д�ļ��쳣  
    }  
    END_CATCH  
        return 0;  
}

DWORD dwTimerTeamNo = 0;
DWORD dwTimerOilPrice = 0;
DWORD dwTimerOilRecord = 0;
DWORD dwTimerRealOilRecord = 0;
DWORD dwTimerOilType = 0;
DWORD dwTimerUpdate = 0;

VOID CALLBACK TimerProcTeamNo(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
    if (dwTimerTeamNo == idEvent)
    {
		
		printf("TimerProcTeamNo\n");

		int iRet = 0;
		//1 ��ѯ��ʼ������
		char fileName[128] = {0};
		char Key[17] = {0};	
		char Value[17] = {0};
		sprintf(fileName,"%s",".\\file\\TeamNO.txt");
		sprintf(Key,"%s","teamNo");
		iRet = myReadFile(fileName,Key,Value);
		if (iRet != 0)
		{
			printf("read file error \n");
			WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
		}
		
        printf("�����ļ���file name %s key = %s value = %s \n",fileName,Key,Value);
		
		//2��ѯ���ݿ�				
		char sSql[1024] = {0};
		CSqlData2 sqldata;
		int ret = 0;
		char DB_TeamNo[17] = {0};
					
		sprintf(sSql, "select 1 ");		
//		WriteLog("All_log",__FILE__,__LINE__,0,sSql);
		sqldata = sqldb_teamNo->LoadResultSql2(sSql);
		ret = sqldata.GetSqlCount();
		if(ret < 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
			printf(" ###########��ѯ���ݿ���� \n");
		}
		else if(ret == 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-2,"��ѯ���ݿ�������");
		}
		
		{
			sqldata.GetAt(0, 0, DB_TeamNo);
		}
		
		//3.�Ƚ����ݿ����ֶ��Ƿ��и���
		if (strncmp(Value,DB_TeamNo,sizeof(DB_TeamNo)) != 0)
		{			
			//4.�и���֪ͨokoil
			CHttpClient aa;
			string strResponse;
			char szUrl[1024] = {0};
			sprintf(szUrl,"%sprivate-server/api/gas_station_data/shift?",gloInterfaceURL);
								
			char szPost[512] = {0};
			sprintf(szPost,"stationId=%s&teamNo=%s&env=1",gloStationId,DB_TeamNo);
			iRet = aa.HttpPost(szUrl,szPost,strResponse);
			
			if (SUCCESS != iRet)
			{
				printf("������Ϣʧ��!\n");
				return;
			}
			//ƥ�䷵�����Ƿ�Ϊ0
			if (strResponse.find(":0,") == std::string::npos)
			{
				WriteLog("All_log",__FILE__,__LINE__,-1,"json����ʧ��");
				return;
			}			
			//5.д�ļ�			
			myWriteFile(fileName,Key,DB_TeamNo);
		}
    }
}  

VOID CALLBACK TimerProcOilPrice(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
    if (dwTimerOilPrice == idEvent) 
    {		
		printf("TimerProcOilPrice\n");

		int iRet = 0;
		bool WriteFlag = false;

		//1 ��ѯ��ʼ������
		char fileName[128] = {0};
		sprintf(fileName,"%s",".\\file\\OilPrice.txt");
		mp_OilPrice.clear();
		myReadFile2(fileName,'=',mp_OilPrice);
		
		map<string,string>::iterator it_Oil;		
		for (it_Oil = mp_OilPrice.begin(); it_Oil != mp_OilPrice.end(); it_Oil++)
		{
			//2��ѯ���ݿ�				
			char sSql[1024] = {0};
			CSqlData2 sqldata;
			int ret = 0;
			char DB_OilPrice[15] = {0};
			sprintf(sSql,"select oilprice from %sT_ZD_OIL where oilcode = '%s' ",gloDBPrefix,mp_TransOilType[it_Oil->first].c_str());			
//			WriteLog("All_log",__FILE__,__LINE__,0,sSql);
			sqldata = sqldb_price->LoadResultSql2(sSql);
			ret = sqldata.GetSqlCount();
			if(ret < 0)
			{				
//				printf("���ݿ����Ӵ���[%d-%s]\n",sqldb_price->DB_ErrorCode,sqldb_price->DB_ErrorInfo);
				WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
				WriteLog("All_log",__FILE__,__LINE__,0,sSql);
				break;
			}
			else if(ret == 0)
			{
				WriteLog("All_log",__FILE__,__LINE__,-2,"��ѯ���ݿ�������");
				WriteLog("All_log",__FILE__,__LINE__,0,sSql);
				continue;
			}		
			//database���ͼ�
			{
				sqldata.GetAt(0, 0, DB_OilPrice);
			}
			 
			//3.�Ƚ����ݿ����ֶ��Ƿ��и���
			if (fabs(atof(it_Oil->second.c_str())-atof(DB_OilPrice)) > 0.001)
			{				
				//4.�и���֪ͨokoil
				CHttpClient aa;
				string strResponse;
				string strUrl;
				char tmpMsg[1024] = {0};
				char szUrl[512] = {0};
				sprintf(szUrl,"%sprivate-server/api/gas_station_data/oil_type_price?",gloInterfaceURL);							
				char szPost[512] = {0};
				sprintf(szPost,"stationId=%s&sellPrice=%s&oilType=%s&action=0&env=1",gloStationId,DB_OilPrice,it_Oil->first.c_str());
				iRet = aa.HttpPost(szUrl,szPost,strResponse);
// 				printf(szUrl);
// 				printf("\n");
// 				printf(szPost);
// 				printf("\n");
// 				printf(strResponse.c_str());
// 				printf("\n");
// 				Sleep(5000);
				
				if (SUCCESS != iRet)
				{	
					sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilPrice��json����ʧ��",szUrl,szPost,strResponse.c_str());
					WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
					return;
				}				
				//ƥ�䷵�����Ƿ�Ϊ0
				if (strResponse.find(":0,") == std::string::npos)
				{
					sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilPrice��json����ʧ��",szUrl,szPost,strResponse.c_str());
					WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
					return;
				}
			//	char tmpMsg[512] = {0};
				sprintf(tmpMsg,"�����ͼ��ļ� name %s key=%s value_old = %s value_new = %s",
					fileName,it_Oil->first.c_str(),it_Oil->second.c_str(),DB_OilPrice);
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				printf("%s \n",tmpMsg);
				
				WriteFlag = true;				
				it_Oil->second = DB_OilPrice;
			}	
		}
		if (WriteFlag)
		{
			//5.д�ļ�
			myWriteFile2(fileName,'=',mp_OilPrice);
		}		
	}
}

VOID CALLBACK TimerProcOilRecord(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
   if (dwTimerOilRecord == idEvent)  
    {   
	   printf("dwTimerOilRecord\n");

	   int iRet = 0;
	   char tmpMsg[1024] = {0};
	   //1 ��ѯ��ʼ������
	   char fileName[128] = {0};
	   char fileKey[17] = {0};	
	   char fileValue[33] = {0};
	   sprintf(fileName,"%s",".\\file\\OilRecord.txt");
	   sprintf(fileKey,"%s","RecodeNo");
	   iRet = myReadFile(fileName,fileKey,fileValue);
	   if (iRet != 0)
	   {
		   printf("read file error \n");
		   WriteLog("All_log",__FILE__,__LINE__,-1,"��ȡ�ļ�RecodeNo����");
//		   myWriteFile(fileName,fileKey,"");
	   }	   
		//2��ѯ���ݿ�				
		char sSql[512] = {0};
		CSqlData2 sqldata;
		int ret = 0;
		char DB_matchNo[21] = {0};	//���ͱ��
		char DB_oilDate[31] = {0};	//��������
		char DB_teamNo[17] = {0};	//�����
		char DB_custNo[11] = {0};	//��վϵͳ�ͻ���
		char DB_cardNo[25] = {0};	//���Ϳ���
		char DB_stationId[11] = {0};	//��վ���
		char DB_pumpNo[11] = {0};	//��ǹ���
		char DB_oilType[11] = {0};	//��Ʒ
		char DB_sellPrice[15] = {0};	//���͵���
		char DB_oilAmount[15] = {0};	//��������
		char DB_oilMoney[15] = {0};		//���ͽ��
	/*						
		sprintf(sSql,"select id,CONVERT(varchar(100),sTime,121),accno,'',asn,'310001',pos_p,\
			case when g_code = '1032' then '92#' when g_code = '1052' then '95#' end,\
			prc,vol,amn from %st_t_operat where id > %d order by id asc ",gloDBPrefix,atol(fileValue));
	*/
		
		sprintf(sSql,"select id,CONVERT(varchar(100),sTime,121),accno,'non',asn,'%s',pos_p,\
			SUBSTRING(oilname,1,3),\
			prc,vol,amn from %st_t_operat where id > %d order by id asc ",gloStationId,gloDBPrefix,atol(fileValue));
		
//		WriteLog("All_log",__FILE__,__LINE__,0,sSql);

		CTime time = CTime::GetCurrentTime(); ///����CTime����		
		CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
		sprintf(tmpMsg,"record begin %s \n",m_strTime);
		printf(tmpMsg);
		sqldata = sqldb_record->LoadResultSql2(sSql,gloFlowControl);
		
		time = CTime::GetCurrentTime(); ///����CTime����
		m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
		sprintf(tmpMsg,"record end %s \n",m_strTime);
		printf(tmpMsg);

		ret = sqldata.GetSqlCount();
		if(ret < 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
			WriteLog("All_log",__FILE__,__LINE__,0,sSql);
			return;
		}
		else if(ret == 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-2,"��ѯ���ݿ�������");
			WriteLog("All_log",__FILE__,__LINE__,0,sSql);
			return;
		}
		if (ret > gloFlowControl)
		{			
			sprintf(tmpMsg,"���ͼ�¼���Ʒ��ͼ�¼����ѯ��¼��Ŀ��%d�����Ƽ�¼��Ŀ��%d",ret,gloFlowControl);
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			ret = gloFlowControl;
		}		
		for(int i=0; i<ret; i++)
		{		
			sqldata.GetAt(i, 0, DB_matchNo);
			sqldata.GetAt(i, 1, DB_oilDate);
			sqldata.GetAt(i, 2, DB_teamNo);
			sqldata.GetAt(i, 3, DB_custNo);
			sqldata.GetAt(i, 4, DB_cardNo);
			sqldata.GetAt(i, 5, DB_stationId);
			sqldata.GetAt(i, 6, DB_pumpNo);
			sqldata.GetAt(i, 7, DB_oilType);
			sqldata.GetAt(i, 8, DB_sellPrice);
			sqldata.GetAt(i, 9, DB_oilAmount);
			sqldata.GetAt(i, 10, DB_oilMoney);

			CURLCode Curl;
			//4.�м��ͼ�¼֪ͨokoil
			CHttpClient aa;
			string strResponse;
			char szUrl[512] = {0};
			sprintf(szUrl,"%sprivate-server/api/gas_station_data/refuel_record?",gloInterfaceURL);
					
			char szPost[1024] = {0};
			sprintf(szPost,"oilDate=%s&teamNo=%s&custNo=%s&cardNo=%s&stationId=%s&sellPrice=%s&"
				"oilType=%s&pumpNo=%s&oilAmount=%s&oilMoney=%s&refuelId=%s&env=1",DB_oilDate,DB_teamNo,DB_custNo,DB_cardNo,
				DB_stationId,DB_sellPrice,Curl.Encode(DB_oilType).c_str(),DB_pumpNo,DB_oilAmount,DB_oilMoney,DB_matchNo);

			iRet = aa.HttpPost(szUrl,szPost,strResponse);

			if (SUCCESS != iRet)
			{
				sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilRecord ������Ϣʧ��",szUrl,szPost,strResponse.c_str());
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				return;
			}
			//ƥ�䷵�����Ƿ�Ϊ0
			if (strResponse.find(":0,") == std::string::npos)
			{
				sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilRecord��json����ʧ��",szUrl,szPost,strResponse.c_str());
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				return;
			}			
		}
		//5.д�ļ�			
		myWriteFile(fileName,fileKey,DB_matchNo);
		
		time = CTime::GetCurrentTime(); ///����CTime����
		m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
		sprintf(tmpMsg,"record write end %s \n",m_strTime);
		printf(tmpMsg);

    }
}

VOID CALLBACK TimerProcRealOilRecord(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
   if (dwTimerRealOilRecord == idEvent)  
    {   
	   printf("dwTimerRealOilRecord\n");

	   int iRet = 0;
	   char tmpMsg[1024] = {0};
	   //1 ��ѯ��ʼ������
	   char fileName[128] = {0};
	   char fileKey[17] = {0};	
	   char fileValue[33] = {0};
	   sprintf(fileName,"%s",".\\file\\RealOilRecord.txt");
	   sprintf(fileKey,"%s","RealRecodeNo");
	   iRet = myReadFile(fileName,fileKey,fileValue);
	   if (iRet != 0)
	   {
		   printf("read file error \n");
		   WriteLog("All_log",__FILE__,__LINE__,-1,"��ȡ�ļ�RealRecodeNo����");
//		   myWriteFile(fileName,fileKey,"");
	   }	   
		//2��ѯ���ݿ�				
		char sSql[512] = {0};
		CSqlData2 sqldata;
		int ret = 0;
		char DB_matchNo[21] = {0};	//���ͱ��
		char DB_oilDate[31] = {0};	//��������
		char DB_teamNo[17] = {0};	//�����
		char DB_custNo[11] = {0};	//��վϵͳ�ͻ���
		char DB_cardNo[25] = {0};	//���Ϳ���
		char DB_stationId[11] = {0};	//��վ���
		char DB_pumpNo[11] = {0};	//��ǹ���
		char DB_oilType[11] = {0};	//��Ʒ
		char DB_sellPrice[15] = {0};	//���͵���
		char DB_oilAmount[15] = {0};	//��������
		char DB_oilMoney[15] = {0};		//���ͽ��

		
		sprintf(sSql,"select a.* from ( \
			select top 16 id,CONVERT(varchar(100),sTime,121) oildate,'non' teamno,'non' custno,asn,'%s' stationid,pos_p,\
			SUBSTRING(oilname,1,3) oiltype,prc,vol,amn from %soperatelog \
			where id > %d order by id desc )a order by id asc",gloStationId,gloDBPrefix,atol(fileValue));
		
//		WriteLog("All_log",__FILE__,__LINE__,0,sSql);

// 		CTime time = CTime::GetCurrentTime(); ///����CTime����		
// 		CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
// 		sprintf(tmpMsg,"Realrecord begin %s \n",m_strTime);
// 		printf(tmpMsg);
		sqldata = sqldb_RealRecord->LoadResultSql2(sSql);
		
// 		time = CTime::GetCurrentTime(); ///����CTime����
// 		m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
// 		sprintf(tmpMsg,"Realrecord end %s \n",m_strTime);
// 		printf(tmpMsg);

		ret = sqldata.GetSqlCount();
		if(ret < 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
			WriteLog("All_log",__FILE__,__LINE__,0,sSql);
			return;
		}
		else if(ret == 0)
		{
			WriteLog("All_log",__FILE__,__LINE__,-2,"��ѯ���ݿ�������");
			WriteLog("All_log",__FILE__,__LINE__,0,sSql);
			return;
		}
		if (ret > gloFlowControl)
		{			
			sprintf(tmpMsg,"ʵʱ���ͼ�¼���ͣ���ѯ��¼��Ŀ��%d�����Ƽ�¼��Ŀ��%d",ret,gloFlowControl);
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			ret = gloFlowControl;
		}		
		for(int i=0; i<ret; i++)
		{		
			sqldata.GetAt(i, 0, DB_matchNo);
			sqldata.GetAt(i, 1, DB_oilDate);
			sqldata.GetAt(i, 2, DB_teamNo);
			sqldata.GetAt(i, 3, DB_custNo);
			sqldata.GetAt(i, 4, DB_cardNo);
			sqldata.GetAt(i, 5, DB_stationId);
			sqldata.GetAt(i, 6, DB_pumpNo);
			sqldata.GetAt(i, 7, DB_oilType);
			sqldata.GetAt(i, 8, DB_sellPrice);
			sqldata.GetAt(i, 9, DB_oilAmount);
			sqldata.GetAt(i, 10, DB_oilMoney);

			CURLCode Curl;			
		//	printf("Curl.Encode(DB_oilType).c_str() is %s",Curl.Encode(DB_oilType).c_str());

			//4.�м��ͼ�¼֪ͨokoil
			CHttpClient aa;
			string strResponse;
			char szUrl[512] = {0};
			sprintf(szUrl,"%sprivate-server/api/gas_station_data/refuel_realtime_record?",gloInterfaceURL);
					
			char szPost[1024] = {0};
			sprintf(szPost,"oilDate=%s&teamNo=%s&custNo=%s&cardNo=%s&stationId=%s&sellPrice=%s&"
				"oilType=%s&pumpNo=%s&oilAmount=%s&oilMoney=%s&refuelId=%s&env=1",DB_oilDate,DB_teamNo,DB_custNo,DB_cardNo,
				DB_stationId,DB_sellPrice,Curl.Encode(DB_oilType).c_str(),DB_pumpNo,DB_oilAmount,DB_oilMoney,DB_matchNo);

			iRet = aa.HttpPost(szUrl,szPost,strResponse);

			if (SUCCESS != iRet)
			{
				sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","RealOilRecord ������Ϣʧ��",szUrl,szPost,strResponse.c_str());
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				return;
			}
			//ƥ�䷵�����Ƿ�Ϊ0
			if (strResponse.find(":0,") == std::string::npos)
			{
				sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","RealOilRecord��json����ʧ��",szUrl,szPost,strResponse.c_str());
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				return;
			}			
		}
		//5.д�ļ�			
		myWriteFile(fileName,fileKey,DB_matchNo);
		
	//	time = CTime::GetCurrentTime(); ///����CTime����
	//	m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	//	sprintf(tmpMsg,"record write end %s \n",m_strTime);
	//	printf(tmpMsg);
    }
}
VOID CALLBACK TimerProcOilType(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
    if (dwTimerOilType == idEvent) 
    {
		printf("TimerProcOilType\n");

		int iRet = 0;
		bool WriteFlag = false;
		char tmpMsg[2048] = {0};
		//1 ��ѯ��ʼ������
		char fileName[128] = {0};		
		sprintf(fileName,"%s",".\\file\\OilType.txt");		
		mp_OilType.clear();
		myReadFile2(fileName,'=',mp_OilType);		
		map<string,string>::iterator it_Oil;		
		for (it_Oil = mp_OilType.begin(); it_Oil != mp_OilType.end(); it_Oil++)
		{			 
			//2��ѯ���ݿ�				
			char sSql[512] = {0};
			CSqlData2 sqldata;
			int ret = 0;
			char DB_OilType[15] = {0};
			sprintf(sSql,"select case when oilcode = '1032' then '92#' when oilcode = '1052' then '95#' end from %sguninfo \
				where gunport = '%s'",gloDBPrefix,it_Oil->first.c_str());
			sqldata = sqldb_Type->LoadResultSql2(sSql);
			ret = sqldata.GetSqlCount();
			if(ret < 0)
			{
				WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ݿ����");
				WriteLog("All_log",__FILE__,__LINE__,0,sSql);
				break;
			}
			else if(ret == 0)
			{
				WriteLog("All_log",__FILE__,__LINE__,-2,"��ѯ���ݿ�������");
				WriteLog("All_log",__FILE__,__LINE__,0,sSql);
				continue;
			}		
			//database����Ʒ
			{
				sqldata.GetAt(0, 0, DB_OilType);
			}
			//3.�Ƚ����ݿ����ֶ��Ƿ��и���
			if (strncmp(it_Oil->second.c_str(),DB_OilType,sizeof(DB_OilType)) != 0)
			{				
				//4.�и���֪ͨokoil
				CHttpClient aa;
				string strResponse;
				string strUrl;
				char szUrl[1024] = {0};				
				sprintf(szUrl,"%sprivate-server/api/gas_station_data/pump_type?",gloInterfaceURL);
				char szPost[512] = {0};
				sprintf(szPost,"stationId=%s&pumpNo=%s&oilType=%s&action=0&env=1",gloStationId,it_Oil->first.c_str(),DB_OilType);
				
				iRet = aa.HttpPost(szUrl,szPost,strResponse);
				
				if (SUCCESS != iRet)
				{
					sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilType������Ϣʧ��",szUrl,szPost,strResponse.c_str());
					WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
					return;
				}
				//ƥ�䷵�����Ƿ�Ϊ0
				if (strResponse.find(":0,") == std::string::npos)
				{
					sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","OilType json����ʧ��",szUrl,szPost,strResponse.c_str());
					WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
					return;
				}
				char tmpMsg[512] = {0};
				sprintf(tmpMsg,"������ǹ-��Ʒ�ļ� name %s key=%s value_old = %s value_new = %s ",
					fileName,it_Oil->first.c_str(),it_Oil->second.c_str(),DB_OilType);
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				
				WriteFlag = true;	
				it_Oil->second = DB_OilType;
			}	
		}
		if (WriteFlag)
		{		
			//5.д�ļ�		
			myWriteFile2(fileName,'=',mp_OilType);
		}		
	}
	
// L_EXITRecond:
// 	gloRecordRuning = false;
}

VOID CALLBACK TimerProcUpdate(HWND hwnd,UINT uMsg, UINT_PTR idEvent, DWORD dwTime)  
{
   if (dwTimerUpdate == idEvent)  
    {   
		printf("dwTimerUpdate\n");

		int iRet = 0;
		char tmpMsg[1024] = {0};
		//1 ��ѯ��ʼ������
		char fileName[128] = {0};
		char fileKey[17] = {0};	
		char fileValue[33] = {0};
		sprintf(fileName,"%s",".\\update\\version.ini");
		//��ѯ�汾����
		sprintf(fileKey,"%s","versionNo");
		iRet = myReadFile(fileName,fileKey,fileValue);
		if (iRet != 0)
		{
		   printf("read file error \n");
		   WriteLog("All_log",__FILE__,__LINE__,-1,"��ѯ���ذ汾�Ŵ���");
		}	   
		
		printf("���ݲɼ�����汾��V%s\n",fileValue);

		//��ѯ��̨ϵͳ�汾��
		CHttpClient aa;
		string strResponse;
		string strUrl;
		char szUrl[512] = {0};
		sprintf(szUrl,"%sprivate-server/api/gas_station_data/get_app_version?",gloInterfaceURL);
		char szPost[512] = {0};
		sprintf(szPost,"env=1",gloStationId);
		
		iRet = aa.HttpPost(szUrl,szPost,strResponse);
		
		if (SUCCESS != iRet)
		{
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n","update��������Ϣʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return;
		}
		//ƥ�䷵�����Ƿ�Ϊ0
		if (strResponse.find(":0,") == std::string::npos)
		{
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","update����json����ʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return;
		}
		char szResponse[1024] = {0};
		int iErrCode = 0;
		char szDownLoadUrl[1024] = {0};
		char szVersionNo[32] = {0};
		sprintf(szResponse,"%s",strResponse.c_str());

		iRet = parseJson(szResponse,&iErrCode,szVersionNo,szDownLoadUrl);
		if (SUCCESS != iRet)
		{
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","json����ʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return;
		}

		if (0 != iErrCode)
		{	
			printf("����jsonʧ�� %d \n",iErrCode);
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","json����ʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return;
		}
	
	//	if (strncmp(fileValue,szVersionNo,strlen(szVersionNo)) < 0 )
		//�汾�Ÿ�ʽManjor . Minor . Commit 
		CString strTmp;
		int nTmp =0;
		strTmp = fileValue;
		nTmp = strTmp.ReverseFind('.');
		int CommitLocal = atoi(strTmp.Right(strTmp.GetLength()-nTmp-1));
		strTmp = strTmp.Left(nTmp);
		nTmp = strTmp.ReverseFind('.');
		int MinorLocal = atoi(strTmp.Right(strTmp.GetLength()-nTmp-1));
		strTmp = strTmp.Left(nTmp);
		int ManjorLocal = atoi(strTmp);
		
		strTmp = szVersionNo;
		nTmp = strTmp.ReverseFind('.');
		int CommitServer = atoi(strTmp.Right(strTmp.GetLength()-nTmp-1));
		strTmp = strTmp.Left(nTmp);
		nTmp = strTmp.ReverseFind('.');
		int MinorServer = atoi(strTmp.Right(strTmp.GetLength()-nTmp-1));
		strTmp = strTmp.Left(nTmp);
		int ManjorServer = atoi(strTmp);

// 		printf(" =%s  = %d  = %d  = %d  \n",fileValue,ManjorLocal,MinorLocal,CommitLocal);
// 		printf(" =%s  = %d  = %d  = %d  \n",szVersionNo,ManjorServer,MinorServer,CommitServer);
// 		Sleep(5000);
		if ((ManjorServer > ManjorLocal) ||((ManjorServer == ManjorLocal) && (MinorServer > MinorLocal)) ||
			 ((ManjorServer == ManjorLocal) && (MinorServer == MinorLocal) && (CommitServer > CommitLocal)))
		{
			CString	strPathName;
			CTime time = CTime::GetCurrentTime(); ///����CTime����		
			CString m_strTime = time.Format("%Y%m%d");			
			
			GetModuleFileName(NULL,strPathName.GetBuffer(256),256);
			strPathName.ReleaseBuffer(256);			
			int nPos = strPathName.ReverseFind('\\');
			strPathName = strPathName.Left(nPos);
			char chPath[512] = {0};
			sprintf(chPath,"%s\\update\\crawlingData_%s.zip",strPathName,m_strTime);

			int a = 0;			
			a = GetFile(szDownLoadUrl,chPath);			
			if (a != 0)
			{
				printf("#############�����ļ�ʧ�� %d \n",a);
				sprintf(tmpMsg,"#############�����ļ�ʧ�� %d ",a);
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
				return;
			}
			else
			{
				printf("#############�����ļ�%s�ɹ� \n",chPath);
				sprintf(tmpMsg,"#############�����ļ�%s�ɹ� ",chPath);
				WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			}

	//		sprintf(chPath,"%s %s %s",".\\update\\update.bat",strProgName,strSubProgName);
			sprintf(chPath,"%s",".\\update\\update.bat");

			if (!Exec(chPath))
			{
				printf("#############ִ��bat�ļ�ʧ�� \n");
				WriteLog("All_log",__FILE__,__LINE__,-1,"ִ��bat�ļ�ʧ�� ");
				return;
			}			
			
			sprintf(tmpMsg,"���³���汾�ļ� name %s value_old = %s value_new = %s \n",
				fileName,fileValue,szVersionNo);
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			printf(tmpMsg);
						
			//5.д�ļ�			
			myWriteFile(fileName,fileKey,szVersionNo);

			Sleep(5000);
			printf("############# reboot begin \n");			
			reboot();	
			printf("############# reboot end \n");

	
		}
    }
}

DWORD CALLBACK ThreadTeamNo(PVOID pvoid)  
{ 
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);

    dwTimerTeamNo = SetTimer(NULL,111,gloTimerTeamNO,(TIMERPROC)TimerProcTeamNo);
	  
    BOOL  bRet;  
	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {
        if(bRet == -1)
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerTeamNo);
//	ExitThread(dwTimerId);
    printf("threadTeamNo   end   here\n");  
    return   0;  
} 

DWORD CALLBACK ThreadOilPrice(PVOID pvoid)  
{
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	
    dwTimerOilPrice = SetTimer(NULL,222,gloTimerPrice,(TIMERPROC)TimerProcOilPrice);
	
    BOOL  bRet;  
	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {  
        if(bRet == -1)  
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerOilPrice);  
    printf("threadOilPrice  end   here\n");  
    return   0;  
} 

DWORD CALLBACK ThreadOilRecord(PVOID pvoid)  
{  
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
    dwTimerOilRecord = SetTimer(NULL,333,gloTimerRecord,(TIMERPROC)TimerProcOilRecord);	
    BOOL bRet;	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {  
        if(bRet == -1)  
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerOilRecord);  
    printf("threadOilRecord end	here\n");  
    return 0;  
} 

DWORD CALLBACK ThreadRealOilRecord(PVOID pvoid)  
{  
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
    dwTimerRealOilRecord = SetTimer(NULL,666,gloTimerRealRecord,(TIMERPROC)TimerProcRealOilRecord);	
    BOOL bRet;	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {  
        if(bRet == -1)  
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerOilRecord);  
    printf("threadOilRecord end	here\n");  
    return 0;  
} 

DWORD CALLBACK ThreadOilType(PVOID pvoid)  
{
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	
    dwTimerOilType = SetTimer(NULL,444,gloTimerOilType,(TIMERPROC)TimerProcOilType);
	
    BOOL  bRet;  
	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {  
        if(bRet == -1)  
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerOilType);  
    printf("threadOilType end here\n");  
    return   0;  
} 

DWORD CALLBACK ThreadUpdate(PVOID pvoid)  
{
    MSG  msg;  
    PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);
	
    dwTimerUpdate = SetTimer(NULL,555,gloTimerUpdate,(TIMERPROC)TimerProcUpdate);
	
    BOOL  bRet;  
	
    while ((bRet = GetMessage(&msg,NULL,0,0)) != 0)  
    {  
        if(bRet == -1)  
        {  
            //handle the error and possibly exit  
        }  
        else  
        {  
            TranslateMessage(&msg);  
            DispatchMessage(&msg);  
        }  
    }  
	
    KillTimer(NULL,dwTimerUpdate);  
    printf("threadUpdate end here\n");  
    return   0;  
} 

/*��Σ�
����ֵ��false ���ʧ��  true  ���ɹ�*/
bool isAlive()
{
	int iRet = 0;
	char szUrl[512] = {0};	
	char tmpMsg[1024] = {0};	
	CHttpClient aa;
	string strResponse;

    try
	{	
		sprintf(szUrl,"%sprivate-server/api/gas_station_data/heartbeat?",gloInterfaceURL);		
		char szPost[128] = {0};
		sprintf(szPost,"stationId=%s&env=1",gloStationId);
		iRet = aa.HttpPost(szUrl,szPost,strResponse);
		
		if (SUCCESS != iRet)
		{
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","����json����ʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return false;		
		}
		if (strResponse.find(":0,") == std::string::npos)
		{
			sprintf(tmpMsg,"%s \n %s \n %s \n %s \n ","����json����ʧ��",szUrl,szPost,strResponse.c_str());
			WriteLog("All_log",__FILE__,__LINE__,-1,tmpMsg);
			return false;
		}
		
		return true;	
	}
	catch(...)
	{
		return false;
	}
}

bool ReadIni(char *inifile)
{
	TIniFile tf;
	
	char szBuf[256];
	
	printf("Begin to read ini-file:%s...\n",inifile);
	if (!tf.Open(inifile))
	{
		sprintf(szBuf,"���ܴ������ļ�<%s>\n",inifile);
		printf(szBuf);
		exit(-1000);
	}
	//���ݿ�ǰ׺���û���+�ܹ�����
	tf.ReadString("COMMON","DBPrefix","",gloDBPrefix,sizeof(gloDBPrefix)-1);
	tf.ReadString("COMMON","InterfaceURL","gloInterfaceURL",gloInterfaceURL,sizeof(gloInterfaceURL)-1);
	//ȥ��'\'
	int i = 0;
	int j = 0;
	for (i=0;gloInterfaceURL[i];i++)
	{
		if(gloInterfaceURL[i] != '\\')
		{
			gloInterfaceURL[j++] = gloInterfaceURL[i];
		}
	}
	gloInterfaceURL[j] = '\0';
	
	tf.ReadString("COMMON","StationId","1",gloStationId,sizeof(gloStationId)-1);
	tf.ReadString("COMMON","FlowControl","50",szBuf,sizeof(szBuf)-1);
	gloFlowControl = atol(szBuf);
	tf.ReadString("COMMON","TimerAlive","60000",szBuf,sizeof(szBuf)-1);
	gloTimerAlive = atol(szBuf);
	tf.ReadString("COMMON","TimerRecord","60000",szBuf,sizeof(szBuf)-1);
	gloTimerRecord = atol(szBuf);
	tf.ReadString("COMMON","TimerRealRecord","60000",szBuf,sizeof(szBuf)-1);
	gloTimerRealRecord = atol(szBuf);
	tf.ReadString("COMMON","TimerPrice","60000",szBuf,sizeof(szBuf)-1);
	gloTimerPrice = atol(szBuf);
	tf.ReadString("COMMON","TimerTeamNo","60000",szBuf,sizeof(szBuf)-1);
	gloTimerTeamNO = atol(szBuf);
	tf.ReadString("COMMON","TimerOilType","60000",szBuf,sizeof(szBuf)-1);
	gloTimerOilType = atol(szBuf);
	tf.ReadString("COMMON","TimerUpdate","1800000",szBuf,sizeof(szBuf)-1);
	gloTimerUpdate = atol(szBuf);
	
	tf.Close();
	return(true);   
}
//����ر�ʱ����Ϣ
BOOL WINAPI HandlerRoutine( DWORD dwCtrlType )
{
	if( CTRL_CLOSE_EVENT == dwCtrlType )
	{
		WriteLog("close.log",__FILE__,__LINE__,-1,"ϵͳ���ر��ˣ�");
    }
	return true;
}
int main(int argc, char* argv[])
{
	bool bRet = ReadIni(".\\ini\\ksbu.ini");
	if (false == bRet)
	{
		printf("��ȡ��ʼ�������ļ�ksbu.ini���� \n");
		WriteLog("All_log",__FILE__,__LINE__,-1,"��ȡksbu.ini�ļ�����");	
		return -1;
	}
	char fileName[128] = {0};
	sprintf(fileName,"%s",".\\ini\\transOilType.ini");
	mp_TransOilType.clear();
	int iRet = 0;
	iRet = myReadFile2(fileName,'=',mp_TransOilType);
	if ( 0 != iRet)
	{	
		printf("��ȡ��Ʒ���������ļ�transOilType.ini���� \n");	
		WriteLog("All_log",__FILE__,__LINE__,-1,"��ȡtransOilType.ini�ļ�����");	
		return -1;
	}
	
	iRet = DatabaseIni();
	if ( 0 != iRet)
	{	
		printf("��ȡ���ݿ������ļ�Database.cfg���� \n");	
		WriteLog("All_log",__FILE__,__LINE__,-1,"��ȡDatabase.cfg�ļ�����");	
		return -1;
	}
	//������Ϣ
//	HANDLE hThreadTeamNo = CreateThread(NULL,0,ThreadTeamNo,0,0,&dwTimerTeamNo);
	//��Ʒ-�ͼ�
	HANDLE hThreadOilPrice = CreateThread(NULL,0,ThreadOilPrice,0,0,&dwTimerOilPrice);
	//���ͼ�¼
	HANDLE hThreadOilRecord = CreateThread(NULL,0,ThreadOilRecord,0,0,&dwTimerOilRecord);
	//ʵʱ���ͼ�¼
	HANDLE hThreadRealOilRecord = CreateThread(NULL,0,ThreadRealOilRecord,0,0,&dwTimerRealOilRecord);
	//��ǹ-��Ʒ
	HANDLE hThreadOilType = CreateThread(NULL,0,ThreadOilType,0,0,&dwTimerOilType);
	//�Զ�����
	HANDLE hThreadUpdate = CreateThread(NULL,0,ThreadUpdate,0,0,&dwTimerUpdate);
	//ץȡϵͳ�ر�ʱ����Ϣ 
//	SetConsoleCtrlHandler( HandlerRoutine, TRUE );
	while(1)
	{
		Sleep(gloTimerAlive);		
		isAlive();
	}
//     CWinApp app((LPCTSTR)argv[0]);  
//     app.InitApplication();  
//     AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);  

goto L_EXIT;

L_EXIT:
	{
// 		TerminateThread(hThreadTeamNo, -1);
// 		hThreadTeamNo = (HANDLE)-1;
		
		TerminateThread(hThreadOilPrice, -1);
		hThreadOilPrice = (HANDLE)-1;

		TerminateThread(hThreadOilType, -1);
		hThreadOilType = (HANDLE)-1;
		
		TerminateThread(hThreadOilRecord, -1);
		hThreadOilRecord = (HANDLE)-1;

		TerminateThread(hThreadUpdate, -1);
		hThreadUpdate = (HANDLE)-1;
	}
	printf("�����˳�!\n");

	return 0;
}