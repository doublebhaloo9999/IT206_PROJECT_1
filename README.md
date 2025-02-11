# Snake Game

This repository contains the implementation of a Snake game in C++. The game allows the user to play a classic Snake game with options for quick match and custom settings, including customization of the snake's head and body characters and colors, as well as the food character and color.

## Key Components

### Global Constants and Variables
- `MAX_LENGTH`: Maximum length of the snake.
- `DIR_UP`, `DIR_DOWN`, `DIR_LEFT`, `DIR_RIGHT`: Constants representing the directions the snake can move.
- `consoleWidth`, `consoleHeight`: Variables to store the dimensions of the console window.

### Functions
- `initScreen()`: Initializes the console screen dimensions.
- `displayMenu()`: Displays the main menu with options for Quick Match and Custom.
- `displayCustomizationMenu()`: Prompts the user to enter custom characters and colors for the snake's head, body, and food.

### Structs
- `Point`: Represents a point on the console screen with `xCoord` and `yCoord`.

### Classes

#### Snake
- **Attributes**:
  - `length`: Length of the snake.
  - `direction`: Current direction of the snake.
  - `body[MAX_LENGTH]`: Array of `Point` representing the snake's body.
- **Methods**:
  - `Snake(int x, int y)`: Constructor to initialize the snake's starting position.
  - `getLength()`: Returns the length of the snake.
  - `changeDirection(char newDirection)`: Changes the direction of the snake.
  - `move(Point food)`: Moves the snake and checks for collisions and food consumption.

#### Board
- **Attributes**:
  - `snake`: Pointer to a `Snake` object.
  - `snakeHeadChar`, `snakeBodyChar`, `foodChar`: Characters representing the snake's head, body, and food.
  - `snakeHeadColor`, `snakeBodyColor`, `foodColor`: Colors for the snake's head, body, and food.
  - `food`: `Point` representing the food's position.
  - `score`: Current score of the game.
  - `snakeSpeed`: Speed of the snake based on difficulty level.
  - `difficultyLevel`: Difficulty level of the game.
  - `isGameWon`: Boolean indicating if the game is won.
- **Methods**:
  - `Board(bool isQuickMatch, char headChar, char bodyChar, char foodChar, int headCol, int bodyCol, int foodCol)`: Constructor to initialize the board with custom settings.
  - `getSnakeSpeed()`: Returns the speed of the snake.
  - `~Board()`: Destructor to clean up resources.
  - `getScore()`: Returns the current score.
  - `spawnFood()`: Spawns food at a random position on the board.
  - `displayCurrentScore()`: Displays the current score on the console.
  - `setCursorPosition(int x, int y)`: Sets the cursor position on the console.
  - `draw()`: Draws the snake and food on the console.
  - `update()`: Updates the game state, moves the snake, and checks for collisions and food consumption.
  - `getInput()`: Gets user input for changing the snake's direction.
  - `getIsGameWon()`: Returns whether the game is won.

### Main Function
- Initializes the game, displays the menu, and handles user input for game settings.
- Creates a `Board` object with the chosen settings.
- Runs the game loop, updating the game state, drawing the board, and handling user input.
- Displays the game result (win or lose) and cleans up resources.

## Usage
1. Compile and run the program.
2. Choose between Quick Match or Custom settings.
3. If Custom is chosen, enter the desired characters and colors for the snake's head, body, and food.
4. Play the game using the `W`, `A`, `S`, `D` keys to control the snake.
5. The game ends when the snake collides with itself or the wall, or when the score reaches 50 (game won).
