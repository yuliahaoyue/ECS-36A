/*
 * This program creates a canvas of size indicated by user and allow user to draw lines on canvas,
 * resize the canvas, add/delete rows/columns, erase spots on canvas, and save/load the canvas.
 */

#include <stdbool.h>
#include "canvas.h"
#include "input.h"

int main(int argc, char* argv[]) {

    canvas* cvs;

    cvs = arg_make_canvas(argc, argv);
    do {
        display_canvas(cvs);
        get_command(cvs);
    } while (true);

}
