#ifndef RUBIK_HEADER
#define RUBIK_HEADER
#include "rubikRot.h"

void rubikInit(t_rubik *rubik, int faceW, int faceH);
void faceInit(r_face *face, int tabV);
int cubeCrownInit(t_rubik *rubik);
bool rubikSolve(t_rubik *rubik);
void rubikMix(t_rubik *rubik, int nbRot);
void freeRubik(t_rubik *rubik);
int t_rand(int min, int max);
#endif
