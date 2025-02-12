#include <iostream>
#include <conio.h>
#include <stdexcept>
#include <windows.h>
#include <ctime>
#include <limits>
#include <mmsystem.h> // Include the header for PlaySound

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

struct Point
{
    int xCoord;
    int yCoord;
    Point() {}
    Point(int x, int y)
    {
        xCoord = x;
        yCoord = y;
    }
};

class Snake
{
    int length;
    char direction;

public:
    Point body[MAX_LENGTH];
    Snake(int x, int y)
    {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength()
    {
        return length;
    }

    void changeDirection(char newDirection)
    {
        if (newDirection == DIR_UP && direction != DIR_DOWN)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_DOWN && direction != DIR_UP)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
        {
            direction = newDirection;
        }
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
        {
            direction = newDirection;
        }
    }

    bool move(Point food)
    {
        for (int i = length - 1; i > 0; i--)
        {
            body[i] = body[i - 1];
        }

        switch (direction)
        {
        case DIR_UP:
            body[0].yCoord--;
            break;
        case DIR_DOWN:
            body[0].yCoord++;
            break;
        case DIR_RIGHT:
            body[0].xCoord++;
            break;
        case DIR_LEFT:
            body[0].xCoord--;
            break;
        }

        // Snake bites itself
        for (int i = 1; i < length; i++)
        {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
            {
                return false;
            }
        }

        // Snake eats food
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord)
        {
            body[length] = Point(body[length - 1].xCoord, body[length - 1].yCoord);
            length++;
            PlaySound(TEXT("eat.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play sound when snake eats food
        }

        return true;
    }
};

class Board
{
    Snake *snake;
    char snakeHeadChar;
    char snakeBodyChar;
    char foodChar;
    int snakeHeadColor;
    int snakeBodyColor;
    int foodColor;
    Point food;
    int score;
    int snakeSpeed;
    int difficultyLevel;
    void setColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

private:
    bool isGameWon;

public:
    Board(bool isQuickMatch, char headChar, char bodyChar, char foodChar, int headCol, int bodyCol, int foodCol)
        : snakeHeadChar(headChar), snakeBodyChar(bodyChar), foodChar(foodChar), snakeHeadColor(headCol), snakeBodyColor(bodyCol), foodColor(foodCol)
    {
        try
        {
            spawnFood();
            snake = new Snake(10, 10);
            if (snake == NULL)
            {
                throw runtime_error("Error: Memory allocation for Snake failed.");
            }
            score = 0;

            if (isQuickMatch)
            {
                difficultyLevel = 2; // Normal difficulty for quick match
                snakeSpeed = 75; // Normal: 75 milliseconds
            }
            else
            {
                // Ask the user for difficulty level until a valid input is provided
                while (true)
                {
                    cout << "Enter difficulty level (1 - Easy, 2 - Normal, 3 - Hard, 4 - Legendary): ";
                    cin >> difficultyLevel;

                    if (cin.fail() || difficultyLevel < 1 || difficultyLevel > 4)
                    {
                        cout << "Invalid input for difficulty level. Please try again." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    else
                    {
                        break;
                    }
                }

                // Set snake speed based on difficulty level
                switch (difficultyLevel)
                {
                    case 1: // Easy
                        snakeSpeed = 100; // 100 milliseconds
                        break;
                    case 2: // Normal
                        snakeSpeed = 75; // 75 milliseconds
                        break;
                    case 3: // Hard
                        snakeSpeed = 50; // 50 milliseconds
                        break;
                    case 4: // Legendary
                        snakeSpeed = 25; // 25 milliseconds
                        break;
                }
            }
        }
        catch (const runtime_error &e)
        {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
        isGameWon = false;
    }

    int getSnakeSpeed() const
    {
        return snakeSpeed;
    }

    ~Board()
    {
        delete snake;
    }

    int getScore()
    {
        return score;
    }

    void spawnFood()
    {
        int x = rand() % (consoleWidth - 3) + 1;
        int y = rand() % (consoleHeight - 3) + 1;
        food = Point(x, y);
    }

    void displayCurrentScore()
    {
        setCursorPosition(consoleWidth / 2 - 8, 0);
        cout << "Current Score: " << score << endl;
        cout.flush();
    }

    void setCursorPosition(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
        {
            throw runtime_error("Error: Unable to set console cursor position.");
        }
    }

    void draw()
    {
        system("cls");
        initScreen();
        setColor(snakeHeadColor);
        setCursorPosition(snake->body[0].xCoord, snake->body[0].yCoord);
        cout << snakeHeadChar;
        setColor(snakeBodyColor);
        for (int i = 1; i < snake->getLength(); i++)
        {
            setCursorPosition(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << snakeBodyChar;
        }

        setColor(foodColor);
        setCursorPosition(food.xCoord, food.yCoord);
        cout << foodChar;
        displayCurrentScore();
    }

    bool update()
    {
        bool isAlive = snake->move(food);
        if (isAlive == false || snake->body[0].xCoord >= consoleWidth - 1 || snake->body[0].yCoord >= consoleHeight - 1 || snake->body[0].xCoord < 0 || snake->body[0].yCoord < 0)
        {
            PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play sound when game is over
            return false;
        }

        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord)
        {
            score++;
            spawnFood();
        }

        // Check if the game is won
        if (score >= 50)
        {
            isGameWon = true;
            PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play sound when game is won
            return false;
        }
        return true;
    }

    void getInput()
    {
        if (_kbhit())
        {
            int key = _getch();
            if (key == 'w' || key == 'W' || key == 72)
            {
                snake->changeDirection(DIR_UP);
            }
            else if (key == 'a' || key == 'A' || key == 75)
            {
                snake->changeDirection(DIR_LEFT);
            }
            else if (key == 's' || key == 'S' || key == 80)
            {
                snake->changeDirection(DIR_DOWN);
            }
            else if (key == 'd' || key == 'D' || key == 77)
            {
                snake->changeDirection(DIR_RIGHT);
            }
        }
    }

    bool getIsGameWon() const
    {
        return isGameWon;
    }
};

void displayMenu()
{
    cout << "1. Quick Match" << endl;
    cout << "2. Custom" << endl;
    cout << "Enter your choice: ";
}

void displayCustomizationMenu(char &snakeHeadChar, char &snakeBodyChar, char &foodChar, int &snakeHeadColor, int &snakeBodyColor, int &foodColor)
{
    cout << "Enter snake head character: ";
    cin >> snakeHeadChar;
    cout << "Enter snake body character: ";
    cin >> snakeBodyChar;
    cout << "Enter food character: ";
    cin >> foodChar;

    cout << "Choose snake head color (0-15): ";
    cin >> snakeHeadColor;
    cout << "Choose snake body color (0-15): ";
    cin >> snakeBodyColor;
    cout << "Choose food color (0-15): ";
    cin >> foodColor;
}

int main()
{
    srand(time(0));
    try
    {
        initScreen();
        char snakeHeadChar = 'O';
        char snakeBodyChar = 'o';
        char foodChar = '*';
        int snakeHeadColor = 10; // Light green
        int snakeBodyColor = 2; // Green
        int foodColor = 12; // Red

        displayMenu();
        int choice;
        cin >> choice;

        if (choice == 2)
        {
            displayCustomizationMenu(snakeHeadChar, snakeBodyChar, foodChar, snakeHeadColor, snakeBodyColor, foodColor);
        }

        bool isQuickMatch = (choice == 1);
        Board *board = new Board(isQuickMatch, snakeHeadChar, snakeBodyChar, foodChar, snakeHeadColor, snakeBodyColor, foodColor);

        PlaySound(TEXT("start.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play sound when game starts

        while (board->update())
        {
            board->getInput();
            board->draw();
            Sleep(board->getSnakeSpeed());
        }
        if (board->getIsGameWon())
        {
            cout << endl << "                            Game Won" << endl;
        }
        else
        {
            auto clrscr();
            cout << endl << "                            Game Over" << endl;
        }
        delete board;
        return 0;
    }
    catch (const exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}