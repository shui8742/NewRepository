//////////////////////////////////// myfunction.h  
#ifndef MYFUNCTION_H  
#define MYFUNCTION_H
#include "myfunction.h" 
#include "profile.h"
#include <string>  
#include <afx.h> 
#include <map>
#include <QUEUE>
#include "mutex.h"
#include "direct.h"
#include "io.h"
#include "afxinet.h"
#include "math.h"

using namespace std;

#define FILEPATH_LENGTH 512

typedef struct{
	char logName[32]; 
	char fileName[128]; 
	int line;  
	int code;
	char logInfo[8192];
}st_log;

typedef deque<st_log> deqStLog;	

typedef struct
{
	deqStLog m_deqStLog;
	CMUTEX m_mutex;
}TdeqLogS;

/* 去除左右空格   */
char* mytrim(char *);

/* 写日志文件   */
int WriteLog(char *,char *,int ,int ,char *);

/* 写记录文件   */
int myWriteFile(char *,char *,char *);

/* 读记录文件   */
int myReadFile(char *,char *,char *);

/* 读整个记录文件   */
int myReadFile2(char *,char,map<string,string> &);

/* 写整个记录文件   */
int myWriteFile2(char *FileName,char separator,map<string,string> &pMap);

/* 四舍五入   */
double interROUND(double d,int n);

/* 保留小数   */
double ROUND(double num, int n);

/* 添加日志队列   */
void AddLogQueue(st_log& myst_log);

/* 写日志   */
int WriteLogSQueue();

/* 清空日志队列   */
void ClearLogSQueue();

#endif // MYFUNCTION_H