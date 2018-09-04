#ifndef __HTTPS_COMM_H__
#define __HTTPS_COMM_H__
#include <memory.h>
#include <curl/curl.h>

typedef struct tagHttpsRsp_t
{
	char*	m_RspBuff;
	size_t  m_RspBuffSize;
}HttpsRspBuffer;

 void HttpsRspBuffetInit(HttpsRspBuffer *Buffer);
 size_t SendMessage(const char* url, HttpsRspBuffer* httpsResponse,const char* pCertFile,const char *pKeyFile,int bIsAuthMsg);
 size_t ResponseCallback(void* content, size_t size, size_t numBytes, void* userPtr);

#endif
