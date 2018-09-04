#include	"msg_parser.h"
#include <string.h>
#define TAG_VALUE_SEPRATOR  ":"
#define END_VALUE ","
#define END_OF_OBJ "}"

void InitString(RpmString_s *str)
{
	str->m_String = NULL;
	str->m_Size  = 0;
	str->m_Index = 0;
}

const char* GetPtr(RpmString_s *Ptr)
{
	return (const char*)Ptr->m_String;
}
RpmString_s* InitStringWithBuffer(char *ptr)
{
	if(ptr==NULL)
	{
		return NULL;
	}
	size_t len = strlen(ptr);
	if(len ==0)
	{
		return NULL;
	}
	RpmString_s * str = (RpmString_s*)malloc(sizeof(RpmString_s));
	str->m_Size = len;
	str->m_Index = 0;
	str->m_String = (char*)malloc(len+1);
	memcpy(str->m_String, ptr,len);
	str->m_String[len] = '\0';
	return str;

}

RpmString_s* InitStringWithBufferSize(char* str, size_t len)
{
	if(str==NULL || len==0)
	{
		return NULL;
	}
	RpmString_s * str1 = (RpmString_s*)malloc(sizeof(RpmString_s));
	str1->m_Size = len;
	str1->m_Index = 0;
	str1->m_String = (char*)malloc(len+1);
	memcpy(str1->m_String, str,len);
	str1->m_String[len+1] = '\0';
	return (str1);
}
size_t GetLength(RpmString_s *str)
{
	return str->m_Size;
}
static size_t StrnCMP(const char* left,size_t lSize,const char* right, size_t rSize)
{
	if(left==NULL||right==NULL)
	{
		return -1;
	}
	for(int i=0; i<lSize;i++)
	{
			int bCheck = 1;
			for(int j=0;j<rSize;j++)
			{
				if((i+j)<lSize)
				{
					if(left[i+j]!=right[j])
					{
						bCheck=0;	
						break;
					}
				}
				else
				{
					return -1;
				}
		    }
			if(bCheck)
			{
				return i;
			}
			
	}
	return -1;
	
}
size_t FindSubString(const RpmString_s *str,void* buffer)
{
	char* buff = (char*)buffer;
	size_t inLen = strlen(buff);
	if(str==NULL || buff==NULL)
	{
		return -1;
	}
	if(str->m_Size < inLen)
	{
		return -1;
	}
	
	return StrnCMP(str->m_String,str->m_Size,buff,inLen);
}
size_t FindSubStringPos(const RpmString_s *str,void* buffer,size_t pos)
{
	if(pos > str->m_Size)
	{
		return -1;
	}
	char* buff = (char*)buffer;
	size_t inLen = strlen(buff);
	if(str==NULL || buff==NULL)
	{
		return -1;
	}
	if(str->m_Size < inLen)
	{
		return -1;
	}
	size_t p = StrnCMP(str->m_String+pos,str->m_Size,buff,inLen);
	if(p == (size_t)-1)
	{
		return -1;
	}
	if((pos+p)>=str->m_Size)
	{
		return -1;
		
	}		
	return pos+p;
	
}
const char* FindSubStringByPos(const RpmString_s*str, int start,int end)
{
	if(start<0 || end<0)
	{
		return NULL;
	}
	if(end < start)
	{
		return NULL;
	}
	int len = end-start;
		
	if(str->m_Size-len<0)
	{
		return NULL;
	}
	char *ptr = (char*)malloc(len+1);
	int j=0;
	for(int i=start;i<end;i++)
	{
		ptr[j++] = str->m_String[i];
	}
	ptr[j]='\0';
	return ptr;
}
int  MsgParseTag(const RpmString_s *input ,RpmString_s *tag , RpmString_s **value )
{
	size_t pos  = FindSubString(input,tag->m_String);
	if(pos ==  (size_t)-1)
	{
		return -1;
	}
	if(value == NULL)
	{
		return -1;
	}
	size_t start_pos_value  = FindSubStringPos(input,TAG_VALUE_SEPRATOR,pos);
	if(start_pos_value== (size_t)-1)
	return -1;
	pos = FindSubStringPos(input,END_VALUE,pos);
	if(pos== (size_t)-1)
	return -1;
	//pos = pos - start_pos_value - 3;
	const char* ptr = FindSubStringByPos(input,start_pos_value+2,pos-1);
	printf("ptr value is %s\r\n",ptr);
	*value = InitStringWithBuffer((char*)ptr);
    free((void*)ptr);   
	return 0;
}


int  MsParseStatusCode(const RpmString_s* input,RpmString_s **LastErrorCode)
{
	if(LastErrorCode==NULL)return -1;	
	size_t pos = FindSubString(input,"status");
	if(pos==(size_t)-1)return -1;
	size_t start_pos_value  = FindSubStringPos(input,TAG_VALUE_SEPRATOR,pos);
	if(start_pos_value == (size_t)-1) return -1;

	pos = FindSubString(input,"message");
	if(pos != (size_t)-1)
	{
		start_pos_value = FindSubStringPos(input,TAG_VALUE_SEPRATOR,pos);
		if(start_pos_value == (size_t)-1) return -1;
		pos = FindSubStringPos(input,END_OF_OBJ,start_pos_value);
		if(pos == (size_t)-1) return -1;
		const char *ptr = FindSubStringByPos(input,start_pos_value+2,pos-2); 
		if(ptr==NULL)
		return -1;
		*LastErrorCode = InitStringWithBuffer((char*)ptr);
		if(*LastErrorCode==NULL)
		{
			return -1;
		}
	}
	
	return 0;

}
void FreeString(RpmString_s *str)
{
	if(str)
	{
		if(str->m_String)
		{
			free(str->m_String);
		}
		str->m_Index = 0;
		str->m_String=NULL;
		str->m_Size = 0;
		free(str);
	}
}
int InitAuthResponse(AuthResponse ** authRsp,RpmString_s*SessionToken,RpmString_s*rqstId,RpmString_s*status )
{
	if(authRsp== NULL ||SessionToken==NULL||rqstId==NULL|| status==NULL)
	{
		return -1;
	}
	*authRsp = (AuthResponse*)malloc(sizeof(AuthResponse));
	if(*authRsp)
	{
		(*authRsp)->m_RequestID = rqstId;
		(*authRsp)->m_sessionToken = SessionToken;
		(*authRsp)->m_Status = status;
		return 0;
	}
	return -1;
}
int MsgParseAuthResponse(const RpmString_s*input, AuthResponse **authData)
{
	if(authData==NULL)
	{
		return -1;
	}
	RpmString_s *LastErrorCode = NULL,*SessionToken=NULL,*SessionTknTag=NULL;
	if(MsParseStatusCode(input,&LastErrorCode)==-1)
	{
		//ERROR_LOG("Failed to parse status code ");
		goto err;
	}
	SessionTknTag = InitStringWithBuffer("sessionToken");
	if(SessionTknTag==NULL)
	{
		return -1;
	}
	if(MsgParseTag(input,SessionTknTag,&SessionToken)==-1)
	{
		FreeString(SessionTknTag);
		SessionTknTag = NULL;
		//ERROR_LOG("Failed to parse tag ");
		goto err;
	}
	FreeString(SessionTknTag);
	SessionTknTag=NULL;
	
	RpmString_s *RqstId = InitStringWithBuffer("auth");
	if(RqstId==NULL)
	{
		
		goto err;
	}
	RpmString_s *status = InitStringWithBuffer("SUCCESS");
	if(status==NULL)
	goto err;

	if(InitAuthResponse(authData,SessionToken,RqstId,status)==-1)goto err;
	return 0;
	err:
	{
		if(SessionToken)
		{
			FreeString(SessionToken);
			SessionToken = NULL;
			SessionToken = InitStringWithBuffer(" ");
		}
		if(status)
		{
			FreeString(status);
			status = NULL;
			status = InitStringWithBuffer("FAILURE");
		}
		if(RqstId)
		{
			FreeString(RqstId);
			RqstId = NULL;
			RqstId = InitStringWithBuffer("auth");
		}
		InitAuthResponse(authData,SessionToken,RqstId,status);
		return -1;
	}
}
/*
int InitKeyStoreObject(KeyStoreKeyObj*keyStoreObj,RpmString_s*name,RpmString_s*algo,CertChain Obj,int ChainSize)
{
	if(keyStoreObj == NULL)
		return -1;
	*keyStoreObj = (KeyStoreKeyObj_s*)malloc(sizeof(KeyStoreKeyObj_s));
	(*keyStoreObj)->m_Algo = algo;
	(*keyStoreObj)->m_Name = name;
	 if(ChainSize == 0)
	 {
		 (*keyStoreObj)->m_CertChainSize = 0;
		 (*keyStoreObj)->m_CertChainObj = NULL;
	 }
	 else
	 {
		(*keyStoreObj)->m_CertChainObj = (CertChain)malloc(sizeof(int)*ChainSize);
		if((*keyStoreObj)->m_CertChainObj== NULL)return -1;
		(*keyStoreObj)->m_CertChainSize = ChainSize;
		for(int i=0;i<ChainSize;i++)
		{
			((*keyStoreObj)->m_CertChainObj)[i] = Obj[i]; 
		}
	 }
	(*keyStoreObj)->m_Algo = algo;

}
int InitKeyStoreResponse(KeyStoreResponse**kspRsp,KeyStoreKeyObj  keyResp)
{
	if(kspRsp == NULL) return -1;
	if(*kspRsp == NULL)
	{
		*kspRsp = (KeyStoreResponse*)malloc(sizeof(KeyStoreResponse)*10);
		if(*kspRsp == NULL)return -1;
		(*kspRsp)->m_SizeofKey = 10;
		(*kspRsp)->NoOfKeys = 0;
		InitKeyStoreObject((&(*kspRsp)->m_Keys),NULL,NULL,NULL,NULL);
	}
	if((*kspRsp)->NoOfKeys < (*kspRsp)->m_SizeofKey)
	{
		
		(*kspRsp)->NoOfKeys++;

		RpmString_s *name = NULL,*algo =NULL;
		name = InitStringWithBuffer(( char*)GetPtr(keyResp->m_Name));
		algo = InitStringWithBuffer(( char*)GetPtr(keyResp->m_Algo));
		(*kspRsp)->m_Keys = NULL;
		InitKeyStoreObject((&(*kspRsp)->m_Keys),name,algo,keyResp->m_CertChainObj,keyResp->m_CertChainSize);
		return 0;
		
	}
	else
	{
		int size = (*kspRsp)->m_SizeofKey*2;
		KeyStoreResponse *ptr = (KeyStoreResponse*)malloc(sizeof(KeyStoreResponse)*size);
		for(int i = 0;i<(*kspRsp)->m_SizeofKey;i++)
		{
			ptr->m_SizeofKey=size;
			InitKeyStoreResponse(&ptr,kspRsp[i]->m_Keys);
		}
		FreeKeyStoreData(*kspRsp);
		*kspRsp = NULL;
		InitKeyStoreResponse(&ptr,keyResp);
		*kspRsp = ptr;
	}

}
*/
static void FreeKeyStoreKeyObj(KeyStoreKeyObj obj)
{
	
	if(obj)
	{
		if(obj->m_Algo)
		{
			FreeString(obj->m_Algo);
			obj->m_Algo=NULL;
		}
		if(obj->m_Name)
		{
			FreeString(obj->m_Name);
			obj->m_Name=NULL;
		}
		for(int i=0;i<obj->m_NoOfChain;i++)
		{
			
				if(((obj->m_CertChainObj[i]).m_CertChain))
				{
					free(((obj->m_CertChainObj[i]).m_CertChain));
					obj->m_CertChainObj[i].m_CertChain=NULL;
					obj->m_CertChainObj[i].m_ChainSize=0;
				}
				
			
		}
		obj->m_NoOfChain = 0;
		free(obj);
		obj=NULL;
	}
}

void FreeKeyStoreData(KeyStoreResponse *kspRsp)
{
	if(kspRsp)
	{
		if(kspRsp->m_RequestID)
		{
			FreeString(kspRsp->m_RequestID);
			kspRsp->m_RequestID=NULL;	
		}
		if(kspRsp->m_Status)
		{
			FreeString(kspRsp->m_Status);
			kspRsp->m_RequestID=NULL;
		}
		if(kspRsp->m_NoOfKeys>0)
		{
			for(int i=0;i<kspRsp->m_NoOfKeys;i++){
				FreeKeyStoreKeyObj(kspRsp->m_Keys[i]);
				kspRsp->m_Keys[i]=NULL;
			}
			kspRsp->m_NoOfKeys=0;
		}
		
		free(kspRsp);
		kspRsp = NULL;
	}
}
static int count =0;
int MsgParseKeyStoreResponse(const RpmString_s*input,KeyStoreResponse **keyStoreData )
{
	size_t namePos  = 0;
	RpmString_s *LastErrorCode = NULL,*name=NULL,*SessionTknTag=NULL;
	if(MsParseStatusCode(input,&LastErrorCode))
    {
		goto err;
    }
	if(keyStoreData==NULL)
	{
		return -1;
	}
	*keyStoreData = (KeyStoreResponse*)malloc(sizeof(KeyStoreResponse));
	if(*keyStoreData==NULL) return -1;
	memset(*keyStoreData,0,sizeof(KeyStoreResponse));
	name = InitStringWithBuffer("name");
	int NoOfKeys = 0;
	while((namePos = FindSubStringPos(input,(void*)GetPtr(name), namePos+1)) !=(size_t)-1)
	{
		KeyStoreKeyObj keyObj;
		keyObj = (KeyStoreKeyObj_s*)malloc(sizeof(KeyStoreKeyObj_s));
		memset(keyObj,0,sizeof(KeyStoreKeyObj_s));
		size_t valueStart = FindSubStringPos(input,TAG_VALUE_SEPRATOR,namePos);
		count++;
		if(valueStart==(size_t)-1)
		{
			goto err;
		}
		size_t valueEnd = FindSubStringPos(input,",",valueStart);
		if(valueEnd==(size_t)-1)
			goto err;
		const char*ptr = FindSubStringByPos(input,valueStart+2,valueEnd-1);
		printf("Name is %s\r\n",ptr);
		if(ptr==NULL)
		{
			goto err;			
		}
		keyObj->m_Name = InitStringWithBuffer((char*)ptr);
		free((void*)ptr);ptr=NULL;
		valueStart = FindSubStringPos(input,"algorithm",valueEnd);
		if(valueStart==(size_t)-1)
			goto err;

		valueStart = FindSubStringPos(input,TAG_VALUE_SEPRATOR,valueStart);
		if(valueStart==(size_t)-1)
			goto err;
		valueEnd =FindSubStringPos(input,",",valueStart);
		if(valueEnd==(size_t)-1)
			goto err;
		ptr = FindSubStringByPos(input,valueStart+2,valueEnd-1);
		keyObj->m_Algo = InitStringWithBuffer((char*)ptr);
		free((void*)ptr);ptr=NULL;
		keyObj->m_NoOfChain = 0;
		int NoOfchain = 0;
		while(1)
		{
			valueStart =FindSubStringPos(input,"[",valueEnd);
			if(valueStart==(size_t)-1)
				goto err;
			
			valueStart = ((GetPtr((RpmString_s*)input))[valueStart+1]=='[') ?valueStart+1:valueStart;
			valueEnd = FindSubStringPos(input,"]",valueStart);
			if(valueEnd==(size_t)-1)
				goto err;
			size_t NoChain = valueEnd - valueStart; 
			
			int *certPtr = (CertChain)malloc(sizeof(int)*NoChain);
			int certSize= 0;
			
			
			for(size_t pos = valueStart+1;pos<valueEnd;)
			{
				size_t posEnd  =FindSubStringPos(input,",",pos);
				if((posEnd != (size_t)-1) && (posEnd >= valueEnd))
				{
					break;
				}
				const char* value =FindSubStringByPos(input,pos,posEnd);
				int v =atoi(value);
				certPtr[certSize++] = v;
				pos += strlen(value)+1;
			}
			keyObj->m_CertChainObj[NoOfchain].m_CertChain = certPtr;
			keyObj->m_CertChainObj[NoOfchain].m_ChainSize = certSize;
			NoOfchain++;
			
			if(input->m_String[valueEnd+1] != ',' && ((input->m_String[valueEnd+1]==']')|| input->m_String[valueEnd+1]=='}'))
			{
					break;
			}
		}
		keyObj->m_NoOfChain = NoOfchain;
		(*keyStoreData)->m_Keys[NoOfKeys]= keyObj;	
		(*keyStoreData)->m_NoOfKeys = NoOfKeys;
		NoOfKeys++;
	
	}
		(*keyStoreData)->m_RequestID = InitStringWithBuffer("keystore_request");
		(*keyStoreData)->m_Status = InitStringWithBuffer("SUCCESS");
	
	return 0;
	err:
	{
		if(*keyStoreData)
		{
			(*keyStoreData)->m_RequestID = InitStringWithBuffer("keystore_request");
			(*keyStoreData)->m_Status = InitStringWithBuffer("FAILURE");
		}
		return -1;
	
	}
}
int StringCompare(RpmString_s *rpmStr, const char*str)
{
	if(rpmStr==NULL||str==NULL)
	{
		return -1;
	}
	if(rpmStr->m_Size!=strlen(str)) return -1;
	return strncmp(rpmStr->m_String,str,rpmStr->m_Size);
}
int MsgParseSignResponse(const RpmString_s*input,SignResponse **signData)
{
	size_t requstId = FindSubString(input,"requestId");
   if(signData==NULL)
   {
       return -1;
   }
   *signData=(SignResponse*)malloc(sizeof(SignResponse));
  memset(*signData,0 , sizeof(SignResponse));
	size_t posSignTag = FindSubString(input,"status");
	size_t startPos = FindSubStringPos(input,":",posSignTag);
	size_t EndPos  = FindSubStringPos(input,",",posSignTag);
	if(EndPos==(size_t)-1)
	{
		EndPos = FindSubStringPos(input,"}",startPos);

	}
	
	size_t pos  = FindSubStringPos(input,":",requstId);
	size_t end  = FindSubStringPos(input,",",pos);
	
	
	const char*ptr = FindSubStringByPos(input,pos+2,end-1);
	RpmString_s *rqstID = NULL,*Status=NULL;
	if(ptr)
	{
		rqstID  = InitStringWithBuffer((char*)ptr);
		free((void*)ptr);ptr=NULL;
		if(!rqstID)
			goto err;
	}
	ptr = FindSubStringByPos(input,startPos+2,EndPos-1);
	if(ptr)
	{
		Status  = InitStringWithBuffer((char*)ptr);
		free((void*)ptr);ptr=NULL;
		if(!Status)
			goto err;
	}
	if(signData==NULL)
	{
		*signData = (SignResponse*)malloc(sizeof(SignResponse));
		if(*signData==NULL) goto err;
	}
	(*signData)->m_RequestId = rqstID;
	(*signData)->m_Status =  Status;
	if(StringCompare(Status,"IN_PROGRESS")==0)
	{
		return 0;
	}
	pos  = FindSubString(input,"signature");
	if(pos == (size_t)-1)
	{
		return 0;
	}
	pos  = FindSubStringPos(input,"[",pos);
	pos  = FindSubStringPos(input,"[",pos);
	end  = FindSubStringPos(input,"]",pos);
	int SignDataSize = end - pos;
	(*signData)->SignDataSize = 0;
	(*signData)->m_SignData = (unsigned char*)malloc(sizeof(unsigned char)*SignDataSize);
	if((*signData)->m_SignData==NULL)
	{
		goto err;
	}
	(*signData)->SignDataSize = SignDataSize;
	int j = 0;
	for(size_t p = pos+1;p < end;)
	{
		size_t e  = FindSubStringPos(input,",",p);
		
		const char* value =FindSubStringByPos(input,p,e);
		(*signData)->m_SignData[j++]=((unsigned char)(atoi(value)));
		p += strlen(value)+1;
	} 

	(*signData)->SignDataSize = j;
	printf("KRISHNA SIN DATA VALUE IS %d\r\n\n\n",(*signData)->SignDataSize);
	return 0;
	err:
	{
		if(*signData==NULL)
		{
			*signData = (SignResponse*)malloc(sizeof(SignResponse));
			memset(*signData,0,sizeof(SignResponse));
		}
		if((*signData)->m_RequestId==NULL)
		{
			(*signData)->m_RequestId = InitStringWithBuffer("sign_data");
		}
		if((*signData)->m_Status)
		{
			FreeString((*signData)->m_Status);
			(*signData)->m_Status= NULL;
		}
		(*signData)->m_Status = InitStringWithBuffer("FAILURE");
		return -1;
	}
}
void FreeSignResposeData(SignResponse *obj)
{
	if(obj)
	{
		if(obj->SignDataSize && obj->m_SignData)
		{
			free(obj->m_SignData);
			obj->m_SignData = NULL;
			obj->SignDataSize = 0;
		}
		if(obj->m_RequestId)
		{
			FreeString(obj->m_RequestId);
			obj->m_RequestId =NULL;
		}
		if(obj->m_Status)
		{
			FreeString(obj->m_Status);
			obj->m_Status=NULL;
		}
		free(obj);
	}
}
void FreeAuthenticateResponse(AuthResponse*obj)
{
	if(obj)
	{
		if(obj->m_RequestID)
		{
			FreeString(obj->m_RequestID);
			obj->m_RequestID=NULL;
		}
		if(obj->m_sessionToken)
		{
			FreeString(obj->m_sessionToken);
			obj->m_sessionToken=NULL;
		}
		if(obj->m_Status)
		{
			FreeString(obj->m_Status);
			obj->m_Status=NULL;
		}
		
	free(obj);
	obj=NULL;
	}
}
