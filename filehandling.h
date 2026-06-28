void save_sudoku_to_file(Sudoku *sudoku, const char *filename);
void read_sudoku_from_file(Sudoku *sudoku, const char *filename);
void delete_file(const char *filename);
void add_filename_to_master(const char *filename, const char *master_file);
void print_all_sudoku_filenames(const char *master_file);