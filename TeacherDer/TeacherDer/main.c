#include <stdio.h>
#include "keymng_msg.h"
#include <string.h>
#include <stdlib.h>

int main()
{

	//teacher
	Teacher tea;
	strcpy(tea.name, "路飞");
	tea.age = 20;
	tea.p = (char*)malloc(100);
	strcpy(tea.p, "我是要成为海贼王的男人");
	tea.plen = strlen(tea.p);


	//MsgKey_Req
	MsgKey_Req msgkey_req;
	msgkey_req.cmdType = 1;
	strcpy(msgkey_req.clientId, "test1");
	strcpy(msgkey_req.AuthCode, "test2");
	strcpy(msgkey_req.serverId, "test3");
	strcpy(msgkey_req.r1, "test4");


	//MsgKey_Res
	MsgKey_Res msgkey_res;
	msgkey_res.rv = 1;
	strcpy(msgkey_res.clientId, "test1");
	strcpy(msgkey_res.serverId, "test2");
	strcpy(msgkey_res.r2, "test3");
	msgkey_res.seckeyid = 10;


	// 编码
	char* outData;
	int outlen;

	MsgEncode(&msgkey_res, ID_MsgKey_Res, &outData, &outlen);

	// 解码
	int type = 0;

	void* temp;

	MsgDecode(outData, outlen, &temp, &type);
	if (type == ID_MsgKey_Teacher)
	{
		Teacher* pt = (Teacher*)temp;
		printf("name:	%s\n", pt->name);
		printf("age:	%d\n", pt->age);
		printf("p:	%s\n", pt->p);
		printf("plen:	%d\n", pt->plen);
		printf("type:	%d\n", type);

	}
	else if (type == ID_MsgKey_Req)
	{
		MsgKey_Req* pt = (MsgKey_Req*)temp;
		printf("cmdType:	%d\n", pt->cmdType);
		printf("clientId:	%s\n", pt->clientId);
		printf("AuthCode:	%s\n", pt->AuthCode);
		printf("serverId:	%s\n", pt->serverId);
		printf("r1:	%s\n", pt->r1);

	}
	else
	{
		MsgKey_Res* pt = (MsgKey_Res*)temp;
		printf("rv:	%d\n", pt->rv);
		printf("clientId:	%s\n", pt->clientId);
		printf("serverId:	%s\n", pt->serverId);
		printf("r2:	%s\n", pt->r2);
		printf("seckeyid:	%d\n", pt->seckeyid);

	}
	MsgMemFree(&temp, type);

	system("pause");

	return 0;
}