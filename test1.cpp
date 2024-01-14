#include <graphics.h>
#include <conio.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define SIZE 30

int map[HEIGHT][WIDTH] = {0};

struct Point {
    int x;
    int y;
} cur, next;

int shapes[7][4][4] = {
    // I
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // J
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // L
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // S
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // T
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Z
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

void drawBlock(int x, int y, int color) {
    setfillcolor(color);
    setlinecolor(BLACK);
    fillrectangle(x * SIZE, y * SIZE, (x + 1) * SIZE, (y + 1) * SIZE);
    rectangle(x * SIZE, y * SIZE, (x + 1) * SIZE, (y + 1) * SIZE);
}

void drawMap() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (map[i][j] != 0) {
                drawBlock(j, i, map[i][j]);
            }
        }
    }
}

bool checkCollision(int x, int y, int shape) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shapes[shape][i][j] != 0) {
                int nx = x + j;
                int ny = y + i;
                if (nx < 0 || nx >= WIDTH || ny >= HEIGHT || (ny >= 0 && map[ny][nx] != 0)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void mergeShape(int x, int y, int shape) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shapes[shape][i][j] != 0) {
                int nx = x + j;
                int ny = y + i;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    map[ny][nx] = shape + 1;
                }
            }
        }
    }
}

void removeFullLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        bool full = true;
        for (int j = 0; j < WIDTH; j++) {
            if (map[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    map[k][j] = map[k - 1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                map[0][j] = 0;
            }
            i++;
        }
    }
}

void init() {
    initgraph(WIDTH * SIZE, HEIGHT * SIZE);
    setbkcolor(WHITE);
    cleardevice();
    settextcolor(BLACK);
    settextstyle(20, 0, _T("宋体"));
    outtextxy(10, 10, _T("操作说明："));
    outtextxy(10, 40, _T("a - 左移"));
    outtextxy(10, 70, _T("d - 右移"));
    outtextxy(10, 100, _T("s - 旋转"));
    outtextxy(10, 130, _T("空格 - 直接落下"));
    cur.x = WIDTH / 2 - 2;
    cur.y = 0;
    next.x = WIDTH / 2 - 2;
    next.y = 0;
    srand((unsigned)time(NULL));
}

void drawNextShape(int shape) {
    setfillcolor(WHITE);
    setlinecolor(BLACK);
    fillrectangle(WIDTH * SIZE + 10, 10, WIDTH * SIZE + 10 + 4 * SIZE, 10 + 4 * SIZE);
    rectangle(WIDTH * SIZE + 10, 10, WIDTH * SIZE + 10 + 4 * SIZE, 10 + 4 * SIZE);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shapes[shape][i][j] != 0) {
                drawBlock(WIDTH + j, i, shape + 1);
            }
        }
    }
}

void generateNextShape() {
    cur.x = next.x;
    cur.y = next.y;
    next.x = WIDTH / 2 - 2;
    next.y = 0;
    int shape = rand() % 7;
    drawNextShape(shape);
    if (checkCollision(cur.x, cur.y, shape)) {
        // Game over
        closegraph();
        exit(0);
    }
    mergeShape(cur.x, cur.y, shape);
}

void moveLeft() {
    if (!checkCollision(cur.x - 1, cur.y, cur.shape)) {
        mergeShape(cur.x, cur.y, 0);
        cur.x--;
        mergeShape(cur.x, cur.y, cur.shape);
    }
}

void moveRight() {
    if (!checkCollision(cur.x + 1, cur.y, cur.shape)) {
        mergeShape(cur.x, cur.y, 0);
        cur.x++;
        mergeShape(cur.x, cur.y, cur.shape);
    }
}

void rotate() {
    int newShape = (cur.shape + 1) % 4;
    if (!checkCollision(cur.x, cur.y, newShape)) {
        mergeShape(cur.x, cur.y, 0);
        cur.shape = newShape;
        mergeShape(cur.x, cur.y, cur.shape);
    }
}

void fall() {
    if (!checkCollision(cur.x, cur.y + 1, cur.shape)) {
        mergeShape(cur.x, cur.y, 0);
        cur.y++;
        mergeShape(cur.x, cur.y, cur.shape);
    } else {
        mergeShape(cur.x, cur.y, cur.shape);
        removeFullLines();
        generateNextShape();
    }
}

int main() {
    init();
    int delay = 500;
    int lastTime = clock();
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a') {
                moveLeft();
            } else if (ch == 'd') {
                moveRight();
            } else if (ch == 's') {
                rotate();
            } else if (ch == ' ') {
                while (!checkCollision(cur.x, cur.y + 1, cur.shape)) {
                    mergeShape(cur.x, cur.y, 0);
                    cur.y++;
                }
                mergeShape(cur.x, cur.y, cur.shape);
                removeFullLines();
                generateNextShape();
            }
        }
        int currentTime = clock();
        if (currentTime - lastTime >= delay) {
            fall();
            lastTime = currentTime;
        }
        drawMap();
        Sleep(10);
    }
    return 0;
}
