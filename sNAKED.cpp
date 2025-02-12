#include <iostream>
#include <conio.h>
#include <stdexcept>
#include <windows.h>
#include <ctime>
#include <limits>

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
                if (difficultyLevel == 1)
                {
                    snakeSpeed = 100; // Easy: 100 milliseconds
                }
                else if (difficultyLevel == 2)
                {
                    snakeSpeed = 75; // Normal: 75 milliseconds
                }
                else if (difficultyLevel == 3)
                {
                    snakeSpeed = 25; // Hard: 50 milliseconds
                }
                else if (difficultyLevel == 4)
                {
                    snakeSpeed = 0; // Legendary: 25 milliseconds
                }
                else
                {
                    throw runtime_error("Error: Invalid difficulty level.");
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
        setColor(7); // White color for the current score
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

    void drawFrame()
    {
        setColor(7); // White color for the frame
        for (int x = 0; x < consoleWidth; x++)
        {
            setCursorPosition(x, 0);
            cout << "#";
            setCursorPosition(x, consoleHeight - 1);
            cout << "#";
        }
        for (int y = 0; y < consoleHeight; y++)
        {
            setCursorPosition(0, y);
            cout << "#";
            setCursorPosition(consoleWidth - 1, y);
            cout << "#";
        }
    }

    void draw()
    {
        static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        static COORD bufferSize = { (SHORT)consoleWidth, (SHORT)consoleHeight };
        static CHAR_INFO *buffer = new CHAR_INFO[consoleWidth * consoleHeight];
        static SMALL_RECT writeRegion = { 0, 0, (SHORT)(consoleWidth - 1), (SHORT)(consoleHeight - 1) };

        // Clear buffer
        for (int i = 0; i < consoleWidth * consoleHeight; i++)
        {
            buffer[i].Char.AsciiChar = ' ';
            buffer[i].Attributes = 0;
        }

        // Draw frame
        for (int x = 0; x < consoleWidth; x++)
        {
            buffer[x].Char.AsciiChar = '#';
            buffer[x].Attributes = 7;
            buffer[x + (consoleHeight - 1) * consoleWidth].Char.AsciiChar = '#';
            buffer[x + (consoleHeight - 1) * consoleWidth].Attributes = 7;
        }
        for (int y = 0; y < consoleHeight; y++)
        {
            buffer[y * consoleWidth].Char.AsciiChar = '#';
            buffer[y * consoleWidth].Attributes = 7;
            buffer[(consoleWidth - 1) + y * consoleWidth].Char.AsciiChar = '#';
            buffer[(consoleWidth - 1) + y * consoleWidth].Attributes = 7;
        }

        // Draw snake
        buffer[snake->body[0].yCoord * consoleWidth + snake->body[0].xCoord].Char.AsciiChar = snakeHeadChar;
        buffer[snake->body[0].yCoord * consoleWidth + snake->body[0].xCoord].Attributes = snakeHeadColor;
        for (int i = 1; i < snake->getLength(); i++)
        {
            buffer[snake->body[i].yCoord * consoleWidth + snake->body[i].xCoord].Char.AsciiChar = snakeBodyChar;
            buffer[snake->body[i].yCoord * consoleWidth + snake->body[i].xCoord].Attributes = snakeBodyColor;
        }

        // Draw food
        buffer[food.yCoord * consoleWidth + food.xCoord].Char.AsciiChar = foodChar;
        buffer[food.yCoord * consoleWidth + food.xCoord].Attributes = foodColor;

        // Write buffer to console
        WriteConsoleOutput(hConsole, buffer, bufferSize, { 0, 0 }, &writeRegion);

        displayCurrentScore();
    }

    bool update()
    {
        bool isAlive = snake->move(food);
        if (isAlive == false || snake->body[0].xCoord >= consoleWidth - 1 || snake->body[0].yCoord >= consoleHeight - 1 || snake->body[0].xCoord < 0 || snake->body[0].yCoord < 0)
        {
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

    void displayGameOver()
    {
        // Open a new terminal
        system("start cmd /c \"title Game Over & color 07 & mode con: cols=80 lines=25\"");

        // Calculate the position to center the game over message
        int x = consoleWidth / 2 - 4;
        int y = consoleHeight / 2;

        // Display the game over message in the middle of the screen
        setCursorPosition(x, y);
        setColor(7); // White color for the game over message
        cout << "Game Over" << endl;

        // Display the current score below the game over message
        setCursorPosition(x - 4, y + 1);
        cout << "Current Score: " << score << endl;
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
        int choice;
        while (true)
        {
            displayMenu();
            cin >> choice;
            if (choice == 1 || choice == 2)
            {
                break;
            }
            else
            {
                cout << "Invalid choice. Please enter 1 for Quick Match or 2 for Custom." << endl;
            }
        }

        char snakeHeadChar = 'O';
        char snakeBodyChar = '*';
        char foodChar = '$';
        int snakeHeadColor = 2; // Green
        int snakeBodyColor = 2; // Green
        int foodColor = 4; // Red

        if (choice == 2)
        {
            displayCustomizationMenu(snakeHeadChar, snakeBodyChar, foodChar, snakeHeadColor, snakeBodyColor, foodColor);
        }

        bool isQuickMatch = (choice == 1);
        Board *board = new Board(isQuickMatch, snakeHeadChar, snakeBodyChar, foodChar, snakeHeadColor, snakeBodyColor, foodColor);

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
            board->displayGameOver();
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