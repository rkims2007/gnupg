#include <stdlib.h>
#include "https_comm.h"

void HttpsRspBuffetInit(HttpsRspBuffer *Buffer)
{
	Buffer->m_RspBuff = NULL;
	Buffer->m_RspBuffSize = 0;
	
}
size_t ResponseCallback(void* content, size_t size, size_t numBytes, void* userPtr)
{  
    //DEBUG_LOG("In ...");	
    size_t realSize = size * numBytes;
    HttpsRspBuffer* httpsResponse = (HttpsRspBuffer*)userPtr;
    if(httpsResponse==NULL)
     {
	printf("User data is NULL\r\n");
	return 0;
     }	
     else
     {
//	printf("ResponseCallback pointer user data %p\r\n",httpsResponse);
     }	

    httpsResponse->m_RspBuff = (char*)realloc(httpsResponse->m_RspBuff, httpsResponse->m_RspBuffSize + realSize + 1);
    if(httpsResponse->m_RspBuff == NULL)
    {
		//ERROR_LOG("Response buffer is NULL");
        //Error Not Enough Memory
        return 0;
    }

	memcpy(&(httpsResponse->m_RspBuff[httpsResponse->m_RspBuffSize]), content, realSize);
    httpsResponse->m_RspBuffSize +=  realSize;
    httpsResponse->m_RspBuff[httpsResponse->m_RspBuffSize] = '\0'; 
	printf("%s\r\n",httpsResponse->m_RspBuff);
    
    //DEBUG_LOG("Exit ...");	
    return realSize; 
}
size_t SendMessage(const char* url, HttpsRspBuffer* httpsResponse,const char* pCertFile,const char *pKeyFile,int bIsAuthMsg)
{
	
	CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT); 
    CURL* curl = curl_easy_init();
    if(!curl)
    {
        curl_global_cleanup();
//		ERROR_LOG("Failed to Initialize Curl");
        return 0;
    }
	 struct curl_slist* header = NULL;
	 curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	/* Specify the GET URL */
	curl_easy_setopt(curl, CURLOPT_URL, url);
	 /* Set Client Certificate Type and File */
	curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);   //TODO Support Certificate
	curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
	curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyFile);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	 /* Send Response to this function */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)httpsResponse);

    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
//		ERROR_LOG("Easy Perform Failed");
        return 0;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
  //  DEBUG_LOG("Exit ...");	 
	return 1;
}
