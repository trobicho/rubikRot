#ifndef PYRAMIX_HEADER
#define PYRAMIX_HEADER
#include "rubikStruct.h"
#include "display3D.h"
#include "../lib/3Dlib/matrix.h"

void pyramix3Dinit(t_form *form, t_rubik *rubik);
void pyramix3Dcreate(t_form *cube, t_rubik *rubik);
int calc_nbPolyFace(int w);
#endif
