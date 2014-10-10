#include "headers.h"


int sign(float x){
	if (x<0)
		return -1;
	if (x>0)
		return 1;
	return 0;
}


splash* getSplashNew(){
	int i;
	for(i=0;i<config.map.splash_max;i++)
		if (config.map.splash_array[i].type==0)
			return &config.map.splash_array[i];
	return 0;
}

npc* getNpcById(int id){
	int i;
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id==id)
			return &config.map.npc_array[i];
	for(i=0;i<config.map.npc_max;i++)
		if (config.map.npc_array[i].id==0)
			return &config.map.npc_array[i];
	return 0;
}

tower* getTowerById(int id){
	int i;
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id==id)
			return &config.map.tower_array[i];
	for(i=0;i<config.map.tower_max;i++)
		if (config.map.tower_array[i].id==0)
			return &config.map.tower_array[i];
	return 0;
}

bullet* getBulletById(int id){
	int i;
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id==id)
			return &config.map.bullet_array[i];
	for(i=0;i<config.map.bullet_max;i++)
		if (config.map.bullet_array[i].id==0)
			return &config.map.bullet_array[i];
	return 0;
}



void frameSync(unsigned int *time){
	if (*time==0){
		*time=SDL_GetTicks();//timeGetTime();
		return;
	}
	unsigned int t,_t;
		t=config.time_per_frame-(_t=(SDL_GetTicks()-*time));
	if (t<0){
		perror("Time");
		t=0;
	}
	_t+=t<=config.time_per_frame?t:0;
//	if (_t<config.global.frame_time)
		config.global.frame_time=(config.global.frame_time+_t)/2.0f;
//	else
//		config.global.frame_time=_t;
	SDL_Delay(t<=config.time_per_frame?t:0);
	*time=SDL_GetTicks();
//	config.map.time_now=*time;
}

void workSync(unsigned int *time){
	#define time_per_frame 20 //1000/50
	if (*time==0){
		*time=SDL_GetTicks();//timeGetTime();
		return;
	}
	unsigned int t;
		t=time_per_frame-(SDL_GetTicks()-*time);
	if (t<0){
		perror("Time");
		t=0;
	}
	SDL_Delay(t<=time_per_frame?t:0);
	*time=SDL_GetTicks();
	return;
//	config.map.time_now=*time;
	#undef time_per_frame
}

void tickSync(unsigned int *time){
	if (*time==0){
		*time=SDL_GetTicks();//timeGetTime();
		return;
	}
		unsigned int t=config.time_per_tick-(SDL_GetTicks()-*time);
	if (t<0){
		perror("Time");
		t=0;
	}
	if (t>config.time_per_tick)
		perror("time");
	SDL_Delay(t<=config.time_per_tick?t:0);
	*time=SDL_GetTicks();
	return;
}


void processNodeAction(){
	if(config.menu.selected==0){
		if (config.map.focus>=0){
//			setActionMenu();
			//add brush here
			processBrush();
			return;
			
		}
		//config.map.action_menu.enable=0;
	}
}

void processObjectAction(int type,int key){
	if(config.menu.selected==0)
		return;
	if(config.menu.selected->touch!=0)
		if(config.menu.selected->action!=0)
			config.menu.selected->action(config.menu.selected);
	if(config.menu.selected->single!=0){
		if(type==KEYBOARD)
			config.global.keys[key]=0;
		if(type==MOUSE)
			config.global.mouse[key]=0;
	}
}

void processEvent(SDL_Event event){
	if (config.text.enable!=0){
		switch (event.type)
               {
                   case SDL_TEXTINPUT:
                       /* Add new text onto the end of our text */
		   printf("get %s\n",event.text.text);
//                       strcat(text, event.text.text);
                       break;
                   case SDL_TEXTEDITING:
                       /*
                       Update the composition text.
                       Update the cursor position.
                       Update the selection length (if any).
                       */
		    printf("get2 %s\n",event.edit.text);
// 	                    composition = event.edit.text;
//			cursor = event.edit.start;
//			selection_len = event.edit.length;
                       break;
		   case SDL_KEYDOWN:
			switch(event.key.keysym.sym){ 
				case SDLK_ESCAPE: 
					config.text.enable=0; 
					SDL_StopTextInput();
					break;
				case SDLK_RETURN:
					config.text.enable=0; 
					SDL_StopTextInput();
					break;
			}
               }
	}
	
		switch(event.type){
			case SDL_QUIT: 
				config.main_running = 0;
				break;
			case SDL_KEYDOWN:
				config.global.keys[event.key.keysym.sym]=1;
				switch(event.key.keysym.sym){ 
					case SDLK_ESCAPE: 
						actionToggleMenu(0); 
						break;
				}
				//if (config.menu.enable!=0)
					processKeysMenu(event);
				//else
					if (config.map.enable!=0)
						processKeysMap(event);
				break;
			case SDL_KEYUP:
				config.global.keys[event.key.keysym.sym]=0;
				break;
			case SDL_MOUSEMOTION:
	//			printf("%d %d\n",event.motion.xrel,event.motion.yrel);
				if (event.motion.xrel!=0||event.motion.yrel!=0){
					cursor.state.x=event.motion.x;
					cursor.state.y=config.options.window.height-event.motion.y;
				}
				//cursorMove(event.motion.xrel,-event.motion.yrel);
				checkCursorBorder();
				break;
			case SDL_MOUSEBUTTONDOWN:
				config.global.mouse[event.button.button]=1;
				//if (config.menu.enable!=0)
					processMouseMenu(event);
				//else
					if (config.map.enable!=0)
						processMouseMap(event);
				break;
			case SDL_MOUSEBUTTONUP:
				config.global.mouse[event.button.button]=0;
				break;
			case SDL_MOUSEWHEEL:
				setZoom(event.wheel.y*CAMERA_ZOOM);
				break;
		}
}

void processKeyboard(){
	//global keys
	//mouse motion
	if (config.global.keys[SDLK_UP])
		cursorMove(0,CURSOR_SPEED);
	if (config.global.keys[SDLK_DOWN])
		cursorMove(0,-CURSOR_SPEED);
	if (config.global.keys[SDLK_LEFT])
		cursorMove(-CURSOR_SPEED,0);
	if (config.global.keys[SDLK_RIGHT])
		cursorMove(CURSOR_SPEED,0);
	//

	if (config.menu.enable==0)
		if (config.map.enable!=0)
			processContKeysMap();
				
};

int checkMouseOnElement(vec2 pos,element * e){
	if (cursor.state.x>=pos.x+e->position.x && 
			cursor.state.x<=pos.x+e->position.x+e->size.x &&
			cursor.state.y>=pos.y+e->position.y && 
			cursor.state.y<=pos.y+e->position.y+e->size.y){
		return 1;
	}
	return 0;
}


object * checkMouseOnObject(object * o){
	int i;
	for(i=0;i<o->elements_size;i++)
		if(checkMouseOnElement(o->position,&o->elements[i])!=0){
			if (o->focus!=0)
				o->in_focus=1;		
			return o;
		}
	if (o->focus!=0)
		o->in_focus=0;
	return 0;
}


void cursorMove(int xrel,int yrel){
	cursor.state.x+=xrel;
	cursor.state.y+=yrel;
}


void checkCursorBorder(){
	if (cursor.state.x<0)
		cursor.state.x=0;
	if (cursor.state.y<0)
		cursor.state.y=0;
	if (cursor.state.y>config.options.window.height)
		cursor.state.y=config.options.window.height;
	if (cursor.state.x>config.options.window.width)
		cursor.state.x=config.options.window.width;
}
//not used
void mouseMotion(){
	cursor.state.x+=(cursor.pos.x-cursor.prev.x)*cursor.sens;
	cursor.state.y+=-(cursor.pos.y-cursor.prev.y)*cursor.sens;
	checkCursorBorder();
	cursor.prev.x=cursor.pos.x;
	cursor.prev.y=cursor.pos.y;
}


int checkMouseState(){
//	mouseMotion();
	cursor.text=0;
	config.menu.selected=0;
	//check mouse on map, if not need it return 0
	checkMouseMap();
	//check menus on screen
	if (config.auth==0){
		checkMouseMenu(&config.auth_menu);
		goto out;
	}
	if (config.menu.enable!=0)
		checkMouseMenu(&config.menu.root);
	else
		if (config.map.enable!=0){
			checkMouseMenu(&config.map.screen_menu);
			checkMouseMenu(&config.map.tower_menu);
			checkMouseMenu(&config.map.npc_menu);
			if (config.map.action_menu.enable!=0)
				checkMouseMenu(&config.map.action_menu);
		}
//			if (config.menu.selected!=0)
//				config.menu.selected->in_focus=1;
//	printf("%d %d\n",cursor.state.x,cursor.state.y);
	
out:	
	if (config.text.enable!=0)
		checkMouseMenu(&config.text.keyboard);
	return 0;
}


int processMouse(){
	if (config.map.enable!=0 && config.menu.enable==0){
		#define border 4
		if (cursor.state.x<border)
			setMove(0,CAMERA_SPEED);
		if (cursor.state.y<border)
			setMove(CAMERA_SPEED,0);
		if (cursor.state.y>config.options.window.height-border)
			setMove(-CAMERA_SPEED,0);
		if (cursor.state.x>config.options.window.width-border)
			setMove(0,-CAMERA_SPEED);
		#undef border
	}
	checkMouseState();
	return 0;	
}

void cursorInit(){
	cursor.sens=1.0;
	cursor.color.r=1;
	cursor.color.g=1;
	cursor.color.b=1;
	cursor.state.x=0;
	cursor.state.y=config.options.window.height;
	
}



int loadGlobalTexture(char * path){
	config.textures[config.textures_size]=loadTGATexture(path);
	return config.textures[config.textures_size++];
}


int loadMapTexture(char * path){
	config.map.textures[config.map.textures_size]=loadTGATexture(path);
	return config.map.textures[config.map.textures_size++];
}


int getNewTexture(){
	glGenTextures(1,(unsigned int*)(config.textures+config.textures_size));
	config.textures_size++;
	return config.textures[config.textures_size-1];
}



void graficsInit(){

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){ 
	 	exit(1);
	} 
	
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,4);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);

	config.window = SDL_CreateWindow("TDef", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.options.window.width, config.options.window.height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL ); //| SDL_WINDOW_FULLSCREEN
	if(config.window == NULL){
		exit(1);
	}
	
	
	cursorInit();
	SDL_ShowCursor(0);
	
	
	
//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
	
	
}


