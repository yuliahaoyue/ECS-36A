//
// Created by Yulia Feng on 3/18/23.
//

#include <stdlib.h>
#include <stdio.h>
#include "canvas.h"

canvas* create_canvas(int rows, int cols) {
    /*
     * Create a new canvas of given numbers of rows and columns.
     * Set all spots to '*' indicating empty.
     * @rows: number of rows
     * @cols: number of columns
     * @modifies: nothing
     * @returns: a new canvas
     */
    //allocate space
    canvas* cvs;
    cvs = (canvas*) malloc(sizeof (canvas));
    cvs->rows = malloc(sizeof (int));
    cvs->cols = malloc(sizeof (int));
    (*cvs->rows) = rows;
    (*cvs->cols) = cols;
    cvs->m = (char**) malloc(rows * sizeof(char*));
    //iterate through rows
    for (int r = 0; r < rows; ++r) {
        cvs->m[r] = (char*) malloc((cols + 1) * sizeof(char));  //allocate space for row
        for (int c = 0; c < cols; ++c) {
            cvs->m[r][c] = '*';             //set all spots to blank
        }
        cvs->m[r][cols] = '\0';             //end row
    }
    return cvs;
}

void display_canvas(canvas* cvs) {
    /*
     * Display current canvas with numbers on the left and bottom indicating row # and column #.
     * @cvs: the canvas to display
     * @modifies: nothing
     * @returns: nothing
     */
    for (int r = (*cvs->rows) - 1; r >= 0; --r) {
        printf("%d", r);                    //print row numbers
        for (int c = 0; c < (*cvs->cols); ++c) {
            if (((*cvs->rows) > 10 || (*cvs->cols) > 10) && !(c == 0 && r > 9)) {
                printf(" ");                        //print an extra space to align with row/column nums >= 10 if any
            }
            printf(" %c", cvs->m[r][c]);    //print spot
        }
        printf("\n");                       //end row
    }
    printf(" ");                            //print space to replace row num
    for (int c = 0; c < (*cvs->cols); ++c) {
        if (((*cvs->rows) > 10 || (*cvs->cols) > 10) && c <= 10) {
            printf(" ");                            //print an extra space to align with row/column nums >= 10 if any
        }
        printf(" %d", c);                   //print column numbers
    }
    printf("\n");                           //end canvas
}

void delete_canvas(canvas* cvs) {
    /*
     * Delete a canvas and free all allocated space for it.
     * @cvs: the canvas to delete
     * @modifies: cvs
     * @returns: nothing
     */
    for (int i = 0; i < *cvs->rows; ++i) {
        free(cvs->m[i]);    //free each row
    }
    free(cvs->m);
    free(cvs);
}


void draw_line(canvas* cvs, int x1, int y1, int x2, int y2) {
    /*
     * Draw a line on a given canvas in accordance to the coordinates of start point and end point.
     * @cvs: the canvas to draw a line
     * @x1: the x coordinate of the start point
     * @y1: the y coordinate of the start point
     * @x2: the x coordinate of the end point
     * @y2: the y coordinate of the end point
     * @modifies: cvs
     * @returns: nothing
     */
    int c = x1;
    int r = y1;
    char ch;
    int num_spots = abs(x1 - x2) + 1;
    // Determine type of line to draw
    if (y1 == y2) {
        ch = '-';           //horizontal char
    } else if (x1 == x2) {
        ch = '|';           //vertical char
        num_spots = abs(y1 - y2) + 1;
    } else if ((x1 - x2) * (y1 - y2) > 0) {
        ch = '/';           //diagonal  char
    } else {
        ch = '\\';          //reverse diagonal  char
    }
    // Draw line
    for (int i = 0; i < num_spots; ++i) {
        if (cvs->m[r][c] != '*' && cvs->m[r][c] != ch) {
            cvs->m[r][c] = '+';         //draw intersection
        } else {
            cvs->m[r][c] = ch;          //draw spot
        }
        //move to next spot
        if (x1 < x2) {
            c++;        //rightwards
        } else if (x1 > x2){
            c--;        //leftwards
        } if (y1 < y2) {
            r++;        //upwards
        } else if (y1 > y2){
            r--;        //downwards
        }
    }
}

void resize_canvas(canvas* cvs, int rows, int cols) {
    /*
     * Resize the canvas in accordance to given numbers of rows and columns.
     * @cvs: the canvas to resize
     * @rows: the new number of rows
     * @cols: the new number of columns
     * @modifies: cvs
     * @returns: nothing
     */
    int row_change = rows - (*cvs->rows);
    int col_change = cols - (*cvs->cols);
    //modify rows
    if (row_change > 0) {           //add rows
        add_rows(cvs, row_change);
    } else if (row_change < 0) {    //delete rows
        for (int r = rows; r < (*cvs->rows); ++r) {
            free(cvs->m[r]);                                                    //free extra rows
        }
        cvs->m = (char **) realloc(cvs->m, rows * sizeof(char *));     //reallocate space for rows
    }
    (*cvs->rows) = rows;
    //modify columns
    if (col_change > 0) {           //add columns
        add_cols(cvs, col_change);
    } else if (col_change < 0) {    //delete columns
        for (int r = 0; r < rows; ++r) {                                        //reallocate space for each row
            cvs->m[r] = (char *) realloc(cvs->m[r], (cols + 1) * sizeof (char));
            cvs->m[r][cols] = '\0';                                             //end row
        }
    }
    (*cvs->cols) = cols;
}

void delete_row(canvas* cvs, int row) {
    /*
     * Delete a certain row in the canvas. Move rows above the deleted row downwards.
     * @cvs: the canvas to delete a row
     * @row: the row number to delete
     * @modifies: cvs
     * @returns: nothing
     */
    (*cvs->rows)--;
    for (int r = row; r < (*cvs->rows); ++r) {
        for (int c = 0; c <= *cvs->cols; ++c) {
            cvs->m[r][c] = cvs->m[r + 1][c];        //copy rows downwards
        }
    }
    free(cvs->m[(*cvs->rows)]);                     //free extra row space
    cvs->m = (char **) realloc(cvs->m, (*cvs->rows) * sizeof(char *));  //reallocate space for rows
}

void delete_column(canvas* cvs, int col){
    /*
     * Delete a certain column in the canvas. Move columns right to the deleted columns leftwards.
     * @cvs: the canvas to delete a column
     * @col: the column number to delete
     * @modifies: cvs
     * @returns: nothing
     */
    for (int r = 0; r < (*cvs->rows); ++r) {
        for (int c = col; c < (*cvs->cols); ++c) {
            cvs->m[r][c] = cvs->m[r][c+1];          //copy columns leftwards
        }
        cvs->m[r] = (char *) realloc(cvs->m[r], (*cvs->cols) * sizeof (char));
    }                                                                       //reallocate space for each row
    (*cvs->cols)--;
}

void insert_row(canvas* cvs, int row) {
    /*
     * Insert a certain row in the canvas. Move rows above the inserted row upwards.
     * @cvs: the canvas to insert a row
     * @row: the row number to insert
     * @modifies: cvs
     * @returns: nothing
     */
    (*cvs->rows)++;
    cvs->m = (char **) realloc(cvs->m, (*cvs->rows) * sizeof(char *));      //reallocate space for rows
    cvs->m[*cvs->rows - 1] = (char *) malloc((*cvs->cols + 1) * sizeof(char));  //allocate apace for new row
    for (int r = *cvs->rows - 1; r > row; --r) {
        for (int c = 0; c <= *cvs->cols; ++c) {
            cvs->m[r][c] = cvs->m[r - 1][c];        //copy rows upwards
        }
    }
    for (int c = 0; c < *cvs->cols; ++c) {
        cvs->m[row][c] = '*';                       //set spots in new row empty
    }
    cvs->m[row][*cvs->cols] = '\0';                 //end new row
}

void insert_column(canvas* cvs, int col){
    /*
     * Insert a certain column in the canvas. Move columns right to the inserted column rightwards.
     * @cvs: the canvas to insert a column
     * @row: the column number to insert
     * @modifies: cvs
     * @returns: nothing
     */
    (*cvs->cols)++;
    for (int r = 0; r < (*cvs->rows); ++r) {                                    //reallocate space for each row
        cvs->m[r] = (char *) realloc(cvs->m[r], (*cvs->cols + 1) * sizeof (char));
        for (int c = (*cvs->cols); c > col; --c) {
            cvs->m[r][c] = cvs->m[r][c - 1];        //copy columns rightwards (including null char)
        }
        cvs->m[r][col] = '*';                       //set spots in new column empty
    }
}



void add_rows(canvas* cvs, int add_r) {
    /*
     * Add a certain number of rows at the top of the canvas.
     * @cvs: the canvas to add rows
     * @add_r: the number of rows to add
     * @modifies: cvs
     * @returns: nothing
     */
    int new_r_num = (*cvs->rows) + add_r;
    cvs->m = (char **) realloc(cvs->m, new_r_num * sizeof(char *));     //reallocate space for rows
    for (int r = (*cvs->rows); r < new_r_num; ++r) {
        cvs->m[r] = (char *) malloc(((*cvs->cols) + 1) * sizeof(char));     //allocate space for new row
        for (int c = 0; c < (*cvs->cols); ++c) {
            cvs->m[r][c] = '*';                     //set spots in new rows empty
        }
        cvs->m[r][(*cvs->cols)] = '\0';             //end new rows
    }
    (*cvs->rows) = new_r_num;
}

void add_cols(canvas* cvs, int add_c) {
    /*
     * Add a certain number of columns at the right end of the canvas.
     * @cvs: the canvas to add columns
     * @add_r: the number of columns to add
     * @modifies: cvs
     * @returns: nothing
     */
    int new_c_num = (*cvs->cols) + add_c;
    for (int r = 0; r < (*cvs->rows); ++r) {                                //reallocate space for each row
        cvs->m[r] = (char *) realloc(cvs->m[r], (new_c_num + 1) * sizeof(char));
        for (int c = (*cvs->cols); c < new_c_num; ++c) {
            cvs->m[r][c] = '*';                     //set spots in new columns empty
        }
        cvs->m[r][new_c_num] = '\0';                //end all rows
    }
    (*cvs->cols) = new_c_num;
}
