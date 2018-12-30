#ifndef RUBIK_STRUCT_HEADER
#define RUBIK_STRUCT_HEADER
#include <SDL/SDL.h>
#include <math.h>
#include <time.h>
#include "/home/tom/projets/c/lib/3Dlib/3Dlib.h"

typedef enum {UNKNOWN, r_CUBE=1, r_PYRAMIX}e_rubikType;
typedef enum {X=0, Y, Z}e_axeRot;
//typedef enum {TOP=0, FRONT, BOTTOM, BACK, LEFT, RIGHT}e_facePos;
typedef enum {FRONT=0, TOP, LEFT, RIGHT, BOTTOM, BACK}e_facePos;

typedef struct r_faceLine{
	int face;
	int *line;
	int nbCase;
}r_faceLine;

typedef struct t_crown{
	int nbFace;
	e_axeRot axe;
	r_faceLine *faceLine;
	int faceAttached;
	int faceRotV;
}t_crown;

typedef struct r_face{
	int nbSide;
	int nbCase;
	int *tab;
	SDL_Rect pos;
	int w;
	int h;
}r_face;

typedef struct t_display{
	Uint32 backgroundColor;
	int caseContLen;
	Uint32 caseContColor;
	bool patron;
	int sideSpace;
}t_display;

typedef struct t_rubik3D{
	t_form form;
	t_camera cam;
	bool b_3D;
	bool b_3DInit;
	bool b_color;
	t_vertex3D center;
	t_triAxe triAxe;
	int contW;
	int VFOV;
	int HFOV;
}t_rubik3D;

typedef struct t_rubik{
	SDL_Surface *surface;
	e_rubikType type;
	r_face *face;
	int nbFace;
	int nbSide;
	Uint32 *color;
	int nbColor;
	t_crown *crown;
	int nbCrown;
	t_display display;
	t_rubik3D r_3D;
}t_rubik;
#endif
