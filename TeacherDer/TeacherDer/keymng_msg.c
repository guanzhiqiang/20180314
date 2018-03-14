#include "keymng_msg.h"
#include "itcast_asn1_der.h"
#include <stdio.h>
#include "teacher.h"

int encodeId_msgkey_req(MsgKey_Req* p, ITCAST_ANYBUF** node)
{
	
	ITCAST_ANYBUF* phead = NULL;
	ITCAST_ANYBUF* pnext = NULL;
	
	

	DER_ItAsn1_WriteInteger(p->cmdType, &phead);
	EncodeChar(p->clientId, strlen(p->clientId) + 1, &phead->next);
	pnext = phead->next;
	EncodeChar(p->AuthCode, strlen(p->AuthCode) + 1, &pnext->next);
	pnext = pnext->next;
	EncodeChar(p->serverId, strlen(p->serverId) + 1, &pnext->next);
	pnext = pnext->next;
	EncodeChar(p->r1, strlen(p->r1) + 1, &pnext->next);

	
	*node = phead;
	return 0;
}
int encodeId_msgkey_res(MsgKey_Res* pstruct, ITCAST_ANYBUF** node)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = NULL;
	ITCAST_ANYBUF* pnext = NULL;
	

	DER_ItAsn1_WriteInteger(pstruct->rv, &phead);
	
	EncodeChar(pstruct->clientId, strlen(pstruct->clientId) + 1, &phead->next);
	pnext = phead->next;
	EncodeChar(pstruct->serverId, strlen(pstruct->serverId) + 1, &pnext->next);
	pnext = pnext->next;
	EncodeChar(pstruct->r2, strlen(pstruct->r2) + 1, &pnext->next);
	pnext = pnext->next;
	DER_ItAsn1_WriteInteger(pstruct->seckeyid, &pnext->next);

	*node = phead;
	return 0;
}

int decodeId_msgkey_req(ITCAST_ANYBUF* node, MsgKey_Req ** p)
{
	
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = node;
	ITCAST_ANYBUF* pnext = NULL;
	// 申请一块内存存储TEacher结构体数据
	MsgKey_Req* msgkey_req = (MsgKey_Req*)malloc(sizeof(MsgKey_Req));
	memset(msgkey_req, 0, sizeof(MsgKey_Req));
	DER_ItAsn1_ReadInteger(phead, &msgkey_req->cmdType);
	DER_ItAsn1_ReadPrintableString(phead->next, &ptemp);
	strcpy(msgkey_req->clientId, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = phead->next;
	DER_ItAsn1_ReadPrintableString(pnext->next, &ptemp);
	strcpy(msgkey_req->AuthCode, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = pnext->next;
	DER_ItAsn1_ReadPrintableString(pnext->next, &ptemp);
	strcpy(msgkey_req->serverId, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = pnext->next;
	DER_ItAsn1_ReadPrintableString(pnext->next, &ptemp);
	strcpy(msgkey_req->r1, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);

		*p = msgkey_req;

	return 0;
}
int decodeId_msgkey_res(ITCAST_ANYBUF* node, MsgKey_Res ** p)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = node;
	ITCAST_ANYBUF* pnext = NULL;
	// 申请一块内存存储TEacher结构体数据
	MsgKey_Res* msgkey_res = (MsgKey_Res*)malloc(sizeof(MsgKey_Res));
	memset(msgkey_res, 0, sizeof(MsgKey_Res));
	DER_ItAsn1_ReadInteger(phead, &msgkey_res->rv);
	DER_ItAsn1_ReadPrintableString(phead->next, &ptemp);
	strcpy(msgkey_res->clientId, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = phead->next;
	DER_ItAsn1_ReadPrintableString(pnext->next, &ptemp);
	strcpy(msgkey_res->serverId, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = pnext->next;
	DER_ItAsn1_ReadPrintableString(pnext->next, &ptemp);
	strcpy(msgkey_res->r2, ptemp->pData);
	DER_ITCAST_FreeQueue(ptemp);
	pnext = pnext->next;
	DER_ItAsn1_ReadInteger(pnext->next, &msgkey_res->seckeyid);

	*p = msgkey_res;

	return 0;
}
// 通用的编码函数
int MsgEncode(void * pStruct, int type, unsigned char ** outData, int * outLen)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = NULL;
	// 1. int type -> ITCAST_ANYBUF
	DER_ItAsn1_WriteInteger(type, &phead);
	// 2. 根据类型判断
	switch (type)
	{
	case ID_MsgKey_Teacher:
		// 对teacher结构体编码函数 - ptemp
		// 得到了一个以ptemp为头的字链表
		encodeTeacher((Teacher*)pStruct, &ptemp);
		break;
	case ID_MsgKey_Req:
		encodeId_msgkey_req((MsgKey_Req*)pStruct, &ptemp);
		break;
	case ID_MsgKey_Res:
		encodeId_msgkey_res((MsgKey_Res*)pStruct, &ptemp);
		break;
	default:
		break;
	}
	// 3. 连接
	phead->next = ptemp;
	// 4. 对整个链表打包
	ITCAST_ANYBUF *all = NULL;
	DER_ItAsn1_WriteSequence(phead, &all);
	// 5. 传出参数赋值
	*outData = all->pData;
	*outLen = all->dataLen;
	// 6. 释放链表
	DER_ITCAST_FreeQueue(phead);

	return 0;
}

// 通用解码函数
int MsgDecode(unsigned char * inData, int inLen, void ** pStruct, int * type)
{
	ITCAST_ANYBUF* ptemp = NULL;
	ITCAST_ANYBUF* phead = NULL;
	// 1. inData -> ITCAST_ANYBUF, 里边存储了整个链表的数据
	DER_ITCAST_String_To_AnyBuf(&ptemp, inData, inLen);
	// 2. 需要将链表从ITCAST_ANYBUF节点中释放出来, 得到了链表的头结点
	DER_ItAsn1_ReadSequence(ptemp, &phead);
	DER_ITCAST_FreeQueue(ptemp);
	// 3. 读第一节点, 取出type的值
	DER_ItAsn1_ReadInteger(phead, type);
	// 4. 根据type判断, 去调用不同的解码函数
	switch (*type)
	{
	case ID_MsgKey_Teacher:
		// 对teacher结构体编码函数 - ptemp
		// 得到了一个以ptemp为头的字链表
		decodeTeacher(phead->next, (Teacher**)pStruct);
		break;
	case ID_MsgKey_Req:
		decodeId_msgkey_req(phead->next, (MsgKey_Req **)pStruct);
		break;
	case ID_MsgKey_Res:
		decodeId_msgkey_res(phead->next, (MsgKey_Res **)pStruct);
		break;
	default:
		break;
	}
	// 5. 释放内存的操作
	DER_ITCAST_FreeQueue(phead);

	return 0;
}

int MsgMemFree(void ** point, int type)
{
	switch (type)
	{
		case ID_MsgKey_Teacher:
			free((*(Teacher**)point)->p); free(*point);
			break;
		case ID_MsgKey_Req:
			free(*point);
			break;
		case ID_MsgKey_Res:
			free(*point);
			break;
		default:
			break;
	}
	return 0;
}
