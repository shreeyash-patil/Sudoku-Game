int tileValue(int n, int **board, int row, int col);
int intToChar(int value);
int getSquare(int n, int row, int col);
bool prepare(int n, int **board, int rows[n], int cols[n], int squares[n]);
bool solveRecur(int n, int **board, int rows[n], int cols[n], int squares[n], int row, int col);
bool solve_bitmanipulation(Sudoku *sudoku);
void printsudoku(int n, int **board);