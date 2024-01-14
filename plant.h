#include <graphics.h>
#include <vector>
using namespace std;

IMAGE Peashooter_scaled, Sunflower_scaled, Wallnut_scaled, PotatoMine_scaled, CherryBomb_scaled, SnowPea_scaled, Repeater_scaled;
IMAGE pea, icepea, peashadow;
IMAGE peahit, icepeahit;
std::vector<IMAGE> Peashooter_(57);
std::vector<IMAGE> Sunflower_(57);
std::vector<IMAGE> Wallnut_(57);
std::vector<IMAGE> Wallnut_cracked1(57);
std::vector<IMAGE> Wallnut_cracked2(57);
std::vector<IMAGE> PotatoMine_(57);
std::vector<IMAGE> CherryBomb_(57);
std::vector<IMAGE> SnowPea_(57);
std::vector<IMAGE> Repeater_(57);

IMAGE id2img(int id, int frame = 0, int issmall = 0){
    if(issmall){
        switch (id){
            case 1: return Peashooter_scaled;
            case 2: return Sunflower_scaled;
            case 3: return Wallnut_scaled;
            case 4: return PotatoMine_scaled;
            case 5: return CherryBomb_scaled;
            case 6: return SnowPea_scaled;
            case 7: return Repeater_scaled;
        }   
        return Peashooter_scaled;
    }
    else{
        switch (id){
            case 1: return Peashooter_[frame];
            case 2: return Sunflower_[frame];
            case 3: return Wallnut_[frame];
            case 4: return PotatoMine_[frame];
            case 5: return CherryBomb_[frame];
            case 6: return SnowPea_[frame];
            case 7: return Repeater_[frame];
        }   
        return Peashooter_[frame];
    }
    
    
}

class Bullet {
public:
    int spawnerid;
    int spawnerx, spawnery;
    int damage;
    int pixelx;
    int line;
    int is_cold_bullet;
    int is_collided = 0, collided_time = 0;

    Bullet(int spawnerid, int spawnerx, int spawnery, int damage, int line, int pixelx, int is_cold_bullet = 0) :
        spawnerid(spawnerid),
        spawnerx(spawnerx),
        spawnery(spawnery),
        damage(damage),
        line(line),
        pixelx(pixelx),
        is_cold_bullet(is_cold_bullet)
        {}
};

class Plant {
public:
    int id;
    int health;
    int damage;
    bool CanSpwanBullet;
    int ShootInterval;
    int lastshoot = 0;
    int x, y;
    int pixelx, pixely;
    long long planttime;

    Plant(int id, int health, int damage, bool CanSpwanBullet, int ShootInterval, int x, int y, long long planttime) : 
        id(id),
        health(health), 
        damage(damage), 
        CanSpwanBullet(CanSpwanBullet),
        ShootInterval(ShootInterval),
        x(x),
        y(y),
        planttime(planttime)
        {}

    int getFrame(int nowms) {
        int framesum;
        switch (id) {
            case 1: framesum = 25; break;
            case 2: framesum = 25; break;
            case 3: framesum = 17; break;
            case 4: framesum = 11; break;
            case 5: framesum = 14; break;
            case 6: framesum = 25; break;
            case 7: framesum = 25; break;
            default: framesum = 1;
        }

        return ((nowms - planttime) / 50) % framesum;
    }

    void takeDamage(int damage) {
        health -= damage;
    }
};

class Peashooter : public Plant {
public:
    Peashooter(int x, int y, long long planttime) : Plant(1, 300, 20, true, 1400, x, y, planttime) {}
};

class Sunflower : public Plant {
public:
    Sunflower(int x, int y, long long planttime) : Plant(2, 300, 20, false, 0, x, y, planttime) {}
};

class Wallnut : public Plant {
public:
    Wallnut(int x, int y, long long planttime) : Plant(3, 3000, 20, false, 0, x, y, planttime) {}
};

class PotatoMine : public Plant {
public:
    PotatoMine(int x, int y, long long planttime) : Plant(4, 300, 20, false, 0, x, y, planttime) {}
};

class CherryBomb : public Plant {
public:
    CherryBomb(int x, int y, long long planttime) : Plant(5, 300, 20, false, 0, x, y, planttime) {}
};

class SnowPea : public Plant {
public:
    SnowPea(int x, int y, long long planttime) : Plant(6, 300, 20, true, 1400, x, y, planttime) {}
};

class Repeater : public Plant {
public:
    Repeater(int x, int y, long long planttime) : Plant(7, 300, 20, true, 1400, x, y, planttime) {}
};
