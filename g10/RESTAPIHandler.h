#ifndef __RESTAPIHANDLER_H__
#define __RESTAPIHANDLER_H__

#include <stdio.h>
#include "msg_parser.h"
#define CERT_PATH_SIZE   10000

int ProcessAuthenticate();
int ProcessKeyStore();
int ProcessSignAPI(char dataSign[],char keyName[],char signature_scheme[]);
int ProcessSignAPI(/*char requestId[]*/);
/*
void AddCert(const char* cert,const char*keyfile)
{
    //strcyp(m_CertFile, cert);
    //strcpy(m_KeyFile, keyfile); 
}*/
#endif
