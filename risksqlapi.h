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
//          SHANGHAI FUDAN KINGSSTAR COMPUTER CO。,LTD
//                        Department of Finance
//
// File Name        : RiskSqlAPI.h
// modelNumber      : 196000
// modelName        : 数据库SQLAPI类
// FunctionIntroduce: 
// Revision History : V2.0
// Author           : songfree     Remark:2006-5-10 13:09:22
// Revision History : V1.0
// Author           : 张玉泉     Remark:
// ChangeHistory    :
// ---------  -----------  -----------------------
// 20-10-02 张玉泉    CREATE
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
#define SQLAPI_FIELD_MAXCOUNT   30     //select数据库表的字段数
#define SQLAPI_FIELD_MAXCOLUMN  256     //字段的最大长度
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
    SACommand    *cmd;                 //Sql命令句炳      
    SAConnection *con;                 //连接数据库句柄     
	char         DB_Type[60];          //数据库类型 0-oracel 1-sybase 2-sqlserver 3-infomix 4-db2 5-odbc 6-mysql 
private:                             //Atrributes
    char         DBConfigFile[100];    //配置文件名
    char         DB_User[60];          //数据库用户名
    char         DB_Passwd[60];        //用户口令
    char         DB_Name[80];          //数据库名
    char         DB_Section[20];        //数据库配置的节点名
    char         EncryptKey[9];        //加密的key
    
    
    
//    int GetConfString(const char *lpAppName,const char *lpKeyName,char *lpResultString); //取配置信息
    bool ConFlag ;                  //是否使用外部连接 true 是，flase否
public:     //Operations
	bool isCount(char *sSql);
	bool isCount(char *sSql,int &b);
    /**************************************************
    *  LoadResultSql执行sql语句，返回CSqlData数据类
    *  此方法在大数据量，记录数超过5000条时不建议使用
    **************************************************/
    CSqlData LoadResultSql(const char *lpSqlBuf);
    CSqlData2 LoadResultSql2(const char *lpSqlBuf,int preFetchRows=-1);
    void SetDBConfig(char *ConfigPath,char *PasswdKey);    //设置配置文件(带路径)及密码的解密Key
    void SetDBConfig(const char *ConfigPath,const char *PasswdKey,const char *DbSection);  //设置配置文件(带路径)及密码的解密Key及配置文件的节点名
    SAConnection *CreateDBConnect();    //仅创建一个连接
    int  OpenDB();                      //连接数据库
    /* HEAP 20100303: 数据库自动重连函数 */
    int  ReOpenDB();			 //重新连接数据库
    int  SetDBConnection(SAConnection *connection);   //设置外部连接
    int  CloseCmd();                    //断开cmd 
    int  Execute(const char *lpSqlBuf); //执行SQL命令
    int GetDBErrCode();                 //取错误代码
    char *GetDBErrInfo();              //取错误信息
    double round(double num, int n);   //四舍五入
/*建议使用下面的方法*/
    int   GetBinary(const char *lpFieldName, char *lpFieldDate);  //获取二进制类型域
    int   GetBinary(int no, char *lpFieldDate);  //获取二进制类型域
    
    int   GetString(const char *lpFieldName, char *lpFieldDate);  //获取字符类型域
    int   GetString(int no, char *lpFieldDate);                   //获取字符类型域
    int  GetDouble(const char *sField,double &lpFieldDate); //获取双精度域
    int  GetDouble(int no,double &lpFieldDate);             //获取双精度域
    int  GetLong(const char *sField,long &lpFieldDate);     //获取整型域
    int  GetLong(int no,long &lpFieldDate);                 //获取整型域
    int  GetShort(const char *sField,short &lpFieldDate);    //获取短整型域
    int   FetchNext();                   //移动游右标
    void  Commit();                      //事务提交
    void  RollBack();                    //事务回滚
    int  GetRows();                      //Update记录数目

    short GetShort(const char *sField);  //获取短整型域
    long  GetLong(const char *sField);   //获取整型域
    long  GetLong(int no);           //获取整型域
    double  GetDouble(const char *sField); //获取双精度域
    double  GetDouble(int no);         //获取双精度域
    
	int  ExecuteHp(); //直接执行SQL命令
};
/*****************************
*       数据记录类
*   每条记录作为一个类保存
******************************/
class KERNELLIB_API CRiskSqlAPIData
{
private: 
    char SqlRecordBuffer[SQLAPI_FIELD_MAXCOUNT][SQLAPI_FIELD_MAXCOLUMN];  //记录数组
    int  SqlFieldCount;                                                   //字段数
public: 
    int Clear();                                                          //清空字段
    int GetCount();                                                       //取字段数
    int SetAt(char *buffer,int nIndex);              //update指定字段 从0开始
    int GetAt(int nIndex,char *FieldData);           //取指定字段 从0开始
    int GetAt(int nIndex,double &FieldData);          //取指定字段 从0开始
    int GetAt(int nIndex,long &FieldData);            //取指定字段 从0开始
    int GetAt(int nIndex,int &FieldData);             //取指定字段 从0开始

    char *GetAt(int nIndex);            //取指定字段的char值 从0开始
    double GetAtD(int nIndex);          //取指定字段 从0开始
    long GetAtL(int nIndex);            //取指定字段 从0开始
    int GetAtI(int nIndex);             //取指定字段 从0开始
	string GetAtS(int nIndex);             //取指定字段 从0开始

    int Add(char *buffer);                           //加一个字段到最后
    ~CRiskSqlAPIData();
    CRiskSqlAPIData();
};
/*****************************
*       数据记录类
*   每条记录作为一个类保存
******************************/
class KERNELLIB_API CSqlAPIData
{
private: 
    //char SqlRecordBuffer[SQLAPI_FIELD_MAXCOUNT][SQLAPI_FIELD_MAXCOLUMN];  //记录数组
    vector <string> SqlRecordBuffer;
    int  SqlFieldCount;                                                   //字段数
public: 
    int Clear();                                                          //清空字段
    int GetCount();                                                       //取字段数
    int SetAt(char *buffer,int nIndex);              //update指定字段 从0开始
	int SetAt(string buffer,int nIndex);              //update指定字段 从0开始
    int GetAt(int nIndex,char *FieldData);           //取指定字段 从0开始
    int GetAt(int nIndex,double &FieldData);          //取指定字段 从0开始
    int GetAt(int nIndex,long &FieldData);            //取指定字段 从0开始
    int GetAt(int nIndex,int &FieldData);             //取指定字段 从0开始
    
	char *GetAt(int nIndex);            //取指定字段的char值 从0开始
    double GetAtD(int nIndex);          //取指定字段 从0开始
    long GetAtL(int nIndex);            //取指定字段 从0开始
    int GetAtI(int nIndex);             //取指定字段 从0开始
	string GetAtS(int nIndex) ;         //取指定字段 从0开始

    int Add(char *buffer);                           //加一个字段到最后
	int Add(string buffer);                           //加一个字段到最后
    ~CSqlAPIData();
    CSqlAPIData();
};
/********************************
*     数据集合类
*  提供方法给调用者取数据
*********************************/
class KERNELLIB_API CSqlData 
{
private: 
    vector <CRiskSqlAPIData> SqlData;  //数据集合
    int  SqlCount;                     //记录数
public: 
    int GetSqlCount();                 //取记录数
    int clear();                       //清空记录
    int GetAt(int Record,int FieldNo,char *FieldData);     //取某一记录的某一字段的char值
    int GetAt(int Record,int FieldNo,double &FieldData);    //取某一记录的某一字段的double值
    int GetAt(int Record,int FieldNo,long &FieldData);      //取某一记录的某一字段的long值
    int GetAt(int Record,int FieldNo,int &FieldData);      //取某一记录的某一字段的int值
	
	char * GetAt(int Record,int FieldNo);     //取某一记录的某一字段的char值
    double GetAtD(int Record,int FieldNo);    //取某一记录的某一字段的double值
    long   GetAtL(int Record,int FieldNo);      //取某一记录的某一字段的long值
    int    GetAtI(int Record,int FieldNo);      //取某一记录的某一字段的int值
	string GetAtS(int Record,int FieldNo);      //取某一记录的某一字段的string值
public: 
    int          DB_ErrorCode;                            //出错代码
    char         DB_ErrorInfo[300];                       //出错信息
    void setSqlCount(int count);                          //设置记录数
    int put(CRiskSqlAPIData sqldata);                     //放入一条记录
    CRiskSqlAPIData GetAt(int index);                     //取一条记录
    ~CSqlData();
    CSqlData();
};
/********************************
*     数据集合类
*  提供方法给调用者取数据
*********************************/
class KERNELLIB_API CSqlData2 
{
private: 
    vector <CSqlAPIData> SqlData;  //数据集合
    int  SqlCount;                     //记录数
public: 
    int GetSqlCount();                 //取记录数
    int clear();                       //清空记录
    int GetAt(int Record,int FieldNo,char *FieldData);     //取某一记录的某一字段的char值
    int GetAt(int Record,int FieldNo,double &FieldData);    //取某一记录的某一字段的double值
    int GetAt(int Record,int FieldNo,long &FieldData);      //取某一记录的某一字段的long值
    int GetAt(int Record,int FieldNo,int &FieldData);      //取某一记录的某一字段的int值

	char * GetAt(int Record,int FieldNo);     //取某一记录的某一字段的char值
	double GetAtD(int Record,int FieldNo);    //取某一记录的某一字段的double值
    long   GetAtL(int Record,int FieldNo);      //取某一记录的某一字段的long值
    int    GetAtI(int Record,int FieldNo);      //取某一记录的某一字段的int值
	string GetAtS(int Record,int FieldNo);      //取某一记录的某一字段的string值

public: 
    int          DB_ErrorCode;                            //出错代码
    char         DB_ErrorInfo[300];                       //出错信息
    void setSqlCount(int count);                          //设置记录数
    int put(CSqlAPIData sqldata);                     //放入一条记录
    CSqlAPIData GetAt(int index);                     //取一条记录
    ~CSqlData2();
    CSqlData2();
};
#endif
