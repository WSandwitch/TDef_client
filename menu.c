#include "headers.h"


/*
╔══════════════════════════════════════════════════════════════╗
║ 										                       ║
║ created by Dennis Yarikov						                       ║
║ jul 2014									                       ║
╚══════════════════════════════════════════════════════════════╝
*/


void setScreenMessage(char * mes){
	sprintf(config.message,"%s",mes);
}

int checkMouseMenu(menu* root){
	if (root==0)
		return 0;
//	if (root->enable==0)
//		return 0;
	menu* m=0;
	m=root;
	int i;
	int o=0;
	for(i=0;i<config.menu.depth;i++)
		m=&m->submenu[config.menu.path[i]];
	for(i=0;i<m->$objects;i++){
		object* tmp;
		if ((tmp=checkMouseOnObject(&m->objects[i]))!=0){
//			config.menu.selected=tmp;
			//if (*tmp->text!=0)
//				cursor.text=tmp->text;
			o++;
		}
	}
	return o;
}


void processMouseMenu(SDL_Event event){	
	if (event.button.button==SDL_BUTTON_LEFT){
		if(config.menu.selected!=0)
			processObjectAction(MOUSE,SDL_BUTTON_LEFT);
		//config.global.mouse[event.button.button]=0;
	}
}

void processKeysMenu(SDL_Event event){
	if (event.key.keysym.sym==SDLK_SPACE){
		if(config.menu.selected!=0)
			processObjectAction(KEYBOARD,SDLK_SPACE);
		//config.global.keys[event.key.keysym.sym]=0;
	}
}


//actions

void actionTestMenu(void * arg){
	printf("test\n");
}

void actionToggleMenu(void * arg){
	config.menu.enable=config.menu.enable==0?1:0;
}

void actionExit(void * arg){
	config.main_running=0;
}

void actionExitRoom(void * arg){
	config.loading.enable=1;
	config.map.enable=0;
	config.menu.enable=0;
	config.loading.enable=0;
}

void actionMapStart(void * arg){
//	config.loading.enable=1;
//	mapStart("global");
//	mapStart("testbig");
	
//	sprintf(config.map.network.server,"%s",??);
//	config.map.network.port= ??
	mapStart("test");
//	config.map.enable=1;
//change to params for new connection	
//	config.loading.enable=0;
}

void actionMapTest(void * arg){
	config.auth++;
	config.map.enable=0;
	config.public.enable=0;
//	config.map.enable=1;
	{
		FILE * file=fopen("connect.txt","r");
		fscanf(file,"%s\n",config.map.network.server);
		fscanf(file,"%d\n",&config.map.network.port);
		fclose(file);
	}
	mapStart("test");
}

void actionAuth(void * arg){
	config.auth++;
//	actionMapStart(0);
	config.map.enable=0;
	config.public.enable=0;
	publicStart();
}

void actionTextTest(void * arg){
	config.text.enable=1;
	SDL_StartTextInput();
}

void actionTextStart(void * arg){
	config.text.enable=1;
	config.text.text[0]=0;
	config.text.pos=strlen(config.text.text);
	SDL_StartTextInput();
}

