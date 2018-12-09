#ifndef CLICK_HEADER
#define CLICK_HEADER
#include "key.h"

int t_click(t_rubik *rubik, SDL_MouseButtonEvent click);
bool dragClick(t_rubik *rubik, int faceN, t_vector3D vector, double margin);
#endif
