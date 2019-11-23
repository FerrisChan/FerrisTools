#pragma once
#include "/libcurl/include/curl/curl.h"
#include <string>
#include <vector>


/*
	windows 编译需要加
	# include-dir:$(LIBCURL_PATH)/libcurl/include
	# lib-dir:$(LIBCURL_PATH)/lib
	# PreProcessor Definition:CURL_STATICLIB (静态库) 
*/
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "Wldap32.lib ")

class CurlUtil
{
private:
	struct MemoryStruct {
		char *memory;
		size_t size;
	};
	static size_t
		WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct *mem = (struct MemoryStruct *)userp;
		mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory == NULL) {
			/* out of memory! */
			printf("not enough memory (realloc returned NULL)\n");
			return 0;
		}
		memcpy(&(mem->memory[mem->size]), contents, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
		return realsize;
	}

public:
	/*
		CurlSend func 返回类型
	*/
	typedef struct _CurlRespMsg{
		std::string strResp; // 应答
		std::string errMsg; // 错误信息
		std::vector<std::string> cookie;
	}CurlRespMsg, *PCurlRespMsg;

	/*
		发送url请求
	*/
	static CurlRespMsg CurlSend(std::string targeturl)
	{
		return CurlSend(targeturl, "");
	}
	/*
		发送带简单cookie的url请求
	*/
	static CurlRespMsg CurlSend(std::string targeturl, std::string cookiestring, std::string var = "")
	{
		CURL *curl_handle;
		CURLcode res;

		struct MemoryStruct chunk;

		chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
		memset(chunk.memory, 0, 1);
		chunk.size = 0;    /* no data at this point */

		//curl_global_init(CURL_GLOBAL_ALL);

		/* init the curl session */
		curl_handle = curl_easy_init();
		curl_easy_setopt(curl_handle, CURLOPT_URL, targeturl.c_str());
				
		std::string strPar;
		strPar.append(var);
		if (strPar.length() > 0)
		{
			curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, strlen(strPar.c_str()));
			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, strPar.c_str());
		}

		/* send all data to this function  */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

		/* some servers don't like requests that are made without a user-agent
		field, so we provide one */
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		
		//add
		curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, 1);

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, 10);
		
		//  Content-Type change to  josn
		struct curl_slist *hs = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
		curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, hs);

		/*use to get cookie*/
		curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "");
		if (!cookiestring.empty())
			curl_easy_setopt(curl_handle, CURLOPT_COOKIE, cookiestring.c_str());
		/* get it! */
		res = curl_easy_perform(curl_handle);

		CurlRespMsg respMsg;
		if (res == CURLE_OK)
		{
			struct curl_slist *cookies = NULL;
			curl_easy_getinfo(curl_handle, CURLINFO_COOKIELIST, &cookies);       //获得cookie数据  

			while (cookies)
			{
				respMsg.cookie.push_back(cookies->data);
				cookies = cookies->next;
			}
		}
		//if (!var.length())
		if (chunk.memory)
			respMsg.strResp.assign(chunk.memory);

		respMsg.errMsg = curl_easy_strerror(res);
		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);

		if (res == CURLE_OK)
		{			
// 			char* str = chunk.memory;  // 回应的报文
// 			std::string strRespon = str;
			// printf("%s, ruturn [ %s ];" __FUNCTION__, strRespon.c_str());
		}
		free(chunk.memory);

		/* we're done with libcurl, so clean it up */
		//curl_global_cleanup();
		return respMsg;
	}
};
