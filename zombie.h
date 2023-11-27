int ZombieLinePos[17] = { -1, 22, 122, 228, 318, 420, -1 };

int ZombieWalkingOffset[57] = { 
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 16, 16, 16,
    16, 16, 16, 16, 17, 19, 22, 24, 27, 30,
    33, 36, 39, 41, 43, 45, 47, 49, 50, 50,
    50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
}; //僵尸走路的帧偏移序列

int ZombieStandingPos[17][2] = {
    {1096, 30},  {986, 37}, {1166, 63}, {1046, 162}, {988, 216},  {1097, 210},  {1218, 209},
    {1152, 249}, {1048, 254}, {993, 301},  {1047, 340}, {1146, 335}, {1208, 401}, {1102, 405}, 
    {1172, 426}, {1054, 438}
}; //加载界面时僵尸站的位置

class Zombie {
public:
    int id;
    int health;
    int damage;
    int line;
    int pixelx, pixely;
    int actualx;
    int resetflag = 0;
    long long spawntime;

    Zombie(int id, int health, int damage, int line, long long spawntime = 0) : 
        id(id),
        health(health), 
        damage(damage), 
        line(line),
        spawntime(spawntime),
        pixelx(800), // 1000正好是屏幕外
        actualx(pixelx),
        pixely(ZombieLinePos[line]) {}


    long long getFrame(long long nowms) {
        return ((nowms - spawntime) / 9 / 16) % 47;
    }

    void takeDamage(int damage) {
        health -= damage;
    }
};

class NormalZombie : public Zombie {
public:
    NormalZombie(int line, long long spawntime) : Zombie(1, 300, 20, line, spawntime) {}
};

class ConeheadZombie : public Zombie {
public:
    ConeheadZombie(int line, long long spawntime) : Zombie(2, 560, 20, line, spawntime) {}
};

class BucketheadZombie : public Zombie {
public:
    BucketheadZombie(int line, long long spawntime) : Zombie(3, 1100, 20, line, spawntime) {}
};
