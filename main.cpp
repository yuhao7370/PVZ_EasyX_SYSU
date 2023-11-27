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
#include <gdiplus.h>
#pragma comment(lib, "winmm.lib")  // 链接winmm.lib库

#include "zombie.h"
#include "plant.h"

using namespace std;

long long start = get_ms(), lastframe = 0;
IMAGE ZombieShadow;
std::vector<IMAGE> StandingZombie(17);              //普通僵尸
std::vector<IMAGE> StandingConeheadZombie(17);      //路障僵尸
std::vector<IMAGE> StandingBucketheadZombie(17);    //铁桶僵尸

std::vector<IMAGE> WalkingZombie(57);              //普通僵尸
std::vector<IMAGE> WalkingConeheadZombie(57);      //路障僵尸
std::vector<IMAGE> WalkingBucketheadZombie(57);    //铁桶僵尸

int randomArray[16], frameoffset[16];
    
void DrawZombie(IMAGE* dstimg, int x, int y, std::vector<IMAGE>& images, int frame = 0, int withshadow = 1) {
    // cout<< "Zombie now at: " << x << "," << y << endl;
    if(withshadow)
        alpha_composite(dstimg, x+80, y+115, &ZombieShadow);
    alpha_composite(dstimg, x, y, &images[frame]);
}

int xMapCoordinateInterval[17] = {36, 118, 194, 278, 355, 440, 520, 596, 672, 760};
int yMapCoordinateInterval[7] = {80, 177, 277, 377, 475, 574};

int PixeltoMapCoordinate(int pixelx, int pixely) {
    int x = 0, y = 0;
    for (int i = 0; i < 9; i++) {
        if (pixelx >= xMapCoordinateInterval[i] && pixelx <= xMapCoordinateInterval[i + 1]) {
            x = i+1;
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (pixely >= yMapCoordinateInterval[i] && pixely <= yMapCoordinateInterval[i + 1]) {
            y = i+1;
            break;
        }
    }
    // printf("x = %d, y = %d\n", x, y);
    return x + y * 100;
}

int mouseType = 0; //0为无，1为铲子，2为植物
int shovelActive = 0; //铲子是否被选中
void mouseLeftButtonEventProcess(MOUSEMSG m) {
    //鼠标坐标分别为m.x和m.y
    if(m.uMsg == WM_LBUTTONDOWN){
        if(m.x >= 604 && m.x <= 674 && m.y >= 0 && m.y <= 72){
            // PlaySound(_T("Resources\\music\\shovel.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            if(shovelActive){
                shovelActive = 0;
                mouseType = 0;
            }
            else{
                 mciSendStringA("play Resources\\music\\shovel.wav", NULL, 0, NULL);
                shovelActive = 1;
                mouseType = 1;
            }
            return ;
        } //单击铲子框的情况

        //选在传送带内的情况
        if(m.x > 103 && m.x < 103 + 492 && m.y > 0 && m.y < 63 + 15){
            shovelActive = 0;
            mouseType = 2;
        }

        //选在地图内的情况
        if(m.x>=36 && m.x<=760 && m.y>=80 && m.y<=574){
            if(shovelActive){
                shovelActive = 0;//铲掉植物 晚点写
            }else{
                ;//种植物 晚点写
            }
            
            int maptmp = PixeltoMapCoordinate(m.x, m.y);
            int mapx = maptmp % 100, mapy = maptmp / 100;
            cout << "Map Coordinate: " << mapx << "," << mapy << endl;
        }

    }

    cout<<"Left button down at ("<<m.x<<","<<m.y<<")"<<endl;
}

struct plant_card{
    int plantid, canmove, pixelx, isselected = 0;
};

std::vector<std::shared_ptr<Zombie>> zombies;
std::vector<std::shared_ptr<Plant>> planted_plants;
std::vector<plant_card> csd_cards;



int main(){
    loadimage(&Peashooter_scaled, _T("Resources\\Plants\\Peashooter\\0\\scaled.png"));
    loadimage(&Sunflower_scaled, _T("Resources\\Plants\\Sunflower\\0\\scaled.png"));
    loadimage(&Wallnut_scaled, _T("Resources\\Plants\\Wallnut\\0\\scaled.png"));
    loadimage(&PotatoMine_scaled, _T("Resources\\Plants\\PotatoMine\\0\\scaled.png"));
    loadimage(&CherryBomb_scaled, _T("Resources\\Plants\\CherryBomb\\0\\scaled.png"));
    loadimage(&SnowPea_scaled, _T("Resources\\Plants\\SnowPea\\0\\scaled.png"));
    loadimage(&Repeater_scaled, _T("Resources\\Plants\\Repeater\\0\\scaled.png"));
    loadImageSequence(Peashooter, "Resources\\Plants\\Peashooter\\Peashooter", 25);
    loadImageSequence(Sunflower, "Resources\\Plants\\Sunflower\\Sunflower", 25);
    loadImageSequence(Wallnut, "Resources\\Plants\\Wallnut\\Wallnut", 17);
    loadImageSequence(PotatoMine, "Resources\\Plants\\PotatoMine\\PotatoMine", 11);
    loadImageSequence(CherryBomb, "Resources\\Plants\\CherryBomb\\CherryBomb", 14);
    loadImageSequence(SnowPea, "Resources\\Plants\\SnowPea\\SnowPea", 25);
    loadImageSequence(Repeater, "Resources\\Plants\\Repeater\\Repeater", 25);


    srand(static_cast<unsigned int>(time(NULL)));
    fillArray(randomArray, 16, 1);//僵尸排列
    fillArray(frameoffset, 16);//僵尸预览帧偏移

    loadimage(&ZombieShadow, _T("Resources/shadow.png")); 
    loadImageSequence(StandingZombie, "Resources/Zombies/Zombie/1", 15);
    loadImageSequence(StandingConeheadZombie, "Resources/Zombies/ConeheadZombie/1", 15);
    loadImageSequence(StandingBucketheadZombie, "Resources/Zombies/BucketheadZombie/1", 15);
    
    loadImageSequence(WalkingZombie, "Resources/Zombies/Zombie/Zombie", 47);

    initgraph(800, 600);
    
    IMAGE selectedx, selectedy;
    loadimage(&selectedx, _T("Resources/selectedx.png"));
    loadimage(&selectedy, _T("Resources/selectedy.png"));

    IMAGE BG1,BG1_Game,imgtemp;
    IMAGE ready, set, plant;
    IMAGE test, shadow;
    IMAGE ShovelBank, Shovel, ShovelHiRes, SeedPacket;
    loadimage(&ShovelBank, _T("Resources/ShovelBank.png"));
    loadimage(&Shovel, _T("Resources/Shovel.png"));
    loadimage(&ShovelHiRes, _T("Resources/Shovel_hi_res.png"));
    loadimage(&SeedPacket, _T("Resources/SeedPacket.png"));

    loadimage(&BG1, _T("Resources/BG1.jpg"));
    loadimage(&BG1_Game, _T("Resources/BG1_Game.jpg")); 
    loadimage(&ready, _T("Resources/ready.png")); 
    loadimage(&set, _T("Resources/set.png")); 
    loadimage(&plant, _T("Resources/plant.png")); 
    
    IMAGE csd, csd_1, temp;
    loadimage(&csd, _T("Resources/csd.png")); 
    loadimage(&csd_1, _T("Resources/csd_1.png")); 


    putimage(0, 0, 800, 600, &BG1, 0 ,0);

    

    BOOL result = PlaySound(_T("Resources\\music\\Choose Plant cut.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
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

        imgtemp = BG1;

        for (int i = 0; i < 16; i++) {
            switch (randomArray[i]) {
                case 1:
                    DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingZombie, ((int)(now - start) / 4 / 16 + frameoffset[i]) % 15 );
                    break;
                case 2:
                    DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingBucketheadZombie, ((int)(now - start) / 4 / 16 + frameoffset[i]) % 15);
                    break;
                case 3:
                    DrawZombie(&imgtemp, ZombieStandingPos[i][0], ZombieStandingPos[i][1], StandingConeheadZombie, ((int)(now - start) / 4 / 16 +  frameoffset[i]) % 15);
                    break;
                default:
                    break;
                }
        } //僵尸预览
        
        if(now - start <= 1500)//过去
            putimage(0, 0, 800, 600, &imgtemp, 600*Cubic_easeInOut(now - start, 0, 1, 1500) ,0);
        else if(now - start <= 4000)
            putimage(0, 0, 800, 600, &imgtemp, 600 ,0);
        else//回来
            putimage(0, 0, 800, 600, &imgtemp, 600 - 380*Cubic_easeInOut(now - start - 4000, 0, 1, 1500) ,0);
        lastframe = now;
    }
    
    putimage(0, 0, 800, 600, &BG1, 220 ,0);
    Sleep(500);
    
    start = get_ms();
    while(1){
        long long now = get_ms();
        if(now - lastframe < 16) continue;
        lastframe = now;
        if(now - start >= 300) break;
        putimage(94, 0, 517, 85 * Cubic_easeOut(now - start, 0, 1, 300), &csd, 0, 85 - 85 * Cubic_easeOut(now - start, 0, 1, 300));
    } //传送带动画

    Sleep(700);
    PlaySound(_T("Resources\\music\\readysetplant.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    
    start = get_ms();
    while(1){
        IMAGE starttmp;
        long long now = get_ms();
        if(now - lastframe < 16) continue;
        lastframe = now;
        if(now - start >= 3000) break;

        if(now - start <= 400){
            starttmp = *scaleImage(&ready, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 400));
            temp = BG1_Game;
            alpha_composite_middle(&temp, 400 + 220, 300, &starttmp);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else if(now - start <= 500){
            putimage(0, 0, 800, 600, &BG1_Game, 220 ,0);
        }
        else if(now - start <= 900){
            starttmp = *scaleImage(&set, 0.9 + 0.1 * Linear_easeIn(now - start, 0, 1, 300));
            temp = BG1_Game;
            alpha_composite(&temp, 400 + 160, 280, &starttmp);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else if(now - start <= 1000){
            putimage(0, 0, 800, 600, &BG1_Game, 220 ,0);
        }
        else if(now - start <= 2500){
            temp = BG1_Game;
            alpha_composite_middle(&temp, 400 + 220, 300, &plant);
            putimage(0, 0, 800, 600, &temp, 220 ,0);
        }
        else{
            putimage(0, 0, 800, 600, &BG1_Game, 220 ,0);
        }
        
        // Sleep(15);
    } //ready set plant 动画

    Sleep(500);

    
    int mapx, mapy;
    PlaySound(_T("Resources\\music\\Ultimate Battle1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    IMAGE background;
    start = get_ms();
    long long nowms;
    MOUSEMSG m;  
    int dopixelmove = 1;
    while(1)
    {   
        nowms = get_ms();
        if(nowms - lastframe < 16) continue;
        while (MouseHit()) {
            m = GetMouseMsg();
            //鼠标坐标分别为m.x和m.y
            if(m.uMsg == WM_LBUTTONDOWN){
                mouseLeftButtonEventProcess(m);
            }        
            else if(m.uMsg == WM_RBUTTONDOWN){
                if(csd_cards.size() < 9)
                    csd_cards.push_back({rand() % 8 + 1, 1, 850});
            }    

        }
        lastframe = nowms;
        background = BG1; //背景

        //铲子选中时的白色半透明条形应该在传送带图层下
        if(shovelActive){
            mapx = PixeltoMapCoordinate(m.x, m.y)%100, mapy = PixeltoMapCoordinate(m.x, m.y)/100; //代表铲子的位置 x是左数第几列 y是上数第几行
            if(mapx >= 1 && mapx <= 9 && mapy >= 1 && mapy <= 5){
                alpha_composite(&background, 150, yMapCoordinateInterval[mapy - 1], &selectedx); //横向白色半透明条
                alpha_composite(&background, xMapCoordinateInterval[mapx - 1] + 220, 0, &selectedy); //横向白色半透明条
            }
        }

        IMAGE* cropped_csd_1 = crop_image(&csd_1, 1 + (((nowms - start) / 32) % 512) , 0, (((nowms - start) / 32) % 512) + 492, 15); //传送带每2帧移动1像素
        //画好传送带
        if (cropped_csd_1) {
            alpha_composite(&background, 220+94, 0, &csd); //绘制传送带底板
            alpha_composite(&background, 103 + 220, 63, cropped_csd_1); //绘制传送带条形
            delete cropped_csd_1;
        }
        else{
            cout << "crop_image failed" << endl; //用此方法解决一些玄学错误
        }
        
        //画好植物卡片
        dopixelmove = 1-dopixelmove;
        int cardlen = csd_cards.size();
        IMAGE cardimg;
        plant_card card;
        for(int i = 0; i < cardlen; ++i){
            card = csd_cards[i];
            cardimg = SeedPacket; //51*71
            alpha_composite_middle(&cardimg, 26, 29, &id2img(card.plantid, 0, 1)); //绘制植物
            if(card.canmove){
                if(card.pixelx >= 765 && card.pixelx <= 765 + 51) alpha_composite(&background, card.pixelx, 7, crop_image(&cardimg, 0, 0, 51 + 765 - card.pixelx, 71));
                else if(card.pixelx <= 765) alpha_composite(&background, card.pixelx, 7, &cardimg);

                if(card.pixelx > 320 + i * 51) csd_cards[i].pixelx -= dopixelmove;
                else card.canmove = 0;
            }else{
                alpha_composite(&background, card.pixelx, 7, &cardimg);
            }
        }

        alpha_composite(&background, 824, 0, &ShovelBank); //绘制铲子框
        if(shovelActive){
            alpha_composite_middle(&background, m.x + 242, m.y - 20, &ShovelHiRes); //铲子被选中
        }else alpha_composite_middle(&background, 859, 36, &Shovel); //绘制铲子 70*72
        
        
        for (auto& zombie : zombies) { //遍历僵尸vector
            if(zombie->getFrame(nowms) == 0 && zombie->resetflag == 0){
                zombie->pixelx -= 50;
                zombie->resetflag = 1;
            } //第0帧处理绘制pixelx
            
            zombie->actualx = zombie->pixelx - ZombieWalkingOffset[zombie->getFrame(nowms)];
            DrawZombie(&background, zombie->actualx, zombie->pixely, WalkingZombie, zombie->getFrame(nowms)); 
        
            if(zombie->getFrame(nowms) == 46){        
                zombie -> resetflag = 0; //最后一帧重置
            } 
        }

        
        putimage(0, 0, 800, 600, &background, 220 ,0);
        std::string str = std::to_string(m.x) + "," + std::to_string(m.y);
        outtextxy(0, 0, _T(str.c_str()));

    }
    

    closegraph();
    return 0;    

}
