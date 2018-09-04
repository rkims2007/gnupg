#include "MessageBuilder.h"
#include <stdlib.h>
#include <stdio.h>

const char* authenticateFormate = "%s/authenticate?api_version=%s&client=%s&client_version=%s&username=%s&password=%s";
const char* keyStoreFormate     = "%s/keystore?api_version=%s&client=%s&client_version=%s&session_token=%s";
const char* signFormateSignData = "%s/sign?api_version=%s&client=%s&client_version=%s&session_token=%s&data_to_sign=%s&key_name=%s&signature_scheme=%s";
const char* signFormateReqId    = "%s/sign?api_version=%s&client=%s&client_version=%s&session_token=%s&request_id=%s";

const char *api_version  = "1.0";
const char *CLIENT="Client1.0";
const char* CLIENT_VERSION="1.0";
	


int BuildAuthenticateMessage(char authenticateUrl[])
{
    
    //if((Singleton<Configuration>::GetInstance()->GetAuthType()==AUTH_TYPE_PASSWORD) || (Singleton<Configuration>::GetInstance()->GetAuthType()==AUTH_TYPE_MUTUAL_TLS_PASSWORD))
    {
	   const char* authenticateFormate = "%s/authenticate?api_version=%s&client=%s&client_version=%s&username=%s&password=%s";
    	snprintf(authenticateUrl, MESSAGE_BUFFER_SIZE, authenticateFormate, REST_API_URL, api_version, CLIENT, CLIENT_VERSION,USERNAME, PASSWORD);
     }
   //  else
     {
	//const char* authenticateFormate = "%s/authenticate?api_version=%s&client=%s&client_version=%s";
	//printf("Running in Only pass \r\n");
	//snprintf(authenticateUrl, MESSAGE_BUFFER_SIZE, authenticateFormate, REST_API_URL, apiVersion.c_str(), client.c_str(), 	clientVersion.c_str());
     }	

    return 0;
}

int BuildKeyStoreMessage(char keyStoreUrl[], char sessionToken[])
{
		printf("session Token %s\r\n",sessionToken);
    
	snprintf(keyStoreUrl, MESSAGE_BUFFER_SIZE, keyStoreFormate, REST_API_URL, api_version, CLIENT,CLIENT_VERSION, sessionToken);
	printf("StoreUrl Token %s\r\n",keyStoreUrl);

    return 0;
}

int BuildSignMessage(char signUrl[],char sessionToken[],char signData[],char keyName[],char schemeType[])
{
    
    snprintf(signUrl, MESSAGE_BUFFER_SIZE, signFormateSignData, REST_API_URL,  api_version, CLIENT,CLIENT_VERSION, sessionToken,signData,keyName,schemeType);

    return 0;
}
/*
int BuildSignMessage(char signUrl[],char sessionToken[],char requestId[])
{
    
    snprintf(signUrl, MESSAGE_BUFFER_SIZE, signFormateReqId, REST_API_URL, api_version, CLIENT,CLIENT_VERSION, sessionToken,requestId);

    return 0;
}*/
