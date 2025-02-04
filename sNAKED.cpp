#include <bits/stdc++.h>
#include <conio.h> // key press kbhit
#include <windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point {
    int xCoord;
    int yCoord;
    Point() {}
    Point(int x, int y) : xCoord(x), yCoord(y) {}
};

// Base Snake class
class Snake {
protected:
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    virtual ~Snake() {} // Virtual destructor for polymorphism

    int getLength() {
        return length;
    }

    void changeDirection(char newDirection) {
        if ((newDirection == DIR_UP && direction != DIR_DOWN) ||
            (newDirection == DIR_DOWN && direction != DIR_UP) ||
            (newDirection == DIR_LEFT && direction != DIR_RIGHT) ||
            (newDirection == DIR_RIGHT && direction != DIR_LEFT)) {
            direction = newDirection;
        }
    }

    // Virtual function for movement - can be overridden by subclasses
    virtual bool move(Point food) {
        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        switch (direction) {
            case DIR_UP: body[0].yCoord--; break;
            case DIR_DOWN: body[0].yCoord++; break;
            case DIR_LEFT: body[0].xCoord--; break;
            case DIR_RIGHT: body[0].xCoord++; break;
        }

        // Snake bites itself
        for (int i = 1; i < length; i++) {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord) {
                return false;
            }
        }

        // Snake eats food
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            body[length] = Point(body[length - 1].xCoord, body[length - 1].yCoord);
            length++;
        }

        return true;
    }
};


class Board {
    Snake *snake; // Polymorphic pointer for Snake or FastSnake
    const char SNAKE_BODY = 'O';
    Point food;
    const char FOOD = 'o';
    int score;

public:
    Board(Snake* s) : snake(s) { // Accept Snake pointer
        spawnFood();
        score = 0;
    }

    ~Board() {
        delete snake; // Proper cleanup
    }

    int getScore() {
        return score;
    }

    void spawnFood() {
        int x = rand() % (consoleWidth - 1);
        int y = rand() % (consoleHeight - 1);
        food = Point(x, y);
    }

    void displayCurrentScore() {
        gotoxy(consoleWidth / 2, 0);
        cout << "Current Score : " << score;
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void draw() {
        system("cls");

        // Draw Snake
        setColor(2); // Green color for Snake
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }

        // Draw Food
        setColor(4); // Red color for Food
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;

        // Reset color to default
        setColor(7); 
        displayCurrentScore();
    }

    bool update() {
        bool isAlive = snake->move(food); // Polymorphic move call
        if (!isAlive) {
            return false;
        }

        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();
        }
        return true;
    }

    void getInput() {
        if (kbhit()) {
            int key = getch();
            if (key == 'w' || key == 'W') {
                snake->changeDirection(DIR_UP);
            } else if (key == 'a' || key == 'A') {
                snake->changeDirection(DIR_LEFT);
            } else if (key == 's' || key == 'S') {
                snake->changeDirection(DIR_DOWN);
            } else if (key == 'd' || key == 'D') {
                snake->changeDirection(DIR_RIGHT);
            }
        }
    }
};

int main() {
    srand(time(0));
    initScreen();

    // Polymorphism: Using base class pointer to handle both Snake and FastSnake
    Snake* snake;
    
    snake = new Snake(10, 10); // Normal Snake
    
    Board* board = new Board(snake);

    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(100);
    }

    cout << "Game over" << endl;
    cout << "Final score is: " << board->getScore() << endl;

    delete board; // Clean up
    return 0;
}