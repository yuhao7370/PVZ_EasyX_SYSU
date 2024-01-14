#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <vector>
#include <tchar.h>
#include <gdiplus.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")  // 链接winmm.lib库

#include "zombie.h"
#include "plant.h"
#include "easing.h"
#include "eximage.h"
#include "utils.h"

#define WindowWidth 800
#define WindowHeight 600
using namespace std;

IMAGE imgSurface;
IMAGE imgButtonBlack1;
IMAGE imgButtonBlack2;
IMAGE imgButtonBlack3;
IMAGE imgButtonWhite1;
IMAGE imgButtonWhite2;
IMAGE imgButtonWhite3;
IMAGE imgName;
IMAGE imgZombieHand[7];

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
}

void StartMenu()//主菜单函数
{
	ExMessage mouse;
	int mouse1 = 0, mouse2 = 0, mouse3 = 0; int flag = 0; int index = 0;  int start = 0; int time = 0; int MSC = 0;
	mciSendString("open Resources/res/Faster.mp3 alias bgm_0", NULL, 0, NULL);
	mciSendString("play bgm_0 repeat", NULL, 0, NULL);
	while (1)
	{
		// BeginBatchDraw();//开始双缓冲
		putimage(0, 0, &imgSurface);// 显示背景
		alpha_composite(NULL, 60, 0, &imgName, 255);
		alpha_composite(NULL, 400, 55, mouse1 ? &imgButtonWhite1 : &imgButtonBlack1,255);
		alpha_composite(NULL,400, 170, mouse2 ? &imgButtonWhite2 : &imgButtonBlack2,255 );
		alpha_composite(NULL, 405, 270, mouse3 ? &imgButtonWhite3 : &imgButtonBlack3, 255);
		if (flag == 1)//播放僵尸手动画
		{
			
			mciSendString("stop bgm_0", NULL, 0, NULL);
			mciSendString("open Resources/res/evillaugh.mp3 alias laugh", NULL, 0, NULL);
			mciSendString("play laugh ", NULL, 0, NULL);
		    alpha_composite(NULL, 290, 300, &imgZombieHand[index], 255);
			if (index>6)
			{   
				start = clock();
				alpha_composite(NULL, 290, 300, &imgZombieHand[6], 255);
				Sleep(4500);
				time = clock() - start;
			if (time > 4500) return;
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
                        PlayAudio("Resources\\music\\ClickError.wav");
						// mciSendString("stop bgm_0", NULL, 0, NULL);
					}
				}
				else mouse2 = 0;
				if (mouse.x > 400 && mouse.x < 400 + 300 && mouse.y>270 && mouse.y < 270 + 145)
				{
					mouse3 = 1;
					if (mouse.message == WM_LBUTTONDOWN)
					{
                        PlayAudio("Resources\\music\\ClickError.wav");
						// mciSendString("stop bgm_0", NULL, 0, NULL);
					}
				}
				else mouse3 = 0;
			}
		}
		EndBatchDraw();//结束双缓冲
	}
	
}


IMAGE Shadow;
int randomArray[16], frameoffset[16];
struct plant_card{
    int plantid, canmove, pixelx, isselected = 0;
};

int xMapCoordinateInterval[17] = {36, 118, 194, 278, 355, 440, 520, 596, 672, 760};
int yMapCoordinateInterval[7] = {80, 177, 277, 377, 475, 574};
std::vector<std::shared_ptr<Zombie>> zombies;
std::vector<std::shared_ptr<Plant>> planted_plants;
std::vector<Bullet> bullets;
std::vector<plant_card> csd_cards;
std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> bullet_collide;

class Timer {
public:
    static long long get_ms() {
        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch().count();

        return value - start_time;
    }

private:
    static const long long start_time;
};

long long start = Timer::get_ms(), lastframe = 0;
const long long Timer::start_time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();

struct mouse_status{
    int mouseType = 0; //0为无，1为铲子，2为植物
    int shovelActive = 0; //铲子是否被选中
    int selectedCard = -1; //选中的卡片
}mouse_status;

int plantmap[17][7];

void handleBullet(int nowms = Timer::get_ms()){
    for(int i = 0; i < bullets.size(); ++i){
        if(bullets[i].is_collided == 1){
            bullet_collide.push_back({{bullets[i].pixelx, yMapCoordinateInterval[bullets[i].line-1]+17}, {bullets[i].is_cold_bullet, bullets[i].collided_time}});
            bullets.erase(bullets.begin() + i);
            continue;
        }
        if(bullets[i].pixelx >= 820){
            bullets.erase(bullets.begin() + i);
            continue;
        }
        for(auto& zombie: zombies){
            if(zombie->line == bullets[i].line && zombie->actualx - 130 <= bullets[i].pixelx && zombie->actualx - 60 >= bullets[i].pixelx && bullets[i].pixelx >= xMapCoordinateInterval[bullets[i].spawnerx]){
                if(zombie->id != 3)
                    switch(rand()%3){
                        case 0: {PlayAudio("Resources\\music\\HitSound\\splat.wav"); break;}
                        case 1: {PlayAudio("Resources\\music\\HitSound\\splat2.wav"); break;}
                        case 2: {PlayAudio("Resources\\music\\HitSound\\splat3.wav"); break;}
                        default: break;
                    }
                else rand()%2 ? PlayAudio("Resources\\music\\HitSound\\shieldhit.wav") : PlayAudio("Resources\\music\\HitSound\\shieldhit2.wav"); 

                zombie->takeDamage(bullets[i].damage);
                bullets[i].is_collided = 1;
                bullets[i].collided_time = nowms;

                if(bullets[i].is_cold_bullet){
                    if(zombie->is_cold == 0){
                        PlayAudio("Resources\\music\\HitSound\\frozen.wav");
                    }
                    zombie->is_cold = 1;
                    zombie->cold_time = nowms;
                }
                break;
            }
        }
    }

}

void updateZombieAttacking(){
    int PixeltoMapCoordinate(int pixelx, int pixely);
    for(auto& zombie: zombies){
        if(zombie->dying)
            continue; //如果僵尸死了就不更新状态

        if(plantmap[PixeltoMapCoordinate(zombie->actualx - 120, zombie->pixely)%100][zombie->line] == 1 && zombie->attacking == 0){    
            if(zombie->actualx >= xMapCoordinateInterval[PixeltoMapCoordinate(zombie->actualx - 100, zombie->pixely)%100] + 52){
                zombie->attacking = 1;
                zombie->attackstarttime = Timer::get_ms();
                zombie->pixelx = zombie->actualx;
                zombie->nowframe = 0;
                zombie->nowframecnt = 0;
            }
        }
        else if(plantmap[PixeltoMapCoordinate(zombie->actualx - 120, zombie->pixely)%100][zombie->line] == 0 && zombie->attacking == 1){
            zombie->pixelx = zombie->actualx;
            zombie->attacking = 0;
            zombie->nowframe = 0;
            zombie->nowframecnt = 0;
            zombie->resetflag = 1;

            zombie->nowframecntattacking = 0;
            zombie->nowframeattacking = 0;
        }
    }
}

void DrawPlant(IMAGE* dstimg, int x, int y, int id, int frame = 0, int withshadow = 1, int health = 100000) {
    // cout<< "Zombie now at: " << x << "," << y << endl;
    if(withshadow)
        alpha_composite_middle(dstimg, x, y+30, &Shadow); 
    switch(id){
        case 1:{alpha_composite_middle(dstimg, x, y, &Peashooter_[frame]); break;}
        case 2:{alpha_composite_middle(dstimg, x, y, &Sunflower_[frame]); break;}
        case 3:{
            if(health>=2000)
                alpha_composite_middle(dstimg, x, y, &Wallnut_[frame]);
            else if(health>=1000)
                alpha_composite_middle(dstimg, x, y, &Wallnut_cracked1[frame]);
            else
                alpha_composite_middle(dstimg, x, y, &Wallnut_cracked2[frame]);
            break;
        }
        case 4:{alpha_composite_middle(dstimg, x, y, &PotatoMine_[frame]); break;}
        case 5:{alpha_composite_middle(dstimg, x, y, &CherryBomb_[frame]); break;}
        case 6:{alpha_composite_middle(dstimg, x, y, &SnowPea_[frame]); break;}
        case 7:{alpha_composite_middle(dstimg, x, y, &Repeater_[frame]); break;}
    }
}

void DrawZombie(IMAGE* dstimg, int x, int y, std::vector<IMAGE>& images, int frame = 0, int withshadow = 1, int is_cold = 0, std::vector<IMAGE>& coldimages = std::vector<IMAGE>()) {
    // cout<< "Zombie now at: " << x << "," << y << endl;
    if(withshadow)
        alpha_composite(dstimg, x+80, y+115, &Shadow);
    // printf("%d\n", is_cold);
    alpha_composite(dstimg, x, y, &images[frame]);
    if(is_cold){
        alpha_composite(dstimg, x, y, &coldimages[frame], 80);
    }  
}



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

void plantPlant(int plantid, int mapx, int mapy, long long planttime){
    switch (plantid)
    {
        case 1: { planted_plants.push_back(std::make_shared<Peashooter>(mapx, mapy, planttime)); break;}
        case 2: { planted_plants.push_back(std::make_shared<Sunflower>(mapx, mapy, planttime)); break;}
        case 3: { planted_plants.push_back(std::make_shared<Wallnut>(mapx, mapy, planttime)); break;}
        case 4: { planted_plants.push_back(std::make_shared<PotatoMine>(mapx, mapy, planttime)); break;}
        case 5: { planted_plants.push_back(std::make_shared<CherryBomb>(mapx, mapy, planttime)); break;}
        case 6: { planted_plants.push_back(std::make_shared<SnowPea>(mapx, mapy, planttime)); break;}
        case 7: { planted_plants.push_back(std::make_shared<Repeater>(mapx, mapy, planttime)); break;}
        default:
            break;
    }
    return ;
}

void mouseLeftButtonEventProcess(MOUSEMSG m) {
    //鼠标坐标分别为m.x和m.y
    if(m.uMsg == WM_LBUTTONDOWN){
        if(m.x >= 604 && m.x <= 674 && m.y >= 0 && m.y <= 72){
            // PlaySound(_T("Resources\\music\\shovel.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            if(mouse_status.shovelActive){
                mouse_status.shovelActive = 0;
                mouse_status.mouseType = 0;
            }
            else{
                PlayAudio("Resources\\music\\shovel.wav");
                mouse_status.shovelActive = 1;
                mouse_status.mouseType = 1;
            }
            for(int j = 0; j < csd_cards.size(); ++j){
                csd_cards[j].isselected = 0;
            }
            return ;
        } //单击铲子框的情况

        //选在传送带内的情况
        if(m.x > 103 && m.x < 103 + 492 && m.y > 0 && m.y < 63 + 15){
            mouse_status.shovelActive = 0;
            mouse_status.mouseType = 0;
            int cardlen = csd_cards.size();
            for(int i = 0; i < cardlen; ++i){
                if(m.x >= csd_cards[i].pixelx - 220 && m.x <= csd_cards[i].pixelx + 51 - 220 && m.y >= 7 && m.y <= 78){
                    if(csd_cards[i].isselected == 1){
                        csd_cards[i].isselected = 0;
                        mouse_status.mouseType = 0;
                        mouse_status.selectedCard = -1;
                        break;
                    }
                    else{
                        csd_cards[i].isselected = 1;
                        PlayAudio("Resources\\music\\seedlift.wav");
                        mouse_status.selectedCard = csd_cards[i].plantid;
                        for(int j = 0; j < cardlen; ++j){
                            if(j != i) csd_cards[j].isselected = 0;
                        }
                        mouse_status.mouseType = 2;
                        break;
                    }
                }
                if(i == cardlen - 1){
                    for(int j = 0; j < cardlen; ++j){
                        csd_cards[j].isselected = 0;
                    }
                    mouse_status.mouseType = 0;
                }
            }
        }

        //选在地图内的情况
        if(m.x>=36 && m.x<=760 && m.y>=80 && m.y<=574){
            int mapx = PixeltoMapCoordinate(m.x, m.y)%100, mapy = PixeltoMapCoordinate(m.x, m.y)/100, plantid = mouse_status.selectedCard;
            if(mouse_status.shovelActive){// 铲植物
                if(plantmap[mapx][mapy] == 1){
                    PlayAudio("Resources\\music\\plant2.wav");
                    for(int j = 0; j < planted_plants.size(); ++j){
                        if(planted_plants[j]->x == mapx && planted_plants[j]->y == mapy){
                            planted_plants.erase(planted_plants.begin() + j);
                            plantmap[mapx][mapy] = 0;
                            break;
                        }
                    }
                }
                mouse_status.shovelActive = 0;
                mouse_status.mouseType = 0; 
            }else if(mouse_status.mouseType == 2){// 种植物
                if(plantmap[mapx][mapy] == 1){mouse_status.mouseType = 0;mouse_status.selectedCard = -1; for(int j = 0; j < csd_cards.size(); ++j) csd_cards[j].isselected = 0; return;}//已经有植物了
                plantPlant(plantid, mapx, mapy, Timer::get_ms());
                // for(auto& plant: planted_plants){
                //     printf("plant: %d %d %d %lld\n", plant->id, plant->x, plant->y, plant->planttime);
                // }
                plantmap[mapx][mapy] = 1;
                PlayAudio("Resources\\music\\plant.wav");
                for(int j = 0; j < csd_cards.size(); ++j){
                    csd_cards[j].canmove = 1; //刷新全部移动状态
                    if(csd_cards[j].isselected == 1)
                        //删掉这张卡
                        csd_cards.erase(csd_cards.begin() + j);
                }
                mouse_status.mouseType = 0;
                mouse_status.selectedCard = -1;
            }
            
        }

    }

    cout<<"Left button down at ("<<m.x<<","<<m.y<<")"<<endl;
}

bool cmp(std::shared_ptr<Zombie> a, std::shared_ptr<Zombie> b){
    if(a->line == b->line)
        return a->actualx < b->actualx;
    else
        return a->line < b->line;
}

int main(){
    initgraph(800, 600);
    initialize();
	StartMenu();

    loadimage(&Peashooter_scaled, _T("Resources\\Plants\\Peashooter\\0\\scaled.png"));
    loadimage(&Sunflower_scaled, _T("Resources\\Plants\\Sunflower\\0\\scaled.png"));
    loadimage(&Wallnut_scaled, _T("Resources\\Plants\\Wallnut\\0\\scaled.png"));
    loadimage(&PotatoMine_scaled, _T("Resources\\Plants\\PotatoMine\\0\\scaled.png"));
    loadimage(&CherryBomb_scaled, _T("Resources\\Plants\\CherryBomb\\0\\scaled.png"));
    loadimage(&SnowPea_scaled, _T("Resources\\Plants\\SnowPea\\0\\scaled.png"));
    loadimage(&Repeater_scaled, _T("Resources\\Plants\\Repeater\\0\\scaled.png"));

    loadImageSequence(Peashooter_, "Resources\\Plants\\Peashooter\\Peashooter", 25);
    loadImageSequence(Sunflower_, "Resources\\Plants\\Sunflower\\Sunflower1", 25);
    loadImageSequence(Wallnut_, "Resources\\Plants\\Wallnut\\Wallnut", 17);
    loadImageSequence(Wallnut_cracked1, "Resources\\Plants\\Wallnut\\Wallnut_cracked1", 17);
    loadImageSequence(Wallnut_cracked2, "Resources\\Plants\\Wallnut\\Wallnut_cracked2", 17);
    loadImageSequence(PotatoMine_, "Resources\\Plants\\PotatoMine\\PotatoMine", 11);
    loadImageSequence(CherryBomb_, "Resources\\Plants\\CherryBomb\\CherryBomb", 14);
    loadImageSequence(SnowPea_, "Resources\\Plants\\SnowPea\\SnowPea", 25);
    loadImageSequence(Repeater_, "Resources\\Plants\\Repeater\\Repeater", 25);

    loadimage(&peashadow, _T("Resources/peashadow.png"));
    loadimage(&pea, _T("Resources/pea.png"));
    loadimage(&icepea, _T("Resources/icepea.png"));
    loadimage(&peahit, _T("Resources/peahit.png"));
    loadimage(&icepeahit, _T("Resources/icepeahit.png"));

    srand(static_cast<unsigned int>(time(NULL)));
    fillArray(randomArray, 16, 1);//僵尸排列
    fillArray(frameoffset, 16);//僵尸预览帧偏移

    loadimage(&Shadow, _T("Resources/shadow.png")); 
    loadImageSequence(StandingZombie, "Resources/Zombies/Zombie/1", 15);
    loadImageSequence(StandingConeheadZombie, "Resources/Zombies/ConeheadZombie/1", 15);
    loadImageSequence(StandingBucketheadZombie, "Resources/Zombies/BucketheadZombie/1", 15);
    
    loadImageSequence(WalkingZombie, "Resources/Zombies/Zombie/Zombie", 47);
    loadImageSequence(WalkingConeheadZombie, "Resources/Zombies/ConeheadZombie/ConeheadZombie", 47);
    loadImageSequence(WalkingBucketheadZombie, "Resources/Zombies/BucketheadZombie/BucketheadZombie", 47);

    loadImageSequence(ZombieAttack, "Resources/Zombies/Zombie/ZombieAttack", 40);
    loadImageSequence(ConeheadZombieAttack, "Resources/Zombies/ConeheadZombie/ConeheadZombieAttack", 40);
    loadImageSequence(BucketheadZombieAttack, "Resources/Zombies/BucketheadZombie/BucketheadZombieAttack", 40);

    loadImageSequence(WalkingZombieCold, "Resources/Zombies/Zombie/Zombie/cold", 47);
    loadImageSequence(WalkingConeheadZombieCold, "Resources/Zombies/ConeheadZombie/ConeheadZombie/cold", 47);
    loadImageSequence(WalkingBucketheadZombieCold, "Resources/Zombies/BucketheadZombie/BucketheadZombie/cold", 47);

    loadImageSequence(ZombieAttackCold, "Resources/Zombies/Zombie/ZombieAttack/cold", 40);
    loadImageSequence(ConeheadZombieAttackCold, "Resources/Zombies/ConeheadZombie/ConeheadZombieAttack/cold", 40);
    loadImageSequence(BucketheadZombieAttackCold, "Resources/Zombies/BucketheadZombie/BucketheadZombieAttack/cold", 40);

    
    
    IMAGE selectedx, selectedy;
    loadimage(&selectedx, _T("Resources/selectedx.png"));
    loadimage(&selectedy, _T("Resources/selectedy.png"));

    IMAGE BG1,BG1_Game,imgtemp;
    IMAGE ready, set, plant;
    IMAGE test, shadow;
    IMAGE ShovelBank, Shovel, ShovelHiRes, SeedPacket, SeedPacketSelected;
    loadimage(&ShovelBank, _T("Resources/ShovelBank.png"));
    loadimage(&Shovel, _T("Resources/Shovel.png"));
    loadimage(&ShovelHiRes, _T("Resources/Shovel_hi_res.png"));
    loadimage(&SeedPacket, _T("Resources/SeedPacket.png"));
    loadimage(&SeedPacketSelected, _T("Resources/SeedPacketSelected.png"));

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
    start = Timer::get_ms();
    while(1){
        long long now = Timer::get_ms();
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
    
    start = Timer::get_ms();
    while(1){
        long long now = Timer::get_ms();
        if(now - lastframe < 16) continue;
        lastframe = now;
        if(now - start >= 300) break;
        putimage(94, 0, 517, 85 * Cubic_easeOut(now - start, 0, 1, 300), &csd, 0, 85 - 85 * Cubic_easeOut(now - start, 0, 1, 300));
    } //传送带动画

    Sleep(700);
    PlaySound(_T("Resources\\music\\readysetplant.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
    
    start = Timer::get_ms();
    while(1){
        IMAGE starttmp;
        long long now = Timer::get_ms();
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
        
    } //ready set plant 动画
    Sleep(500);

    int cardlen;
    IMAGE cardimg;
    plant_card card;
    IMAGE* cropped_csd_1;
    IMAGE* cropped_card;
    int mapx, mapy;
    PlaySound(_T("Resources\\music\\Ultimate Battle1.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT| SND_LOOP);
    // PlayAudio("Resources\\music\\Ultimate Battle1.wav");
    IMAGE background;
    start = Timer::get_ms();
    long long nowms;
    MOUSEMSG m;  
    int dopixelmove = 1;
    int levelplants[7] = {-1, 1, 3, 6, 7, -1};
    int framecnt = 0;
    int existZombie[17][7];
    int plantxytosubscript[17][7];

    // planted_plants.push_back(std::make_shared<SnowPea>(1, 1, Timer::get_ms()));
    while(1)
    {        
        nowms = Timer::get_ms();
        if(nowms - lastframe < 16) continue;
        sort(zombies.begin(), zombies.end(), cmp);
        background = BG1; //背景

        ++framecnt;
        if(framecnt % 200 == 50){
            if(csd_cards.size() < 9)
                csd_cards.push_back({levelplants[rand() % 4 + 1], 1, 850}); //每200帧随机生成一张卡片
                // csd_cards.push_back({1, 1, 850});
        }

        if(framecnt % 50 == 0){
            int randomnum = rand() % 6 + 1, randomline = rand() % 5 + 1;
            if(framecnt >= 4500)
                randomnum += 2;
            else if (framecnt >= 9000)
                randomnum += 4;
            
            if(framecnt<=4500)
                if(framecnt % 250 !=0)
                    continue;
            else if(framecnt <= 9000)
                if(framecnt % 125 !=0)
                    continue;

            if(randomnum<=3)
                zombies.push_back(std::make_shared<NormalZombie>(randomline, Timer::get_ms()));
            else if(randomnum<=5)
                zombies.push_back(std::make_shared<ConeheadZombie>(randomline, Timer::get_ms()));
            else
                zombies.push_back(std::make_shared<BucketheadZombie>(randomline, Timer::get_ms()));
                // csd_cards.push_back({1, 1, 850});

        }

        while (MouseHit()) {
            m = GetMouseMsg();
            //鼠标坐标分别为m.x和m.y
            if(m.uMsg == WM_LBUTTONDOWN){  
                mouseLeftButtonEventProcess(m);
            }        
            else if(m.uMsg == WM_RBUTTONDOWN){
                // csd_cards.push_back({levelplants[rand() % 4 + 1], 1, 320});
                // printf("add plant card\n");
                // zombies.push_back(std::make_shared<BucketheadZombie>(PixeltoMapCoordinate(m.x, m.y) / 100, Timer::get_ms()));
                // zombies[zombies.size() - 1]->pixelx = m.x + 160;
                ;
            }   
        }

        //处理是否存在僵尸
        memset(existZombie, 0, sizeof(existZombie));
        for (auto& zombie : zombies) {
            for(int i = 1; i<= 9; ++i){
                if(xMapCoordinateInterval[i] + 70 < zombie->actualx){
                    existZombie[i][zombie->line] = 1;
                }
            }
            if(zombie->health<=0){
                zombie->dying = 1;
                zombie->dyingstarttime = nowms;
            }
        }

        for(int i=0; i<planted_plants.size(); ++i){
            if(planted_plants[i]->health <= 0){
                PlayAudio("Resources\\music\\HitSound\\gulp.wav");
                plantmap[planted_plants[i]->x][planted_plants[i]->y] = 0;
                planted_plants.erase(planted_plants.begin() + i); 
            }
        }
        for(int i=0; i<planted_plants.size(); ++i){
            plantxytosubscript[planted_plants[i]->x][planted_plants[i]->y] = i;
        }

        handleBullet(nowms);
        updateZombieAttacking();

        //尝试生成子弹
        for(int i = 0; i<planted_plants.size(); ++i){ //遍历植物vector 铲子之下传送带之上
            auto plant = planted_plants[i];
            if(plant->CanSpwanBullet && plant->lastshoot + plant->ShootInterval <= nowms){
                if(existZombie[plant->x][plant->y] == 1){
                    plant->lastshoot = nowms;
                    switch (plant->id)
                    { //(int spawnerid, int spawnerx, int spawnery, int damage, int line, int is_cold_bullet = 0)
                        case 1: {bullets.push_back(Bullet(1, plant->x, plant->y, 20, plant->y, xMapCoordinateInterval[plant->x] - 10)); break;}
                        case 6: {bullets.push_back(Bullet(1, plant->x, plant->y, 20, plant->y, xMapCoordinateInterval[plant->x] - 10, 1)); break;}
                        case 7: {
                            bullets.push_back(Bullet(1, plant->x, plant->y, 20, plant->y, xMapCoordinateInterval[plant->x] - 10)); 
                            bullets.push_back(Bullet(1, plant->x, plant->y, 20, plant->y, xMapCoordinateInterval[plant->x] - 60));
                            break;
                        }default: break;
                    }
                }
            }
        }

        //铲子选中时的白色半透明条形应该在传送带图层下
        if(mouse_status.shovelActive){
            mapx = PixeltoMapCoordinate(m.x, m.y)%100, mapy = PixeltoMapCoordinate(m.x, m.y)/100; //代表铲子的位置 x是左数第几列 y是上数第几行
            if(mapx >= 1 && mapx <= 9 && mapy >= 1 && mapy <= 5){
                alpha_composite(&background, 150, yMapCoordinateInterval[mapy - 1], &selectedx); //横向白色半透明条
                alpha_composite(&background, xMapCoordinateInterval[mapx - 1] + 220, 0, &selectedy); //横向白色半透明条
            }
        }else if(mouse_status.mouseType == 2){
            mapx = PixeltoMapCoordinate(m.x, m.y)%100, mapy = PixeltoMapCoordinate(m.x, m.y)/100; //代表植物的位置 x是左数第几列 y是上数第几行
            if(plantmap[mapx][mapy] == 0 && mapx >= 1 && mapx <= 9 && mapy >= 1 && mapy <= 5){     
                switch (mouse_status.selectedCard){
                    case 1:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &Peashooter_[0], 128); break;}
                    case 2:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &Sunflower_[0], 128); break;}
                    case 3:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &Wallnut_[0], 128); break;}
                    case 4:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &PotatoMine_[0], 128); break;}
                    case 5:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &CherryBomb_[0], 128); break;}
                    case 6:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &SnowPea_[0], 128); break;}
                    case 7:{alpha_composite_middle(&background, xMapCoordinateInterval[mapx - 1] + 260, yMapCoordinateInterval[mapy - 1] + 40, &Repeater_[0], 128); break;}
                }
                switch (mouse_status.selectedCard){
                    case 1:{alpha_composite_middle(&background, m.x + 220, m.y, &Peashooter_[0]); break;}
                    case 2:{alpha_composite_middle(&background, m.x + 220, m.y, &Sunflower_[0]); break;}
                    case 3:{alpha_composite_middle(&background, m.x + 220, m.y, &Wallnut_[0]); break;}
                    case 4:{alpha_composite_middle(&background, m.x + 220, m.y, &PotatoMine_[0]); break;}
                    case 5:{alpha_composite_middle(&background, m.x + 220, m.y, &CherryBomb_[0]); break;}
                    case 6:{alpha_composite_middle(&background, m.x + 220, m.y, &SnowPea_[0]); break;}
                    case 7:{alpha_composite_middle(&background, m.x + 220, m.y, &Repeater_[0]); break;}
                }
            }
        }

      
        cropped_csd_1 = crop_image(&csd_1, 1 + (((nowms - start) / 32) % 512) , 0, (((nowms - start) / 32) % 512) + 492, 15); //传送带每2帧移动1像素
        //画好传送带
        if (cropped_csd_1) {
            alpha_composite(&background, 220 + 94, 0, &csd); //绘制传送带底板
            alpha_composite(&background, 103 + 220, 63, cropped_csd_1); //绘制传送带条形
            delete cropped_csd_1;
        }
        else{
            cout << "crop_image failed" << endl; //用此方法解决一些玄学错误
        }
        
        //画好植物卡片
        dopixelmove = 1-dopixelmove;
        cardlen = csd_cards.size();
        for(int i = 0; i < cardlen; ++i){
            card = csd_cards[i];
            cardimg = SeedPacket; //51*71
            switch (card.plantid){
                case 1:{alpha_composite_middle(&cardimg,  26, 29, &Peashooter_scaled); break;}
                case 2:{alpha_composite_middle(&cardimg,  26, 29, &Sunflower_scaled); break;}
                case 3:{alpha_composite_middle(&cardimg,  26, 29, &Wallnut_scaled); break;}
                case 4:{alpha_composite_middle(&cardimg,  26, 29, &PotatoMine_scaled); break;}
                case 5:{alpha_composite_middle(&cardimg,  26, 29, &CherryBomb_scaled); break;}
                case 6:{alpha_composite_middle(&cardimg,  26, 29, &SnowPea_scaled); break;}
                case 7:{alpha_composite_middle(&cardimg,  26, 29, &Repeater_scaled); break;}
            }
            if(card.isselected) alpha_composite(&cardimg, 0, 0, &SeedPacketSelected); //绘制阴影
            if(card.canmove){
                if(card.pixelx >= 765 && card.pixelx <= 765 + 51){
                    cropped_card = crop_image(&cardimg, 0, 0, 51 + 765 - card.pixelx, 71);
                    if (cropped_card) {
                        alpha_composite(&background, card.pixelx, 7, cropped_card); 
                        delete cropped_card;
                    }
                    else{
                        cout << "crop_image failed" << endl; //用此方法解决一些玄学错误
                    }
                } 
                else if(card.pixelx <= 765) alpha_composite(&background, card.pixelx, 7, &cardimg);

                if(card.pixelx > 320 + i * 51) csd_cards[i].pixelx -= dopixelmove;
                else card.canmove = 0;
            }else{
                alpha_composite(&background, card.pixelx, 7, &cardimg);
            }
        }

        
        for(int i=0; i<bullets.size(); ++i){
            auto bullet = bullets[i];
            alpha_composite_middle(&background, bullet.pixelx + 220, yMapCoordinateInterval[bullet.line - 1] + 72, &peashadow);    
        }

        for(int i = 0; i<planted_plants.size(); ++i){ //遍历植物vector 铲子之下传送带之上
            auto plant = planted_plants[i];
            DrawPlant(&background, xMapCoordinateInterval[plant->x - 1] + 260, yMapCoordinateInterval[plant->y - 1] + 40, plant->id, plant->getFrame(nowms), 1, plant->health); //绘制植物
        }

        alpha_composite(&background, 824, 0, &ShovelBank); //绘制铲子框
        if(mouse_status.shovelActive){
            alpha_composite_middle(&background, m.x + 242, m.y - 20, &ShovelHiRes); //铲子被选中
        }else alpha_composite_middle(&background, 859, 36, &Shovel); //绘制铲子 70*72
        
        for(int i=0;i<zombies.size();++i){
            if(zombies[i]->dying){
                zombies.erase(zombies.begin() + i);
            }
        }

        for (auto& zombie : zombies) { //遍历僵尸vector
            if(zombie->is_cold == 1){
                if(zombie->cold_time + 10000 <= nowms){
                    zombie->is_cold = 0;
                }
            }
            zombie->updateFrame(zombie->is_cold);

            if(zombie->attacking){
                zombie->updateAttackingFrame(nowms);
                switch (zombie->id){
                    case 1:{DrawZombie(&background, zombie->actualx, zombie->pixely, ZombieAttack, zombie->getAttackingFrame(nowms), 1, zombie->is_cold, ZombieAttackCold); break;}
                    case 2:{DrawZombie(&background, zombie->actualx, zombie->pixely, ConeheadZombieAttack, zombie->getAttackingFrame(nowms), 1, zombie->is_cold, ConeheadZombieAttackCold); break;}
                    case 3:{DrawZombie(&background, zombie->actualx, zombie->pixely, BucketheadZombieAttack, zombie->getAttackingFrame(nowms), 1, zombie->is_cold, BucketheadZombieAttackCold); break;}
                }
                if(zombie->getAttackingFrame(nowms) == 2 || zombie->getAttackingFrame(nowms) == 22){
                    if(planted_plants[plantxytosubscript[PixeltoMapCoordinate(zombie->actualx - 120, zombie->pixely)%100][zombie->line]]->health - zombie->damage > 0 && zombie->nowframecntattacking == 0){
                        rand()%2?PlayAudio("Resources\\music\\HitSound\\chomp.wav"):PlayAudio("Resources\\music\\HitSound\\chomp2.wav");        
                    }      
                    if(zombie->nowframecntattacking == 0)
                        planted_plants[plantxytosubscript[PixeltoMapCoordinate(zombie->actualx - 120, zombie->pixely)%100][zombie->line]]->takeDamage(zombie->damage);       
                }
                continue;
            }

            if(zombie->nowframe == 0 && zombie->resetflag == 0){
                    zombie->pixelx -= 50;
                    zombie->resetflag = 1;
            } //第0帧处理绘制pixelx        

            zombie->actualx = zombie->pixelx - ZombieWalkingOffset[zombie->nowframe];

            switch (zombie->id)
            {
                case 1:{DrawZombie(&background, zombie->actualx, zombie->pixely, WalkingZombie, zombie->nowframe, 1, zombie->is_cold, WalkingZombieCold); break;}
                case 2:{DrawZombie(&background, zombie->actualx, zombie->pixely, WalkingConeheadZombie, zombie->nowframe, 1, zombie->is_cold, WalkingConeheadZombieCold); break;}
                case 3:{DrawZombie(&background, zombie->actualx, zombie->pixely, WalkingBucketheadZombie, zombie->nowframe, 1, zombie->is_cold, WalkingBucketheadZombieCold); break;}
                default: break;
            }
            

            if(zombie->nowframe == 46){        
                zombie -> resetflag = 0; //最后一帧重置
            } 
        }

        for(int i=0; i<bullets.size(); ++i){
            auto bullet = bullets[i];
            // printf("%d", bullets.size());
            if(bullet.pixelx >= xMapCoordinateInterval[bullet.spawnerx] && bullet.is_collided == 0){
                if(bullet.is_cold_bullet == 0){
                    alpha_composite_middle(&background, bullet.pixelx + 220, yMapCoordinateInterval[bullet.line - 1] + 22, &pea); 
                }else{
                    alpha_composite_middle(&background, bullet.pixelx + 220, yMapCoordinateInterval[bullet.line - 1] + 22, &icepea); 
                }
            }
            
            bullets[i].pixelx += 5;            
        }

        for(int i=0; i<bullet_collide.size(); ++i){
            auto bullet = bullet_collide[i];
            if(bullet.second.second + 150 <= nowms){
                bullet_collide.erase(bullet_collide.begin() + i);
                continue;
            }
            if(bullet.second.first == 0){
                alpha_composite_middle(&background, bullet.first.first + 220, bullet.first.second, &peahit); 
            }else{
                alpha_composite_middle(&background, bullet.first.first + 220, bullet.first.second, &icepeahit); 
            }
        }

        cleardevice();
        putimage(0, 0, 800, 600, &background, 220 ,0);
        // std::string str = std::to_string(m.x) + "," + std::to_string(m.y);
        // outtextxy(0, 0, _T(str.c_str()));
        // str = "MouseType:" + std::to_string(mouse_status.mouseType);
        // outtextxy(0, 20, _T(str.c_str()));
        // str = std::to_string(Timer::get_ms());
        // outtextxy(0, 40, _T(str.c_str()));

        // for (auto& zombie : zombies) { //遍历僵尸vector
        //     str = std::to_string(zombie->health);
        //     // cout<< str << endl;
        //     outtextxy(zombie->actualx - 130, zombie->pixely + 70, _T(str.c_str()));

        //     str = std::to_string(zombie->dying);
        //     outtextxy(zombie->actualx - 130, zombie->pixely + 90, _T(str.c_str()));

        //     str = std::to_string(zombie->actualx);
        //     outtextxy(zombie->actualx - 130, zombie->pixely + 110, _T(str.c_str()));

        //     str = std::to_string(zombie->getAttackingFrame(nowms));
        //     outtextxy(zombie->actualx - 130, zombie->pixely + 130, _T(str.c_str()));
        // }

        // for (auto& plant : planted_plants){
        //     str = std::to_string(plant->health);
        //     outtextxy(xMapCoordinateInterval[plant->x - 1] + 20, yMapCoordinateInterval[plant->y - 1] + 60, _T(str.c_str()));
        // }

        lastframe = nowms;
    }
      
   
    

    closegraph();
    return 0;    

}
