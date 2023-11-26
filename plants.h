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
