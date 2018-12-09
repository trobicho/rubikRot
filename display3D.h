#ifndef DISPLAY_3D_HEADER
#define DISPLAY_3D_HEADER
#include "rubikStruct.h"

void draw3dRubik(t_rubik *rubik);
void rubik3Dinit(t_form *form, t_rubik *rubik);
void rubik3Dcreate(t_form *cube, t_rubik *rubik);
void faceSelected_3D(t_rubik *rubik, int faceN);
//t_vertex3D* polyToCam(SDL_Surface *surface, t_poly *polyRet, t_poly *poly, t_camera cam, int minZ, int maxZ);
#endif
