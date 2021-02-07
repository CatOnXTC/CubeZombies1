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


typedef struct point{
    SDL_Rect rect;
    int desx, desy, k;
    float vx, vy;
    struct point* next;
} point;

typedef struct zombie{
    SDL_Rect zrect;
    float zvx, zvy;
    float zxmo, zymo;
    int hp, score;
    struct zombie* znext;
} zombie;

typedef struct gun{
    int dmg;
    int del;
    int speed;
    int range;
} gun;

point* anfang = NULL;
point* next = NULL;
zombie* zanfang = NULL;
zombie* znext = NULL;

void init();
void load();
void destroy();
void list(int x, int y);
void kill(int max, gun *g);
void move(point *p, float xmo, float ymo, gun *gp);
void shoot(int *pspeed, int *pshot, gun *g);
int zmove(zombie* z);
int die();
int brejk(int *wep, int *score);
void get_ttf(int x, int y, int h, int w, int *text, SDL_Rect *ttrect);
int options(int *musicvol);
int controls();

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

zombie zcreate(){
    int r = rand()%10;
    zombie a;
    if(r>=0&&r<3){
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

    a.hp=12;
    a.score=1;
    }

    if(r>=3&&r<6){
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
    norm = 0.4/(sqrt(pow(a.zxmo,2)+pow(a.zymo,2)));
    a.zxmo*=norm; a.zymo*=norm;

    a.hp=18;
    a.score=1;
    }

    if(r>=6&&r<9){
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

    a.hp=5;
    a.score=1;
    }

     if(r==9){
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

    a.hp=36;
    a.score=1;
    }
    return a;
}

gun gcreate(int c){
    gun a;
    if(c==1){a.dmg=5;a.del=12;a.range=400;a.speed=10;}
    else if(c==2){a.dmg=20;a.del=45;a.range=550;a.speed=15;}
    else {a.dmg=6;a.del=30;a.range=300;a.speed=8;}
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

    key[0] = NULL;

    key[1] = NULL;

    key[2] = NULL;

    key[4] = loadBild("data/play.png");
    if(key[4]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    key[5] = loadBild("data/score.png");
    if(key[5]==NULL)printf("SDL Error: %s\n",SDL_GetError());

    font = TTF_OpenFont("data/Zombie Queen.ttf", 72);

    music = Mix_LoadMUS("data/Sister.mp3");
    if( music == NULL )printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
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

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}

void minidestroy(){
    point *p;
    zombie *zp;

    if(anfang!=NULL){
        p=anfang->next;
        while(p!=NULL){
          anfang->next=p->next;
          free(p);
          p=anfang->next;
        }
        free(anfang);
        anfang=NULL;
    }

    if(zanfang!=NULL){
        zp=zanfang->znext;
        while(zp!=NULL){
          zanfang->znext=zp->znext;
          free(zp);
          zp=zanfang->znext;
        }
        free(zanfang);
        zanfang=NULL;
    }
}

void move(point* p, float xmo, float ymo, gun *gp){
    p->vx -=(xmo*(gp->speed));
    p->vy -=(ymo*(gp->speed));
    p->rect.x = p->vx;
    p->rect.y = p->vy;
    p->k+=(gp->speed);
}

int zmove(zombie* z){
    if(z->zrect.x>(B/2)-50 && z->zrect.x<(B/2)+10 && z->zrect.y>(H/2)-50 && z->zrect.y<(H/2)+10) return 1;
    else{
    z->zvx+=(z->zxmo);
    z->zvy+=(z->zymo);
    z->zrect.x = z->zvx;
    z->zrect.y = z->zvy;
    return 0;
    }
}

void list(int x, int y){

    point* p;

    if (anfang == NULL){
        if((anfang = malloc(sizeof(point)))==NULL) return;

        *anfang=create(x,y);
        anfang->next=NULL;
    }
    else{
        p=anfang;
        while(p->next!=NULL)p=p->next;
        if((p->next=malloc(sizeof(point)))==NULL) return;

        p=p->next;
        *p=create(x,y);
        p->next=NULL;
    }
}

void zlist(){

    zombie* p;

    if (zanfang == NULL){
        if((zanfang = malloc(sizeof(zombie)))==NULL) return;

        *zanfang=zcreate();
        zanfang->znext=NULL;
    }
    else{
        p=zanfang;
        while(p->znext!=NULL)p=p->znext;
        if((p->znext=malloc(sizeof(zombie)))==NULL) return;

        p=p->znext;
        *p=zcreate();
        p->znext=NULL;
    }
}

void kill(int max, gun *g){
    zombie *zp;
    point *p, *pp;
    if(zanfang==NULL){
    if(anfang!=NULL){
    p=anfang;
    LOOP:
        if(p->k>max-1){
            if(p->next==NULL){free(anfang); anfang=NULL; free(p); return;}
            else{
            anfang=p->next;
            free(p);
            p=anfang;
            goto LOOP;
            }
        }
        pp=p; p=p->next;
        while(p!=NULL){
            if(p->k>max-1){
                pp->next=p->next;
                free(p);
                p=pp->next;
            }
            else{pp=p; p=p->next;}
        }
    }
    }
    else {
    zp=zanfang;
    while(zp!=NULL){
        if(anfang!=NULL){
    p=anfang;
    LOOPP:
        if(p->k>max-1 || (p->rect.x>zp->zrect.x-5 && p->rect.x<zp->zrect.x+(zp->zrect.w-5) && p->rect.y>zp->zrect.y-5 && p->rect.y<zp->zrect.y+(zp->zrect.h-5))){
            if(p->rect.x>zp->zrect.x-5 && p->rect.x<zp->zrect.x+(zp->zrect.w-5) && p->rect.y>zp->zrect.y-5 && p->rect.y<zp->zrect.y+(zp->zrect.h-5)) zp->hp-=g->dmg;
            if(p->next==NULL){free(anfang); anfang=NULL; free(p); return;}
            else{
            anfang=p->next;
            free(p);
            p=anfang;
            goto LOOPP;
            }
        }
        pp=p; p=p->next;
        while(p!=NULL){
            if(p->k>max-1 || (p->rect.x>zp->zrect.x-5 && p->rect.x<zp->zrect.x+(zp->zrect.w-5) && p->rect.y>zp->zrect.y-5 && p->rect.y<zp->zrect.y+(zp->zrect.h-5))){
                if(p->rect.x>zp->zrect.x-5 && p->rect.x<zp->zrect.x+(zp->zrect.w-5) && p->rect.y>zp->zrect.y-5 && p->rect.y<zp->zrect.y+(zp->zrect.h-5)) zp->hp-=g->dmg;
                pp->next=p->next;
                free(p);
                p=pp->next;
            }
            else{pp=p; p=p->next;}
        }
    }
    zp=zp->znext;
    }
    }
    return;
}

int die(){
    int scor;
    zombie *zp, *zpp;
    if(zanfang!=NULL){
        zp = zanfang;
            if(zp->hp<1){
                scor=zp->score;
                if(zp->znext==NULL){free(zanfang); zanfang=NULL; free(zp); return scor;}
                else{
                zanfang=zp->znext;
                free(zp);
                zp=zanfang;
                return scor;
                }
            }
           zpp=zp; zp=zp->znext;
        while(zp!=NULL){
            if(zp->hp<1){
                scor=zp->score;
                zpp->znext=zp->znext;
                free(zp);
                zp=zpp->znext;
                return scor;
            }
            else{zpp=zp; zp=zp->znext;}
        }
    }
    return 0;
}

void shoot(int *pspeed, int *pshot, gun *g){
    int x, y;
    if(*pshot==1){
        if(*pspeed%g->del==0){
        SDL_GetMouseState(&x,&y);
        list(x-3,y-3);
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

int brejk(int *wep, int *score){
int quit=0;
int x,y;
point *p;
zombie *zp;

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
                        if(x>500&&x<900&&y>50&&y<350 && *score>2)*wep=1;
                        if(x>100&&x<500&&y>350&&y<650 && *score>6)*wep=2;
                    break;
                }
            break;
            }
   }
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render,50,210,50,255);
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, NULL, NULL, NULL);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    p=anfang;
    while(p!=NULL){
    SDL_RenderFillRect(render,&(p->rect));
    p=p->next;
    }

    SDL_SetRenderDrawColor(render,0,150,0,255);
    zp=zanfang;
    while(zp!=NULL){
    SDL_RenderFillRect(render,&(zp->zrect));
    zp=zp->znext;
    }

    bild = key[3];
    SDL_RenderCopy(render, bild, NULL, &invent);

    SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0xFF, 0xFF);
    SDL_Rect outline;
    outline.h=299; outline.w=398;
    if(*wep==0){outline.x=102; outline.y=52;}
    if(*wep==1){outline.x=500; outline.y=52;}
    if(*wep==2){outline.x=102; outline.y=351;}
    SDL_RenderDrawRect(render, &outline);

    if (*score==0){
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

int options(int *musicvol){

int quit=0;
int x,y;

key[7] = loadBild("data/options.png");
if(key[7]==NULL)printf("SDL Error: %s\n",SDL_GetError());

SDL_Rect balk1;
balk1.h=50;
balk1.x=250;
balk1.y=490;

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
                    break;
                }
            break;
        }
   }

   balk1.w=(50*(*musicvol));

    SDL_RenderClear(render);

    bild = key[7];
    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, bild, NULL, NULL);

    SDL_SetRenderDrawColor(render,255,255,0,255);
    SDL_RenderFillRect(render,&balk1);

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

int main(int argc, char* args[]){

    srand(time(NULL));

    init();
    load();

    int quit = 0;
    int overquit = 0;
    int x,y;
    int musicvol=7;
    SDL_Event e;
    Mix_PlayMusic(music, -1);

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
                    else if(x>360 && x<600 && y>500 && y<580)overquit=options(&musicvol);
                    else if(x>350 && x<620 && y>580 && y<660)overquit=controls();
                break;
            }
        }
    }

    Mix_VolumeMusic((MIX_MAX_VOLUME/20)*musicvol);

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
    point *p;
    zombie *zp;
    int max,shot,speed,t, hord, score, wep, spawnrate;
    float xto, yto, norm;
    shot=0; speed=0; t=0; hord=70; score=0; wep=0; spawnrate=8;

    zlist();
    zlist();

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

    shoot(&speed,&shot,&weapon);

    SDL_RenderClear(render);


    SDL_RenderSetViewport(render, NULL);
    SDL_RenderCopy(render, NULL, NULL, NULL);

    SDL_SetRenderDrawColor(render,0,0,0,255);
    p=anfang;
    while(p!=NULL){
    xto=(B/2)-(p->desx+2);xto/=100;
    yto=(H/2)-(p->desy+2);yto/=100;
    norm = 1/(sqrt(pow(xto,2)+pow(yto,2)));
    xto*=norm; yto*=norm;
    move(&*p, xto, yto, &weapon);
    kill(max, &weapon);
    SDL_RenderFillRect(render,&(p->rect));
    p=p->next;
    }

    bild = key[4];
    SDL_Rect mid;
    mid.h=40; mid.w=40; mid.x=(B/2)-20; mid.y=(H/2)-20;
    SDL_RenderCopy(render, bild, NULL, &mid);

    SDL_SetRenderDrawColor(render,0,150,0,255);
    zp=zanfang;
    score+=die();
    while(zp!=NULL){
    if(quit!=1)quit = zmove(&*zp);
    SDL_RenderFillRect(render,&(zp->zrect));
    zp=zp->znext;
    }

    if (score==0){
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

    SDL_Delay(1000/60);
    SDL_RenderPresent(render);
    }

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

    if (score==0){
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
