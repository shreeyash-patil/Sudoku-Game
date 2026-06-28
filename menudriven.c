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

int main(){
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
    while(choice_1){
        printf("\nMain Menu:\n");
        printf("1 - To start a new game..\n");
        printf("2 - To load a saved game..\n");
        printf("3 - To delete a game..\n");
        printf("0 - To exit game..\n");
        printf("Enter your choice: ");
        scanf("%d", &choice_1);
        printf("\n");
        switch(choice_1){
            case 1:
                choice_2 = 1;
                while(choice_2){
                    printf("Secondary Menu..\n");
                    printf("1 - To select sudoku size...\n");
                    printf("0 - To return to main menu...\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice_2);
                    printf("\n");
                    switch(choice_2){
                        case 1:
                            printf("Enter Size of sudoku 4, 9, 16.\n");
                            scanf("%d", &size);
                            printf("Enter Difficulty :\n1 - Easy.\n2 - Medium.\n3 - Hard.\n");
                            scanf("%d", &difficulty);
                            printf("\n");
                            if((size == 4 || size == 9 || size == 16) && (difficulty == 1 || difficulty == 2 || difficulty == 3)){
                                start_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, size, difficulty, &U, &R);
                                print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                choice_3 = 1;
                                while(choice_3){
                                    printf("Tertiary Menu..\n");
                                    printf("1 - To enter a number in sudoku..\n");
                                    printf("2 - To get a hint..\n");
                                    printf("3 - To undo.\n");
                                    printf("4 - To redo.\n");
                                    printf("5 - To autosolve the sudoku.\n");
                                    printf("6 - To save sudoku and return to secondary menu..\n");
                                    printf("7 - To check if any input is wrong..\n");
                                    printf("0 - To return to secondary menu without saving\n");
                                    printf("Enter your choice: ");
                                    scanf("%d", &choice_3);
                                    printf("\n");
                                    switch(choice_3){
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
                                            printf("Enter game name to be saves as:\n");
                                            scanf("%s", gamename); 
                                            save_sudoku_to_file(&take_ip_sudoku, gamename);
                                            printf("File saved as %s.\n", gamename);
                                            strcat(gamename, "_skeleton");
                                            save_sudoku_to_file(&skeleton_sudoku, gamename);
                                            printf("Returning to Secondary Menu..\n\n");
                                            row = col = num = -1;
                                            size = difficulty = 0;
                                            gamename[0] = '\0';
                                            end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                                            choice_3 = 0;
                                            break;
                                        case 7:
                                            check_sudoku(&take_ip_sudoku, &solved_sudoku);
                                            print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                                            break;
                                        case 0:
                                            end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                                            row = col = num = -1;
                                            size = difficulty = 0;
                                            printf("Returning to secondary menu...\n\n");
                                            break;
                                        default:
                                            printf("Invalid choice!! Try Again..\n\n");
                                    }                                    
                                }
                            }

                            else{
                                printf("Invalid Size or Difficulty!! Select Again..\n");
                            }
                            break;

                        case 0:
                            printf("Returning to Main Menu..\n");
                            break;

                        default:
                            printf("Invalid choice!!  Please try again.\n");
                    }
                }
                break;

            case 2:
                print_all_sudoku_filenames("all_sudoku_files.txt");
                printf("Enter game to be loaded\n");
                scanf("%s", gamename);
                strcpy(gamename_copy, gamename);
                read_sudoku_from_file(&take_ip_sudoku, gamename);
                strcat(gamename, "_skeleton");
                read_sudoku_from_file(&skeleton_sudoku, gamename);
                copy_sudoku(&sudoku, &take_ip_sudoku);
                copy_sudoku(&solved_sudoku, &take_ip_sudoku);
                solve_bitmanipulation(&solved_sudoku);
                init_undo_redo(&U, &R);
                print_sudoku(&take_ip_sudoku, &skeleton_sudoku);
                choice_4 = 1;
                while(choice_4){
                    printf("Tertiary Menu..\n");
                    printf("1 - To enter a number in sudoku..\n");
                    printf("2 - To get a hint..\n");
                    printf("3 - To undo.\n");
                    printf("4 - To redo.\n");
                    printf("5 - To autosolve the sudoku.\n");
                    printf("6 - To save sudoku and return to secondary menu..\n");
                    printf("7 - To check if any input is wrong..\n");
                    printf("0 - To return to secondary menu without saving\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice_4);
                    printf("\n");
                    switch(choice_4){
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
                            delete_file(gamename_copy);
                            delete_file(gamename);
                            save_sudoku_to_file(&take_ip_sudoku, gamename_copy);
                            printf("File saved as %s.\n", gamename_copy);
                            strcat(gamename_copy, "_skeleton");
                            save_sudoku_to_file(&skeleton_sudoku, gamename);
                            printf("Returning to Main Menu..\n\n");
                            row = col = num = -1;
                            size = difficulty = 0;
                            gamename_copy[0] = '\0';
                            gamename[0] = '\0';
                            end_game(&sudoku, &take_ip_sudoku, &skeleton_sudoku, &solved_sudoku, &U, &R);
                            choice_4 = 0;
                            break;
                        case 7:
                            check_sudoku(&take_ip_sudoku, &solved_sudoku);
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
                print_all_sudoku_filenames("all_sudoku_files.txt");
                printf("Enter file name to be deleted..\n");
                scanf("%s", gamename);
                delete_file(gamename);
                strcat(gamename, "_skeleton");
                delete_file(gamename);
                gamename[0] = '\0';
                break;
            case 0:
                printf("Exiting program...\nEnded.\n\n");
                break;

            default:
                printf("Invalid choice_1. Please try again.\n");
        }
    }
    return 0;
}