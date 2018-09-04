#ifndef __MESSAGEBUILDER_H__
#define __MESSAGEBUILDER_H__

//#include "Configuration.h"

#define MESSAGE_BUFFER_SIZE     2048
#define REST_API_URL            "https://35.155.214.151:8443/CodeSigningRestService"
#define USERNAME "ktmuser"
#define PASSWORD "password"
#define KEY_PEM_FILE "api_created.key.pem"
#define CRT_PEM_FILE "api_created.crt.pem"
#define KEY_WRITE_FILE "rsa_private_key.pem"
#define AUTH_TYPE 1
#define LOG_LEVEL 1

int BuildAuthenticateMessage(char authenticateUrl[]);
int BuildKeyStoreMessage(char keyStoreUrl[], char sessionToken[]);
int BuildSignMessage(char signUrl[],char sessionToken[],char signData[],char keyName[],char schemeType[]);
//int BuildSignMessage(char signUrl[],char sessionToken[],char requestId[]);

#endif  //__MESSAGEBUILDER_H__
