#include "StdAfx.h"  
#include "URLCode.h"

string CURLCode::Encode(const char* lpszData)
{
	string strResult = "";
	
	unsigned char* pInTmp = (unsigned char*)lpszData;
	// do encoding
	while (*pInTmp)
	{
		if(isalnum(*pInTmp))
			strResult += *pInTmp;
		else
			if(isspace(*pInTmp))
				strResult += '+';
			else
			{
				strResult += '%';
				strResult += toHex(*pInTmp>>4);
				strResult += toHex(*pInTmp%16);
			}
			pInTmp++;
	}
	return strResult;
}

string CURLCode::Decode(string inTmp)
{
	string strResult = "";
	int inlen = inTmp.length();
	for(int i=0;i<inlen;i++)
	{
		if(inTmp.at(i)=='%')
		{
			i++;
			char c = fromHex(inTmp.at(i++));
			c = c << 4;
			c += fromHex(inTmp.at(i));
			strResult += c;
		}
		else if(inTmp.at(i)=='+')
			strResult += ' ';
		else
			strResult += inTmp.at(i);
	}
	return strResult;
}