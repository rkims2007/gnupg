#ifndef __MSG_PARSER_H__
#define __MSG_PARSER_H__
#include <stdlib.h>
//#include <unistd.h>

typedef struct tagRpmString_t
{
	char* 		m_String;
	size_t 		m_Size;
	int 		m_Index;
}RpmString_s;



void InitString(RpmString_s *str);
const char* GetPtr(RpmString_s *Ptr);
RpmString_s* InitStringWithBuffer(char *ptr);
RpmString_s* InitStringWithBufferSize(char* str, size_t len);
size_t GetLength(RpmString_s *str);
size_t FindSubString(const RpmString_s *str,void* buffer);
const char* FindSubStringByPos(const RpmString_s*str, int start,int end);
size_t FindSubStringPos(const RpmString_s *str,void* buffer,size_t pos);
void FreeString(RpmString_s *str);
int StringCompare(RpmString_s *, const char*);
typedef struct AuthResponse_t
{
   RpmString_s *m_sessionToken;
   RpmString_s *m_RequestID;
   RpmString_s *m_Status;		
}AuthResponse;

typedef int*	CertChain;
typedef struct CertificateChainValue_t
{
	CertChain   m_CertChain;
	int 		m_ChainSize;
}CertificateChainValue_s;



typedef struct  KeyStoreKeyObj_t
{
	RpmString_s *m_Name;
	RpmString_s *m_Algo;
	CertificateChainValue_s   m_CertChainObj[1024];
	int						  m_NoOfChain;
	
}KeyStoreKeyObj_s;

typedef KeyStoreKeyObj_s*  KeyStoreKeyObj;

typedef struct KeyStoreResponse_t
{
	KeyStoreKeyObj  m_Keys[100];
	size_t          m_NoOfKeys;
	RpmString_s *m_RequestID;
   	RpmString_s *m_Status;
}KeyStoreResponse;

typedef unsigned char*  SignData_t;
typedef struct SignResponse_t
{
	SignData_t		m_SignData;
	int				SignDataSize;
	RpmString_s     *m_Status;
	RpmString_s 	*m_RequestId;
}SignResponse;

typedef struct tagMsgParser_t
{
	RpmString_s  m_Input;
	RpmString_s  m_LastErrorCode;
}tagMsgParser_s;

int InitAuthResponse(AuthResponse ** authRsp,RpmString_s*,RpmString_s*,RpmString_s* );
int InitKeyStoreResponse(KeyStoreResponse**,KeyStoreKeyObj  keyResp);
int InitKeyStoreObject(KeyStoreKeyObj*keyStoreObj, RpmString_s*name,RpmString_s*algo,CertChain Obj,int ChainSize);
void FreeKeyStoreData(KeyStoreResponse *kspRsp);
void FreeSignResposeData(SignResponse *);
void FreeAuthenticateResponse(AuthResponse*);
int  MsgParseTag(const RpmString_s *input ,RpmString_s *tag , RpmString_s **value );
int  MsParseStatusCode(const RpmString_s* input,RpmString_s **LastErrorCode);
int  MsgParseAuthResponse(const RpmString_s*input, AuthResponse **authData);
int  MsgParseKeyStoreResponse(const RpmString_s*input,KeyStoreResponse **keyStoreData );
int  MsgParseSignResponse(const RpmString_s*input,SignResponse **signData);
#endif
