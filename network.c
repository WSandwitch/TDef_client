#include <math.h>

#include "main.h"
#include "network.h"

#define checkMask(x,y) x&y

#define recvMap(x) if(recvData(config.map.network.socket,&x,sizeof(x))<0) return -1



int networkConnMap(char * addr,int port){
	config.map.network.socket=networkConn(addr,port);
	return config.map.network.socket;
}


npc* getNpcId(int id){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id==id)
			return &config.map.npc_array[i];
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id==0)
			return &config.map.npc_array[i];
}

tower* getTowerId(int id){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id==id)
			return &config.map.tower_array[i];
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id==0)
			return &config.map.tower_array[i];
}

bullet* getBulletId(int id){
	int i;
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id==id)
			return &config.map.bullet_array[i];
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id==0)
			return &config.map.bullet_array[i];
}

int recvNpcMap(){
	npc * n;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	n=getNpcId(id);
	recvMap(bit_mask);
	if (checkMask(bit_mask,NPC_CREATE)){
		recvMap(n->isfriend);
		recvMap(n->type);
//		printf("%d\n",n->type);
//		recvMap(n->destination);
	}
//	if (checkMask(bit_mask,NPC_POSITION)){
	recvMap(n->destination);
	if (checkMask(bit_mask,NPC_CREATE))
		memcpy(&n->position,&n->destination,sizeof(vec2));
	float length;
	vec2 dir={n->destination.x-n->position.x,n->destination.y-n->position.y};
	length=sqrt(sqr(dir.x)+sqr(dir.y));
	dir.x/=5;
	dir.y/=5;
	memcpy(&n->direction,&dir,sizeof(vec2));
//	}
	if(checkMask(bit_mask,NPC_HEALTH) || checkMask(bit_mask,NPC_CREATE))
		recvMap(n->health);
	if (n->id==0)
		n->id=id;
	return 0;
}


int recvTowerMap(){
	tower * t;
	int id,bit_mask;
	recvMap(id);
	//find npc by id
	t=getTowerId(id);
	recvMap(bit_mask);
	if (checkMask(bit_mask,TOWER_CREATE)){
		recvMap(t->type);
		recvMap(t->owner);
		int position;
		recvMap(position);
	}
	if (checkMask(bit_mask,TOWER_TARGET) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->target);
	if(checkMask(bit_mask,TOWER_HEALTH) || checkMask(bit_mask,TOWER_CREATE))
		recvMap(t->health);
	if (t->id==0)
		t->id=id;
	return 0;
}

int recvBulletMap(){
	bullet * b;
	int id;
	if (b->id==0)
		b->id=id;
	return 0;
}

int recvMesMap(){
	char mes;
	int err;
	networkSetNonBlock(config.map.network.socket);
	recvMap(mes);
	networkSetBlock(config.map.network.socket);
	if (mes==MSG_NPC){
		return recvNpcMap();
	}
	if (mes==MSG_TOWER){
		return recvTowerMap();
	}
	if (mes==MSG_BULLET){
		return recvBulletMap();
	}
	
	return 0;
}