//////////////////////////////////// HttpClient.h  
#ifndef ENCODE_H  
#define ENCODE_H  

#include <afxinet.h>  
#include <string>  
using namespace std;  
class CURLCode 
{
public:
	CURLCode(){};
	virtual ~CURLCode(){};
	string Encode(const char* lpszData);
	string Decode(string inTmp);
	
private:
	inline BYTE toHex(const BYTE &x){ return x > 9 ? x + 55: x + 48; }
	inline BYTE fromHex(const BYTE &x){ return x > 64 ? x - 55 : x - 48; }
};

#endif // ENCODE_H