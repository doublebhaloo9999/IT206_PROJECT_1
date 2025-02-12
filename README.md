# Snake Game

## Description
This is a terminal-based Snake game implemented in C++ using basic programming constructs. The game features both a Quick Match and a Custom mode, where players can customize the snake's appearance and difficulty level. The game dynamically updates the screen using Windows console functions.

## Features
- **Single-player mode**: Control a snake to eat food and grow in length.
- **Customizable gameplay**: Players can modify the snake's appearance, food appearance, and difficulty level.
- **Dynamic difficulty levels**: Choose from Easy, Normal, Hard, and Legendary.
- **Game-winning condition**: Reach a score of 50 to win the game.
- **Game-over handling**: The game ends if the snake collides with itself or the wall.
- **Quick Match mode**: Instantly start a game with default settings.
- **Custom mode**: Personalize your snake and game settings.

## Controls
- `W` or `Arrow Up`: Move Up
- `A` or `Arrow Left`: Move Left
- `S` or `Arrow Down`: Move Down
- `D` or `Arrow Right`: Move Right

## Installation
### Requirements
- Windows OS (due to the use of Windows-specific console functions)
- C++ Compiler (such as MinGW or MSVC)

### Steps to Compile and Run
1. Clone the repository or download the source code.
2. Open a terminal or command prompt in the project directory.
3. Compile the program using a C++ compiler, for example:
   ```sh
   g++ -o snake_game snake_game.cpp -std=c++11
   ```
4. Run the executable:
   ```sh
   ./snake_game
   ```

## Customization
In Custom mode, players can set:
- **Snake Head Character**
- **Snake Body Character**
- **Food Character**
- **Snake Head Color (0-15)**
- **Snake Body Color (0-15)**
- **Food Color (0-15)**

## Known Issues
- The game is currently limited to Windows OS due to its reliance on `windows.h`.
- The console resizing might affect the gameplay.

## Future Improvements
- Implement support for Linux/macOS.
- Add a multiplayer mode.
- Enhance graphics with a better rendering technique.

