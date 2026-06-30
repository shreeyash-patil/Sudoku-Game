# C CLI Sudoku Game

A fully featured, interactive Command Line Interface (CLI) Sudoku game written entirely in C. 

This project allows players to generate Sudoku puzzles of varying sizes (4x4, 9x9, 16x16) and difficulties, play them in the terminal, and save their progress to continue later. It also features an advanced autosolver powered by bit manipulation.

## 🌟 Features

* **Multiple Grid Sizes:** Play standard 9x9, beginner-friendly 4x4, or massive 16x16 grids.
* **Variable Difficulty:** Choose between Easy, Medium, and Hard puzzle generation.
* **Save & Load System:** Save your game progress at any time. Games are securely stored in an automatically generated `saves/` directory.
* **Undo/Redo System:** Made a mistake? Seamlessly step backward or forward through your move history using a custom Stack implementation.
* **Hint System & Error Checking:** Get a hint if you are stuck, or check your current board against the solution to highlight incorrect entries.
* **Blazing Fast Autosolver:** Watch the computer solve any puzzle instantly using either standard backtracking or a highly optimized bit manipulation algorithm (includes execution time comparisons).

## 📂 Project Structure

Ensure all of the following files are in the same directory before compiling:

* `menudriven.c` - The main game loop and terminal interface.
* `logic.c` - Core game mechanics, Sudoku generation, solving algorithms, and file handling.
* `*.h` (Header Files) - Contains definitions, structures, and function declarations.

## ⚙️ How to Compile

To compile this project, you will need a C compiler installed on your system (such as **GCC**).

Open your terminal or command prompt, navigate to the folder containing your source files, and run the following command to link and compile all `.c` files together into a single executable:

```bash
gcc menudriven.c logic.c -o sudoku_game
```
## ⚙️ How to Run

**On Windows**
```bash
sudoku_game.exe 
```
**On Mac/Linux**
```bash
./sudoku_game
```
