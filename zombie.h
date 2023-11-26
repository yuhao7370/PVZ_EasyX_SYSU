int ZombieLinePos[17] = { -1, 20, 160, 240, 320, 400, 480 };

class Zombie {
public:
    int id;
    int health;
    int damage;
    int line;
    int pixelx, pixely;
    long long spawntime;

    Zombie(int id, int health, int damage, int line, long long spawntime = 0) : 
        id(id),
        health(health), 
        damage(damage), 
        line(line),
        spawntime(spawntime),
        pixelx(850),
        pixely(ZombieLinePos[line]) {}


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
