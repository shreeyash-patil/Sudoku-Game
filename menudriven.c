#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "sudoku.h"
#include "stack.h"
#include "mixed.h"
#include "header.h"
#include "filehandling.h"

#define N 9
#define EMPTY 0

int main() {
    int choice_1, choice_2, choice_3, choice_4;
    choice_1 = choice_2 = choice_3 = choice_4 = 1;
    int row, col, num;
    row = col = num = -1;
    int size = 0;
    int difficulty = 0;
    char gamename[100], gamename_copy[100];
    stack U, R;
    Sudoku sudoku, skeleton_sudoku, take_ip_sudoku, solved_sudoku, extra_sudoku;
    clock_t start, end;
    double time;
    bool is_saved = false;

    while(choice_1) {
        printf("\nMain Menu:\n");
        printf("1 - New Game..\n");
        printf("2 - Load Game..\n");
        printf("3 - Delete Game..\n");
        printf("0 - Quit Game..\n");
        printf("Enter your choice: ");
        scanf("%d", &choice_1);
        printf("\n");

        switch(choice_1) {
            case 1:
                choice_2 = 1;
                while(choice_2) {
                    printf("Secondary Menu..\n");
                    printf("1 - Select sudoku size...\n");
                    printf("0 - Return to main menu...\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice_2);
                    printf("\n");

                    switch(choice_2) {
                        case 1:
                            printf("Enter Size of sudoku 4, 9, 16.\nSize: ");
                            scanf("%d", &size);
                            printf("\n");
                            printf("Enter Difficulty :\n1 - Easy.\n2 - Medium.\n3 - Hard.\nDifficulty: ");
                            scanf("%d", &difficulty);
                            printf("\n");

                            if((size == 4 || size == 9 || size == 16) && (difficulty == 1 || difficulty == 2 || difficulty == 3)) {
                                start_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, size, difficulty, &U, &R);
                                printf("\n\t\tGame Begins!!\n\n");
                                print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                choice_3 = 1;

                                while(choice_3) {
                                    printf("Tertiary Menu..\n");
                                    printf("1 - Enter a number in sudoku..\n");
                                    printf("2 - Get a hint..\n");
                                    printf("3 - Undo.\n");
                                    printf("4 - Redo.\n");
                                    printf("5 - Show Answer.\n");
                                    printf("6 - Save sudoku and return..\n");
                                    printf("7 - Check wrong Inputs..\n");
                                    printf("8 - Reset Sudoku\n");
                                    printf("0 - Return without saving\n");
                                    printf("Enter your choice: ");
                                    scanf("%d", &choice_3);
                                    printf("\n");

                                    switch(choice_3) {
                                        case 1:
                                            printf("Enter row - ");
                                            scanf("%d", &row);
                                            printf("Enter column - ");
                                            scanf("%d", &col);
                                            printf("Enter number - ");
                                            scanf("%d", &num);
                                            take_input(&U, &R, &take_ip_sudoku, &skeleton_sudoku, row, col, num);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 2:
                                            hint(&take_ip_sudoku, &solved_sudoku, &skeleton_sudoku, &U, &R);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 3:
                                            undo(&U, &R, &take_ip_sudoku);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 4:
                                            redo(&U, &R, &take_ip_sudoku);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 5:
                                            init_sudoku(&extra_sudoku, sudoku.size);
                                            copy_sudoku(&extra_sudoku, &take_ip_sudoku);
                                            start = clock();
                                            solve_sudoku(&sudoku);
                                            end = clock();
                                            time = (double)(end - start)/CLOCKS_PER_SEC;
                                            print_sudoku(&solved_sudoku, &skeleton_sudoku);
                                            printf("Execution time = %f secs\n\n", time);
                                            copy_sudoku(&extra_sudoku, &take_ip_sudoku);
                                            start = clock();
                                            solve_bitmanipulation(&sudoku);
                                            end = clock();
                                            time = (double)(end - start)/CLOCKS_PER_SEC;
                                            print_sudoku(&solved_sudoku, &skeleton_sudoku);
                                            printf("Execution time = %f secs\n\n", time);
                                            free_sudoku(&extra_sudoku);
                                            break;
                                        case 6:
                                            is_saved = false;
                                            do {
                                                printf("Enter game name:   (exit)\n");
                                                scanf("%s", gamename); 
                                                if (strcmp(gamename, "exit") == 0) {
                                                    break;
                                                }
                                                is_saved = save_sudoku_to_file(&take_ip_sudoku, gamename);
                                            } while(!is_saved);

                                            if (strcmp(gamename, "exit") != 0) {
                                                printf("File saved as %s.\n", gamename);
                                                strcat(gamename, "_skeleton");
                                                save_sudoku_to_file(&skeleton_sudoku, gamename);
                                                printf("Returning to Secondary Menu..\n\n");
                                                row = col = num = -1;
                                                size = difficulty = 0;
                                                gamename[0] = '\0';
                                                end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                                                choice_3 = 0; 
                                            }
                                            break;
                                        case 7:
                                            check_sudoku(&take_ip_sudoku, &solved_sudoku);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 8:
                                            reset_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            printf("Sudoku has been reset.\n\n");
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 0:
                                            end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                                            row = col = num = -1;
                                            size = difficulty = 0;
                                            printf("Returning to secondary menu...\n\n");
                                            choice_3 = 0;
                                            break;
                                        default:
                                            printf("Invalid choice!! Try Again..\n\n");
                                    }                                    
                                }
                            } else {
                                printf("Invalid Size or Difficulty!! Select Again..\n");
                            }
                            break;

                        case 0:
                            printf("Returning to Main Menu..\n");
                            choice_2 = 0;
                            break;

                        default:
                            printf("Invalid choice!!  Please try again.\n");
                    }
                }
                break;

            case 2:
                print_all_sudoku_filenames();
                printf("Enter game to be loaded (or 'exit' to cancel): \n");
                scanf("%s", gamename);
                
                if (strcmp(gamename, "exit") == 0) {
                    break;
                }
                
                strcpy(gamename_copy, gamename);
                
                if (!read_sudoku_from_file(&take_ip_sudoku, gamename)) {
                    printf("Returning to Main Menu...\n\n");
                    free_sudoku(&take_ip_sudoku);
                    break; 
                }

                strcat(gamename, "_skeleton");
                read_sudoku_from_file(&skeleton_sudoku, gamename);
                init_sudoku(&sudoku, take_ip_sudoku.size);
                init_sudoku(&solved_sudoku, take_ip_sudoku.size);
                
                copy_base_sudoku(&sudoku, &take_ip_sudoku, &skeleton_sudoku);
                copy_sudoku(&solved_sudoku, &sudoku);
                solve_bitmanipulation(&solved_sudoku);
                init_undo_redo(&U, &R);
                printf("\n\tGame Loaded\n\n");
                print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                choice_4 = 1;

                while(choice_4) {
                    printf("Tertiary Menu..\n");
                    printf("1 - Enter a number in sudoku..\n");
                    printf("2 - Get a hint..\n");
                    printf("3 - Undo.\n");
                    printf("4 - Redo.\n");
                    printf("5 - Show Answer.\n");
                    printf("6 - Save sudoku and return..\n");
                    printf("7 - Check wrong Inputs..\n");
                    printf("8 - Reset Sudoku\n");
                    printf("0 - Return without saving\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice_4);
                    printf("\n");

                    switch(choice_4) {
                        case 1:
                            printf("Enter row - ");
                            scanf("%d", &row);
                            printf("Enter column - ");
                            scanf("%d", &col);
                            printf("Enter number - ");
                            scanf("%d", &num);
                            take_input(&U, &R, &take_ip_sudoku, &skeleton_sudoku, row, col, num);
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 2:
                            hint(&take_ip_sudoku, &solved_sudoku, &skeleton_sudoku, &U, &R);
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 3:
                            undo(&U, &R, &take_ip_sudoku);
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 4:
                            redo(&U, &R, &take_ip_sudoku);
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 5:
                            copy_sudoku(&extra_sudoku, &sudoku);
                            start = clock();
                            solve_sudoku(&extra_sudoku);
                            end = clock();
                            time = (double)(end - start)/CLOCKS_PER_SEC;
                            print_sudoku(&solved_sudoku, &skeleton_sudoku);
                            printf("Execution time = %f secs\n\n", time);
                            copy_sudoku(&extra_sudoku, &sudoku);
                            start = clock();
                            solve_bitmanipulation(&extra_sudoku);
                            end = clock();
                            time = (double)(end - start)/CLOCKS_PER_SEC;
                            print_sudoku(&solved_sudoku, &skeleton_sudoku);
                            printf("Execution time = %f secs\n\n", time);
                            free_sudoku(&extra_sudoku);
                            break;
                        case 6:
                            delete_file(gamename_copy);
                            delete_file(gamename);
                            is_saved = false;
                            do {
                                printf("Enter game name:   (exit)\n");
                                scanf("%s", gamename); 
                                if (strcmp(gamename, "exit") == 0) {
                                    break;
                                }
                                is_saved = save_sudoku_to_file(&take_ip_sudoku, gamename);
                            } while(!is_saved);

                            if (strcmp(gamename, "exit") != 0) {
                                printf("File saved as %s.\n", gamename);
                                strcat(gamename, "_skeleton");
                                save_sudoku_to_file(&skeleton_sudoku, gamename);
                                printf("Returning to Main Menu..\n\n"); 
                                row = col = num = -1;
                                size = difficulty = 0;
                                gamename[0] = '\0';
                                end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                                choice_4 = 0; 
                            }
                            break;
                        case 7:
                            check_sudoku(&take_ip_sudoku, &solved_sudoku);
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 8:
                            reset_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            printf("Sudoku has been reset.\n\n");
                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                            break;
                        case 0:
                            end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                            row = col = num = -1;
                            size = difficulty = 0;
                            choice_4 = 0; 
                            gamename_copy[0] = '\0';
                            gamename[0] = '\0';
                            printf("Returning to Main Menu...\n\n");
                            break;
                        default:
                            printf("Invalid choice!! Try Again..\n\n");
                    }                                    
                }
                break;

            case 3:
                print_all_sudoku_filenames();
                printf("Enter file name to be deleted..\n");
                scanf("%s", gamename);
                delete_file(gamename);
                strcat(gamename, "_skeleton");
                delete_file(gamename);
                gamename[0] = '\0';
                break;
            case 0:
                printf("Exiting program...\nEnded.\n\n");
                choice_1 = 0;
                break;

            default:
                printf("Invalid choice_1. Please try again.\n");
        }
    }
    return 0;
}