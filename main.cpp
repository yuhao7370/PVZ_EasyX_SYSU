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

#include "Zombie.h"

using namespace std;

long long start = get_ms(), lastframe = 0;
IMAGE ZombieShadow;
std::vector<IMAGE> StandingZombie(17);              //普通僵尸
std::vector<IMAGE> StandingConeheadZombie(17);      //路障僵尸
std::vector<IMAGE> StandingBucketheadZombie(17);    //铁桶僵尸

std::vector<IMAGE> WalkingZombie(57);              //普通僵尸
std::vector<IMAGE> WalkingConeheadZombie(57);      //路障僵尸
std::vector<IMAGE> WalkingBucketheadZombie(57);    //铁桶僵尸
int ZombieStandingPos[17][2] = {
    {1096, 30},  {986, 37}, {1166, 63}, {1046, 162}, {988, 216},  {1097, 210},  {1218, 209},
    {1152, 249}, {1048, 254}, {993, 301},  {1047, 340}, {1146, 335}, {1208, 401}, {1102, 405}, 
    {1172, 426}, {1054, 438}
};
int randomArray[16], frameoffset[16];
    
void DrawZombie(IMAGE* dstimg, int x, int y, std::vector<IMAGE>& images, int frame = 0) {
    // cout<< "Zombie now at: " << x << "," << y << endl;
    alpha_composite(dstimg, x+80, y+115, &ZombieShadow);
    alpha_composite(dstimg, x, y, &images[frame]);
}

void mouseLeftButtonEventProcess(MOUSEMSG m) {
    //鼠标坐标分别为m.x和m.y
    if(m.uMsg == WM_LBUTTONDOWN)
        cout<<"Left button down at ("<<m.x<<","<<m.y<<")"<<endl;
}

std::vector<std::shared_ptr<Zombie>> zombies;

int main(){
    srand(static_cast<unsigned int>(time(NULL)));
    fillArray(randomArray, 16, 1);//僵尸排列
    fillArray(frameoffset, 16);//僵尸预览帧偏移

    loadimage(&ZombieShadow, _T("Resources/shadow.png")); 
    loadImageSequence(StandingZombie, "Resources/Zombies/Zombie/1", 15);
    loadImageSequence(StandingConeheadZombie, "Resources/Zombies/ConeheadZombie/1", 15);
    loadImageSequence(StandingBucketheadZombie, "Resources/Zombies/BucketheadZombie/1", 15);
    
    loadImageSequence(WalkingZombie, "Resources/Zombies/Zombie/Zombie", 47);

    initgraph(800, 600);
   
    IMAGE img,img2,imgtemp;
    IMAGE ready, set, plant;
    IMAGE test, shadow;
    loadimage(&img, _T("Resources/BG1.jpg"));
    loadimage(&img2, _T("Resources/BG1_Game.jpg")); 
    loadimage(&ready, _T("Resources/ready.png")); 
    loadimage(&set, _T("Resources/set.png")); 
    loadimage(&plant, _T("Resources/plant.png")); 
    
    loadimage(&test, _T("Resources/test.gif")); 
    IMAGE csd, csd_1, temp;
    loadimage(&csd, _T("Resources/csd.png")); 
    loadimage(&csd_1, _T("Resources/csd_1.png")); 


    putimage(0, 0, 800, 600, &img, 0 ,0);

    

    // BOOL result = PlaySound(_T("Resources\\music\\Choose Plant cut.wav"), NULL, SND_FILENAME | SND_ASYNC);
    // if (!result) {
    //     DWORD error = GetLastError();
    //     cout << L"PlaySound failed with error code: " << error << endl;
    // }

    // Sleep(1500);
    // start = get_ms();
    // while(1){
    //     long long now = get_ms();
    //     if(now - lastframe < 16) continue;
    //     if(now - start >= 5500) break;

    //     imgtemp = img;

    //     for (int i = 0; i < 16; i++) {
    //         switch (randomArray[i]) {
    //             case 1:
    //                 DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingZombie, ((int)(now - start) / 4 / 16 + frameoffset[i]) % 15 );
    //                 break;
    //             case 2:
    //                 DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingBucketheadZombie, ((int)(now - start) / 4 / 16 + frameoffset[i]) % 15);
    //                 break;
    //             case 3:
    //                 DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingConeheadZombie, ((int)(now - start) / 4 / 16 +  frameoffset[i]) % 15);
    //                 break;
    //             default:
    //                 break;
    //             }
    //     } //僵尸预览
        
    //     if(now - start <= 1500)//过去
    //         putimage(0, 0, 800, 600, &imgtemp, 600*Cubic_easeInOut(now - start, 0, 1, 1500) ,0);
    //     else if(now - start <= 4000)
    //         putimage(0, 0, 800, 600, &imgtemp, 600 ,0);
    //     else//回来
    //         putimage(0, 0, 800, 600, &imgtemp, 600 - 380*Cubic_easeInOut(now - start - 4000, 0, 1, 1500) ,0);
    //     lastframe = now;
    // }
    
    // putimage(0, 0, 800, 600, &img, 220 ,0);
    // Sleep(500);
    
    // start = get_ms();
    // while(1){
    //     long long now = get_ms();
    //     if(now - lastframe < 16) continue;
    //     lastframe = now;
    //     if(now - start >= 300) break;
    //     putimage(94, 0, 517, 85 * Cubic_easeOut(now - start, 0, 1, 300), &csd, 0, 85 - 85 * Cubic_easeOut(now - start, 0, 1, 300));
    // } //传送带动画

    // Sleep(700);
    // PlaySound(_T("Resources\\music\\readysetplant.wav"), NULL, SND_FILENAME | SND_ASYNC);
    
    // start = get_ms();
    // while(1){
    //     IMAGE starttmp;
    //     long long now = get_ms();
    //     if(now - lastframe < 16) continue;
    //     lastframe = now;
    //     if(now - start >= 3000) break;

    //     if(now - start <= 400){
    //         starttmp = *scaleImage(&ready, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 400));
    //         temp = img2;
    //         alpha_composite_middle(&temp, 400 + 220, 300, &starttmp);
    //         putimage(0, 0, 800, 600, &temp, 220 ,0);
    //     }
    //     else if(now - start <= 500){
    //         putimage(0, 0, 800, 600, &img2, 220 ,0);
    //     }
    //     else if(now - start <= 900){
    //         starttmp = *scaleImage(&set, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 300));
    //         temp = img2;
    //         alpha_composite(&temp, 400 + 160, 280, &starttmp);
    //         putimage(0, 0, 800, 600, &temp, 220 ,0);
    //     }
    //     else if(now - start <= 1000){
    //         putimage(0, 0, 800, 600, &img2, 220 ,0);
    //     }
    //     else if(now - start <= 2500){
    //         temp = img2;
    //         alpha_composite_middle(&temp, 400 + 220, 300, &plant);
    //         putimage(0, 0, 800, 600, &temp, 220 ,0);
    //     }
    //     else{
    //         putimage(0, 0, 800, 600, &img2, 220 ,0);
    //     }
        
    //     // Sleep(15);
    // } //ready set plant 动画

    Sleep(500);


    PlaySound(_T("Resources\\music\\Ultimate Battle1.wav"), NULL, SND_FILENAME | SND_ASYNC);
    IMAGE background;
    start = get_ms();
    long long nowms;
    MOUSEMSG m;  

    nowms = get_ms();
    zombies.push_back(std::make_shared<NormalZombie>(1, nowms));
    while(1)
    {
        nowms = get_ms();
        if(nowms - lastframe < 16) continue;
        while (MouseHit()) {
            m = GetMouseMsg();
            //鼠标坐标分别为m.x和m.y
            if(m.uMsg == WM_LBUTTONDOWN){
                mouseLeftButtonEventProcess(m);
                zombies.push_back(std::make_shared<NormalZombie>(1, nowms));
            }
                

        }
        lastframe = nowms;
        background = img2;

        //画好传送带
        alpha_composite(&background, 103 + 220, 63, crop_image(&csd_1, 1 + (((nowms - start) / 32) % 512) , 0, (((nowms - start) / 32) % 512) + 500, 15)); //传送带

        for (auto& zombie : zombies) {
            zombie->pixely = ZombieLinePos[zombie->line];
            cout<< "Zombie now at: " << zombie->pixelx - (nowms - (zombie->spawntime)) / 200<< "," << zombie->pixely << endl;
            DrawZombie(&background, zombie->pixelx - (lastframe - zombie->spawntime) / 200 , zombie->pixely, WalkingZombie, ((int)(nowms - start) / 4 / 16) % 47);
        }

        
        putimage(0, 0, 800, 600, &background, 220 ,0);
        std::string str = std::to_string(m.x) + "," + std::to_string(m.y);
        outtextxy(0, 0, _T(str.c_str()));

    }
    

    closegraph();
    return 0;    

}
