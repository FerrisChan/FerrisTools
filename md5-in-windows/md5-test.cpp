#include "md5/md5Hash.h"
#include <string.h>
#include "md5/stdinc.h"
#ifndef WIN_32
#define _snprintf_s snprintf 
#endif

BOOL CalculateString32bitMD5(const char* szSrcContent, char(&pMD5Buffer)[33]){
	memset(pMD5Buffer, 0, sizeof(szSrcContent));
	md5Hash objHash;
	objHash.add(reinterpret_cast<const BYTE*>(szSrcContent), strlen(szSrcContent));
	const BYTE* md5 = objHash.getHash();
	char szMD5[33];
	_snprintf_s(pMD5Buffer, sizeof(pMD5Buffer) - 1, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		md5[0], md5[1], md5[2], md5[3], md5[4], md5[5], md5[6], md5[7], md5[8], md5[9], md5[10], md5[11], md5[12], md5[13], md5[14], md5[15]);
	szMD5[32] = '\0';
	return TRUE;
}

const BYTE* CalculateMD5(const char* szSrcContent){
	md5Hash objHash;
	objHash.add(reinterpret_cast<const BYTE*>(szSrcContent), strlen(szSrcContent));
	const BYTE* md5 = objHash.getHash();
	return md5;
}

int main(){
	std::string  strTest = "test md5";
    char szTestMd5[33];
    CalculateString32bitMD5(strTest.c_str(), szTestMd5);
	const BYTE*  vecByteMdt5 = CalculateMD5(strTest.c_str());
    return 0;
}