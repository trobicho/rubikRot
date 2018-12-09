#ifndef KEY_HEADER
#define KEY_HEADER
#include "rubik.h"

typedef struct t_key
{
	char key;
	e_axeRot axe;
	int value;
}t_key;

void initKeyTab(char *keyMap);
int keyRot(t_rubik *rubik, char key, int rotV, bool ctrl);
#endif
