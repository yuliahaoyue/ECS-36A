//
// Created by Yulia Feng on 3/18/23.
//

#ifndef FINAL_PROJECT_INPUT_H
#define FINAL_PROJECT_INPUT_H

#include "canvas.h"


bool isnum(char* str);
bool valid_1st_and_count(char cm[10][50], int n);
bool rc_valid(char cm[10][50]);
canvas* arg_make_canvas(int argc, char* argv[]);
void get_command(canvas* cvs);
void print_help();
void command_draw(canvas* cvs, char cm[10][50]);
void command_resize(canvas* cvs, char cm[10][50]);
void command_add(canvas* cvs, char cm[10][50]);
void command_delete(canvas* cvs, char cm[10][50]);
void command_erase(canvas* cvs, char cm[10][50]);
void command_save(canvas* cvs, char cm[10][50]);
void command_load(canvas* cvs, char cm[10][50]);


#endif //FINAL_PROJECT_INPUT_H
