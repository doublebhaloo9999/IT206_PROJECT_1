# IT206_PROJECT1
# IT206_PROJECT1

## Ksnake.cpp

### Overview
`Ksnake.cpp` is a C++ implementation of the classic Snake game. The game is played in the console and features different difficulty levels, a scoring system, and a game-over condition when the snake collides with itself or the borders.

### Features
- **Snake Movement**: The snake can move in four directions (up, down, left, right) using the keyboard.
- **Food Consumption**: The snake grows in length when it eats food, and the score increases.
- **Difficulty Levels**: The game offers four difficulty levels: Easy, Normal, Hard, and Legendary.
- **Game Over Conditions**: The game ends if the snake collides with itself or the borders.
- **Winning Condition**: The game is won if the player reaches a score of 50.

### Code Structure
- **Global Variables**:
  - `consoleWidth` and `consoleHeight`: Dimensions of the console window.
  - Direction constants: `DIR_UP`, `DIR_DOWN`, `DIR_LEFT`, `DIR_RIGHT`.

- **Functions**:
  - `initScreen()`: Initializes the console screen dimensions.
  - `setCursorPosition(int x, int y)`: Sets the cursor position in the console.
  - `setColor(int color)`: Sets the text color in the console.

- **Classes**:
  - `Point`: Represents a point in the console with `xCoord` and `yCoord`.
  - `Snake`: Represents the snake with a body of points, length, and direction. Handles movement and collision detection.
  - `Board`: Manages the game board, including the snake, food, score, and game logic.

### How to Play
1. **Compile the Code**: Use a C++ compiler to compile `Ksnake.cpp`.
   ```sh
   g++ Ksnake.cpp -o Ksnake.exe