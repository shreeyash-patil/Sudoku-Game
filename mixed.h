#define N 9
#define EMPTY 0

void take_input(stack *u, stack *r, Sudoku *sudoku, Sudoku *skeleton, int row, int col, int num);
void undo(stack *u, stack *r, Sudoku *sudoku);
void redo(stack *u, stack *r, Sudoku *sudoku);
void hint(Sudoku *take_ip_sudoku, Sudoku *solved_sudoku, Sudoku *skeleton_sudoku, stack *u, stack *r);
void start_game(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, int size, int level, stack *u, stack *r);
void end_game(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, stack *u, stack *r);