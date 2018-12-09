#include "display2D.h"

void drawFace(t_rubik *rubik, r_face *face)
{
	int i, x, y, caseW=face->pos.w/face->w, caseH=face->pos.h/face->h;
	for(x=0; x<face->w; x++)
	{
		for(y=0; y<face->h; y++)
		{
			setPixel_WH(rubik->surface, face->pos.x+x*caseW, face->pos.y+y*caseH, caseW, caseH,\
					rubik->color[face->tab[face->w*y+x]]);
		}
		setPixel_WH(rubik->surface, face->pos.x+x*caseW, face->pos.y, rubik->display.caseContLen,\
				caseH*face->h, rubik->display.caseContColor);
	}
	for(y=0; y<=face->h; y++)
	{
		setPixel_WH(rubik->surface, face->pos.x, face->pos.y+y*caseH, caseW*face->w,\
				rubik->display.caseContLen, rubik->display.caseContColor);
	}
	setPixel_WH(rubik->surface, face->pos.x+face->w*caseW, face->pos.y, rubik->display.caseContLen,\
			caseH*face->h+rubik->display.caseContLen, rubik->display.caseContColor);
}

void centerRubik(t_rubik *rubik)
{
	int i, w=rubik->nbFace/2, h=2, faceW=rubik->surface->w/w, faceH=rubik->surface->h/h;
	if(rubik->display.patron && rubik->type==r_CUBE)
		cubePatron(rubik);
	else
	{
		for(i=0; i<rubik->nbFace; i++)
		{
			rubik->face[i].pos.w=(faceW<faceH)?faceW:faceH;
			rubik->face[i].pos.h=(faceW<faceH)?faceW:faceH;
			rubik->face[i].pos.x=(i%w)*(faceW)+rubik->display.sideSpace*i;
			rubik->face[i].pos.y=(i/w)*(faceH)+rubik->display.sideSpace*i;
		}
	}
}

void cubePatron(t_rubik *rubik)
{
	int i, w=3, h=4, space=rubik->display.sideSpace, faceW=(rubik->surface->w-space*(w-1))/w, faceH=(rubik->surface->h-space*(h-1))/h;
	faceW=(faceW<faceH)?faceW:faceH;
	if(rubik->display.patron && rubik->type==r_CUBE)
	{
		for(i=0; i<rubik->nbFace; i++)
		{
			rubik->face[i].pos.w=faceW;
			rubik->face[i].pos.h=faceW;
			switch(i)
			{
				case TOP:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2;
					rubik->face[i].pos.y=rubik->surface->h/2-faceW*2-space*(h-1)/2;
					break;
				case FRONT:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2;
					rubik->face[i].pos.y=rubik->surface->h/2-faceW+space-space*(h-1)/2;
					break;
				case BOTTOM:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2;
					rubik->face[i].pos.y=rubik->surface->h/2+space*2-space*(h-1)/2;
					break;
				case BACK:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2;
					rubik->face[i].pos.y=rubik->surface->h/2+faceW+space*3-space*(h-1)/2;
					break;
				case LEFT:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2-faceW-space;
					rubik->face[i].pos.y=rubik->surface->h/2-faceW+space-space*(h-1)/2;
					break;
				case RIGHT:
					rubik->face[i].pos.x=rubik->surface->w/2-faceW/2+faceW+space;
					rubik->face[i].pos.y=rubik->surface->h/2-faceW+space-space*(h-1)/2;
					break;
			}
		}
	}
}

void faceSelected_2D(t_rubik *rubik, int faceN)
{
	int i, w=rubik->face->w, f=faceN/(w*w), caseW=rubik->face[f].pos.w/w, caseH=rubik->face[f].pos.h/w; 
	int contLen=rubik->display.caseContLen;
	int x=rubik->face[f].pos.x+caseW*(faceN%w)+contLen;
	int y=rubik->face[f].pos.y+caseH*(faceN%(w*w)/w)+contLen;
	setAlphaPixel_WH(rubik->surface, x, y, caseW-contLen, caseH-contLen, 0, 100);
}
