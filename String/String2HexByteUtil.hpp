#ifndef __STRINGUTIL__
#define __STRINGUTIL__
namespace StringUtil
{

/**
		*@brief 十六进制字符串转十六进制数
		*
		*@param [in]	pInHexString 需转换的字符串
		*@param [in]	nInLen 需要转换的字符串长度
		*@param [out]	pOut 十六进制数存放的缓存
		*@param [out]	nOutLen 转换后的十六进制数长度
		*@return TRUE 转换成功 FALSE 转换失败
		*
		*/
static bool HexStringToHexBytes(const char *pInHexString, int nInLen, unsigned char *pOut, int &nOutLen)
{
    if (pInHexString == NULL || pOut == NULL || (nInLen % 2) || nInLen / 2 > nOutLen)
    {
        return false;
    }

    int nIndex = 0;
    try
    {

        unsigned char nData = 0;
        unsigned char nOut = 0;
        char Temp[2];

        for (int i = 0; i < nInLen; i += 2)
        {

            Temp[0] = pInHexString[i];
            Temp[1] = pInHexString[i + 1];

            for (int j = 0; j < 2; ++j)
            {
                switch (Temp[j])
                {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    nData = Temp[j] - '0';
                    nOut += nData << ((1 - j) * 4);
                    break;

                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                    nData = Temp[j] - 'A' + 10;
                    nOut += nData << ((1 - j) * 4);
                    break;

                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                    nData = Temp[j] - 'a' + 10;
                    nOut += nData << ((1 - j) * 4);
                    break;

                default:
                    return false;
                }
            }
            pOut[nIndex++] = nOut;
            nOut = 0;
        }
    }
    catch (...)
    {

        return false;
    }
    nOutLen = nIndex;
    return true;
}

/**
		*@brief 十六进制数转十六进制字符串
		*
		*@param [in]  pInHexBytes 需要转换的十六进制数
		*@param [in]  nInLen 需要转换的十六进制数长度
		*@param [out] pOutHexString 转换后的字符串长度
		*@param [out] nOutLen 转换后的字符串长度
		*@return TRUE 转换成功 FALSE 转换失败
		*
		*/
static bool HexBytesToHexString(unsigned char *pInHexBytes, int nInLen, char *pOutHexString, int &nOutLen)
{
    if (pInHexBytes == NULL || pOutHexString == NULL || nOutLen < 2 * nInLen)
    {
        return false;
    }

    for (int i = 0; i < nInLen; i++)
    {
        sprintf(pOutHexString, "%02x", pInHexBytes[i]);
        pOutHexString += 2;
    }

    nOutLen = nInLen * 2;
    return true;
}

} // namespace StringUtil
#endif