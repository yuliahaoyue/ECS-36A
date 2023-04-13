//
// Created by Yulia Feng on 3/18/23.
//

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "canvas.h"

bool isnum(char* str) {
    /*
     * Determine if a given string is a number.
     * @str: the string to check
     * @modifies: nothing
     * @returns: true if the string is a number; false otherwise
     */
    if (!isdigit(str[0]) && str[0] != '-') {
        return false;       //first char not digit or sign
    }
    for (int i = 1; i < strlen(str); i++){
        if (!isdigit(str[i])) {
            return false;   //current char not digit
        }
    }
    return true;
}

bool valid_1st_and_count(char cm[10][50], int n) {
    /*
     * Check if the first word of a sentence is a single char
     * and if the sentence have exactly n words
     * @cm: the sentence to check
     * @n: number of words cm should have
     * @modifies: nothing
     * @returns: true if cm[0] is char adn cm has n words; false otherwise
     */
    if (strlen(cm[0]) == 1 && cm[n-1][0] != '\0' && cm[n][0] == '\0') {
        return true;
    }
    return false;
}

bool rc_valid(char cm[10][50]) {
    /*
     * Check if a sentence is formatted as single_char 'r'/'c' number
     * @cm: the sentence to check
     * @modifies: nothing
     * @returns: true if cm is formatted as single_char 'r'/'c' number; false otherwise
     */
    if (valid_1st_and_count(cm, 3) && strlen(cm[1]) == 1 &&
    ((cm[1][0]) == 'r' || cm[1][0] == 'c') && isnum(cm[2])) {
        return true;
    }
    return false;
}

canvas* arg_make_canvas(int argc, char* argv[]) {
    /*
     * Initialize the canvas in accordance to command line parameters.
     * Create a canvas of size indicated by user if correctly formatted.
     * Create a 10X10 canvas otherwise.
     * @argc: number of command line parameters
     * @argv: array of command line parameters
     * @modifies: argv
     * @returns: the initialized canvas
     */
    int rows;
    int cols;
    if (argc == 1) {
        goto make;      //no command line arguments
    } if (argc != 3) {
        printf("Wrong number of command line arguments entered.\n");
        printf("Usage: ./paint.out [num_rows num_cols]\n");
        goto invalid;   //wrong number of arguments
    }
    //check if arguments contain number
    for (int i = 1; i < 3; ++i) {
        for (int j = 0; j < 50; ++j) {
            if (argv[i][j] == ' ') {
                argv[i][j] = '\0';          //drop chars after space
                break;
            } else if (argv[i][j] == '\0') {
                break;                      //end of parameter
            }
        }
        if (!isnum(argv[i])) {
            goto invalid;       //argument not number
        }
    }
    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    //check numbers of rows and columns
    if (rows < 1) {
        printf("The number of rows is less than 1.\n");
        goto invalid;
    } if (cols < 1) {
        printf("The number of columns is less than 1.\n");
        goto invalid;
    }
    //create canvas
    return create_canvas(rows, cols);
    invalid:
    printf("Making default board of 10 X 10.\n");
    make:
    return create_canvas(10, 10);
}

void get_command(canvas* cvs) {
    /*
     * Recognize new command from user and call functions to update accordingly.
     * @cvs: the canvas to update
     * @modifies: cvs
     * @returns: nothing
     */
    char cm[10][50];
    char ch;
    int i = 0;
    int j = 0;
    bool word = false;
    printf("Enter your command: ");
    //make empty array of strings
    for (int k = 0; k < 6; ++k) {
        cm[k][0] = '\0';
    }
    //make line input into array of strings
    while ((ch = fgetc(stdin)) != '\n') {
        if (isspace(ch)) {
            if (word) {         //end of a word
                cm[i][j] = '\0';    //end current word
                i++;                //next word
                j = 0;              //reset char index
                word = false;       //not in a word
            }
        } else {                //store char
            cm[i][j] = ch;
            j++;                    //next char
            if (!word) {
                word = true;        //in a word
            }
        }
    }
    cm[i][j] = '\0';            //end last word
    //recognize first word in command
    if (cm[0][0] == 'h' && strlen(cm[0]) == 1 && cm[1][0] == '\0') {        //single word with single char
            print_help();
    } else if (cm[0][0] == 'q' && strlen(cm[0]) == 1 && cm[1][0] == '\0') { //single word with single char
        delete_canvas(cvs);
        exit(0);
    } else if (cm[0][0] == 'w') {
        command_draw(cvs, cm);
    } else if (cm[0][0] == 'r') {
        command_resize(cvs, cm);
    } else if (cm[0][0] == 'a') {
        command_add(cvs, cm);
    } else if (cm[0][0] == 'd') {
        command_delete(cvs, cm);
    } else if (cm[0][0] == 'e') {
        command_erase(cvs, cm);
    } else if (cm[0][0] == 's') {
        command_save(cvs, cm);
    } else if (cm[0][0] == 'l') {
        command_load(cvs, cm);
    } else {
        printf("Unrecognized command. Type h for help.\n");
    }
}

void command_draw(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the draw command is proper and call function to draw line if proper
     * @cvs: canvas to draw a line
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!valid_1st_and_count(cm, 5)) {
        goto invalid;
    }
    for (int i = 1; i <= 4; ++i) {
        if (!isnum(cm[i])) {
            goto invalid;   //coordinate not a number
        }
    }
    int y1 = atoi(cm[1]);
    int x1 = atoi(cm[2]);
    int y2 = atoi(cm[3]);
    int x2 = atoi(cm[4]);
    //check if numbers are valid
    if (x1 < 0 || x2 < 0 || x1 >= (*cvs->cols) || x2 >= (*cvs->cols)          //check if start point is in canvas
    || y1 < 0 || y2 < 0 || y1 >= (*cvs->rows) || y2 >= (*cvs->rows)           //check if end point is in canvas
    || !(x1 == x2 || y1 == y2 || abs(x1 - x2) == abs(y1 - y2))) {             //check if straight line is formed
        goto invalid;
    }
    //valid
    draw_line(cvs, x1, y1, x2, y2);
    return;
    invalid:
    printf("Improper draw command.\n");

}

void command_resize(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the resize command is proper and call function to resize canvas if proper
     * @cvs: canvas to resize
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!valid_1st_and_count(cm, 3)) {
        printf("Improper resize command.\n");
        return;
    } if (!isnum(cm[1])) {
        printf("The number of rows is not an integer.\n");
        return;
    } if (!isnum(cm[2])) {
        printf("The number of columns is not an integer.\n");
        return;
    }
    int rows = atoi(cm[1]);
    int cols = atoi(cm[2]);
    if (rows < 1) {
        printf("The number of rows is less than 1.\n");
        return;
    } if (cols < 1) {
        printf("The number of columns is less than 1.\n");
        return;
    }
    //valid
    resize_canvas(cvs, rows, cols);
}

void command_add(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the add command is proper and call function to update canvas if proper
     * @cvs: canvas to update
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!rc_valid(cm)) {
        goto invalid;
    }
    int add_num = atoi(cm[2]);
    if (cm[1][0] == 'r' && add_num >= 0 && add_num <= (*cvs->rows)) {
        insert_row(cvs, add_num);       //valid
    } else if (cm[1][0] == 'c' && add_num >= 0 && add_num <= (*cvs->cols)) {
        insert_column(cvs, add_num);     //valid
    } else {
        goto invalid;           //row/column num out of range
    }
    return;
    invalid:
    printf("Improper add command.\n");
}

void command_delete(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the add command is proper and call function to update canvas if proper
     * @cvs: canvas to update
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!rc_valid(cm)) {
        goto invalid;
    }
    int del_num = atoi(cm[2]);
    if (cm[1][0] == 'r' && del_num >= 0 && del_num < (*cvs->rows)) {
        delete_row(cvs, del_num);       //valid
    } else if (cm[1][0] == 'c' && del_num >= 0 && del_num < (*cvs->cols)) {
        delete_column(cvs, del_num);     //valid
    } else {
        goto invalid;           //row/column num out of range
    }
    return;
    invalid:
    printf("Improper delete command.\n");
}

void command_erase(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the erase command is proper and call function to update canvas if proper
     * @cvs: canvas to update
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!valid_1st_and_count(cm, 3)) {
        goto invalid;
    }
    if (!isnum(cm[1]) || !isnum(cm[2])) {
        goto invalid;           //row/column num not a number
    }
    int row = atoi(cm[1]);
    int col = atoi(cm[2]);
    if (row < 0 || col < 0 || row >= (*cvs->rows) || col >= (*cvs->cols)) {
        goto invalid;           //row/column num out of range
    }
    //valid
    cvs->m[row][col] = '*';
    return;
    invalid:
    printf("Improper erase command.\n");
}

void command_save(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the save command is proper and save canvas if proper
     * @cvs: canvas to save
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!valid_1st_and_count(cm, 2)) {
        goto invalid;
    }
    FILE* fp = fopen(cm[1], "w");
    if (fp == NULL) {
        goto invalid;           //file could not be created
    }
    //valid
    fprintf(fp, "%d %d\n", (*cvs->rows), (*cvs->cols)); //numbers of rows and columns in the first row
    for (int r = (*cvs->rows) - 1; r >= 0; --r) {
        fprintf(fp, "%s\n", cvs->m[r]);                 //print canvas without numbers or whitespaces
    }
    fclose(fp);
    return;
    invalid:
    printf("Improper save command or file could not be created.\n");
}

void command_load(canvas* cvs, char cm[10][50]) {
    /*
     * Check if the load command is proper and load file canvas to current canvas if proper
     * @cvs: current canvas
     * @cm: array of words in user's command
     * @modifies: cvs
     * @returns: nothing
     */
    if (!valid_1st_and_count(cm, 2)) {
        goto invalid;
    }
    FILE* fp = fopen(cm[1], "r");
    if (fp == NULL) {
        goto invalid;           //file could not be opened
    }
    //valid
    int rows;
    int cols;
    char ch;
    fscanf(fp, "%d %d\n", &rows, &cols);
    resize_canvas(cvs, rows, cols);
    for (int r = rows - 1; r >= 0; --r) {
        for (int c = 0; c < cols; ++c) {
            ch = fgetc(fp);         //read next char
            if(ch == '\n') {
                ch = fgetc(fp);     //skip '\n' and read next char
            }
            cvs->m[r][c] = ch;      //store char
        }
    }
    fclose(fp);
    return;
    invalid:
    printf("Improper load command or file could not be opened.\n");
}

void print_help() {
    /*
     * Print command instruction
     * @modifies: nothing
     * @returns: nothing
     */
    printf("Commands:\n");
    printf("Help: h\n");
    printf("Quit: q\n");
    printf("Draw line: w row_start col_start row_end col_end\n");
    printf("Resize: r num_rows num_cols\n");
    printf("Add row or column: a [r | c] pos\n");
    printf("Delete row or column: d [r | c] pos\n");
    printf("Erase: e row col\n");
    printf("Save: s file_name\n");
    printf("Load: l file_name\n");
}