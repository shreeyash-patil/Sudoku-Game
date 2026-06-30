#define N 9
#define EMPTY 0

typedef struct Sudoku{
    int size;
    int **box;
}Sudoku;


void init_sudoku(Sudoku *sudoku, int size);
void init_all_sudokus(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, int size);
void free_sudoku(Sudoku *sudoku);
void copy_from_matrix(Sudoku *sudoku, int values[9][9]);
void free_all_sudokus(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku);
int valid_in_grid(Sudoku *sudoku, int row, int col, int val);
int valid_in_row(Sudoku *sudoku, int row, int val);
int valid_in_col(Sudoku *sudoku, int col, int val);
int is_safe(Sudoku *sudoku, int row, int col, int num);
int solve_sudoku(Sudoku *sudoku);
void print_sudoku(Sudoku *sudoku, Sudoku *skeleton);
void copy_sudoku(Sudoku *source, Sudoku *dest);
void copy_base_sudoku(Sudoku *sudoku, Sudoku *ip_sudoku, Sudoku *skeleton);
void generate_skeleton_sudoku(Sudoku *sudoku, Sudoku *skeleton);
int count_row_elements(Sudoku *sudoku, int row);
int count_col_elements(Sudoku *sudoku, int col);
int count_grid_elements(Sudoku *sudoku, int row, int col);
void copy_from_sudoku(Sudoku *sudoku);
void check_sudoku(Sudoku *sudoku, Sudoku *solved_sudoku);
bool generate_full_sudoku(Sudoku *sudoku);
bool find_empty_cell(Sudoku *sudoku, int *row, int *col);
void remove_cells(Sudoku *sudoku, int cells_to_remove);
void shuffle(int arr[], int n);
void generate_sudoku(Sudoku *sudoku, int difficulty);
void reset_sudoku(Sudoku *sudoku, Sudoku *skeleton_sudoku);