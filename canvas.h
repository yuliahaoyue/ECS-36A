//
// Created by Yulia Feng on 3/18/23.
//

#ifndef FINAL_PROJECT_CANVAS_H
#define FINAL_PROJECT_CANVAS_H


typedef struct canvas_struct {
    int* rows;
    int* cols;
    char** m;
} canvas;

canvas* create_canvas(int rows, int cols);
void display_canvas(canvas* cvs);
void delete_canvas(canvas* cvs);
void draw_line(canvas* cvs, int x1, int y1, int x2, int y2);
void resize_canvas(canvas* cvs, int rows, int cols);
void delete_row(canvas* cvs, int row);
void delete_column(canvas* cvs, int col);
void insert_row(canvas* cvs, int row);
void insert_column(canvas* cvs, int col);
void add_rows(canvas* cvs, int add_r);
void add_cols(canvas* cvs, int add_c);


#endif //FINAL_PROJECT_CANVAS_H
