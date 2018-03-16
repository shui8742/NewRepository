#include "SQLAPI.h"
#include <vector>
#ifdef HP_UX
#include <string>
#endif


//#include "public.h"



using namespace std;
//***********************************************************
//               stock index system
//
//          SHANGHAI FUDAN KINGSSTAR COMPUTER CO��,LTD
//                        Department of Finance
//
// File Name        : RiskSqlAPI.h
// modelNumber      : 196000
// modelName        : ���ݿ�SQLAPI��
// FunctionIntroduce: 
// Revision History : V2.0
// Author           : songfree     Remark:2006-5-10 13:09:22
// Revision History : V1.0
// Author           : ����Ȫ     Remark:
// ChangeHistory    :
// ---------  -----------  -----------------------
// 20-10-02 ����Ȫ    CREATE
// *********************************************************
#ifdef LIBKERNEL_EXPORTS
    #define KERNELLIB_API __declspec(dllexport)
#else
    #ifdef WIN_DLL
        #define KERNELLIB_API __declspec(dllimport)
    #else
        #define KERNELLIB_API 
    #endif
#endif

#ifndef CRISKSQLAPI_H
#define CRISKSQLAPI_H
#define SQLAPI_FIELD_MAXCOUNT   30     //select���ݿ����ֶ���
#define SQLAPI_FIELD_MAXCOLUMN  256     //�ֶε���󳤶�
class CSqlData;
class CSqlData2;
class KERNELLIB_API CRiskSqlAPI
{
public:                             //Construction
    CRiskSqlAPI(const char *lpDBConfigFile,const char *PasswdKey);
    CRiskSqlAPI(const char *lpDBConfigFile);
    CRiskSqlAPI();
    ~CRiskSqlAPI();

    int          DB_ErrorCode;
    //modified by jhz on 20070917--begin--
    //char         DB_ErrorInfo[300];
    char         DB_ErrorInfo[1024];
    //Added by jhz on 20070917--end--
    SACommand    *cmd;                 //Sql������      
    SAConnection *con;                 //�������ݿ���     
	char         DB_Type[60];          //���ݿ����� 0-oracel 1-sybase 2-sqlserver 3-infomix 4-db2 5-odbc 6-mysql 
private:                             //Atrributes
    char         DBConfigFile[100];    //�����ļ���
    char         DB_User[60];          //���ݿ��û���
    char         DB_Passwd[60];        //�û�����
    char         DB_Name[80];          //���ݿ���
    char         DB_Section[20];        //���ݿ����õĽڵ���
    char         EncryptKey[9];        //���ܵ�key
    
    
    
//    int GetConfString(const char *lpAppName,const char *lpKeyName,char *lpResultString); //ȡ������Ϣ
    bool ConFlag ;                  //�Ƿ�ʹ���ⲿ���� true �ǣ�flase��
public:     //Operations
	bool isCount(char *sSql);
	bool isCount(char *sSql,int &b);
    /**************************************************
    *  LoadResultSqlִ��sql��䣬����CSqlData������
    *  �˷����ڴ�����������¼������5000��ʱ������ʹ��
    **************************************************/
    CSqlData LoadResultSql(const char *lpSqlBuf);
    CSqlData2 LoadResultSql2(const char *lpSqlBuf,int preFetchRows=-1);
    void SetDBConfig(char *ConfigPath,char *PasswdKey);    //���������ļ�(��·��)������Ľ���Key
    void SetDBConfig(const char *ConfigPath,const char *PasswdKey,const char *DbSection);  //���������ļ�(��·��)������Ľ���Key�������ļ��Ľڵ���
    SAConnection *CreateDBConnect();    //������һ������
    int  OpenDB();                      //�������ݿ�
    /* HEAP 20100303: ���ݿ��Զ��������� */
    int  ReOpenDB();			 //�����������ݿ�
    int  SetDBConnection(SAConnection *connection);   //�����ⲿ����
    int  CloseCmd();                    //�Ͽ�cmd 
    int  Execute(const char *lpSqlBuf); //ִ��SQL����
    int GetDBErrCode();                 //ȡ�������
    char *GetDBErrInfo();              //ȡ������Ϣ
    double round(double num, int n);   //��������
/*����ʹ������ķ���*/
    int   GetBinary(const char *lpFieldName, char *lpFieldDate);  //��ȡ������������
    int   GetBinary(int no, char *lpFieldDate);  //��ȡ������������
    
    int   GetString(const char *lpFieldName, char *lpFieldDate);  //��ȡ�ַ�������
    int   GetString(int no, char *lpFieldDate);                   //��ȡ�ַ�������
    int  GetDouble(const char *sField,double &lpFieldDate); //��ȡ˫������
    int  GetDouble(int no,double &lpFieldDate);             //��ȡ˫������
    int  GetLong(const char *sField,long &lpFieldDate);     //��ȡ������
    int  GetLong(int no,long &lpFieldDate);                 //��ȡ������
    int  GetShort(const char *sField,short &lpFieldDate);    //��ȡ��������
    int   FetchNext();                   //�ƶ����ұ�
    void  Commit();                      //�����ύ
    void  RollBack();                    //����ع�
    int  GetRows();                      //Update��¼��Ŀ

    short GetShort(const char *sField);  //��ȡ��������
    long  GetLong(const char *sField);   //��ȡ������
    long  GetLong(int no);           //��ȡ������
    double  GetDouble(const char *sField); //��ȡ˫������
    double  GetDouble(int no);         //��ȡ˫������
    
	int  ExecuteHp(); //ֱ��ִ��SQL����
};
/*****************************
*       ���ݼ�¼��
*   ÿ����¼��Ϊһ���ౣ��
******************************/
class KERNELLIB_API CRiskSqlAPIData
{
private: 
    char SqlRecordBuffer[SQLAPI_FIELD_MAXCOUNT][SQLAPI_FIELD_MAXCOLUMN];  //��¼����
    int  SqlFieldCount;                                                   //�ֶ���
public: 
    int Clear();                                                          //����ֶ�
    int GetCount();                                                       //ȡ�ֶ���
    int SetAt(char *buffer,int nIndex);              //updateָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,char *FieldData);           //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,double &FieldData);          //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,long &FieldData);            //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,int &FieldData);             //ȡָ���ֶ� ��0��ʼ

    char *GetAt(int nIndex);            //ȡָ���ֶε�charֵ ��0��ʼ
    double GetAtD(int nIndex);          //ȡָ���ֶ� ��0��ʼ
    long GetAtL(int nIndex);            //ȡָ���ֶ� ��0��ʼ
    int GetAtI(int nIndex);             //ȡָ���ֶ� ��0��ʼ
	string GetAtS(int nIndex);             //ȡָ���ֶ� ��0��ʼ

    int Add(char *buffer);                           //��һ���ֶε����
    ~CRiskSqlAPIData();
    CRiskSqlAPIData();
};
/*****************************
*       ���ݼ�¼��
*   ÿ����¼��Ϊһ���ౣ��
******************************/
class KERNELLIB_API CSqlAPIData
{
private: 
    //char SqlRecordBuffer[SQLAPI_FIELD_MAXCOUNT][SQLAPI_FIELD_MAXCOLUMN];  //��¼����
    vector <string> SqlRecordBuffer;
    int  SqlFieldCount;                                                   //�ֶ���
public: 
    int Clear();                                                          //����ֶ�
    int GetCount();                                                       //ȡ�ֶ���
    int SetAt(char *buffer,int nIndex);              //updateָ���ֶ� ��0��ʼ
	int SetAt(string buffer,int nIndex);              //updateָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,char *FieldData);           //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,double &FieldData);          //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,long &FieldData);            //ȡָ���ֶ� ��0��ʼ
    int GetAt(int nIndex,int &FieldData);             //ȡָ���ֶ� ��0��ʼ
    
	char *GetAt(int nIndex);            //ȡָ���ֶε�charֵ ��0��ʼ
    double GetAtD(int nIndex);          //ȡָ���ֶ� ��0��ʼ
    long GetAtL(int nIndex);            //ȡָ���ֶ� ��0��ʼ
    int GetAtI(int nIndex);             //ȡָ���ֶ� ��0��ʼ
	string GetAtS(int nIndex) ;         //ȡָ���ֶ� ��0��ʼ

    int Add(char *buffer);                           //��һ���ֶε����
	int Add(string buffer);                           //��һ���ֶε����
    ~CSqlAPIData();
    CSqlAPIData();
};
/********************************
*     ���ݼ�����
*  �ṩ������������ȡ����
*********************************/
class KERNELLIB_API CSqlData 
{
private: 
    vector <CRiskSqlAPIData> SqlData;  //���ݼ���
    int  SqlCount;                     //��¼��
public: 
    int GetSqlCount();                 //ȡ��¼��
    int clear();                       //��ռ�¼
    int GetAt(int Record,int FieldNo,char *FieldData);     //ȡĳһ��¼��ĳһ�ֶε�charֵ
    int GetAt(int Record,int FieldNo,double &FieldData);    //ȡĳһ��¼��ĳһ�ֶε�doubleֵ
    int GetAt(int Record,int FieldNo,long &FieldData);      //ȡĳһ��¼��ĳһ�ֶε�longֵ
    int GetAt(int Record,int FieldNo,int &FieldData);      //ȡĳһ��¼��ĳһ�ֶε�intֵ
	
	char * GetAt(int Record,int FieldNo);     //ȡĳһ��¼��ĳһ�ֶε�charֵ
    double GetAtD(int Record,int FieldNo);    //ȡĳһ��¼��ĳһ�ֶε�doubleֵ
    long   GetAtL(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�longֵ
    int    GetAtI(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�intֵ
	string GetAtS(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�stringֵ
public: 
    int          DB_ErrorCode;                            //�������
    char         DB_ErrorInfo[300];                       //������Ϣ
    void setSqlCount(int count);                          //���ü�¼��
    int put(CRiskSqlAPIData sqldata);                     //����һ����¼
    CRiskSqlAPIData GetAt(int index);                     //ȡһ����¼
    ~CSqlData();
    CSqlData();
};
/********************************
*     ���ݼ�����
*  �ṩ������������ȡ����
*********************************/
class KERNELLIB_API CSqlData2 
{
private: 
    vector <CSqlAPIData> SqlData;  //���ݼ���
    int  SqlCount;                     //��¼��
public: 
    int GetSqlCount();                 //ȡ��¼��
    int clear();                       //��ռ�¼
    int GetAt(int Record,int FieldNo,char *FieldData);     //ȡĳһ��¼��ĳһ�ֶε�charֵ
    int GetAt(int Record,int FieldNo,double &FieldData);    //ȡĳһ��¼��ĳһ�ֶε�doubleֵ
    int GetAt(int Record,int FieldNo,long &FieldData);      //ȡĳһ��¼��ĳһ�ֶε�longֵ
    int GetAt(int Record,int FieldNo,int &FieldData);      //ȡĳһ��¼��ĳһ�ֶε�intֵ

	char * GetAt(int Record,int FieldNo);     //ȡĳһ��¼��ĳһ�ֶε�charֵ
	double GetAtD(int Record,int FieldNo);    //ȡĳһ��¼��ĳһ�ֶε�doubleֵ
    long   GetAtL(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�longֵ
    int    GetAtI(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�intֵ
	string GetAtS(int Record,int FieldNo);      //ȡĳһ��¼��ĳһ�ֶε�stringֵ

public: 
    int          DB_ErrorCode;                            //�������
    char         DB_ErrorInfo[300];                       //������Ϣ
    void setSqlCount(int count);                          //���ü�¼��
    int put(CSqlAPIData sqldata);                     //����һ����¼
    CSqlAPIData GetAt(int index);                     //ȡһ����¼
    ~CSqlData2();
    CSqlData2();
};
#endif
