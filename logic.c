#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "sudoku.h"
#include "stack.h"
#include "mixed.h"
#include "header.h"
#include "filehandling.h"
#define N 9
#define EMPTY 0

#ifdef _WIN32
#include <direct.h>
// Windows requires the folder path
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
// Mac/Linux require the path and permissions
#define MAKE_DIR(path) mkdir(path, 0777)
#endif

#define DEFAULT_SAVE_PATH "saves/"


void initstack(stack *s){
    s -> top = NULL;
    return;
}

void init_undo_redo(stack *u, stack *r){
    initstack(u);
    initstack(r);
    return;
}

int stack_Empty(stack *s){
    if(s -> top == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void push(stack *s, data prev_step){
    stacknode *nn = (stacknode *)malloc(sizeof(stacknode));
    nn -> d.num = prev_step.num;
    nn -> d.row = prev_step.row;
    nn -> d.col = prev_step.col;
    nn -> next = s -> top;
    s -> top = nn;
    return;
}

data pop(stack *s){
    data x;
    x.num = -1;
    x.row = -1;
    x.col = -1;
    if(stack_Empty(s)){
        return x;
    }
    else{
        stacknode *temp = s -> top;
        x.num = temp -> d.num;
        x.row = temp -> d.row;
        x.col = temp -> d.col;
        s -> top = s -> top -> next;
        free(temp);
        return x;
    }
}

data peek(stack *s){
    data x;
    x.num = -1;
    x.row = -1;
    x.col = -1;
    if(stack_Empty(s)){
        return x;
    }
    else{
        stacknode *temp = s -> top;
        x.num = temp -> d.num;
        x.row = temp -> d.row;
        x.col = temp -> d.col;
        return x;
    }
}

void freestack(stack *s){
    if(stack_Empty(s))
        return;
    while(!stack_Empty(s)){
        stacknode *temp = s -> top;
        s -> top = s -> top -> next;
        free(temp);
    }
    return;
}

void free_undo_redo(stack *u, stack *r){
    freestack(u);
    freestack(r);
    return;
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

void init_sudoku(Sudoku *sudoku, int size){
    sudoku -> size = size;
    sudoku -> box = (int**)malloc(size * sizeof(int*));
    for(int i = 0; i < size; i++){
        sudoku -> box[i] = (int*)malloc(size * sizeof(int));
        for(int j = 0; j < size; j++){
            sudoku -> box[i][j] = 0;
        }
    }
}

void init_all_sudokus(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, int size){
    init_sudoku(sudoku, size);
    init_sudoku(take_ip_sudoku, size);
    init_sudoku(solved_sudoku, size);
    init_sudoku(skeleton_sudoku, size);
    return;
}

void copy_from_matrix(Sudoku *sudoku, int values[9][9]){
    int size = sudoku -> size;
    init_sudoku(sudoku, size);
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            sudoku -> box[row][col] = values[row][col];
        }
    }
    return;
}

void free_sudoku(Sudoku *sudoku){
    for(int i = 0; i < sudoku -> size; i++) {
        free(sudoku -> box[i]);
    }
    free(sudoku ->box);
    sudoku -> size = 0;
    sudoku->box = NULL;
    return;
}

void free_all_sudokus(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku){
    free_sudoku(sudoku);
    free_sudoku(solved_sudoku);
    free_sudoku(skeleton_sudoku);
    free_sudoku(take_ip_sudoku);
    return;
}

int valid_in_grid(Sudoku *sudoku, int row, int col, int val){
    int size = sudoku -> size;
    row /= sqrt(size);
    col /= sqrt(size);
    row *= sqrt(size);
    col *= sqrt(size);
    for(int i = row; i < row + sqrt(size); i++){
        for(int j = col; j < col + sqrt(size); j++){
            if(sudoku -> box[i][j] == val)
                return 0;
        }
    }
    return 1;
}

int valid_in_row(Sudoku *sudoku, int row, int val){
    int size = sudoku -> size;
    for(int col = 0; col < size; col++){
        if(sudoku -> box[row][col] == val)
            return 0;
    }
    return 1;
}

int valid_in_col(Sudoku *sudoku, int col, int val){
    int size = sudoku -> size;
    for(int row = 0; row < size; row++){
        if(sudoku -> box[row][col] == val)
            return 0;
    }
    return 1;
}

int is_safe(Sudoku *sudoku, int row, int col, int num){
    if(valid_in_grid(sudoku, row, col, num) && valid_in_row(sudoku, row, num) && valid_in_col(sudoku, col, num))
        return 1;
    return 0;
}

int is_empty_cell(Sudoku *sudoku, int row, int col) {
    if(sudoku -> box[row][col] == EMPTY)
        return 1;
    return 0;
}

void copy_sudoku(Sudoku *dest, Sudoku *source){
    int size = source -> size;
    init_sudoku(dest, size);
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            dest -> box[row][col] = source  -> box[row][col];
        }
    }
    return;
}

void generate_skeleton_sudoku(Sudoku *sudoku, Sudoku *skeleton){
    int size = sudoku -> size;
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(sudoku -> box[row][col] != 0)
                skeleton -> box[row][col] = 0;      //permanent element
            else    
                skeleton -> box[row][col] = 1;      //editable element
        }
    }
    return;
}

void print_sudoku(Sudoku *sudoku, Sudoku *skeleton){
    int size = sudoku->size;
    int cell_width = (size > 9) ? 3 : 2; 

    printf("    "); 
    for(int col = 0; col < size; col++){
        printf("\033[1;33m%*d\033[0m ", cell_width, col + 1); 
        if((col + 1) % (int)sqrt(size) == 0)
            printf(" ");
    }
    printf("\n");

    for(int row = 0; row < size; row++){
        printf("\033[1;33m%2d\033[0m  ", row + 1); 

        for(int col = 0; col < size; col++){
            if(skeleton->box[row][col] == 0)
                printf("\033[1;31m%*d\033[0m ", cell_width, sudoku->box[row][col]); 
            else if(skeleton->box[row][col] == 1 && sudoku->box[row][col] != 0)
                printf("\033[1;32m%*d\033[0m ", cell_width, sudoku->box[row][col]); 
            else
                printf("\033[1m%*d\033[0m ", cell_width, sudoku->box[row][col]); 

            if((col + 1) % (int)sqrt(size) == 0)
                printf(" ");
        }
        printf("\n");

        if ((row + 1) % (int)sqrt(size) == 0)
            printf("\n");
    }
    printf("\n");
    return;
}


int solve_sudoku(Sudoku *sudoku){
    int size = sudoku -> size;
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(is_empty_cell(sudoku, row, col)){
                for(int num = 1; num <= size; num++){
                    if(is_safe(sudoku, row, col, num)){
                        sudoku -> box[row][col] = num;
                        if(solve_sudoku(sudoku))
                            return 1;
                        sudoku -> box[row][col] = EMPTY;
                    }
                }
                return 0;
            }   
        }
    }
    return 1;
}

void take_input(stack *u, stack *r, Sudoku *sudoku, Sudoku *skeleton, int row, int col, int num){
    int size = sudoku -> size;
    if((row > size || row < 1) || (col > size || col < 1) || (num > size || num < 0) || skeleton -> box[row - 1][col - 1] == 0){
        printf("Invalid inputs!\nRetry.\n\n");
        return;
    }
    data prev_step, next_step;
    prev_step.row = next_step.row = row;
    prev_step.col = next_step.col = col;
    prev_step.num = sudoku -> box[row - 1][col - 1];
    next_step.num = num;
    push(u, prev_step);
    freestack(r);
    sudoku -> box[row - 1][col - 1] = num;
    return;    
}

void undo(stack *u, stack *r, Sudoku *sudoku){
    if(stack_Empty(u)){
        printf("No more steps to undo.\n\n");
        return;
    }
    data prev_step = pop(u);
    data next_step;
    next_step.row = prev_step.row;
    next_step.col = prev_step.col;
    next_step.num = sudoku -> box[prev_step.row - 1][prev_step.col - 1];
    push(r, next_step);
    if(prev_step.num == -1)
        return;
    sudoku -> box[prev_step.row - 1][prev_step.col - 1] = prev_step.num;
    return;
}

void redo(stack *u, stack *r, Sudoku *sudoku){
    if(stack_Empty(r)){
        printf("No more steps to redo.\n\n");
        return;
    }
    data next_step = pop(r);
    data prev_step;
    prev_step.row = next_step.row;
    prev_step.col = next_step.col;
    prev_step.num = sudoku -> box[next_step.row - 1][next_step.col - 1];
    push(u, prev_step);
    if(prev_step.num == -1)
        return;
    sudoku -> box[next_step.row - 1][next_step.col - 1] = next_step.num;
    return;
}

int count_row_elements(Sudoku *sudoku, int row){
    int size = sudoku -> size;
    int count = 0;
    for(int col = 0; col < size; col++){
        if(sudoku -> box[row][col] != 0)
            count++;
    }
    return count;
}

int count_col_elements(Sudoku *sudoku, int col){
    int size = sudoku -> size;
    int count = 0;
    for(int row = 0; row < size; row++){
        if(sudoku -> box[row][col] != 0)
            count++;
    }
    return count;
}

int count_grid_elements(Sudoku *sudoku, int row, int col){
    int size = sudoku -> size;
    int count = 0;
    row /= sqrt(size);
    col /= sqrt(size);
    row *= sqrt(size);
    col *= sqrt(size);
    for(int i = row; i < row + sqrt(size); i++){
        for(int j = col; j < col + sqrt(size); j++){
            if(sudoku -> box[i][j] != 0)
                count++;
        }
    }
    return count;
}

void hint(Sudoku *take_ip_sudoku, Sudoku *solved_sudoku, Sudoku *skeleton_sudoku, stack *u, stack *r){
    int size = take_ip_sudoku -> size;
    int hint_col, hint_row, row_count, col_count, grid_count, temp_sum, min_sum;
    hint_row = hint_col = -1;
    temp_sum = min_sum = col_count = row_count = grid_count = 999;
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(take_ip_sudoku -> box[row][col] == 0){
                row_count = count_row_elements(take_ip_sudoku, row);
                col_count = count_col_elements(take_ip_sudoku, col);
                grid_count = count_grid_elements(take_ip_sudoku, row, col);
                temp_sum = row_count + col_count + grid_count;
                if(temp_sum < min_sum){
                    min_sum = temp_sum;
                    hint_row = row + 1;
                    hint_col = col + 1;
                }
            }
        }
    }
    if(hint_row == -1 && hint_col == -1){
        printf("No more hints available!!\n\n");
        return;
    }
    int num = solved_sudoku -> box[hint_row - 1][hint_col - 1];
    take_input(u, r, take_ip_sudoku, skeleton_sudoku, hint_row, hint_col, num);
    printf("Element in row %d and column %d is inserted.\n\n", hint_row, hint_col);
    return;
}

void shuffle(int arr[], int n){
    for(int i = 0; i < n; i++){
        int rand_index = rand() % n;
        int temp = arr[i];
        arr[i] = arr[rand_index];
        arr[rand_index] = temp;
    }
    return;
}

bool find_empty_cell(Sudoku *sudoku, int *row, int *col){
    int size = sudoku->size;
    for(int r = 0; r < size; r++){
        for(int c = 0; c < size; c++){
            if(sudoku->box[r][c] == EMPTY){
                *row = r;
                *col = c;
                return true;
            }
        }
    }
    return false;
}

bool generate_full_sudoku(Sudoku *sudoku){
    int size = sudoku->size;
    int nums[size];
    for(int i = 0; i < size; i++){
        nums[i] = i + 1;
    }

    int block_size = (int)sqrt(size);
    for(int i = 0; i < size; i += block_size) {
        shuffle(nums, size);
        for(int row = 0; row < block_size; row++) {
            for (int col = 0; col < block_size; col++){
                sudoku->box[i + row][i + col] = nums[row * block_size + col];
            }
        }
    }
    return solve_sudoku(sudoku);
}

void remove_cells(Sudoku *sudoku, int cells_to_remove){
    int size = sudoku->size;
    int total_cells = size * size;
    int positions[total_cells][2];

    int index = 0;
    for(int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            positions[index][0] = row;
            positions[index][1] = col;
            index++;
        }
    }

    for(int i = 0; i < total_cells; i++){
        int rand_index = rand() % total_cells;
        int temp_row = positions[i][0];
        int temp_col = positions[i][1];
        positions[i][0] = positions[rand_index][0];
        positions[i][1] = positions[rand_index][1];
        positions[rand_index][0] = temp_row;
        positions[rand_index][1] = temp_col;
    }

    int count = 0;
    for(int i = 0; i < total_cells && count < cells_to_remove; i++){
        int row = positions[i][0];
        int col = positions[i][1];
        if(sudoku->box[row][col] != 0){
            sudoku->box[row][col] = 0;
            count++;
        }
    }
    return;
}

void generate_sudoku(Sudoku *sudoku, int difficulty){
    int size = sudoku->size;
    int cells_to_remove = 0;
    switch(difficulty){
        case 1:
            if(size == 4) 
                cells_to_remove = 4;
            else if(size == 9) 
                cells_to_remove = 25;
            else if(size == 16) 
                cells_to_remove = 70;
            break;
        case 2:
            if(size == 4) 
                cells_to_remove = 6;
            else if(size == 9) 
                cells_to_remove = 32;
            else if(size == 16) 
                cells_to_remove = 90;
            break;
        case 3:
            if(size == 4) 
                cells_to_remove = 8;
            else if(size == 9) 
                cells_to_remove = 40;
            else if(size == 16) 
                cells_to_remove = 110;
            break;
        default:
            cells_to_remove = 0;  
    }

    if(!generate_full_sudoku(sudoku)){
        printf("Error: Could not generate a full Sudoku grid.\n");
        return;
    }

    remove_cells(sudoku, cells_to_remove);
    return;
}

void start_game(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, int size, int level, stack *u, stack *r){
    init_undo_redo(u, r);
    init_all_sudokus(sudoku, skeleton_sudoku, take_ip_sudoku, solved_sudoku, size);
    generate_sudoku(sudoku, level);
    generate_skeleton_sudoku(sudoku, skeleton_sudoku);
    copy_sudoku(solved_sudoku, sudoku);
    copy_sudoku(take_ip_sudoku, sudoku);
    solve_sudoku(solved_sudoku);
    return;
}

void end_game(Sudoku *sudoku, Sudoku *take_ip_sudoku, Sudoku *skeleton_sudoku, Sudoku *solved_sudoku, stack *u, stack *r){
    free_all_sudokus(sudoku, skeleton_sudoku, take_ip_sudoku, solved_sudoku);
    free_undo_redo(u, r);
    return;
}

void check_sudoku(Sudoku *sudoku, Sudoku *solved_sudoku){
    int size = sudoku -> size;
    for (int row = 0; row < size; row++){
        for (int col = 0; col < size; col++){
            if(sudoku -> box[row][col] != 0 && sudoku -> box[row][col] != solved_sudoku -> box[row][col]){
                printf("Wrong input at row %d and column %d\n", row + 1, col + 1);
                return;
            }
        }
    }
    printf("All inputs are Correct\n");
    return;
}


void reset_sudoku(Sudoku *sudoku, Sudoku *skeleton_sudoku){
    int size = sudoku -> size;
    for(int row = 0; row < size; row++){
        for(int col = 0; col < size; col++){
            if(skeleton_sudoku -> box[row][col] == 1){
                sudoku -> box[row][col] = 0;
            }
        }    
    }
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//basically this function returns how many times we have to leftshift 1 so that it will be in its correct position
int tileValue(int n, int **board, int row, int col) {
    /* get the integer value of a tile
    return -1 if the tile is empty */
    if (board[row][col] == 0) {
        return -1;
    } else {
        return board[row][col] - 1;
    }
}

int intToChar(int value) {
    /* convert an integer into a sudoku number (0 -> '1', 1 -> '2', etc.)*/
    //basically index 0 in rows[row] tells us if '1' is present in that row
    return (1 + value);
}

int getSquare(int n, int row, int col) {
    int k = (int)sqrt(n);
    /* calculate a tile's sqaure based on its row and column */
    return ((row / k) * k) + (col / k);
}

bool prepare(int n, int **board, int rows[n], int cols[n], int squares[n]) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            int square = getSquare(n, row, col);
            int intValue = tileValue(n, board, row, col);
            
            if (intValue != -1) {
                int candidateBits = rows[row] | cols[col] | squares[square];
                if ((candidateBits >> intValue) & 1) {
                    return false; // error
                }               
                // Set the bit for this value
                rows[row] |= (1 << intValue);
                cols[col] |= (1 << intValue);
                squares[square] |= (1 << intValue);
            }
        }
    }
    return true;
}

bool solveRecur(int n, int **board, int rows[n], int cols[n], int squares[n], int row, int col) {
    /* recursively solve the entire board */
    if (row == n) {
        return true;
    }
    int nextrow = (col == (n-1) ? row + 1 : row);
    int nextcol = (col == (n-1) ? 0 : col + 1);

    int square = getSquare(n, row, col);

    int intValue = tileValue(n, board, row, col);

    if (intValue != -1) { // skip tiles that already have numbers
        return solveRecur(n, board, rows, cols, squares, nextrow, nextcol);
    }
    // get the possible values for the tile's row, column and square
    int rowBits = rows[row];
    int colBits = cols[col];

    int squareBits = squares[square];

    // OR them all together to get the final candidates
    int candidateBits = rowBits | colBits | squareBits;

    for (int i = 0; i < n; i++) {
        // iterate through each bit and only attempt the 0 bits 
        if ((candidateBits >> i & 1) == 0) { 
            // set the bit to 1
            rows[row] |= (1 << i);//right shifting 1, i times and doing OR with rows[row]
            cols[col] |= (1 << i);
            squares[square] |= (1 << i);
            board[row][col] = intToChar(i);
            if (solveRecur(n, board, rows, cols, squares, nextrow, nextcol)) {
                return true;
            }
            // if the bit didn't work out, reset it to 0
            rows[row] &= ~(1 << i);//resetting that specific bit since it didnt work
            cols[col] &= ~(1 << i);
            squares[square] &= ~(1 << i);
        }
    }

    board[row][col] = 0;
    return false;
}

bool solve_bitmanipulation(Sudoku *sudoku){
    int n = sudoku -> size; 
    int **board = sudoku -> box;
    //sets the 3 bitmasking arrays
    int rows[n];
    int cols[n];
    int squares[n];
    for (int i = 0; i < n; i++) {
        rows[i] = 0;
        cols[i] = 0;
        squares[i] = 0;
    }
    if (!prepare(n, board, rows, cols, squares)) {
        return false;
    }
    return solveRecur(n, board, rows, cols, squares, 0, 0);
}


void printsudoku(int n, int **board) {
    int k = (int)sqrt(n);
    for (int row = 0; row < n; row++) {
        if (row % k == 0 && row != 0) {
            for (int i = 0; i < n + k - 1; i++) {
                printf("-");
            }
            printf("\n");
        }
        for (int col = 0; col < n; col++) {
            if (col % k == 0 && col != 0) {
                printf(" |");
            }
            printf(" %d", board[row][col]);
        }
        printf("\n");
    }
}

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//Helper function to create directory
void ensure_save_folder_exists() {
    // If it already exists, this safely fails in the background and code continues
    MAKE_DIR("saves");
}

// Function to save Sudoku to a file
bool save_sudoku_to_file(Sudoku *sudoku, const char *filename) {
    if (strcmp(filename, "exit") == 0) {
        return false;
    }
    
    ensure_save_folder_exists();
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", DEFAULT_SAVE_PATH, filename);

    //Check if filename already exits
    FILE *check_file = fopen(filepath, "r");
    if (check_file) {
        fclose(check_file);
        printf("Error: A save file with the name '%s' already exists.\n", filename);
        return false;
    }

    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        return false;
    }

    fprintf(file, "%d\n", sudoku->size);

    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fprintf(file, "%d ", sudoku->box[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    // ONLY add to the master file if it is NOT a skeleton file
    if (strstr(filename, "_skeleton") == NULL) {
        char master_filepath[256];
        snprintf(master_filepath, sizeof(master_filepath), "%sall_sudoku_files.txt", DEFAULT_SAVE_PATH);
        add_filename_to_master(filename, master_filepath);
    }

    printf("Game successfully saved as '%s'.\n", filename);
    return true;
}

// Function to read Sudoku from a file
bool read_sudoku_from_file(Sudoku *sudoku, const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", DEFAULT_SAVE_PATH, filename);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Error: Game '%s' not found.\n", filename);
        return false; // Tells main.c the load failed
    }

    fscanf(file, "%d", &sudoku->size);

    init_sudoku(sudoku, sudoku->size);

    for (int i = 0; i < sudoku->size; i++) {
        for (int j = 0; j < sudoku->size; j++) {
            fscanf(file, "%d", &sudoku->box[i][j]);
        }
    }

    fclose(file);
    return true; // Tells main.c the load was successful
}

// Function to delete a file
void delete_file(const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", DEFAULT_SAVE_PATH, filename);
    // Attempt to delete the file
    if (remove(filepath) == 0) {
        printf("File '%s' deleted successfully.\n", filename);

        // Set up paths for the master file and a temporary file
        char master_file[256];
        snprintf(master_file, sizeof(master_file), "%sall_sudoku_files.txt", DEFAULT_SAVE_PATH);
                
        char temp_file[256];
        snprintf(temp_file, sizeof(temp_file), "%stemp_master_file.txt", DEFAULT_SAVE_PATH);

        FILE *master = fopen(master_file, "r");
        if (!master) {
            printf("Error: Unable to open master file %s.\n", master_file);
            return;
        }

        FILE *temp = fopen(temp_file, "w");
        if (!temp) {
            printf("Error: Unable to create temporary file.\n");
            fclose(master);
            return;
        }

        char line[256];
        while (fgets(line, sizeof(line), master)) {
            // Remove trailing newline from the line
            line[strcspn(line, "\n")] = '\0';

            // Write to the temp file only if the line does not match the filename
            if (strcmp(line, filename) != 0) {
                fprintf(temp, "%s\n", line);
            }
        }

        fclose(master);
        fclose(temp);

        // Replace the master file with the updated temp file
        if (remove(master_file) != 0) {
            printf("Error: Unable to remove old master file.\n");
            return;
        }
        if (rename(temp_file, master_file) != 0) {
            printf("Error: Unable to rename temporary file to master file.\n");
        }
    } 
    else {
        printf("Error: Unable to delete file '%s'.\n", filename);
    }
}

// Function to add a filename to the master file
void add_filename_to_master(const char *filename, const char *master_file) {
    FILE *file = fopen(master_file, "a");
    if (!file) {
        printf("Error: Unable to open master file %s.\n", master_file);
        return;
    }

    fprintf(file, "%s\n", filename);
    fclose(file);
}

// Function to print all Sudoku filenames from the master file
void print_all_sudoku_filenames() {
    char master_file[256];
    snprintf(master_file, sizeof(master_file), "%sall_sudoku_files.txt", DEFAULT_SAVE_PATH);

    FILE *file = fopen(master_file, "r");
    if (!file) {
        printf("No saved games found.\n");
        return;
    }

    char filename[256];
    printf("\n--- Saved Games ---\n");
        
    bool has_games = false;
    while (fgets(filename, sizeof(filename), file)) {
        // Remove trailing newline character
        filename[strcspn(filename, "\n")] = '\0';
        printf("- %s\n", filename);
        has_games = true;
    }

    if (!has_games) {
        printf("(Empty)\n");
    }
    printf("-------------------\n\n");

    fclose(file);
}


