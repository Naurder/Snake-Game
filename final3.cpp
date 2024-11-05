#include <bits/stdc++.h>    //that library is good for Release, but not for other collaborators
#include <conio.h>
#include <windows.h>

using namespace std;        //consider removing this line and use std::
                            //bringing all the names from the std namespace into the global namespace. This can lead to name collisions
#define MAXLEN 1000

const char DIR_UP = 72;
const char DIR_DOWN = 80;
const char DIR_LEFT = 75;
const char DIR_RIGHT = 77;

int consolewidth, consoleheight;

void initscreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleheight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consolewidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct point {
    int xcoord;
    int ycoord;
    point(int x, int y) {
        xcoord = x;
        ycoord = y;
    }
    point() : xcoord(0), ycoord(0) {}
};

class snake {
    int length;
    char direction;
public:
    point body[MAXLEN];
    snake(int x, int y) {
        length = 1;
        body[0] = point(x, y);
        direction = DIR_RIGHT;
    }
    int getlength() {
        return length;
    }
    void changeDirection(char newdirection) {
        if (newdirection == DIR_UP && direction != DIR_DOWN) {
            direction = newdirection;
        } else if (newdirection == DIR_DOWN && direction != DIR_UP) {
            direction = newdirection;
        } else if (newdirection == DIR_LEFT && direction != DIR_RIGHT) {
            direction = newdirection;
        } else if (newdirection == DIR_RIGHT && direction != DIR_LEFT) {
            direction = newdirection;
        }
    }
    bool move(point food) {
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }
        switch (direction) {
            int val;
        case DIR_UP:
            val = body[0].ycoord;
            body[0].ycoord = val - 1;
            if (body[0].ycoord < 0) body[0].ycoord = consoleheight - 1; // Reappear from bottom
            Sleep(50); // Add sleep for up direction
            break;
        case DIR_DOWN:
            val = body[0].ycoord;
            body[0].ycoord = val + 1;
            if (body[0].ycoord >= consoleheight) body[0].ycoord = 0; // Reappear from top
            Sleep(50); // Add sleep for down direction
            break;
        case DIR_LEFT:
            val = body[0].xcoord;
            body[0].xcoord = val - 1;
            if (body[0].xcoord < 0) body[0].xcoord = consolewidth - 1; // Reappear from right
            break;
        case DIR_RIGHT:
            val = body[0].xcoord;
            body[0].xcoord = val + 1;
            if (body[0].xcoord >= consolewidth) body[0].xcoord = 0; // Reappear from left
            break;
        default:
            break;
        }
        for (int i = 1; i < length; i++) {
            if (body[0].xcoord == body[i].xcoord && body[0].ycoord == body[i].ycoord) {
                return false;
            }
        }
        if (food.xcoord == body[0].xcoord && food.ycoord == body[0].ycoord) {
            body[length] = point(body[length - 1].xcoord, body[length - 1].ycoord);
            length++;
        }
        return true;
    }
};

class board {
    snake* Snake;
    const char SNAKE = 'O';
    point food;
    const char FOOD = '+';
    int score;
public:
    board() {
        spawnFood();
        Snake = new snake(10, 10);
        score = 0;
    }
    ~board() {
        delete Snake;
    }
    int getscore() {
        return score;
    }
    void spawnFood() {
        int x = rand() % consolewidth;
        int y = rand() % consoleheight;
        food = point(x, y);
    }
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    void draw() {
        system("cls");
        for (int i = 0; i < Snake->getlength(); i++) {
            gotoxy(Snake->body[i].xcoord, Snake->body[i].ycoord);
            cout << SNAKE;
        }
        gotoxy(food.xcoord, food.ycoord);
        cout << FOOD;
    }

    bool update() {
        bool isAlive = Snake->move(food);
        if (!isAlive) {
            return false;
        }
        if (food.xcoord == Snake->body[0].xcoord && food.ycoord == Snake->body[0].ycoord) {
            score++;
            spawnFood();
        }
        return true;
    }
    void getinput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) { // Arrow keys generate a two-part code
                key = _getch();
            }
            if (key == 72 || key == 'w' || key == 'W') {
                Snake->changeDirection(DIR_UP);
            } else if (key == 80 || key == 's' || key == 'S') {
                Snake->changeDirection(DIR_DOWN);
            } else if (key == 75 || key == 'a' || key == 'A') {
                Snake->changeDirection(DIR_LEFT);
            } else if (key == 77 || key == 'd' || key == 'D') {
                Snake->changeDirection(DIR_RIGHT);
            }
        }
    }
};

int main() {
    srand(  static_cast<unsigned int>(time(0))    );      //fixed warning about possible data loss;    pls read about year 2038 problem 32-bit systems
    initscreen();
    board* Board = new board();
    while (Board->update()) {
        Board->getinput();
        Board->draw();
        Sleep(100);
    }
    cout << "GAME OVER\n";
    system("pause");
    delete Board;
    return 0;
}
