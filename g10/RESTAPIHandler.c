#include "RESTAPIHandler.h"
#include "MessageBuilder.h"
#include "https_comm.h"
#include "msg_parser.h"

AuthResponse     *g_authData=NULL;
KeyStoreResponse *g_keystoreData=NULL;
SignResponse     *g_signData=NULL;
char             *g_CertFile=NULL;
char             *g_KeyFile=NULL;


int ProcessAuthenticate()
{
	HttpsRspBuffer httpsResponse;
	HttpsRspBuffetInit(&httpsResponse);
	printf("ProcessAuthenticate %p\r\n",&httpsResponse);
   // DEBUG_LOG("ProcessAuthenticate\n");	

    //Prepare Authenticate Message
    char messageUrl[MESSAGE_BUFFER_SIZE] = "";
    //MessageBuilder objMessageBuilder;
    if(BuildAuthenticateMessage(messageUrl))
    {
        //Failed to create Authenticate Message
        return -1;
    }

    //DEBUG_LOG(" msg url %s\n",messageUrl);
    //Send Message
    SendMessage(messageUrl, &httpsResponse,CRT_PEM_FILE,KEY_PEM_FILE,1);
    //DEBUG_LOG(" response is %s\n",httpsResponse.m_ResponseBuffer);
    if(httpsResponse.m_RspBuff==NULL)
	{
		printf("No user data\r\n");
	}
    //Parse Response
	RpmString_s  *in = InitStringWithBufferSize(httpsResponse.m_RspBuff, httpsResponse.m_RspBuffSize);	
    //MsgParser parser(in);
	if(g_authData)
	{
		FreeAuthenticateResponse(g_authData);
		g_authData = NULL;
	}
	//printf("%s\r\n",in->m_String);
    if(MsgParseAuthResponse(in,&g_authData))
    {
		FreeString(in);
		in=NULL;
       // ERROR_LOG(" failed to parse\n");
	    return -1;
    }
	if(!g_authData)
	{
		FreeString(in);
		in=NULL;
		return -1;		
	}
    //DEBUG_LOG(" session token value is %s\n",m_authData.m_sessionToken.data());	
    return 0;
}

int ProcessKeyStore()
{
	HttpsRspBuffer httpsResponse;
	HttpsRspBuffetInit(&httpsResponse);

    //Prepare KeyStore Message
    char messageUrl[MESSAGE_BUFFER_SIZE] = "";
	if(BuildKeyStoreMessage(messageUrl, (char*)GetPtr(g_authData->m_sessionToken)))
    {
        
        //Failed to create Authenticate Message
        return -1;
    }

    //Send Message
    SendMessage(messageUrl, &httpsResponse,CRT_PEM_FILE,KEY_PEM_FILE,0);
	//printf("URL KEYSTORE %s\r\n",messageUrl);
    //DEBUG_LOG(" key store  response is %s\n",httpsResponse.m_ResponseBuffer);
    		
    	
    //Parse Response
	RpmString_s *in= InitStringWithBufferSize(httpsResponse.m_RspBuff , httpsResponse.m_RspBuffSize);
	if(g_keystoreData)
	{
		FreeKeyStoreData(g_keystoreData);
		g_keystoreData = NULL;
	}
	if(MsgParseKeyStoreResponse(in,&g_keystoreData))
    {
		if(in)
		{
			FreeString(in);
			in=NULL;

		}
        return -1;
    }
    //DEBUG_LOG(" API GET Success\n");
	if(g_keystoreData==NULL)
	{
		if(in)
		{
			FreeString(in);
			in=NULL;

		}
		return -1;
	}
    return 0;
}

int ProcessSignAPI(char dataSign[],char keyName[],char signature_scheme[])
{
    HttpsRspBuffer httpsResponse;
	HttpsRspBuffetInit(&httpsResponse);

    //Prepare KeyStore Message
    char messageUrl[MESSAGE_BUFFER_SIZE] = "";
	if(BuildSignMessage(messageUrl, (char*)(char*)GetPtr(g_authData->m_sessionToken),dataSign,keyName,signature_scheme))
    {
        //DEBUG_LOG(" failed to send  data\n");
        //Failed to create Authenticate Message
        return -1;
    }

    /////Send Message
	//DEBUG_LOG(" sign url %s\n",messageUrl);
	SendMessage(messageUrl, &httpsResponse,CRT_PEM_FILE,KEY_PEM_FILE,0);
    	
	//DEBUG_LOG(" Sign API response is %s\n",httpsResponse.m_ResponseBuffer);

	

	//Parse Response
	RpmString_s* in= InitStringWithBufferSize(httpsResponse.m_RspBuff, httpsResponse.m_RspBuffSize);
	
	if(g_signData)
	{
		FreeSignResposeData(g_signData);
		g_signData=NULL;
	}
	if(MsgParseSignResponse(in,&g_signData))
	{
		if(in)
		{
			FreeString(in);
			in=NULL;

		}
        return -1;
    }
	if(g_signData==NULL)
	{
		if(in)
		{
			FreeString(in);
			in=NULL;

		}
		return -1;
	}
    //DEBUG_LOG(" SIGN API GET Success RequestID = %s\n", m_signData.m_RequestId.c_str());
	return 0;
}
#if 0
bool ProcessSignAPI(/*char requestId[]*/)
{
    HTTPSResponse httpsResponse;

    //Prepare Sign Message
    DEBUG_LOG("ProcessSignAPI\n");
    char messageUrl[MESSAGE_BUFFER_SIZE] = "";
    char requestId[MESSAGE_BUFFER_SIZE] = "";
    snprintf(requestId, MESSAGE_BUFFER_SIZE-1, "%s", m_signData.m_RequestId.c_str());
    MessageBuilder objMessageBuilder;
    if(!objMessageBuilder.BuildSignMessage(messageUrl, (char*)m_authData.m_sessionToken.data(), requestId))
    {
        DEBUG_LOG("failed to send  data\n");
        //Failed to create Authenticate Message
        return false;
    }

    /////Send Message
	DEBUG_LOG("sign url %s\n",messageUrl);
	SendMessage(messageUrl, httpsResponse,m_CertFile,m_KeyFile);
    	
	DEBUG_LOG("Sign API response is %s\n",httpsResponse.m_ResponseBuffer);

	//Parse Response
	const std::string in(httpsResponse.m_ResponseBuffer, httpsResponse.m_ResponseSize);
	MsgParser parser(in);

	if(!parser.ParseSignResponse(m_signData))
	{
	    return false;
    }
    
    DEBUG_LOG("SIGN API GET Success RequestID = %s\n", m_signData.m_RequestId.c_str());
	return true;
}
#endif
