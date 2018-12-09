#include "display3D.h"
#include "../lib/3Dlib/matrix.h"

Uint32 getColorFace(t_rubik *rubik, int faceN);

void draw3dRubik(t_rubik *rubik)
{
	int i, triAxeSize=60;
	t_vertex3D triAxePos={.x=triAxeSize+10.0, .y=rubik->surface->h-triAxeSize-10.0, .z=0};
	if(!rubik->r_3D.b_3DInit)
	{
		rubik3Dinit(&rubik->r_3D.form, rubik);
	}
	if(rubik->r_3D.b_color)
	{
		for(i=0; i<rubik->r_3D.form.nbPoly; i++)
			drawFormPoly3D_UniColor(&rubik->r_3D.form.poly[i], NULL, getColorFace(rubik, i), 1);
	}
	else
		drawForm_sk(&rubik->r_3D.form, NULL, 2, 0x0);
	drawTriAxe(rubik->surface, rubik->r_3D.triAxe, triAxePos, triAxeSize, 3);
}

void rubik3Dinit(t_form *form, t_rubik *rubik)
{
	form->texture=NULL;
	form->vertexNormal=NULL;
	form->nbGroupe=1;
	rubik->r_3D.triAxe=(t_triAxe){
		.x=(t_vector3D){.x=1, .y=0, .z=0},
		.y=(t_vector3D){.x=0, .y=1, .z=0},
		.z=(t_vector3D){.x=0, .y=0, .z=1},
		.colorX=0xFF0000,
		.colorY=0x00FF00,
		.colorZ=0x0000FF,
	};
	rubik3Dcreate(form, rubik);
	rubik->r_3D.b_3DInit=TRUE;
	rubik->r_3D.center=(t_vertex3D){.x=0.5, .y=0.5, .z=0.5};
	rubik->r_3D.cam=(t_camera){.pos=(t_vertex3D){.x=0.5, .y=0.5, .z=0}, .targetVector=(t_vertex3D){.x=0, .y=0, .z=1}, .angle=0};
	t_vertex3D min={.x=rubik->surface->w/10, .y=rubik->surface->h/10, .z=10.0};
	t_vertex3D max={.x=rubik->surface->w-rubik->surface->w/10, .y=rubik->surface->h-rubik->surface->h/10, .z=1000};
	set_perspective(0);
	set_min(-1, -1, 0);
	set_max(2, 2, 2);
	set_screen(rubik->surface);
	t_fitSpaceToScreen(1);
}

void rubik3Dcreate(t_form *cube, t_rubik *rubik)
{
	int i;
	double min=0.0, max=1.0;
	int w=rubik->face->w, sqrtW=w*w;
	int nbV_FB=(w+1)*(w+1), nbV_M=w*4;
	int x=0, y=0, z=0;
	cube->nbPoly=6*sqrtW;
	cube->poly=malloc(sizeof(t_formPoly3D)*cube->nbPoly);
	cube->nbVertex=nbV_FB*2+nbV_M*(w-1);
	cube->vertex=malloc(sizeof(t_vertex3D)*cube->nbVertex);
	for(i=0; i<cube->nbVertex; i++, x++)
	{
		if((((y==0 || y==w) || (z==0 || z==w)) && x>w)||(((y>0 && y<w) && (z>0 && z<w)) && x>1))
		{
			y++;
			x=0;
		}
		if(y>w)
		{
			z++;
			y=0;
		}
		cube->vertex[i].x=(1.0/w)*(((y>0 && y<w)&&(z>0 && z<w))?x*w:x);
		cube->vertex[i].y=(1.0/w)*y;
		cube->vertex[i].z=(1.0/w)*z;
	}
	for(i=0; i<cube->nbPoly; i++)
	{
		cube->poly[i].nbVertex=4;
		cube->poly[i].vertex=malloc(sizeof(t_vertex3D**)*4);
		cube->poly[i].texture_type=COLOR;
		cube->poly[i].doubleNormal=0;
		switch(i/sqrtW)
		{
			case 0:
				cube->poly[i].vertex[0]=&cube->vertex[i+i/w];
				cube->poly[i].vertex[1]=&cube->vertex[i+i/w+1];
				cube->poly[i].vertex[2]=&cube->vertex[i+i/w+w+2];
				cube->poly[i].vertex[3]=&cube->vertex[i+i/w+w+1];
				cube->poly[i].vertexOrder=CLOCKWISE;
				break;
			case 1:
				cube->poly[i].vertex[0]=&cube->vertex[i%w+(((i%sqrtW)/w)?nbV_FB+nbV_M*((i%sqrtW)/w-1):0)];
				cube->poly[i].vertex[1]=&cube->vertex[i%w+(((i%sqrtW)/w)?nbV_FB+nbV_M*((i%sqrtW)/w-1):0)+1];
				cube->poly[i].vertex[2]=&cube->vertex[i%w+(nbV_FB+nbV_M*((i%sqrtW)/w))+1];
				cube->poly[i].vertex[3]=&cube->vertex[i%w+(nbV_FB+nbV_M*((i%sqrtW)/w))];
				cube->poly[i].vertexOrder=COUNTERCLOCKWISE;
				break;
			case 2:
				cube->poly[i].vertex[0]=&cube->vertex[(((i%sqrtW)/w)?(i%w?w+1+(i%w-1)*2:0)+nbV_FB+nbV_M*((i%sqrtW)/w-1)\
						:i%w*(w+1))];
				cube->poly[i].vertex[1]=&cube->vertex[(((i%sqrtW)/w)?1+w+(i%w)*2+nbV_FB+nbV_M*((i%sqrtW)/w-1):(i%w+1)*(w+1))];
				cube->poly[i].vertex[2]=&cube->vertex[(1+w+(((i%sqrtW)/w==w-1)?(i%w)*(w+1):(i%w)*2)+nbV_FB+nbV_M*((i%sqrtW)/w))];
				cube->poly[i].vertex[3]=&cube->vertex[((i%w?w+1:0)+((i%w)?(((i%sqrtW)/w==w-1)?(i%w-1)*(w+1):(i%w-1)*2):0)\
						+nbV_FB+nbV_M*((i%sqrtW)/w))];
				cube->poly[i].vertexOrder=CLOCKWISE;
				break;
			case 3:
				cube->poly[i].vertex[0]=&cube->vertex[(((i%sqrtW)/w)?w+i%w*2+nbV_FB+nbV_M*((i%sqrtW)/w-1):(i%w+1)*w+i%w)];
				cube->poly[i].vertex[1]=&cube->vertex[(((i%sqrtW)/w)?w+(i%w+1)*2+(i%w==w-1?w-1:0)+\
						nbV_FB+nbV_M*((i%sqrtW)/w-1):(i%w+1)*(w+1)+w)];
				cube->poly[i].vertex[2]=&cube->vertex[w+(((i%sqrtW)/w==w-1?(i%w+1)*(w+1):(i%w+1)*2+(i%w==w-1?w-1:0))+\
						nbV_FB+nbV_M*((i%sqrtW)/w))];
				cube->poly[i].vertex[3]=&cube->vertex[w+((i%sqrtW)/w==w-1?(i%w)*(w+1):(i%w)*2)+nbV_FB+nbV_M*((i%sqrtW)/w)];
				cube->poly[i].vertexOrder=COUNTERCLOCKWISE;
				break;
			case 4:
				cube->poly[i].vertex[0]=&cube->vertex[i%w+nbV_FB+nbV_M*((i%sqrtW)/w)-w-1];
				cube->poly[i].vertex[1]=&cube->vertex[i%w+nbV_FB+nbV_M*((i%sqrtW)/w)-w];
				cube->poly[i].vertex[2]=&cube->vertex[i%w+nbV_FB+nbV_M*((i%sqrtW)/w)+((i%sqrtW)/w==w-1?nbV_FB:nbV_M)-w];
				cube->poly[i].vertex[3]=&cube->vertex[i%w+nbV_FB+nbV_M*((i%sqrtW)/w)+((i%sqrtW)/w==w-1?nbV_FB:nbV_M)-w-1];
				cube->poly[i].vertexOrder=CLOCKWISE;
				break;
			case 5:
				cube->poly[i].vertex[0]=&cube->vertex[i%(sqrtW)+(i%sqrtW)/w+nbV_FB+nbV_M*(w-1)];
				cube->poly[i].vertex[1]=&cube->vertex[i%(sqrtW)+(i%sqrtW)/w+1+nbV_FB+nbV_M*(w-1)];
				cube->poly[i].vertex[2]=&cube->vertex[i%(sqrtW)+(i%sqrtW)/w+w+2+nbV_FB+nbV_M*(w-1)];
				cube->poly[i].vertex[3]=&cube->vertex[i%(sqrtW)+(i%sqrtW)/w+w+1+nbV_FB+nbV_M*(w-1)];
				cube->poly[i].vertexOrder=COUNTERCLOCKWISE;
				break;
		}
	}
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
