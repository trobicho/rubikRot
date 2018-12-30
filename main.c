#include "human.h"
#include "load.h"

void algo1(SDL_Surface *screen, t_rubik *rubik);
void algo2(SDL_Surface *screen, t_rubik *rubik);
void algo3(SDL_Surface *screen, t_rubik *rubik);

void main(int argc, char **argv)
{
	int i, c, faceW=3;
	char fileColor[1000];
	bool colorDef=FALSE;
	int rType = r_CUBE;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *screen=SDL_SetVideoMode(800, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	SDL_WM_SetCaption("RubikRot", NULL);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			switch(argv[i][1])
			{
				case 'w':
					sscanf(argv[i+1], "%d", &faceW);
					i++;
					break;
				case 'c':
					sscanf(argv[i+1], "%s", fileColor);
					colorDef=1;
					i++;
					break;
				case 't':
					sscanf(argv[i+1], "%d", &rType);
					i++;
					break;
				case 'h':
					printf("use -w ()");
					break;
			}
		}
	}
	t_rubik rubik={
		.surface=screen,
		.type=((rType > 2 || rType == 0) ? r_CUBE : rType),
		.nbFace=(rType == 2 ? 4 : 6),
		.nbSide=(rType == 2 ? 3 : 4),
		.nbColor=rubik.nbFace,
		.nbCrown=rubik.nbFace,
		.display={
			.backgroundColor=0x505050,
			.caseContLen=2,
			.caseContColor=0,
			.patron=TRUE,
			.sideSpace=10,
		},
		.r_3D={
			.b_3D=TRUE,
			.b_3DInit=FALSE,
			.b_color=TRUE,
			//.b_color=FALSE,
			.contW=2,
			.VFOV=800,
			.HFOV=800,
		},
	};
	printf("L'algo de reso c'est pour coco.\n");
	SDL_FillRect(screen, NULL, rubik.display.backgroundColor);
	rubikInit(&rubik, faceW, faceW);
	initKeyTab("fr");
	for(i=0; i<rubik.nbFace; i++)
	{
		faceInit(&rubik.face[i], i);
	}
	if(!colorDef)
	{
		rubik.color[TOP]=SDL_MapRGB(screen->format, 255, 255, 255);
		rubik.color[FRONT]=SDL_MapRGB(screen->format, 255, 0, 0);
		rubik.color[BOTTOM]=SDL_MapRGB(screen->format, 255, 255, 0);
		rubik.color[BACK]=SDL_MapRGB(screen->format, 255, 100, 0);
		rubik.color[LEFT]=SDL_MapRGB(screen->format, 0, 255, 0);
		rubik.color[RIGHT]=SDL_MapRGB(screen->format, 0, 0, 255);
	}
	else
		load_color(&rubik, fileColor);
	centerRubik(&rubik);
	drawRubik(&rubik);
	t_human(&rubik);
	SDL_Flip(screen);
	SDL_Quit();
	freeRubik(&rubik);
}
