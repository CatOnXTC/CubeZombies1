#include <stdio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL_ttf.h>
#include <string.h>

#define B 1000
#define H 1000

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
Mix_Music* music = NULL;
SDL_Texture* loadBild(char path[100]);
SDL_Texture* key[9];
SDL_Texture* bild = NULL;
SDL_Texture* ttfont = NULL;
TTF_Font* font = NULL;
Mix_Chunk* shotsound = NULL;
Mix_Chunk* cubezomb = NULL;
Mix_Chunk* smerc = NULL;
Mix_Chunk* explosionbum = NULL;
Mix_Chunk* flamenwerfer = NULL;


typedef struct point{
    SDL_Rect rect;
    int desx, desy, k;
    float vx, vy;
} point;

typedef struct zombie{
    SDL_Rect zrect;
    float zvx, zvy;
    float zxmo, zymo;
    int hp, score;
    int burning;
} zombie;

typedef struct gun{
    int dmg;
    int del;
    int speed;
    int range;
    int fire;
} gun;

typedef struct explo{
    SDL_Rect exrect;
    int timer;
} explo;

typedef struct fire{
    SDL_Rect frect;
} fire;

point p[50];
zombie zp[100];
explo explode[10];
fire burnball[45];

void init();
void load();
void destroy();
void list(int x, int y);
void kill(int max, gun *g);
void move(int i, float xmo, float ymo, gun *gp);
void shoot(int *pspeed, int *pshot, gun *g);
int zmove(int i);
int die();
int brejk(int *wep, int *score);
void get_ttf(int x, int y, int h, int w, int *text, SDL_Rect *ttrect);
int options(int *musicvol, int *soundvol);
int controls();
point dcreate();
zombie dzcreate();
int highscore();
void list(int x, int y);
void bumbum(gun *g);
void fireshoot(int *pspeed, int *pshot, gun *g);
void setonfire();

point create(int x, int y){
    point a;
    a.desx=x;
    a.desy=y;
    a.rect.w=5;
	a.rect.h=5;
	a.rect.x=(B/2)-2;
	a.rect.y=(H/2)-2;
	a.vx=(B/2)-2;
	a.vy=(H/2)-2;
	a.k=0;
	return a;
}

point dcreate(){
    point a;
    a.desx=0;
    a.desy=0;
    a.rect.w=0;
	a.rect.h=0;
	a.rect.x=0;
	a.rect.y=0;
	a.vx=0;
	a.vy=0;
	a.k=0;
	return a;
}

zombie zcreate(){
    int r = rand()%19;
    zombie a;
    if(r>=0&&r<5){
    int s;
    float norm;
    s=rand()%4;
    if(s==0){
    a.zrect.x=0;
    a.zrect.y=rand()%960;
    }
    if(s==1){
    a.zrect.x=rand()%960;
    a.zrect.y=0;
    }
    if(s==2){
    a.zrect.x=960;
    a.zrect.y=rand()%960;
    }
    if(s==3){
    a.zrect.x=rand()%960;
    a.zrect.y=960;
    }
    a.zrect.h=40;
    a.zrect.w=40;
    a.zvx=a.zrect.x;
    a.zvy=a.zrect.y;

    a.zxmo=(B/2)-(a.zrect.x+(a.zrect.w/2));a.zxmo/=100;
    a.zymo=(H/2)-(a.zrect.y+(a.zrect.h/2));a.zymo/=100;
    norm = 0.7/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=50;
    a.score=1;
    }

    if(r>=5&&r<10){
    int s;
    float norm;
    s=rand()%4;
    if(s==0){
    a.zrect.x=0;
    a.zrect.y=rand()%950;
    }
    if(s==1){
    a.zrect.x=rand()%950;
    a.zrect.y=0;
    }
    if(s==2){
    a.zrect.x=950;
    a.zrect.y=rand()%950;
    }
    if(s==3){
    a.zrect.x=rand()%950;
    a.zrect.y=950;
    }
    a.zrect.h=50;
    a.zrect.w=50;
    a.zvx=a.zrect.x;
    a.zvy=a.zrect.y;

    a.zxmo=(B/2)-(a.zrect.x+(a.zrect.w/2));a.zxmo/=100;
    a.zymo=(H/2)-(a.zrect.y+(a.zrect.h/2));a.zymo/=100;
    norm = 0.5/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=90;
    a.score=1;
    }

    if(r>=10&&r<15){
    int s;
    float norm;
    s=rand()%4;
    if(s==0){
    a.zrect.x=0;
    a.zrect.y=rand()%970;
    }
    if(s==1){
    a.zrect.x=rand()%970;
    a.zrect.y=0;
    }
    if(s==2){
    a.zrect.x=970;
    a.zrect.y=rand()%970;
    }
    if(s==3){
    a.zrect.x=rand()%970;
    a.zrect.y=970;
    }
    a.zrect.h=30;
    a.zrect.w=30;
    a.zvx=a.zrect.x;
    a.zvy=a.zrect.y;

    a.zxmo=(B/2)-(a.zrect.x+(a.zrect.w/2));a.zxmo/=100;
    a.zymo=(H/2)-(a.zrect.y+(a.zrect.h/2));a.zymo/=100;
    norm = 1/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=20;
    a.score=1;
    }

     if(r>=15&&r<17){
    int s;
    float norm;
    s=rand()%4;
    if(s==0){
    a.zrect.x=0;
    a.zrect.y=rand()%920;
    }
    if(s==1){
    a.zrect.x=rand()%920;
    a.zrect.y=0;
    }
    if(s==2){
    a.zrect.x=920;
    a.zrect.y=rand()%920;
    }
    if(s==3){
    a.zrect.x=rand()%920;
    a.zrect.y=920;
    }
    a.zrect.h=80;
    a.zrect.w=80;
    a.zvx=a.zrect.x;
    a.zvy=a.zrect.y;

    a.zxmo=(B/2)-(a.zrect.x+(a.zrect.w/2));a.zxmo/=100;
    a.zymo=(H/2)-(a.zrect.y+(a.zrect.h/2));a.zymo/=100;
    norm = 0.3/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=260;
    a.score=2;
    }
    a.burning=0;

    if(r>=17&&r<19){
    int s;
    float norm;
    s=rand()%4;
    if(s==0){
    a.zrect.x=0;
    a.zrect.y=rand()%965;
    }
    if(s==1){
    a.zrect.x=rand()%965;
    a.zrect.y=0;
    }
    if(s==2){
    a.zrect.x=920;
    a.zrect.y=rand()%965;
    }
    if(s==3){
    a.zrect.x=rand()%965;
    a.zrect.y=965;
    }
    a.zrect.h=35;
    a.zrect.w=35;
    a.zvx=a.zrect.x;
    a.zvy=a.zrect.y;

    a.zxmo=(B/2)-(a.zrect.x+(a.zrect.w/2));a.zxmo/=100;
    a.zymo=(H/2)-(a.zrect.y+(a.zrect.h/2));a.zymo/=100;
    norm = 0.4/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=35;
    a.score=2;
    a.burning=-1;
    }
    return a;
}

zombie dzcreate(){
    zombie a;
    a.zrect.x=0;
    a.zrect.y=0;
    a.zrect.h=0;
    a.zrect.w=0;
    a.zvx=0;
    a.zvy=0;
    a.zxmo=0; a.zymo=0;
    a.hp=0;
    a.score=0;
    a.burning=0;
    return a;
}

gun gcreate(int c){
    gun a;
    if(c==1){a.dmg=20;a.del=10;a.range=400;a.speed=12;a.fire=0;}
    else if(c==2){a.dmg=150;a.del=40;a.range=550;a.speed=16;a.fire=0;}
    else if(c==3){a.dmg=100;a.del=40;a.range=400;a.speed=6;a.fire=1;}
    else if(c==4){a.dmg=18;a.del=30;a.range=250;a.speed=12;a.fire=2;}
    else if(c==5){a.dmg=0;a.del=3;a.range=0;a.speed=0;a.fire=3;}
    else {a.dmg=30;a.del=20;a.range=300;a.speed=10;a.fire=0;}
    return a;
}

explo excreate(int x, int y){
    explo a;
    a.exrect.h=110;
    a.exrect.w=110;
    a.exrect.x=x-55;
    a.exrect.y=y-55;
    a.timer=0;
    return a;
}

explo dexcreate(){
    explo a;
    a.exrect.h=0;
    a.exrect.w=0;
    a.exrect.x=0;
    a.exrect.y=0;
    a.timer=0;
    return a;
}

fire burncreate(int x, int y){
    fire a;
    a.frect.h=20;
    a.frect.w=20;
    a.frect.x=x;
    a.frect.y=y;
    return a;
}

fire dburncreate(){
    fire a;
    a.frect.h=0;
    a.frect.w=0;
    a.frect.x=0;
    a.frect.y=0;
    return a;
}

SDL_Texture* loadBild(char path[100]){
    SDL_Texture* nBild = NULL;

    SDL_Surface* loaded = IMG_Load(path);
    if(loaded == NULL){
        printf("SDL_image Error: %s\n", IMG_GetError());
    }
    else{
        SDL_SetColorKey(loaded, SDL_TRUE, SDL_MapRGB(loaded->format,0xFF, 0xFF, 0xFF));
        nBild = SDL_CreateTextureFromSurface(render, loaded);
        if(nBild == NULL){
        printf("SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(loaded);
    }
    return nBild;
}

void get_ttf(int x, int y, int h, int w, int *text, SDL_Rect *ttrect){
 SDL_Surface *surf;
 SDL_Color textColor = {255, 0, 0, 255};
 SDL_Color bg = {150,150,150,255};
 char c[10];

 sprintf(c, "%d", *text);

 surf = TTF_RenderText_Shaded(font, c, textColor,bg);
 ttfont = SDL_CreateTextureFromSurface(render, surf);
 SDL_FreeSurface(surf);
 ttrect->h=h;
 ttrect->w=w;
 ttrect->x=x;
 ttrect->y=y;
}

void init() {

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    window = SDL_CreateWindow("Cube Zombies", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, B, H, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048);
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    if(TTF_Init() == -1) printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
}

void load(){

    key[0] = loadBild("data/back.png");
    if(key[4]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    key[2] = loadBild("data/zomb.png");
    if(key[2]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    key[4] = loadBild("data/play.png");
    if(key[4]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    key[5] = loadBild("data/score.png");
    if(key[5]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    font = TTF_OpenFont("data/Zombie Queen.ttf", 72);

    music = Mix_LoadMUS("data/Sister.mp3");
    if( music == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );

    shotsound = Mix_LoadWAV("data/strzal.wav");
    if( shotsound == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );

    cubezomb = Mix_LoadWAV("data/cubezomb.wav");
    if( cubezomb == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );

    smerc = Mix_LoadWAV("data/smerc.wav");
    if( smerc == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );

    explosionbum = Mix_LoadWAV("data/bum.wav");
    if( explosionbum == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );

    flamenwerfer = Mix_LoadWAV("data/flamenwerfer.wav");
    if( flamenwerfer == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
}

void destroy(){

    for( int i = 0; i < 9; ++i )
	{
		if(key[i]!=NULL)SDL_DestroyTexture(key[i]);
		key[i] = NULL;
	}

	SDL_DestroyTexture(ttfont);
	ttfont = NULL;

	TTF_CloseFont(font);
	font = NULL;

	SDL_DestroyTexture(bild);
	bild = NULL;

    SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	window = NULL;
	render = NULL;

	Mix_FreeMusic(music);
	music = NULL;
	Mix_FreeChunk(shotsound);
	shotsound=NULL;
	Mix_FreeChunk(cubezomb);
	cubezomb=NULL;
	Mix_FreeChunk(smerc);
	smerc=NULL;

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void minidestroy(){

    for(int i = 0; i<50; i++){
        p[i]=dcreate();
    }

    for(int i = 0; i<100; i++){
        zp[i]=dzcreate();
    }

    for(int i = 0; i<10; i++){
        explode[i]=dexcreate();
    }

    for(int i =0; i<45; i++){
        burnball[i]=dburncreate();
    }
}

void move(int i, float xmo, float ymo, gun *gp){
    if(p[i].rect.w!=0){
    p[i].vx -=(xmo*(gp->speed));
    p[i].vy -=(ymo*(gp->speed));
    p[i].rect.x = p[i].vx;
    p[i].rect.y = p[i].vy;
    p[i].k+=(gp->speed);
    }
}

int zmove(int i){
    if(zp[i].zrect.w!=0){
    if((B/2)>zp[i].zrect.x && (B/2)<zp[i].zrect.x+zp[i].zrect.w && (H/2)>zp[i].zrect.y && (H/2)<zp[i].zrect.y+zp[i].zrect.h) return 1;
    else{
    zp[i].zvx+=(zp[i].zxmo);
    zp[i].zvy+=(zp[i].zymo);
    zp[i].zrect.x = zp[i].zvx;
    zp[i].zrect.y = zp[i].zvy;
    return 0;
    }
    }
return 0;
}

void list(int x, int y){
    int such=1, i=0;

    while(such!=0){
        if(p[i].rect.w==0){p[i]=create(x,y);such=0;Mix_PlayChannel(-1,shotsound,0);}
        else i++;
        if(i>49)break;
    }
}

void flist(int x, int y){
    int such=1, i=0;

    while(such!=0){
        if(burnball[i].frect.w==0){burnball[i]=burncreate(x,y);such=0;}
        else i++;
        if(i>44)break;
    }
}

void exlist(int x, int y){
    int such=1, i=0;

    while(such!=0){
        if(explode[i].exrect.w==0){explode[i]=excreate(x,y);such=0;}
        else i++;
        if(i>9)break;
    }
}

void zlist(){
int such=1, i=0;

    while(such!=0){
        if(zp[i].zrect.w==0){zp[i]=zcreate();such=0;Mix_PlayChannel(-1,cubezomb,0);}
        else i++;
        if(i>99)break;
    }
}

void kill(int max, gun *g){
    int nextx, nexty;
    for(int j=0; j<100; j++){
    if(zp[j].zrect.w==0){
        for(int i =0; i<50; i++){
            if(p[i].rect.w!=0)nextx= p[i].rect.x; nexty= p[i].rect.y;
            if(p[i].k>max-1 && p[i].rect.w!=0){if(g->fire==1)exlist(nextx, nexty);p[i]=dcreate();}
        }
    }
    else {
        for(int i =0; i<50; i++){
            if(p[i].rect.w!=0)nextx= p[i].rect.x; nexty= p[i].rect.y;
            if(p[i].k>max-1 || (p[i].rect.x>zp[j].zrect.x-5 && p[i].rect.x<zp[j].zrect.x+(zp[j].zrect.w-5) && p[i].rect.y>zp[j].zrect.y-5 && p[i].rect.y<zp[j].zrect.y+(zp[j].zrect.h-5))){
                if(g->fire==1)exlist(nextx, nexty);
                if(p[i].rect.x>zp[j].zrect.x-5 && p[i].rect.x<zp[j].zrect.x+(zp[j].zrect.w-5) && p[i].rect.y>zp[j].zrect.y-5 && p[i].rect.y<zp[j].zrect.y+(zp[j].zrect.h-5) && g->fire!=1) zp[j].hp-=g->dmg;
                p[i]=dcreate();
            }
        }
    }
}
    return;
}

void setonfire(){
    for(int j=0; j<100; j++){
       if(zp[j].zrect.w!=0){
            for(int i=0; i<45; i++){
                if(burnball[i].frect.w!=0){
                    if(burnball[i].frect.x+15>zp[j].zrect.x && burnball[i].frect.x<zp[j].zrect.x+(zp[j].zrect.w-5) && burnball[i].frect.y+15>zp[j].zrect.y && burnball[i].frect.y<zp[j].zrect.y+(zp[j].zrect.h-5)) zp[j].burning=27;
                }
            }
            if(zp[j].burning>0){zp[j].hp-=1; if(zp[j].burning==27)zp[j].hp-=2; zp[j].burning--;}
       }
    }
}

void bumbum(gun *g){
    for(int j=0; j<100; j++){
    if(zp[j].zrect.w==0){
        for(int i =0; i<10; i++){
            if(explode[i].timer>59 && explode[i].exrect.w!=0)explode[i]=dexcreate();
        }
    }
    else {
        for(int i =0; i<10; i++){
            if(explode[i].timer==30 && explode[i].exrect.w!=0){
                Mix_PlayChannel(-1,explosionbum,0);
                if(zp[j].zrect.x+3<explode[i].exrect.x+140 && zp[j].zrect.x+(zp[j].zrect.w-3)>explode[i].exrect.x &&
                   zp[j].zrect.y+3<explode[i].exrect.y+140 && zp[j].zrect.y+(zp[j].zrect.h-3)>explode[i].exrect.y) {zp[j].hp-=g->dmg;}
            }
            if(explode[i].timer>59 && explode[i].exrect.w!=0)explode[i]=dexcreate();
        }
    }
}
    return;
}

int die(){
    int scor;
    for(int j = 0; j<100; j++){
            if(zp[j].hp<1 && zp[j].zrect.w!=0){
                scor=zp[j].score;
                zp[j]=dzcreate();
                return scor;
            }
        }
    return 0;
}

void shoot(int *pspeed, int *pshot, gun *g){
    int x, y, cx, cy;
    float xx, yy, sx, sy;
    if(*pshot==1){
        if(*pspeed%g->del==0){
        SDL_GetMouseState(&x,&y);
        list(x-3,y-3);
        if(g->fire==2){
            xx=x-500; yy=y-500;
            sx=xx*cos(M_PI/24)-yy*sin(M_PI/24);
            sy=xx*sin(M_PI/24)+yy*cos(M_PI/24);
            cx=sx+500;
            cy=sy+500;
            list(cx-3,cy-3);
            xx=x-500; yy=y-500;
            sx=xx*cos(M_PI/48)-yy*sin(M_PI/48);
            sy=xx*sin(M_PI/48)+yy*cos(M_PI/48);
            cx=sx+500;
            cy=sy+500;
            list(cx-3,cy-3);
            xx=x-500; yy=y-500;
            sx=xx*cos(-M_PI/48)-yy*sin(-M_PI/48);
            sy=xx*sin(-M_PI/48)+yy*cos(-M_PI/48);
            cx=sx+500;
            cy=sy+500;
            list(cx-3,cy-3);
            xx=x-500; yy=y-500;
            sx=xx*cos(-M_PI/24)-yy*sin(-M_PI/24);
            sy=xx*sin(-M_PI/24)+yy*cos(-M_PI/24);
            cx=sx+500;
            cy=sy+500;
            list(cx-3,cy-3);
        }
        }
    }
    if(*pshot==-1){
        if(*pspeed%g->del==g->del-1){
            *pshot=0;
        }
    }
    if(*pshot==0) *pspeed=0;
    else *pspeed+=1;
}

void fireshoot(int *pspeed, int *pshot, gun *g){
    int x, y, cx, cy, ccx, ccy, cccx, cccy;
    float xx, yy, sx, sy, xvol1, yvol1, xvol2, yvol2, xvol3, yvol3, norm;
    if(*pshot==1){
        SDL_GetMouseState(&x,&y);
        xvol1=(B/2)-(x+10);xvol1/=100;
        yvol1=(H/2)-(y+10);yvol1/=100;
        norm = 1/(sqrt(pow(xvol1,2)+pow(yvol1,2)));
        xvol1*=norm; yvol1*=norm;

        xx=x-500; yy=y-500;
        sx=xx*cos(M_PI/24)-yy*sin(M_PI/24);
        sy=xx*sin(M_PI/24)+yy*cos(M_PI/24);
        ccx=sx+500;
        ccy=sy+500;

        xvol2=(B/2)-(ccx+10);xvol2/=100;
        yvol2=(H/2)-(ccy+10);yvol2/=100;
        norm = 1/(sqrt(pow(xvol2,2)+pow(yvol2,2)));
        xvol2*=norm; yvol2*=norm;

        xx=x-500; yy=y-500;
        sx=xx*cos(-M_PI/24)-yy*sin(-M_PI/24);
        sy=xx*sin(-M_PI/24)+yy*cos(-M_PI/24);
        cccx=sx+500;
        cccy=sy+500;

        xvol3=(B/2)-(cccx+10);xvol3/=100;
        yvol3=(H/2)-(cccy+10);yvol3/=100;
        norm = 1/(sqrt(pow(xvol3,2)+pow(yvol3,2)));
        xvol3*=norm; yvol3*=norm;

        for(int i =0; i<15; i++){
                cx=(490-xvol1*(12*(i+3)));
                cy=(490-yvol1*(12*(i+3)));
                flist(cx,cy);
        }
        for(int i =0; i<15; i++){
                cx=(490-xvol2*(12*(i+3)));
                cy=(490-yvol2*(12*(i+3)));
                flist(cx,cy);
        }
        for(int i =0; i<15; i++){
                cx=(490-xvol3*(12*(i+3)));
                cy=(490-yvol3*(12*(i+3)));
                flist(cx,cy);
        }
    }
    if(*pshot==-1){
        if(*pspeed%g->del==g->del-1){
            *pshot=0;
        }
    }
    if(*pshot==0) *pspeed=0;
    else *pspeed+=1;
    if(*pshot==1)Mix_PlayChannel(-1,flamenwerfer,0);
}

int brejk(int *wep, int *score){
int quit=0;
int x,y;

    key[3] = loadBild("data/inventory.png");
    if(key[3]==NULL)printf("SDL Error: %s\n",SDL_GetError());

SDL_Rect invent;
invent.h=900; invent.w=800; invent.x=100; invent.y=50;

SDL_Event e;
while(quit==0){
   while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                SDL_DestroyTexture(key[3]);
                key[3] = NULL;
            return 1;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    quit = 1;
                    SDL_DestroyTexture(key[3]);
                    key[3] = NULL;
                return 2;

                case SDLK_i:
                    quit = 1;
                    SDL_DestroyTexture(key[3]);
                    key[3] = NULL;
                break;

                case SDLK_e:
                    quit = 1;
                    SDL_DestroyTexture(key[3]);
                    key[3] = NULL;
                break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                    case SDL_BUTTON_LEFT:
                        SDL_GetMouseState(&x,&y);
                        if(x>100&&x<500&&y>50&&y<350)*wep=0;
                        if(x>500&&x<900&&y>50&&y<350 && *score>=5)*wep=1;
                        if(x>100&&x<500&&y>350&&y<650 && *score>=15)*wep=2;
                        if(x>100&&x<500&&y>650&&y<950 && *score>=50)*wep=5;
                        if(x>500&&x<900&&y>350&&y<650 && *score>=30)*wep=3;
                        if(x>500&&x<900&&y>650&&y<950 && *score>=75)*wep=4;
                    break;
                }
            break;
            }
   }
    SDL_RenderClear(render);

    bild = key[0];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    for(int i =0; i<50; i++){
            if(p[i].rect.w!=0){
    SDL_RenderFillRect(render,&(p[i].rect));
            }
    }

    SDL_SetRenderDrawColor(render,0,150,0,255);
    for(int j =0; j<100; j++){
            if(zp[j].zrect.w!=0){
    SDL_RenderFillRect(render,&(zp[j].zrect));
            }
    }

    bild = key[3];
    SDL_RenderCopy(render, bild, NULL, &invent);

    SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_Rect outline;
    outline.h=299; outline.w=398;
    if(*wep==0){outline.x=102; outline.y=52;}
    if(*wep==1){outline.x=500; outline.y=52;}
    if(*wep==2){outline.x=102; outline.y=351;}
    if(*wep==5){outline.x=102; outline.y=650;}
    if(*wep==3){outline.x=500; outline.y=351;}
    if(*wep==4){outline.x=500; outline.y=650;}
    SDL_RenderDrawRect(render, &outline);

    if (*score<10){
        SDL_Rect textscore;
        get_ttf(970,0,40,30,&*score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (*score<100){
        SDL_Rect textscore;
        get_ttf(940,0,40,60,&*score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else {
        SDL_Rect textscore;
        get_ttf(910,0,40,90,&*score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    SDL_Delay(1000/30);
    SDL_RenderPresent(render);
}
SDL_DestroyTexture(key[3]);
key[3] = NULL;
return 0;
}

int options(int *musicvol, int *soundvol){

int quit=0;
int x,y;

key[7] = loadBild("data/options.png");
if(key[7]==NULL)printf("SDL Error: %s\n",SDL_GetError());

SDL_Rect balk1;
balk1.h=50;
balk1.x=250;
balk1.y=490;

SDL_Rect balk2;
balk2.h=50;
balk2.x=250;
balk2.y=690;

SDL_Event e;
while(quit==0){
   while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                SDL_DestroyTexture(key[7]);
                key[7] = NULL;
                return 1;
            break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    quit = 1;
                    SDL_DestroyTexture(key[7]);
                    key[7] = NULL;
                break;
                }

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                    case SDL_BUTTON_LEFT:
                        SDL_GetMouseState(&x,&y);
                        if(x>430 && x<580 && y>860 && y<940)quit=1;
                        else if(x>200 && x<250 && y>490 && y<540)*musicvol=0;
                        else if(x>250 && x<300 && y>490 && y<540)*musicvol=1;
                        else if(x>300 && x<350 && y>490 && y<540)*musicvol=2;
                        else if(x>350 && x<400 && y>490 && y<540)*musicvol=3;
                        else if(x>400 && x<450 && y>490 && y<540)*musicvol=4;
                        else if(x>450 && x<500 && y>490 && y<540)*musicvol=5;
                        else if(x>500 && x<550 && y>490 && y<540)*musicvol=6;
                        else if(x>550 && x<600 && y>490 && y<540)*musicvol=7;
                        else if(x>600 && x<650 && y>490 && y<540)*musicvol=8;
                        else if(x>650 && x<700 && y>490 && y<540)*musicvol=9;
                        else if(x>700 && x<750 && y>490 && y<540)*musicvol=10;

                        else if(x>200 && x<250 && y>690 && y<740)*soundvol=0;
                        else if(x>250 && x<300 && y>690 && y<740)*soundvol=1;
                        else if(x>300 && x<350 && y>690 && y<740)*soundvol=2;
                        else if(x>350 && x<400 && y>690 && y<740)*soundvol=3;
                        else if(x>400 && x<450 && y>690 && y<740)*soundvol=4;
                        else if(x>450 && x<500 && y>690 && y<740)*soundvol=5;
                        else if(x>500 && x<550 && y>690 && y<740)*soundvol=6;
                        else if(x>550 && x<600 && y>690 && y<740)*soundvol=7;
                        else if(x>600 && x<650 && y>690 && y<740)*soundvol=8;
                        else if(x>650 && x<700 && y>690 && y<740)*soundvol=9;
                        else if(x>700 && x<750 && y>690 && y<740)*soundvol=10;
                    break;
                }
            break;
        }
   }

   balk1.w=(50*(*musicvol));
   balk2.w=(50*(*soundvol));

    SDL_RenderClear(render);

    bild = key[7];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_SetRenderDrawColor(render,255,255,0,255);
    SDL_RenderFillRect(render,&balk1);
    SDL_RenderFillRect(render,&balk2);

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
}
SDL_DestroyTexture(key[7]);
key[7] = NULL;
return 0;
}

int controls(){

int quit=0;
int x,y;

key[8] = loadBild("data/controls.png");
if(key[8]==NULL)printf("SDL Error: %s\n",SDL_GetError());

SDL_Event e;
while(quit==0){
   while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                SDL_DestroyTexture(key[8]);
                key[8] = NULL;
                return 1;
            break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    quit = 1;
                    SDL_DestroyTexture(key[8]);
                    key[8] = NULL;
                break;
                }

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                    case SDL_BUTTON_LEFT:
                        SDL_GetMouseState(&x,&y);
                        if(x>430 && x<580 && y>860 && y<940)quit=1;
                    break;
                }
            break;
        }
   }

    SDL_RenderClear(render);

    bild = key[8];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
}
SDL_DestroyTexture(key[8]);
key[8] = NULL;
return 0;
}

int highscore(){
    int quit=0;
    int x,y;
    int ausgab[6];
    char ausgabe[6][10];

key[1] = loadBild("data/highscore.png");
if(key[1]==NULL)printf("SDL Error: %s\n",SDL_GetError());

FILE* fh = fopen("score/highscore.txt","r");
for(int i = 0; i<5; i++){fgets(ausgabe[i],10,fh);sscanf(ausgabe[i],"%d",&ausgab[i]);}
fclose(fh);

FILE* fa = fopen("score/lastscore.txt","r");
fgets(ausgabe[5],10,fa);sscanf(ausgabe[5],"%d",&ausgab[5]);
fclose(fa);

SDL_Event e;
while(quit==0){
   while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                SDL_DestroyTexture(key[1]);
                key[1] = NULL;
                return 1;
            break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    quit = 1;
                    SDL_DestroyTexture(key[1]);
                    key[1] = NULL;
                break;
                }

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                    case SDL_BUTTON_LEFT:
                        SDL_GetMouseState(&x,&y);
                        if(x>430 && x<580 && y>860 && y<940)quit=1;
                    break;
                }
            break;
        }
   }

    SDL_RenderClear(render);

    bild = key[1];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    if (ausgab[0]<10){
        SDL_Rect textscore;
        get_ttf(650,320,80,30,&ausgab[0], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[0]<100){
        SDL_Rect textscore;
        get_ttf(620,320,80,60,&ausgab[0], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,320,80,90,&ausgab[0], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    if (ausgab[1]<10){
        SDL_Rect textscore;
        get_ttf(650,410,80,30,&ausgab[1], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[1]<100){
        SDL_Rect textscore;
        get_ttf(620,410,80,60,&ausgab[1], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,410,80,90,&ausgab[1], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    if (ausgab[2]<10){
        SDL_Rect textscore;
        get_ttf(650,500,80,30,&ausgab[2], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[2]<100){
        SDL_Rect textscore;
        get_ttf(620,500,80,60,&ausgab[2], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,500,80,90,&ausgab[2], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    if (ausgab[3]<10){
        SDL_Rect textscore;
        get_ttf(650,590,80,30,&ausgab[3], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[3]<100){
        SDL_Rect textscore;
        get_ttf(620,590,80,60,&ausgab[3], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,590,80,90,&ausgab[3], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    if (ausgab[4]<10){
        SDL_Rect textscore;
        get_ttf(650,680,80,30,&ausgab[4], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[4]<100){
        SDL_Rect textscore;
        get_ttf(620,680,80,60,&ausgab[4], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,680,80,90,&ausgab[4], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    if (ausgab[5]<10){
        SDL_Rect textscore;
        get_ttf(650,770,80,30,&ausgab[5], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (ausgab[5]<100){
        SDL_Rect textscore;
        get_ttf(620,770,80,60,&ausgab[5], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(590,770,80,90,&ausgab[5], &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
}
SDL_DestroyTexture(key[1]);
key[1] = NULL;
return 0;
}

int main(int argc, char* args[]){

    srand(time(NULL));

    init();
    load();

    int quit = 0;
    int overquit = 0;
    int x,y;
    int musicvol=7;
    int soundvol=7;
    SDL_Event e;
    Mix_PlayMusic(music, -1);
    char c[6][10];
    char cc[10];
    int folge[6];
    int burntime = 0;

    MENUSTART:

    quit = overquit;

    key[6] = loadBild("data/menu.png");
    if(key[6]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    while(quit==0){
        while(SDL_PollEvent(&e)!=0){
            switch(e.type){
                case SDL_QUIT:
                quit = 1;
                overquit = 1;
                SDL_DestroyTexture(key[6]);
                key[6] = NULL;
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                case SDL_BUTTON_LEFT:
                    SDL_GetMouseState(&x,&y);
                    if(x>330 && x<660 && y>400 && y<490)quit=1;
                    else if(x>330 && x<630 && y>850 && y<930){quit=1; overquit=1;}
                    else if(x>360 && x<600 && y>500 && y<580)overquit=options(&musicvol,&soundvol);
                    else if(x>350 && x<620 && y>580 && y<660)overquit=controls();
                    else if(x>330 && x<640 && y>670 && y<760)overquit=highscore();
                break;
            }
        }
    }

    Mix_VolumeMusic((MIX_MAX_VOLUME/20)*musicvol);
    Mix_VolumeChunk(shotsound, (MIX_MAX_VOLUME/40)*soundvol);
    Mix_VolumeChunk(cubezomb, (MIX_MAX_VOLUME/20)*soundvol);
    Mix_VolumeChunk(smerc, (MIX_MAX_VOLUME/20)*soundvol);
    Mix_VolumeChunk(flamenwerfer, (MIX_MAX_VOLUME/50)*soundvol);
    Mix_VolumeChunk(explosionbum, (MIX_MAX_VOLUME/30)*soundvol);

    SDL_RenderClear(render);

    bild = key[6];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);

    }

    SDL_DestroyTexture(key[6]);
    key[6] = NULL;

    quit=overquit;
    int max,shot,speed,t, hord, score, wep, spawnrate;
    float xto, yto, norm;
    shot=0; speed=0; t=0; hord=70; score=0; wep=0; spawnrate=9;

    for(int i = 0; i<50; i++){
        p[i]=dcreate();
    }

    for(int j = 0; j<100; j++){
        zp[j]=dzcreate();
    }

    for(int j = 0; j<2; j++){zlist();}

    while(quit==0){
        while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                overquit = 1;
            break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                case SDLK_ESCAPE:
                    minidestroy();
                    goto MENUSTART;
                break;

                case SDLK_i:
                    quit = brejk(&wep, &score);
                    if(quit==2){minidestroy(); goto MENUSTART;}
                    overquit = quit;
                break;

                case SDLK_e:
                    quit = brejk(&wep, &score);
                    if(quit==2){minidestroy(); goto MENUSTART;}
                    overquit = quit;
                break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                switch(e.button.button){
                case SDL_BUTTON_LEFT:
                shot=1;
                break;
                }
            break;

            case SDL_MOUSEBUTTONUP:
                switch(e.button.button){
                case SDL_BUTTON_LEFT:
                shot=-1;
                break;
                }
            break;
            }
        }

    if(t<600){
        if(t%hord==0)zlist();
        t++;
    }
    else {
        t=1;
        hord-=spawnrate; spawnrate--; if(spawnrate<0)spawnrate=0;
    }

    gun weapon = gcreate(wep);
    max=weapon.range;

    for(int i =0; i<10; i++){
        if(explode[i].exrect.h!=0){
        explode[i].timer +=1;
        bumbum(&weapon);}
    }

    if(weapon.fire!=3)shoot(&speed,&shot,&weapon);
    else fireshoot(&speed,&shot,&weapon);

    SDL_RenderClear(render);

    bild = key[0];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    for(int i =0; i<50; i++){
        if(p[i].rect.w !=0){
    xto=(B/2)-(p[i].desx+2);xto/=100;
    yto=(H/2)-(p[i].desy+2);yto/=100;
    norm = 1/(sqrt(pow(xto,2)+pow(yto,2)));
    xto*=norm; yto*=norm;
    move(i, xto, yto, &weapon);
    kill(max, &weapon);
    SDL_RenderFillRect(render,&(p[i].rect));
        }
    }

    bild = key[4];
    SDL_Rect mid;
    mid.h=40; mid.w=40; mid.x=(B/2)-20; mid.y=(H/2)-20;
    SDL_RenderCopy(render, bild, NULL, &mid);

    if(burntime==3){
        setonfire();
        burntime=0;
    }
    else burntime++;

    bild = key[0];
    score+=die();
    for(int j =0; j<100; j++){
    if(zp[j].zrect.w!=0){
    if(quit!=1)quit = zmove(j);
    if(zp[j].burning==-1)SDL_RenderCopy(render, bild, NULL, &zp[j].zrect);
    else if(zp[j].burning<1){SDL_SetRenderDrawColor(render,0,150,0,255); SDL_RenderFillRect(render,&(zp[j].zrect));}
    else if(zp[j].burning>0){SDL_SetRenderDrawColor(render,250,150,0,255); SDL_RenderFillRect(render,&(zp[j].zrect));}
    }
    }

    bild = key[2];
    for(int i=0;i<10;i++){
        if(explode[i].exrect.w!=0 && explode[i].timer>29)
        SDL_RenderCopy(render, bild, NULL, &explode[i].exrect);
    }
    for(int i=0;i<45;i++){
        if(burnball[i].frect.w!=0)
        SDL_RenderCopy(render, bild, NULL, &burnball[i].frect);
    }

    if (score<10){
        SDL_Rect textscore;
        get_ttf(970,0,40,30,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (score<100){
        SDL_Rect textscore;
        get_ttf(940,0,40,60,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(910,0,40,90,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

     for(int i =0; i<45; i++){
        burnball[i]=dburncreate();
    }

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
    }

    Mix_PlayChannel(-1,smerc,0);

    if(score>0){
    sprintf(cc,"%d",score);
    FILE* actu = fopen("score/lastscore.txt","w");
    fprintf(actu, "%s",cc);
    fclose(actu);
    FILE* fp = fopen("score/highscore.txt","r");
    for(int h=0; h<6; h++){fgets(c[h],10,fp);}
    fp = fopen("score/highscore.txt","w");
    strcpy(c[5],cc);
    for(int h=0; h<6; h++){sscanf(c[h],"%d",&folge[h]);}

    int temp;
	for (int i=0; i<5; i++) {
		for (int j=0; j<6-i-1;j++) {
			if (folge[j]<folge[j+1]) {
			temp=folge[j];
			folge[j]=folge[j+1];
			folge[j+1]=temp;
			}
		}
	}
	for (int i=0; i<6; i++) {sprintf(c[i],"%d\n",folge[i]);fprintf(fp, "%s",c[i]);}
    fclose(fp);
    }

    minidestroy();
    quit = overquit;

    while (quit==0){
        while(SDL_PollEvent(&e)!=0){
            switch(e.type){
            case SDL_QUIT:
                quit = 1;
                overquit = 1;
            break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_ESCAPE:
                        minidestroy();
                        goto MENUSTART;
                    break;
                }
            break;
            }
        }
    SDL_RenderClear(render);

    bild = key[5];
    SDL_SetRenderDrawColor(render,150,150,150,255);
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    if (score<10){
        SDL_Rect textscore;
        get_ttf(440,420,160,120,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else if (score<100){
        SDL_Rect textscore;
        get_ttf(380,420,160,240,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }
    else{
        SDL_Rect textscore;
        get_ttf(320,420,160,360,&score, &textscore);
        SDL_RenderCopy(render, ttfont, NULL, &textscore);
    }

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
    }

    minidestroy();
    destroy();
    return 0;
}
