#include "display3D.h"
#include "../lib/3Dlib/matrix.h"

Uint32 getColorFace(t_rubik *rubik, int faceN);

void draw3dRubik(t_rubik *rubik)
{
	int i, triAxeSize=60;
	t_pos2D triAxePos={.x=triAxeSize+10.0, .y=rubik->surface->h-triAxeSize-10.0};
	if(!rubik->r_3D.b_3DInit)
	{
		if(rubik->type == r_PYRAMIX)
			pyramix3Dinit(&rubik->r_3D.form, rubik);
		else
			rubik3Dinit(&rubik->r_3D.form, rubik);
	}
	if(rubik->r_3D.b_color)
	{
		for(i=0; i<rubik->r_3D.form.nbPoly; i++)
			drawFormPoly3D_UniColor(&rubik->r_3D.form.poly[i], NULL, getColorFace(rubik, i), 1);
	}
	else
		drawForm_sk(&rubik->r_3D.form, NULL, 2, 0x0);
	drawTriAxe(rubik->r_3D.triAxe, triAxePos, triAxeSize, 3);
}

Uint32 getColorFace(t_rubik *rubik, int faceN)
{
	int w=rubik->face->w, sqrtW=w*w, n=faceN%sqrtW;
	switch(faceN/sqrtW)
	{
		case 0:
			return rubik->color[rubik->face[FRONT].tab[n]];
			break;
		case 1:
			return rubik->color[rubik->face[TOP].tab[(w-n/w-1)*w+(n%w)]];
			break;
		case 2:
			return rubik->color[rubik->face[LEFT].tab[(n%w)*w+(w-n/w-1)]];
			break;
		case 3:
			return rubik->color[rubik->face[RIGHT].tab[(n%w)*w+(n/w)]];
			break;
		case 4:
			return rubik->color[rubik->face[BOTTOM].tab[n]];
			break;
		case 5:
			return rubik->color[rubik->face[BACK].tab[(w-n/w-1)*w+(n%w)]];
			break;
	}
	return 0;
}

void faceSelected_3D(t_rubik *rubik, int faceN)
{
	//drawPoly3D_UniColor(rubik->r_3D.face[faceN], 11111, 0, 100);
}

/*
t_vertex3D* polyToCam(SDL_Surface *surface, t_poly *polyRet, t_poly *poly, t_camera cam, int minZ, int maxZ)
{
	int i, nbVertex;
	*polyRet=*poly;
	t_returnClipping Return;
	t_vertex3D center={(double)surface->w/2, (double)surface->h/2, 0.0};
	t_vertex3D *vPoly;
	
	*polyRet=*poly;
	polyRet->vertex=malloc(sizeof(t_vertex3D*)*poly->nbVertex);
	vPoly=cameraPoly(*poly, cam, center);
	for(i=0; i<poly->nbVertex; i++)
	{
		polyRet->vertex[i]=&vPoly[i];
	}
	if(backPolyCulling(polyRet))
	{
		Return=clipping(surface, *polyRet, minZ, maxZ, &nbVertex, 0, 0, get_perspective());
		vPoly=Return.vertex;
		if(nbVertex!=poly->nbVertex)
		{
			polyRet->vertex=realloc(polyRet->vertex, sizeof(t_vertex3D*)*nbVertex);
			for(i=0; i<poly->nbVertex; i++)
			{
				polyRet->vertex[i]=&vPoly[i];
			}
		}
		if(!nbVertex)
			return NULL;
		return vPoly;
	}
	return NULL;
}
*/
