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

/* ȥ�����ҿո�   */
char* mytrim(char *);

/* д��־�ļ�   */
int WriteLog(char *,char *,int ,int ,char *);

/* д��¼�ļ�   */
int myWriteFile(char *,char *,char *);

/* ����¼�ļ�   */
int myReadFile(char *,char *,char *);

/* ��������¼�ļ�   */
int myReadFile2(char *,char,map<string,string> &);

/* д������¼�ļ�   */
int myWriteFile2(char *FileName,char separator,map<string,string> &pMap);

/* ��������   */
double interROUND(double d,int n);

/* ����С��   */
double ROUND(double num, int n);

/* �����־����   */
void AddLogQueue(st_log& myst_log);

/* д��־   */
int WriteLogSQueue();

/* �����־����   */
void ClearLogSQueue();

#endif // MYFUNCTION_H