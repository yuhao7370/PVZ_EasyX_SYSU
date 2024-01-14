#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include<stdlib.h>
#include <easyx.h>
#include"eximage.h"
#include<Windows.h>
#include<mmsystem.h>
#include<time.h>
#pragma comment(lib,"winmm.lib")
#define WindowWidth   800
#define WindowHeight 600
IMAGE imgSurface;
IMAGE imgButtonBlack1;
IMAGE imgButtonBlack2;
IMAGE imgButtonBlack3;
IMAGE imgButtonWhite1;
IMAGE imgButtonWhite2;
IMAGE imgButtonWhite3;
IMAGE imgName;
IMAGE imgZombieHand[7];
IMAGE Klee[250];
void initialize()//初始化
{
	char Hand[64] = {};
	for (int i = 0; i < 7; i++)
	{
		sprintf_s(Hand, sizeof(Hand), "Resources/res/ZombieHand_%d.png", i + 1);
		loadimage(&imgZombieHand[i],Hand,330,360);
	}
	loadimage(&imgSurface,("Resources/res/Surface.png"),800,600);
	loadimage(&imgButtonBlack1,("Resources/res/ButtonBlack.png"),340,147);
	loadimage(&imgButtonWhite1,("Resources/res/ButtonWhite.png"),340,147);
	loadimage(&imgButtonBlack2, ("Resources/res/ButtonBlack2.png"),325,145);
	loadimage(&imgButtonBlack3, ("Resources/res/ButtonBlack3.png"), 295, 145);
	loadimage(&imgButtonWhite2, ("Resources/res/ButtonWhite2.png"), 325, 145);
	loadimage(&imgButtonWhite3, ("Resources/res/ButtonWhite3.png"), 295, 145);
	loadimage(&imgName, ("Resources/res/Name.png"));
	initgraph(WindowWidth, WindowHeight);
}
void Klee()
{
	int i, index = 0;
	char path[1000] = { 0 };
	for (i = 0; i < 251; i++)
	{
		sprintf_s(path, "Resources/res/Klee/Klee (%d).png", i + 1);
		loadimage(Klee1 + i, path, WindowWidth, WindowHeight);
	}
	const int frameDelay = 1000 / 60;
	int frameStart = 0;
	int frameTime = 0;
	int speed = 70;
	int sum = 0;
	mciSendString("open res/Klee.mp3 alias bgm_1", 0, 0, 0L);
	mciSendString("play bgm_1", 0, 0, 0);
	mciSendString("seek bgm_1 to 13*1000 ", 0, 0, 0);
	while (1)
	{
		frameStart = clock();
		BeginBatchDraw();
		cleardevice();
		putimage(0, 0, Klee1 + index);
		EndBatchDraw();
		if (index < 250) index = clock() / speed;
		if (index >= 251) index = 251;
		frameTime = clock() - frameStart;
		sum += frameTime;
		if (sum >= 9000)
		{
			mciSendString("close bgm_1", 0, 0, 0);
			return;
			index = 0;
		}
	}
}
void StartMenu()//主菜单函数
{
	ExMessage mouse;
	int mouse1 = 0, mouse2 = 0, mouse3 = 0; int flag = 0; int index = 0;  int start = 0; int time = 0; int MSC = 0;
	mciSendString("open res/Faster.mp3 alias bgm_0", NULL, 0, NULL);
	mciSendString("play bgm_0 repeat", NULL, 0, NULL);
	while (1)
	{
		BeginBatchDraw();//开始双缓冲
		putimage(0, 0, &imgSurface);// 显示背景
		alpha_composite(NULL, 60, 0, &imgName, 255);
		alpha_composite(NULL, 400, 55, mouse1 ? &imgButtonWhite1 : &imgButtonBlack1,255);
		alpha_composite(NULL,400, 170, mouse2 ? &imgButtonWhite2 : &imgButtonBlack2,255 );
		alpha_composite(NULL, 405, 270, mouse3 ? &imgButtonWhite3 : &imgButtonBlack3, 255);
		if (flag == 1)//播放僵尸手动画
		{
			
			mciSendString("stop bgm_0", NULL, 0, NULL);
			mciSendString("open res/evillaugh.mp3 alias laugh", NULL, 0, NULL);
			mciSendString("play laugh ", NULL, 0, NULL);
		    alpha_composite(NULL, 290, 300, &imgZombieHand[index], 255);
			if (index>6)
			{   
				start = clock();
				alpha_composite(NULL, 290, 300, &imgZombieHand[6], 255);
				Sleep(4500);
				time = clock() - start;
			if (time > 4500)return;
			}
			Sleep(100);
			index++;
		}
		if (MSC == 0)//开始鼠标反馈
		{
			if (peekmessage(&mouse))
			{
				if (mouse.x > 400 && mouse.x < 400 + 345 && mouse.y>55 && mouse.y < 170)
				{
					mouse1 = 1;//按键变白
					if (mouse.message == WM_LBUTTONDOWN)
					{
						flag = 1;//僵尸手部动画 
						MSC = 1;//停止鼠标反馈
					}
				}
				else mouse1 = 0;
				if (mouse.x > 395 && mouse.x < 395 + 330 && mouse.y>170 && mouse.y < 270)
				{
					mouse2 = 1;
					if (mouse.message == WM_LBUTTONDOWN)
					{
						mciSendString("stop bgm_0", NULL, 0, NULL);
					}
				}
				else mouse2 = 0;
				if (mouse.x > 400 && mouse.x < 400 + 300 && mouse.y>270 && mouse.y < 270 + 145)
				{
					mouse3 = 1;
					if (mouse.message == WM_LBUTTONDOWN)
					{
						mciSendString("stop bgm_0", NULL, 0, NULL);
					}
				}
				else mouse3 = 0;
			}
		}
		EndBatchDraw();//结束双缓冲
	}
	
}
int main() {
	initialize();
	StartMenu();
	return 0;
}