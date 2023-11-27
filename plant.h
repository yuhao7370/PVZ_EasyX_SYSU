#include <graphics.h>
#include <vector>
using namespace std;

IMAGE Peashooter_scaled, Sunflower_scaled, Wallnut_scaled, PotatoMine_scaled, CherryBomb_scaled, SnowPea_scaled, Repeater_scaled;
std::vector<IMAGE> Peashooter(57);
std::vector<IMAGE> Sunflower(57);
std::vector<IMAGE> Wallnut(57);
std::vector<IMAGE> PotatoMine(57);
std::vector<IMAGE> CherryBomb(57);
std::vector<IMAGE> SnowPea(57);
std::vector<IMAGE> Repeater(57);

IMAGE id2img(int id, int frame = 0, int issmall = 0){
    if(issmall){
        switch (id){
            case 1: return Peashooter_scaled;
            case 2: return Sunflower_scaled;
            case 3: return Wallnut_scaled;
            case 4: return PotatoMine_scaled;
            case 5: return CherryBomb_scaled;
            case 6: return SnowPea_scaled;
            case 8: return Repeater_scaled;
        }   
        return Peashooter_scaled;
    }
    else{
        switch (id){
            case 1: return Peashooter[frame];
            case 2: return Sunflower[frame];
            case 3: return Wallnut[frame];
            case 4: return PotatoMine[frame];
            case 5: return CherryBomb[frame];
            case 6: return SnowPea[frame];
            case 8: return Repeater[frame];
        }   
        return Peashooter[frame];
    }
    
    
}

class Plant {
public:
    int id;
    int health;
    int damage;
    bool CanSpwanBullet;
    int ShootInterval;
    int x, y;
    int pixelx, pixely;

    Plant(int id, int health, int damage, bool CanSpwanBullet, int ShootInterval, int x, int y) : 
        id(id),
        health(health), 
        damage(damage), 
        CanSpwanBullet(CanSpwanBullet),
        ShootInterval(ShootInterval),
        x(x),
        y(y)
        {}

    void attack(Zombie& zombie) {
        zombie.takeDamage(damage);
    }

    void takeDamage(int damage) {
        health -= damage;
    }
};

class Peashooter : public Plant {
public:
    Peashooter(int x, int y) : Plant(1, 300, 20, true, 1000, x, y) {}
};

class Sunflower : public Plant {
public:
    Sunflower(int x, int y) : Plant(2, 300, 0, false, 0, x, y) {}
};

class Wallnut : public Plant {
public:
    Wallnut(int x, int y) : Plant(3, 4000, 0, false, 0, x, y) {}
};

class PotatoMine : public Plant {
public:
    PotatoMine(int x, int y) : Plant(4, 300, 1800, false, 0, x, y) {}
};

class CherryBomb : public Plant {
public:
    CherryBomb(int x, int y) : Plant(5, 300, 1800, false, 0, x, y) {}
};

class SnowPea : public Plant {
public:
    SnowPea(int x, int y) : Plant(6, 300, 20, true, 1000, x, y) {}
};

class Repeater : public Plant {
public:
    Repeater(int x, int y) : Plant(8, 300, 20, true, 1000, x, y) {}
};
