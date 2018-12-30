#include "rubik.h"
#include "pyramix.h"

void rubikInit(t_rubik *rubik, int faceW, int faceH)
{
	int i=0;
	rubik->face=malloc(sizeof(r_face)*rubik->nbFace);
	rubik->color=malloc(sizeof(Uint32)*rubik->nbColor);
	for(i=0; i<rubik->nbFace; i++)
	{
		rubik->face[i].nbSide=rubik->nbSide;
		rubik->face[i].w=faceW;
		rubik->face[i].h=faceH;
		if(rubik->type == r_PYRAMIX)
			rubik->face[i].nbCase=calc_nbPolyFace(faceW);
		else
			rubik->face[i].nbCase=faceW*faceH;
		rubik->face[i].tab=malloc(sizeof(int)*rubik->face[i].nbCase);
	}
	cubeCrownInit(rubik);
}

void faceInit(r_face *face, int tabV)
{
	int i;
	for(i=0; i<face->w*face->h; i++)
	{
		face->tab[i]=tabV;
	}
}

int cubeCrownInit(t_rubik *rubik)
{
	int a, b, i, face;
	if(rubik->type==r_CUBE)
	{
		rubik->nbCrown=3*rubik->face[0].w;
		rubik->crown=malloc(sizeof(t_crown)*rubik->nbCrown);
	}
	else
		return -1;
	for(i=0; i<rubik->nbCrown; i++)
	{
		rubik->crown[i].nbFace=4;
		rubik->crown[i].axe=i/rubik->face[0].w;
		rubik->crown[i].faceLine=malloc(sizeof(r_faceLine)*rubik->crown[i].nbFace);
	}
	rubik->crown[0].faceLine[0].face=TOP;
	rubik->crown[0].faceLine[1].face=BACK;
	rubik->crown[0].faceLine[2].face=BOTTOM;
	rubik->crown[0].faceLine[3].face=FRONT;
	rubik->crown[rubik->face[0].w].faceLine[0].face=FRONT;
	rubik->crown[rubik->face[0].w].faceLine[1].face=LEFT;
	rubik->crown[rubik->face[0].w].faceLine[2].face=BACK;
	rubik->crown[rubik->face[0].w].faceLine[3].face=RIGHT;
	rubik->crown[rubik->face[0].w*2].faceLine[0].face=TOP;
	rubik->crown[rubik->face[0].w*2].faceLine[1].face=RIGHT;
	rubik->crown[rubik->face[0].w*2].faceLine[2].face=BOTTOM;
	rubik->crown[rubik->face[0].w*2].faceLine[3].face=LEFT;
	for(i=0; i<rubik->nbCrown; i++)
	{
		for(a=0; a<rubik->crown[i].nbFace; a++)
		{
			if(i%rubik->face[0].w)
				rubik->crown[i].faceLine[a].face=rubik->crown[i-i%rubik->face[0].w].faceLine[a].face;
			face=rubik->crown[i].faceLine[a].face;
			rubik->crown[i].faceLine[a].nbCase=rubik->face[0].w;
			rubik->crown[i].faceLine[a].line=malloc(sizeof(r_faceLine)*rubik->face[0].w);
			rubik->crown[i].faceAttached=-1;
			for(b=0; b<rubik->face[0].w && b<rubik->crown[i].faceLine[a].nbCase; b++)
			{
				switch(rubik->crown[i].axe)
				{
					case X:
						rubik->crown[i].faceLine[a].line[b]=b*rubik->face[0].w+(i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=LEFT;
							rubik->crown[i].faceRotV=1;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=RIGHT;
							rubik->crown[i].faceRotV=3;
						}
						break;
					case Y:
						rubik->crown[i].faceLine[a].line[b]=((face==BACK)?b:rubik->face[0].w-(b+1))+rubik->face[0].w*((face==BACK)?rubik->face[0].w-(i%rubik->face[0].w+1):i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=TOP;
							rubik->crown[i].faceRotV=3;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=BOTTOM;
							rubik->crown[i].faceRotV=1;
						}
						break;
					case Z:
						if(face==TOP || face==BOTTOM)
							rubik->crown[i].faceLine[a].line[b]=((face==TOP)?b:rubik->face[0].w-(b+1))+fabs(((face==TOP)?rubik->face[0].w-1:0)-\
									(i%rubik->face[0].w))*rubik->face[0].w;
						else
							rubik->crown[i].faceLine[a].line[b]=((face==RIGHT)?b:rubik->face[0].w-(b+1))*rubik->face[0].w+\
																fabs(((face==LEFT)?rubik->face[0].w-1:0)-i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=FRONT;
							rubik->crown[i].faceRotV=3;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=BACK;
							rubik->crown[i].faceRotV=1;
						}
						break;
				}
			}
		}
	}
}

int pyramixCrownInit(t_rubik *rubik)
{
	int a, b, i, face;
	if(rubik->type==r_PYRAMIX)
	{
		rubik->nbCrown=4*rubik->face[0].w;
		rubik->crown=malloc(sizeof(t_crown)*rubik->nbCrown);
	}
	else
		return -1;
	for(i=0; i<rubik->nbCrown; i++)
	{
		rubik->crown[i].nbFace=3;
		rubik->crown[i].axe=i/rubik->face[0].w;
		rubik->crown[i].faceLine=malloc(sizeof(r_faceLine)*rubik->crown[i].nbFace);
	}
	rubik->crown[0].faceLine[0].face=TOP;
	rubik->crown[0].faceLine[1].face=BACK;
	rubik->crown[0].faceLine[2].face=BOTTOM;
	rubik->crown[rubik->face[0].w].faceLine[0].face=FRONT;
	rubik->crown[rubik->face[0].w].faceLine[1].face=LEFT;
	rubik->crown[rubik->face[0].w].faceLine[2].face=BACK;
	rubik->crown[rubik->face[0].w*2].faceLine[0].face=TOP;
	rubik->crown[rubik->face[0].w*2].faceLine[1].face=RIGHT;
	rubik->crown[rubik->face[0].w*2].faceLine[2].face=BOTTOM;
	for(i=0; i<rubik->nbCrown; i++)
	{
		for(a=0; a<rubik->crown[i].nbFace; a++)
		{
			if(i%rubik->face[0].w)
				rubik->crown[i].faceLine[a].face=rubik->crown[i-i%rubik->face[0].w].faceLine[a].face;
			face=rubik->crown[i].faceLine[a].face;
			rubik->crown[i].faceLine[a].nbCase=(rubik->face[0].w-i%rubik->face[0].w)*2-1;
			rubik->crown[i].faceLine[a].line=malloc(sizeof(r_faceLine)*rubik->crown[i].faceLine[a].nbCase);
			rubik->crown[i].faceAttached=-1;
			for(b=0; b<rubik->crown[i].faceLine[a].nbCase; b++)
			{
				switch(rubik->crown[i].axe)
				{
					case X:
						rubik->crown[i].faceLine[a].line[b]=b*rubik->face[0].w+(i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=LEFT;
							rubik->crown[i].faceRotV=1;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=RIGHT;
							rubik->crown[i].faceRotV=3;
						}
						break;
					case Y:
						rubik->crown[i].faceLine[a].line[b]=((face==BACK)?b:rubik->face[0].w-(b+1))+rubik->face[0].w*((face==BACK)?rubik->face[0].w-(i%rubik->face[0].w+1):i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=TOP;
							rubik->crown[i].faceRotV=3;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=BOTTOM;
							rubik->crown[i].faceRotV=1;
						}
						break;
					case Z:
						if(face==TOP || face==BOTTOM)
							rubik->crown[i].faceLine[a].line[b]=((face==TOP)?b:rubik->face[0].w-(b+1))+fabs(((face==TOP)?rubik->face[0].w-1:0)-\
									(i%rubik->face[0].w))*rubik->face[0].w;
						else
							rubik->crown[i].faceLine[a].line[b]=((face==RIGHT)?b:rubik->face[0].w-(b+1))*rubik->face[0].w+\
																fabs(((face==LEFT)?rubik->face[0].w-1:0)-i%rubik->face[0].w);
						if(i%rubik->face[0].w==0)
						{
							rubik->crown[i].faceAttached=FRONT;
							rubik->crown[i].faceRotV=3;
						}
						else if(i%rubik->face[0].w==rubik->face[0].w-1)
						{
							rubik->crown[i].faceAttached=BACK;
							rubik->crown[i].faceRotV=1;
						}
						break;
				}
			}
		}
	}
}

bool rubikSolve(t_rubik *rubik)
{
	int i, a, s;
	for(i=0; i<rubik->nbFace; i++)
	{
		for(a=1, s=rubik->face[i].tab[0]; a<rubik->face[i].nbCase; a++)
		{
			if(rubik->face[i].tab[a]!=s)
				return FALSE;
		}
	}
	return TRUE;
}

void rubikMix(t_rubik *rubik, int nbRot)
{
	int i;
	for(i=0; i<nbRot; i++)
	{
		rot(rubik, t_rand(0, rubik->face->w*3), 1);
	}
}

void freeRubik(t_rubik *rubik)
{
	int i, a;
	for(i=0; i<rubik->nbFace; i++)
	{
		free(rubik->face[i].tab);
	}
	for(i=0; i<rubik->nbCrown; i++)
	{
		for(a=0; a<rubik->crown[a].nbFace; a++)
		{
			free(rubik->crown[i].faceLine[a].line);
		}
		free(rubik->crown[i].faceLine);
	}
	free(rubik->face);
	free(rubik->color);
	free(rubik->crown);
}

int t_rand(int min, int max)
{
	return rand()%((max-min)-1)+min;
}
