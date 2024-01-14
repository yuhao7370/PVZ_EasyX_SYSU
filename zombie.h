std::vector<IMAGE> StandingZombie(17);              //普通僵尸
std::vector<IMAGE> StandingConeheadZombie(17);      //路障僵尸
std::vector<IMAGE> StandingBucketheadZombie(17);    //铁桶僵尸

std::vector<IMAGE> WalkingZombie(57);              //普通僵尸
std::vector<IMAGE> WalkingConeheadZombie(57);      //路障僵尸
std::vector<IMAGE> WalkingBucketheadZombie(57);    //铁桶僵尸

std::vector<IMAGE> ZombieAttack(57);              //普通僵尸
std::vector<IMAGE> ConeheadZombieAttack(57);      //路障僵尸
std::vector<IMAGE> BucketheadZombieAttack(57);    //铁桶僵尸

std::vector<IMAGE> WalkingZombieCold(57);              //普通僵尸
std::vector<IMAGE> WalkingConeheadZombieCold(57);      //路障僵尸
std::vector<IMAGE> WalkingBucketheadZombieCold(57);    //铁桶僵尸

std::vector<IMAGE> ZombieAttackCold(57);              //普通僵尸
std::vector<IMAGE> ConeheadZombieAttackCold(57);      //路障僵尸
std::vector<IMAGE> BucketheadZombieAttackCold(57);    //铁桶僵尸

std::vector<IMAGE> ZombieLostHead(57);
std::vector<IMAGE> ZombieLostHeadAttack(57);
std::vector<IMAGE> ZombieDie(57);
std::vector<IMAGE> ZombieHead(57);

std::vector<IMAGE> ZombieLostHeadCold(57);
std::vector<IMAGE> ZombieLostHeadAttackCold(57);
std::vector<IMAGE> ZombieDieCold(57);
std::vector<IMAGE> ZombieHeadCold(57);

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
    int is_cold = 0, cold_time;
    int nowframe = 0;
    int picframe = 5;
    int nowframecnt = 0;
    int attacking = 0, attackstarttime = 0;
    int dying = 0, dyingstarttime = 0;
    long long spawntime;

    int nowframeattacking = 0;
    int picframeattacking = 5;
    int nowframecntattacking = 0;

    Zombie(int id, int health, int damage, int line, long long spawntime = 0) : 
        id(id),
        health(health), 
        damage(damage), 
        line(line),
        spawntime(spawntime),
        pixelx(1000), // 1000正好是屏幕外
        actualx(pixelx),
        pixely(ZombieLinePos[line]) {}


    int getAttackingFrame(long long nowms) {
        return nowframeattacking;
    }

    void updateAttackingFrame(long long nowms, int iscold = 0) {
        if(is_cold) picframeattacking = 2;
        else picframeattacking = 1;

        if(nowframecntattacking >= picframeattacking){
            nowframecntattacking = 0;
            ++nowframeattacking;
        }
        else{
            nowframecntattacking++;
        }

        nowframeattacking %= 40;
    }

    void updateFrame(long long nowms, int iscold = 0) {
        if(is_cold) picframe = 9;
        else picframe = 5;

        if(nowframecnt == picframe){
            nowframecnt = 0;
            ++nowframe;
        }
        else{
            nowframecnt++;
        }

        nowframe %= 47;
    }

    void takeDamage(int damage) {
        health -= damage;
    }
};

class NormalZombie : public Zombie {
public:
    NormalZombie(int line, long long spawntime) : Zombie(1, 181, 45, line, spawntime) {}
};

class ConeheadZombie : public Zombie {
public:
    ConeheadZombie(int line, long long spawntime) : Zombie(2, 370+181, 45, line, spawntime) {}
};

class BucketheadZombie : public Zombie {
public:
    BucketheadZombie(int line, long long spawntime) : Zombie(3, 1100+181, 45, line, spawntime) {}
};
