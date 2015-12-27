#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include<string>
#include <sstream>

#include <SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

using namespace std;

 typedef enum {LEFT,RIGHT}BUTTONPOS;//枚举按钮在精灵图中的编号
 typedef enum {a,b,c,d,e,f,g,OTHER}BUTTONFLAG;//枚举按钮
 
 //屏幕尺寸
 #define SCREENWIDTH  555
 #define SCREENHEIGH  659
  #define SCREENWIDTH2  555
 #define SCREENHEIGH2  200
 
 //定义各个按钮的位置
int position[8][4] = {{45,170,193,147},{289,170,193,147},
								{45,321,193,147},{289,321,193,147},
								{45,460,193,147},{289,460,193,147},
								{220,530,100,30},{450,620,100,30}};

 SDL_Rect pos[9] = {{45,170,193,147},{289,170,193,147},
								{45,321,193,147},{289,321,193,147},
								{45,460,193,147},{289,460,193,147},
 {220,530,100,30},{450,620,100,30},{220,570, 100,30}};

 //SDL_Rect pos[8] = {{45,170,193,147},{289,170,193,147},
								//{45,321,193,147},{289,321,193,147},
								//{45,460,193,147},{289,460,193,147},
								//{0,0,100,30},{455,0,100,30}};

 SDL_Window *Init()
{
	SDL_Window *pScreen = NULL;
	//SDL初始化
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf ("SDL initialize fail:%s\n", SDL_GetError());
		return NULL;
	}

	//创建窗口（窗体名称，在屏幕中显示的水平位置，垂直位置，窗体宽，高，可视）
	pScreen = SDL_CreateWindow("Hit the mouse",400,100 , SCREENWIDTH,SCREENHEIGH, SDL_WINDOW_SHOWN);  
    if (NULL == pScreen)//检测是否初始化成功  
        printf ("SDL_CreateWindow fail:%s\n", SDL_GetError());

    if (TTF_Init() == -1 )//检测字体是否初始化成功   
		printf ("TTF_Init fail:%s\n", SDL_GetError());

	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1)
		printf ("Mix_Init fail:%s\n", SDL_GetError());

	return pScreen;
}
  SDL_Window *Init2()
{
	SDL_Window *pScreen = NULL;
	//SDL初始化
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf ("SDL initialize fail:%s\n", SDL_GetError());
		return NULL;
	}

	//创建窗口（窗体名称，在屏幕中显示的水平位置，垂直位置，窗体宽，高，可视）
	pScreen = SDL_CreateWindow("SCORE",400,300 , SCREENWIDTH2,SCREENHEIGH2, SDL_WINDOW_SHOWN);  
    if (NULL == pScreen)//检测是否初始化成功  
        printf ("SDL_CreateWindow fail:%s\n", SDL_GetError());

    if (TTF_Init() == -1 )//检测字体是否初始化成功   
		printf ("TTF_Init fail:%s\n", SDL_GetError());

	return pScreen;
}

//释放
void Quit()
{
	TTF_Quit();	//释放字体

	SDL_Quit();	//退出SDL
}

//载入图片
SDL_Texture *Load_image(char *filename,SDL_Renderer *render)
{
	SDL_Surface *img = NULL;
	SDL_Texture *texture = NULL;

	img=IMG_Load(filename);//读入图片，放入临时变量

	if (img == NULL)//检测图片是否加载成功
		printf ("IMG_Load fail:%s", SDL_GetError());

	texture = SDL_CreateTextureFromSurface(render, img);//若成功，载入

	if (texture == NULL) 
        printf("SDL_CreateTextureFromSurface fail:%s", SDL_GetError());

	//释放临时变量
	SDL_FreeSurface(img);

	return texture;
}

//加载字体
SDL_Texture* RenderText(SDL_Renderer * render,string message, string fontFile, SDL_Color color, int fontSize)
{
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);//打开字体文件，及字体大小
    if (font == nullptr)
        throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);//设置字体颜色
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

//复制渲染
void Copy_render(SDL_Renderer * render, SDL_Texture *texture, int x = 0, int y = 0, int w = SCREENWIDTH, int h = SCREENHEIGH)
{
	double angle = 0.0;//旋转角
	int p_x = 0;//圆心
	int p_y = 0;

	SDL_Rect rect = {x, y, w, h};//原矩形
	SDL_Point center = {p_x, p_y};

	//以原矩形的圆形为中心，放置
	SDL_RenderCopyEx(render, texture, NULL, &rect, angle, &center, SDL_FLIP_NONE);
}



 SDL_Texture *gpBackground = NULL;//背景
 SDL_Texture *gpFM = NULL;
 SDL_Texture *gpButton = NULL;
 SDL_Texture *gpStart=NULL;
 SDL_Texture *gpQuit=NULL;
 SDL_Texture *gpTitle=NULL;
 SDL_Texture *gpBumb=NULL;
 SDL_Texture *gpRad=NULL;
 SDL_Texture *word = NULL;
 SDL_Texture *word2 = NULL;
 Mix_Music *Music1 = NULL;
 Mix_Music *Music2 = NULL;



 SDL_Event myEvent;//事件
 
 int isOnButton(int aX,int aY,int aIndex);

 int main(int argc,char *argv[])
 {
     int quit = 1,k;
     int x,y;
	 time_t t;
     BUTTONFLAG selected = OTHER;//按钮选中标志
     int current=-1;
	 int i = 0;

	 stringstream stream;
	 string str;
	 /*stream << i;
	 stream >> str; */

	 //SDL初始化
	SDL_Window *gpScreen = Init();


	//初始化窗体
	SDL_Renderer *render = SDL_CreateRenderer(gpScreen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//窗体背景色
	SDL_SetRenderDrawColor(render,255,255,255,255);

	Music1=Mix_LoadMUS("death_box.ogg");
	Music2=Mix_LoadMUS("mis.wav");

     //载入各个图片
     gpBackground = Load_image("bground_01.jpg",render);
	 gpTitle = Load_image("1_03.png",render);
	 gpFM = Load_image("fengmian.png",render);

	 SDL_RenderClear(render);
	 Copy_render(render, gpFM, 0, 0, SCREENWIDTH,SCREENHEIGH);//放置设置
	 Copy_render(render, gpTitle, 120, 150, 320,200);
	 
	 gpButton = Load_image("hit.png",render);

     gpStart = Load_image("start.png",render);
	 Copy_render(render, gpStart,position[6][0],position[6][1],position[6][2],position[6][3]);//放置设置

	 gpQuit = Load_image("quit_03.png",render);
	 Copy_render(render, gpQuit, 220, 570, 100,30);//放置设置
	 SDL_RenderPresent(render);

	 gpBumb=Load_image("zhong.png",render);

	 gpRad=Load_image("00_03.png",render);

     if (!gpBackground||!gpButton)
	 {
         exit(0);
     }

	 while(quit)
	 {
		 while(SDL_PollEvent(&myEvent))
		 {
			 switch (myEvent.type)
			 {
			 case SDL_MOUSEBUTTONDOWN:
				 switch(myEvent.button.button)
				 {
				 case SDL_BUTTON_LEFT:
					 x=myEvent.button.x;
					 y=myEvent.button.y;
					  if (isOnButton(x,y,6)==1)
                     {
						 selected = g;
                         quit=0;
                     }

					  if (isOnButton(x,y,8)==1)
                     {
                         Quit();
                     }
				 }
			 }
		 }
	 }
	
     //事件处理
     while (!quit)
     {
		 /*stream << i;
	     stream >> str; */

		srand((unsigned)time(&t));
		k=rand()%6;
		if(k<6)
		{ 
			SDL_RenderClear(render);
			Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
			Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
			Copy_render(render, gpRad, position[k][0], position[k][1], position[k][2],position[k][3]);//放置设置

			//SDL_Color color = {0, 0, 0};//颜色
			///*word = RenderText(render,str, "simfang.ttf", color, 64);*/
			//word = RenderText(render,"SCORE:"+i, "simfang.ttf", color, 64);
			//Copy_render(render, word, 0,0,100,30);
			SDL_RenderPresent(render);
		}
		
         while (SDL_PollEvent(&myEvent))
         {

			 /*stream << i;
			 stream >> str; */

             switch (myEvent.type)//检测事件类型
             {
             case SDL_QUIT:
                 quit = 1;
                 break;
             case SDL_MOUSEBUTTONDOWN://鼠标按下
                 switch(myEvent.button.button)
                 {
                 case SDL_BUTTON_LEFT:
                     x = myEvent.button.x;//得到当前鼠标的坐标
                     y = myEvent.button.y;                   

                     //判断鼠标是否落在注册按钮里
                     if (isOnButton(x,y,0)==1)
                     {
                         selected=a;    
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[0][0], position[0][1], position[0][2],position[0][3]);//放置设置

						 SDL_RenderPresent(render);
                     }
                     else if (isOnButton(x,y,1)==1)//判断鼠标是否落在注册按钮里
                     {
                         selected=b;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[1][0], position[1][1], position[1][2],position[1][3]);//放置设置

						 SDL_RenderPresent(render);
                     }
					 else if (isOnButton(x,y,2)==1)//判断鼠标是否落在注册按钮里
                     {
                         selected=c;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[2][0], position[2][1], position[2][2],position[2][3]);//放置设置

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,3)==1)//判断鼠标是否落在注册按钮里
                     {
                         selected=d;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[3][0], position[3][1], position[3][2],position[3][3]);//放置设置

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,4)==1)//判断鼠标是否落在注册按钮里
                     {
                         selected=e;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[4][0], position[4][1], position[4][2],position[4][3]);//放置设置

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,5)==1)//判断鼠标是否落在注册按钮里
                     {
                         selected=f;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[5][0], position[5][1], position[5][2],position[5][3]);//放置设置

						 SDL_RenderPresent(render);
                     }
					 else if (isOnButton(x,y,7)==1)//判断鼠标是否落在注册按钮里
					 {
                        quit=1;
						
                     }  
                     break;                
                 }
                 break;
             case SDL_MOUSEBUTTONUP:
                 switch(myEvent.button.button)
                 {
                 case SDL_BUTTON_LEFT:
                     x = myEvent.button.x;//得到当前鼠标的坐标
                     y = myEvent.button.y;                    

                     //判断鼠标是否落在注册按钮里
                     if (selected==a&&k==0)
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[0][0], position[0][1], position[0][2],position[0][3]);//放置设置

						 SDL_RenderPresent(render);

						 if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);
						 
						 printf("%s%d","分数为：",i);
                     }
                     else if (selected==b&&k==1)//判断鼠标是否落在注册按钮里
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[1][0], position[1][1], position[1][2],position[1][3]);//放置设置

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","分数为：",i);
                     }
					  else if (selected==c&&k==2)//判断鼠标是否落在注册按钮里
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[2][0], position[2][1], position[2][2],position[2][3]);//放置设置

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","分数为：",i);
                     } 
					  else if (selected==d&&k==3)//判断鼠标是否落在注册按钮里
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[3][0], position[3][1], position[3][2],position[3][3]);//放置设置

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","分数为：",i);
                     } 
					   else if (selected==e&&k==4)//判断鼠标是否落在注册按钮里
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[4][0], position[4][1], position[4][2],position[4][3]);//放置设置

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","分数为：",i);
                     } 
					   else if (selected==f&&k==5)//判断鼠标是否落在注册按钮里
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[5][0], position[5][1], position[5][2],position[5][3]);//放置设置

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","分数为：",i);
                     } 
					   else if (selected==g){}
					   else
					   {
						 selected = OTHER;

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music2,0);

						 printf("%s%d","分数为：",i);	
					   } break;
                 }
                 break;
             }            
         }
     }
	 stream << i;
	     stream >> str; 
	 SDL_Window *gpScreen2 = Init2();

	 while(quit)
	{
		 //初始化窗体
	SDL_Renderer *render2 = SDL_CreateRenderer(gpScreen2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	 SDL_SetRenderDrawColor(render2,183,255,159,255);	//浅蓝色

	 SDL_RenderClear(render2);
	 //ApplyString(render,image, word, keyboard, letter, buf, clor

			SDL_Color color = {0, 0, 0};//颜色
			word = RenderText(render2,"Your final score is :  "+str, "simfang.ttf", color, 32);
			word2 = RenderText(render2,"press Esc to exit... ", "simfang.ttf", color, 12);
			Copy_render(render2, word, 20,55,535,50);
			Copy_render(render2, word2, 180,130,250,20);
			SDL_RenderPresent(render2);


				if (SDL_PollEvent (&myEvent))	
	{
		switch (myEvent.type)
		{
		case  SDL_QUIT:	//如果是离开事件，退出程序
			quit = 0;
			break;
		case SDL_KEYDOWN:	//如果是键盘按键事件，且按键为ESC，退出程序
			if (myEvent.key.keysym.sym == SDLK_ESCAPE)
				quit = 0;
			break;
		}
		
	}
	 }

	 
	//SDL_FreeSurface(letter);
	SDL_DestroyTexture(word);

	SDL_DestroyTexture(gpBackground);//释放内存
	SDL_DestroyTexture(gpFM);
	SDL_DestroyTexture(gpButton);
    SDL_DestroyTexture(gpStart);
	SDL_DestroyTexture(gpQuit);
	SDL_DestroyTexture(gpBumb);
	SDL_DestroyTexture(gpRad);
	SDL_DestroyTexture(gpTitle);
	Mix_FreeMusic(Music1);
	Mix_FreeMusic(Music2);

	 SDL_DestroyRenderer(render);
	 //SDL_DestroyRenderer(render2);
     SDL_DestroyWindow(gpScreen);
	 SDL_DestroyWindow(gpScreen2);

     Quit();
     system("pause"); 
     return 0; 
 
	 }
 
 /*--------------------------------------------------------------------
     函数名:    isOnButton
     参  数:    (x,y)鼠标当前坐标;
             index要判断的那个按钮,0表示登录按钮，1表示注册按钮
     返回值: 如果落在指定按钮上返回1，否则返回0
     功  能:    判断鼠标是否落在指定按钮上面
     备  注:
 ----------------------------------------------------------------------*/
 int isOnButton(int aX,int aY, int aIndex)
 {
     return aX>=pos[aIndex].x && aX<=(pos[aIndex].x+pos[aIndex].w)&&
         aY>=pos[aIndex].y && aY<=(pos[aIndex].y + pos[aIndex].h);
 }
 
 
 