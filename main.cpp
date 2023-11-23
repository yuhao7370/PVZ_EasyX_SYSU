#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include "easing.h"
#include "eximage.h"
#include "utils.h"
#include <tchar.h>
#pragma comment(lib, "winmm.lib")  // 链接winmm.lib库

using namespace std;
const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN), ScreenHeight = GetSystemMetrics(SM_CYSCREEN);




long long start = get_ms(), lastframe = 0;

int main(){
    
    // int x = ScreenWidth/4, y = ScreenHeight/4, r = 50, dx = 10, dy = 10;
    // cout<<"ScreenWidth: "<<ScreenWidth<<endl<< "ScreenHeight: "<<ScreenHeight<<endl;
    // cout<<Cubic_easeInOut(1320, 0, 1, 1320);
    initgraph(800, 600);
    

    std::vector<IMAGE> zombie(17); 
    loadImageSequence(zombie, "Resources/Zombies/Zombie/1", 15);
    
    // exit(0);
    // for (int i = 0; i < 17; i++) {
    //     std::string filename = "Resources/Zombies/Zombie/frame_" + std::to_string(i) + ".png";
    //     loadimage(&zombie[i], filename.c_str());
    // }
   
    IMAGE img,img2,imgtemp;
    IMAGE ready, set, plant;
    IMAGE test, shadow;
    loadimage(&img, _T("Resources/BG1.jpg"));
    loadimage(&img2, _T("Resources/BG1_Game.jpg")); 
    loadimage(&ready, _T("Resources/ready.png")); 
    loadimage(&set, _T("Resources/set.png")); 
    loadimage(&plant, _T("Resources/plant.png")); 
    loadimage(&shadow, _T("Resources/shadow.png")); 
    loadimage(&test, _T("Resources/test.gif")); 
    putimage(0, 0, 800, 600, &img, 0 ,0);

    

    BOOL result = PlaySound(_T("Resources\\music\\Choose Plant cut.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (!result) {
        DWORD error = GetLastError();
        cout << L"PlaySound failed with error code: " << error << endl;
    }

    Sleep(1500);
    start = get_ms();
    while(1){
        long long now = get_ms();
        if(now - lastframe < 16) continue;
        if(now - start >= 5500) break;

        imgtemp = img;
        alpha_composite(&imgtemp, 1180, 415, &shadow);
        alpha_composite(&imgtemp, 1100, 300, &zombie[((int)(now - start) / 4 / 16) % 15]);
        
        if(now - start <= 1500)//过去
            putimage(0, 0, 800, 600, &imgtemp, 600*Cubic_easeInOut(now - start, 0, 1, 1500) ,0);
        else if(now - start <= 4000)
            putimage(0, 0, 800, 600, &imgtemp, 600 ,0);
        else//回来
            putimage(0, 0, 800, 600, &imgtemp, 600 - 380*Cubic_easeInOut(now - start - 4000, 0, 1, 1500) ,0);
        lastframe = now;
    }
    
    putimage(0, 0, 800, 600, &img, 220 ,0);
    Sleep(500);
    IMAGE csd, csd_1, temp;
    loadimage(&csd, _T("Resources/csd.png")); 
    loadimage(&csd_1, _T("Resources/csd_1.png")); 

    start = get_ms();
    while(1){
        long long now = get_ms();
        if(now - lastframe < 16) continue;
        lastframe = now;
        if(now - start >= 300) break;
        putimage(94, 0, 517, 85 * Cubic_easeOut(now - start, 0, 1, 300), &csd, 0, 85 - 85 * Cubic_easeOut(now - start, 0, 1, 300));
    } //传送带动画

    Sleep(700);
    PlaySound(_T("Resources\\music\\readysetplant.wav"), NULL, SND_FILENAME | SND_ASYNC);
    
    start = get_ms();
    while(1){
        IMAGE starttmp;
        long long now = get_ms();
        if(now - lastframe < 16) continue;
        lastframe = now;
        if(now - start >= 3000) break;

        if(now - start <= 400){
            starttmp = *scaleImage(&ready, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 400));
            temp = img2;
            alpha_composite_middle(&temp, 400 + 220, 300, &starttmp);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else if(now - start <= 500){
            putimage(0, 0, 800, 600, &img2, 220 ,0);
        }
        else if(now - start <= 900){
            starttmp = *scaleImage(&set, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 300));
            temp = img2;
            alpha_composite(&temp, 400 + 160, 280, &starttmp);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else if(now - start <= 1000){
            putimage(0, 0, 800, 600, &img2, 220 ,0);
        }
        else if(now - start <= 2500){
            temp = img2;
            alpha_composite_middle(&temp, 400 + 220, 300, &plant);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else{
            putimage(0, 0, 800, 600, &img2, 220 ,0);
        }
        
        // Sleep(15);
    } //ready set plant 动画

    Sleep(500);


    PlaySound(_T("Resources\\music\\Ultimate Battle1.wav"), NULL, SND_FILENAME | SND_ASYNC);
    IMAGE background;
    start = get_ms();
    long long nowms;
    while(1)
    {
        nowms = get_ms();
        if(nowms - lastframe < 16) continue;
        lastframe = nowms;

        background = img2;


        putimage(0, 0, 800, 600, &img2, 220 ,0);
        putimage(103, 63, 500, 15, &csd_1, 3 + (((nowms - start) / 32) % 512) ,0);
    }
    

    closegraph();
    return 0;    

}
