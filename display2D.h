#ifndef DISPLAY_2D_HEADER
#define DISPLAY_2D_HEADER
#include "rubikStruct.h"

void drawFace(t_rubik *rubik, r_face *face);
void centerRubik(t_rubik *rubik);
void cubePatron(t_rubik *rubik);
void faceSelected_2D(t_rubik *rubik, int faceN);
#endif
