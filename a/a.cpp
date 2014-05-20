#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "PixelWorld.h"
#include<string>
#define COL_START 6
#define LIN_MAX 19
#define COL_MAX 19
#include<cstdio>
#define HP1 100
#include<ctime>
#include "alex.h"
/***
 NOTE: Player1 is really Player1
       Player is actually Player2
 TO DO LATER: A good way to make the code more simple is to make players objects...
***/
player player1,player2;
Mix_Chunk *sound;
Mix_Music *music=NULL;
SDL_Color textColor={0,900,200}, color1={0,900,0},color2={450,0,0},color3{240,240,70};
TTF_Font *font=NULL;
SDL_Surface *message=NULL,*clear=NULL,*burn=NULL,*player2=NULL;
char pp,mute=0;
Uint8 *keystates=SDL_GetKeyState(NULL);
int time_ex,beg1,beg2,end1,end2,t,t1,t2,flag_player;
int power1,power2,power3,power4;
player1.hp=HP1,player2.hp=HP1,player2.mana=HP1,player1.mana=HP1;
int up,down,right,left,atack_left,atack_right,atack1_left,atack1_right;
player1.lin=LIN_MAX/2,player1.col=COL_MAX/2-5,player2.lin=LIN_MAX/2,player2.col=COL_MAX/2+5;
player1.money,player2.money,player2.xp,player1.xp;
int v[LIN_MAX+3][COL_MAX+3],obs[LIN_MAX+3][COL_MAX+3];
SDL_Surface *image=NULL;
SDL_Surface *screen=NULL;
SDL_Surface *player=NULL;
SDL_Surface *player1=NULL;
SDL_Surface *background=NULL;
char file[100];
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
 SDL_Rect offset;
 offset.x=x;
 offset.y=y;
 SDL_BlitSurface(source,NULL,destination,&offset);
}
int max(int a,int b)
{
 if(a>b)
    return a;
 else
    return b;
}
int min(int a,int b)
{
 if(a<b)
    return a;
 else
    return b;
}
void put_back(int l,int c)
{
 switch(v[l][c])
	   {
	    case 1: {
			   image=SDL_LoadBMP("tree.bmp");
			   break;
			  }
	    case 2: {
	    	        image=SDL_LoadBMP("grass.bmp");
			   break;
			  }
	    case 3: {
	    	        image=SDL_LoadBMP("bricks.bmp");
	    	        break;
			  }
	   }
 apply_surface((c+COL_START)*40,l*40,image,screen);
}
void load_level(char *name)
{
 int n=strlen(name),i,j;
 strcpy(file,name);
 file[n]='.';
 file[n+1]='l';
 file[n+2]='v';
 file[n+3]='l';
 file[n+4]=NULL;
 FILE *fin=fopen(file,"r");
 for(i=0;i<=LIN_MAX;i++)
     for(j=0;j<=COL_MAX;j++)
         fscanf(fin,"%d",&v[i][j]);
fclose(fin);
}
void print_level()
{
 int i,j,i1=1,j1=1;
 for(i=0,i1=0;i1<=LIN_MAX;i+=40,i1++)
     for(j=0,j1=0;j1<=COL_MAX;j+=40,j1++)
         {
         	if(obs[i1][j1]<=1){
          switch(v[i1][j1])
          {
           case 1:  {image=SDL_LoadBMP("tree.bmp");
                     obs[i1][j1]=1;
                     break;}
           case 2:  {image=SDL_LoadBMP("grass.bmp");  break;}
           case 3:  {image=SDL_LoadBMP("bricks.bmp");  break;}
           case 4:  {image=SDL_LoadBMP("fire.bmp");  break;}
           case 5:  {image=SDL_LoadBMP("bricks.bmp");
                     obs[i1][j1]=1;
                     break;}
          }
          //SDL_BlitSurface(image,NULL,screen,NULL);
          apply_surface(j+COL_START*40,i,image,screen);}
         }
}
void print_background()
{
 background=SDL_LoadBMP("background1.bmp");
 int i,j;
 for(i=0;i<=LIN_MAX;i++)
     {
	 apply_surface((COL_MAX+COL_START+1)*40,i*40,background,screen);
	 apply_surface((COL_START-1)*40,i*40,background,screen);
	}
}
void get_burn(int i,int j)
{
 burn=SDL_LoadBMP("burn.bmp");
}
void welcome_message()
{
 SDL_Surface *message1=TTF_RenderText_Solid(font,"Welcome to Pixel World:Duel Mode! ",color2);
 apply_surface(280,160,message1,screen);
 SDL_FreeSurface(message1);
 SDL_Surface *message2=TTF_RenderText_Solid(font,"Code:Alex Cioltan and Stefan Enescu ",color2);
 apply_surface(280,200,message2,screen);
 SDL_Surface *message3=TTF_RenderText_Solid(font,"Graphics: Stefan Enescu,",color2);
 apply_surface(280,240,message3,screen);
 SDL_Surface *message4=TTF_RenderText_Solid(font,"Alex Cioltan, Victor Ionescu and",color2);
 apply_surface(280,280,message4,screen);
 message4=TTF_RenderText_Solid(font,"Duta Alexandru",color2);
 apply_surface(280,320,message4,screen);
 SDL_Flip(screen);
 while(getkey(VK_RETURN)==0);
}
void put_arena_wall()
{
 for(int i=1;i<=LIN_MAX-1;i++)
	{
	 v[i][COL_MAX/2]=5;
	}
}
void clear_arena_wall()
{
 for(int i=1;i<=LIN_MAX-1;i++)
	{
	 v[i][COL_MAX/2]=2;
	 obs[i][COL_MAX/2]=0;
	}
}
int main( int argc, char* args[] )
{
 SDL_Init(SDL_INIT_EVERYTHING);
 Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 1, 4096 );
 //screen=SDL_SetVideoMode((LIN_MAX+1)*40,(COL_MAX+1)*40,32,SDL_FULLSCREEN/*SDL_SWSURFACE*/);
 screen=SDL_SetVideoMode(0,0,32,SDL_FULLSCREEN);
 TTF_Init();
 font=TTF_OpenFont("font2.ttf",40);
 music=Mix_LoadMUS("bck.wav");
 clear=SDL_LoadBMP("wooden_background.bmp");
 apply_surface(0,0,clear,screen);
 if(mute==0)
    Mix_PlayMusic(music, -1 );
 load_level("a");
 print_level();
 //welcome_message();
 clear=SDL_LoadBMP("clear.bmp");
 print_background();
 burn=SDL_LoadBMP("fire.bmp");
 background=SDL_LoadBMP("name_background.bmp");
 message=TTF_RenderText_Solid(font,"Player 2",color1);
 apply_surface((COL_MAX+COL_START+2)*40,0,background,screen);
 apply_surface((COL_MAX+COL_START+2)*40+5,0,message,screen);
 SDL_Flip(screen);
 message=TTF_RenderText_Solid(font,"Player 1",color1);
 apply_surface((0)*40,0,background,screen);
 apply_surface((0)*40+5,0,message,screen);
 SDL_Flip(screen);
 player=SDL_LoadBMP("warrior_on_grass.bmp");
 player1=SDL_LoadBMP("warrior1_on_grass.bmp");
 obs[player2.lin][player2.col]=2;
 obs[player1.lin][player1.col]=3;
 print_hp(player2.hp,1,COL_MAX+COL_START+2);
 print_hp(player1.hp,1,0);
 print_mana(player2.mana,2,COL_MAX+COL_START+2);
 print_mana(player2.mana,2,0);
 time_ex=0;
 if(mute<=1)
    sound=Mix_LoadWAV("hit.wav");
 print_level();
 int start_t=0;
 put_arena_wall();
 player1_load_save("player1");
 player2_load_save("player2");
 while(keystates[SDLK_ESCAPE]==NULL && player1.hp>0 && player2.hp>0)
       {flag_player=0;
	   t=time(NULL);
        if(t-t1>=2)
           {
            start_t++;
            if(start_t>=4)
               {
			    clear_arena_wall();
               }
            t1=t;
            if(player2.mana<=99)
			{
			 player2.mana++;
			 print_mana(player2.mana,2,COL_MAX+COL_START+2);
			}
		  if(player1.mana<=99)
			{
			 player1.mana++;
			 print_mana(player1.mana,2,0);
			}
            //load_level("a");
            print_level();
            player2.permission=player1.permission=0;
           }
	   if(t-t2>=1)
		 {
		  t2=t;
		  player2.permission_basic=player1.permission_basic=0;
		 }
        up=keystates[SDLK_UP];
        down=keystates[SDLK_DOWN];
        right=keystates[SDLK_RIGHT];
        left=keystates[SDLK_LEFT];
        atack_left=keystates[SDLK_RCTRL];
        atack_right=keystates[SDLK_RCTRL];
        power1=keystates[SDLK_u];
        power2=keystates[SDLK_i];
        power3=keystates[SDLK_o];
        power4=keystates[SDLK_p];
        atack1_left=keystates[SDLK_z];
        atack1_right=keystates[SDLK_z];
        if(power1==1 && player2.mana>=10 && player2.hp<=90)
           {
            player2.mana-=10;
            player2.hp+=10;
            print_mana(player2.mana,2,COL_MAX+COL_START+2);
            print_hp(player2.hp,1,COL_MAX+COL_START+2);
           }
        if(power2==1 && player2.mana>=30 && player2.permission==0)
           {
            player2.mana-=30;
            player2.permission=1;
            if(max(player1.lin,player2.lin)-min(player1.lin,player2.lin)<=2 && max(player1.col,player2.col)-min(player1.col,player2.col)<=2)
               {
                player1.hp-=40+player2.fire_dmg*3/10-player1.fire_res*3/10;
                print_hp(player1.hp,1,0);
               }
            if(player2.lin-2<=0)
               beg1=1;
            else
               beg1=player2.lin-2;
            if(player2.lin+2>=LIN_MAX)
               end1=LIN_MAX;
            else
               end1=player2.lin+2;
            if(player2.col-2<=0)
               beg2=1;
            else
               beg2=player2.col-2;
            if(player2.col+2>=COL_MAX)
               end2=COL_MAX;
            else
               end2=player2.col+2;
            for(int i=beg1;i<=end1;i++)
                for(int j=beg2;j<=end2;j++)
                    {
                     if(obs[i][j]==0)
                        {
                         //get_burn(i,j);
                         //burn=SDL_LoadBMP("fire.bmp");
                         apply_surface((j+COL_START)*40,i*40,burn,screen);
                        }
                    }
            print_mana(player2.mana,2,COL_MAX+COL_START+2);
           }
        if(power3==1 && player2.hp>10)
           {
            player2.hp-=10;
            player2.mana+=10;
            print_mana(player2.mana,2,COL_MAX+COL_START+2);
            print_hp(player2.hp,1,COL_MAX+COL_START+2);
           }
        if(atack_left==1 && player2.permission_basic==0)
           {
            player=SDL_LoadBMP("warrior_on_grass_left.bmp");
            if(obs[player2.lin][player2.col-1]==3)
               {
			 player2.permission_basic=1;
			 player1.hp-=10+player2.attack/10-player1.block/10;
                print_hp(player1.hp,1,0);
                Mix_PlayChannel(-1, sound, 0);
               }
           }
        if(atack_right==1 && player2.permission_basic==0)
           {
            player=SDL_LoadBMP("warrior_on_grass.bmp");
            if(obs[player2.lin][player2.col+1]==3)
               {
			    player2.permission_basic=1;
			    player1.hp-=10+player2.attack/10-player1.block/10;
                print_hp(player1.hp,1,0);
                Mix_PlayChannel(-1, sound, 0);
               }
           }
        if(up==1 && player2.lin>1)
           {
            time_ex++;
            if(obs[player2.lin-1][player2.col]==0)
               {
                obs[player2.lin][player2.col]=0;
			 put_back(player2.lin,player2.col);
                player2.lin--;
                obs[player2.lin][player2.col]=2;
               }
           }
        if(down==1 && player2.lin<COL_MAX)
           {
		  time_ex++;
            if(obs[player2.lin+1][player2.col]==0)
               {
                obs[player2.lin][player2.col]=0;
			 put_back(player2.lin,player2.col);
                player2.lin++;
                obs[player2.lin][player2.col]=2;
               }
           }
        if(left==1 && player2.col>1)
           {
		  time_ex++;
            if(obs[player2.lin][player2.col-1]==0)
               {
                obs[player2.lin][player2.col]=0;
			 put_back(player2.lin,player2.col);
                player2.col--;
                obs[player2.lin][player2.col]=2;
                player=SDL_LoadBMP("warrior_on_grass_left.bmp");
               }
           }
        if(right==1 && player2.col<LIN_MAX)
           {
            time_ex++;
            if(obs[player2.lin][player2.col+1]==0)
               {
                obs[player2.lin][player2.col]=0;
			 put_back(player2.lin,player2.col);
                player2.col++;
                obs[player2.lin][player2.col]=2;
                player=SDL_LoadBMP("warrior_on_grass.bmp");
               }
           }
        apply_surface((player2.col+COL_START)*40,player2.lin*40,player,screen);
        up=keystates[SDLK_w];
        down=keystates[SDLK_s];
        right=keystates[SDLK_d];
        left=keystates[SDLK_a];
        power1=keystates[SDLK_1];
        power2=keystates[SDLK_2];
        power3=keystates[SDLK_3];
        power4=keystates[SDLK_4];
        if(power1==1 && player1.mana>=10 && player1.hp<=90)
           {
            player1.mana-=10;
            player1.hp+=10;
            print_mana(player1.mana,2,0);
            print_hp(player1.hp,1,0);
           }
        if(power2==1 && player1.mana>=30 && player1.permission==0)
           {
            player1.mana-=30;
            player1.permission=1;
            if(max(player2.lin,player1.lin)-min(player1.lin,player2.lin)<=2 && max(player1.col,player2.col)-min(player1.col,player2.col)<=2)
               {
                player2.hp-=40+player1.fire_dmg*3/10-player2.fire_res*3/10;
                print_hp(player2.hp,1,COL_MAX+COL_START+2);
               }
            if(player1.lin-2<=0)
               beg1=1;
            else
               beg1=player1.lin-2;
            if(player1.lin+2>=LIN_MAX)
               end1=LIN_MAX;
            else
               end1=player1.lin+2;
            if(player1.col-2<=0)
               beg2=1;
            else
               beg2=player1.col-2;
            if(player1.col+2>=COL_MAX)
               end2=COL_MAX;
            else
               end2=player1.col+2;
            for(int i=beg1;i<=end1;i++)
                for(int j=beg2;j<=end2;j++)
                    {
                     if(obs[i][j]==0)
                        {
                         //get_burn(i,j);
                         //burn=SDL_LoadBMP("fire.bmp");
                         apply_surface((j+COL_START)*40,i*40,burn,screen);
                        }
                    }
            print_mana(player1.mana,2,0);
           }
        if(power3==1 && player1.hp>10)
           {
            player1.hp-=10;
            player1.mana+=10;
            print_mana(player1.mana,2,0);
            print_hp(player1.hp,1,0);
           }
        if(atack1_left==1 && player1.permission_basic==0 && flag_player==0)
           {
            if(obs[player1.lin][player1.col-1]==2)
               {
			 player1.permission_basic=1;
                player1=SDL_LoadBMP("warrior1_on_grass_left.bmp");
			 player2.hp-=10+player1.attack/10-player2.block/10;
                //SDL_FreeSurface(player);
                print_hp(player2.hp,1,COL_MAX+COL_START+2);
                /*player2=SDL_LoadBMP("hit_warrior_on_grass.bmp");
                apply_surface(player2.col*40,player2.lin*40,player,screen);*/
                SDL_Flip(screen);
                Mix_PlayChannel(-1, sound, 0);
               }
           }
        if(atack1_right==1 && player1.permission_basic==0 && flag_player==0)
           {
            if(obs[player1.lin][player1.col+1]==2)
               {
			    player1.permission_basic=1;
                player1=SDL_LoadBMP("warrior1_on_grass.bmp");
			    player2.hp-=10+player1.attack/10-player2.block/10;
                print_hp(player2.hp,1,COL_MAX+COL_START+2);
                //SDL_FreeSurface(player);
                player2=SDL_LoadBMP("hit_warrior_on_grass.bmp");
                /*apply_surface(player2.col*40,player2.lin*40,player,screen);
                SDL_Flip(screen);*/
                Mix_PlayChannel(-1, sound, 0);
               }
           }
        if(up==1 && player1.lin>1)
           {
            time_ex++;
            if(obs[player1.lin-1][player1.col]==0)
               {
                obs[player1.lin][player1.col]=0;
			    put_back(player1.lin,player1.col);
                player1.lin--;
                obs[player1.lin][player1.col]=3;
               }
           }
        if(down==1 && player1.lin<COL_MAX)
           {
		  time_ex++;
            if(obs[player1.lin+1][player1.col]==0)
               {
                obs[player1.lin][player1.col]=0;
			    put_back(player1.lin,player1.col);
                player1.lin++;
                obs[player1.lin][player1.col]=3;
               }
           }
        if(left==1 && player1.col>1)
           {
            time_ex++;
            if(obs[player1.lin][player1.col-1]==0)
			{
			 obs[player1.lin][player1.col]=0;
			 put_back(player1.lin,player1.col);
                player1.col--;
                obs[player1.lin][player1.col]=3;
                player1=SDL_LoadBMP("warrior1_on_grass_left.bmp");
               }
           }
        if(right==1 && player1.col<LIN_MAX)
           {
            time_ex++;
            if(obs[player1.lin][player1.col+1]==0)
               {
                obs[player1.lin][player1.col]=0;
			 put_back(player1.lin,player1.col);
                player1.col++;
                obs[player1.lin][player1.col]=3;
                player1=SDL_LoadBMP("warrior1_on_grass.bmp");
               }
           }
        SDL_Delay(50);
        apply_surface((player1.col+COL_START)*40,player1.lin*40,player1,screen);
        SDL_Flip(screen);
        SDL_PumpEvents();
       }
 if(player2.hp>player1.hp)
    {
     player1=SDL_LoadBMP("dead_warrior1_on_grass.bmp");
     apply_surface((player1.col+COL_START)*40,player1.lin*40,player1,screen);
     message=TTF_RenderText_Solid(font,"Player 2 wins!",textColor);
     player2.money+=player2.hp+player2.mana+player1.money/50;
     player1.money+=player2.money/75+20;
     player2.xp+=100;
     player1.xp+=20;
    }
 else
    if(player2.hp!=player1.hp)
       {
        player=SDL_LoadBMP("dead_warrior_on_grass.bmp");
        apply_surface((player2.col+COL_START)*40,player2.lin*40,player,screen);
        message=TTF_RenderText_Solid(font,"Player 1 wins!",textColor);
        player1.money+=player1.hp+player1.mana+player2.money/50;
        player2.money+=player1.money/75+20;
        player1.xp+=100;
        player2.xp+=20;
       }
    else
       {
        player1=SDL_LoadBMP("dead_warrior1_on_grass.bmp");
        apply_surface((player1.col+COL_START)*40,player1.lin*40,player1,screen);
        player=SDL_LoadBMP("dead_warrior_on_grass.bmp");
        apply_surface((player2.col+COL_START)*40,player2.lin*40,player,screen);
	    message=TTF_RenderText_Solid(font,"Round draw!",textColor);
        int cash=(player2.money+player1.money)/2/25;
        player1.money+=cash;
        player2.money+=cash;
        player2.xp+=50;
        player1.xp+=50;
       }
 player1.save_player1("player1");
 player2.save_player("player2");
 Mix_CloseAudio();
 Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096);
 sound=Mix_LoadWAV("win.wav");
 Mix_PlayChannel(-1,sound,0);
 apply_surface(((COL_MAX+COL_START)/2-1)*40,(LIN_MAX/2-1)*40,message,screen);
 SDL_Flip(screen);
 SDL_Delay(2000);
 TTF_CloseFont(font);
 SDL_Quit();
return 0;
}
