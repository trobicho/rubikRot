#ifndef HUMAN_HEADER
#define HUMAN_HEADER
#include "click.h"

void t_human(t_rubik *rubik);
void humanClick(t_rubik *rubik, SDL_Event *event);
bool human3D(t_rubik *rubik, SDL_Event *event, Uint8 *keystates);
#endif
