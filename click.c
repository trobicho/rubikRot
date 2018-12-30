#include "click.h"
#include "../lib/3Dlib/vector.h"
#include "../lib/3Dlib/projection.h"

int clickFace2D(t_rubik *rubik, int x, int y);
int clickFace3D(t_rubik *rubik, int x, int y);
bool dragClick2D(t_rubik *rubik, int faceN, t_vector3D vector, double margin);
bool dragClick3D(t_rubik *rubik, int faceN, t_vector3D vector, double margin);
int margeVect(t_rubik *rubik, t_vector3D v, int faceN, double margin, bool *invert);
bool inPoly3D(t_poly3D *poly, int x, int y, POINTORDER vOrder);
double deltaRL(t_vertex3D v1, t_vertex3D v2, t_vertex3D p);
t_vector3D vectDelZ(t_vector3D v);

int t_click(t_rubik *rubik, SDL_MouseButtonEvent click)
{
	if(rubik->r_3D.b_3D)
		return clickFace3D(rubik, click.x, click.y);
	else
		return clickFace2D(rubik, click.x, click.y);
}

bool dragClick(t_rubik *rubik, int faceN, t_vector3D vector, double margin)
{
	if(rubik->r_3D.b_3D)
		return dragClick3D(rubik, faceN, vector, margin);
	else
		return dragClick2D(rubik, faceN, vector, margin);
}

int clickFace2D(t_rubik *rubik, int x, int y)
{
	int i;
	for(i=0; i<rubik->nbFace; i++)
	{
		if((x>=rubik->face[i].pos.x && y>=rubik->face[i].pos.y)\
				&&(x<rubik->face[i].pos.x+rubik->face[i].pos.w && y<rubik->face[i].pos.y+rubik->face[i].pos.h))
		{
			x-=rubik->face[i].pos.x;
			y-=rubik->face[i].pos.y;
			x/=rubik->face[i].pos.w/rubik->face[i].w;
			y/=rubik->face[i].pos.h/rubik->face[i].h;
			return (i*rubik->face[i].w*rubik->face[i].h)+(x+y*rubik->face[i].w);
		}
	}
	return -1;
}

int clickFace3D(t_rubik *rubik, int x, int y)
{
	int i;
	t_poly3D poly;
	bool valid;
	for(i=0; i<rubik->r_3D.form.nbPoly; i++)
	{
		poly = projecting_formPoly3D(&rubik->r_3D.form.poly[i], &rubik->r_3D.cam);
		if(inPoly3D(&poly, x, y, rubik->r_3D.form.poly[i].vertexOrder))
			valid=1;
		free(poly.vertex);
		if(valid)
			return i;
	}
	return -1;
}

bool dragClick2D(t_rubik *rubik, int faceN, t_vector3D vector, double margin)
{
	int crown, w=rubik->face->w, f=faceN/(w*w);
	e_axeRot axe;
	bool rotInvert;

	if(1.0-fabs(vector.x)<=margin)
	{
		axe=Y;
		crown=(faceN/w)%w;
		if(vector.x<0.0)
			rotInvert=1;
	}
	else if(1.0-fabs(vector.y)<=margin)
	{
		axe=X;
		crown=faceN%w;
		if(vector.y<0.0)
			rotInvert=1;
	}
	else
		return 0;
	switch(f)
	{
		case BOTTOM:
			rotInvert=1-rotInvert;
		case TOP:
			if(axe==Y)
				axe=Z;
			else if(axe==X && f==TOP)
				rotInvert=1-rotInvert;
			if(axe==Z && f==TOP)
				crown=w-crown-1;
			break;
		case LEFT:
			rotInvert=1-rotInvert;
			if(axe==X)
				crown=w-crown-1;
		case RIGHT:
			if(axe==X)
				axe=Z;
			else if(axe==Y && f==RIGHT)
				rotInvert=1-rotInvert;
			break;
		case BACK:
			if(axe==Y)
				crown=w-crown-1;
		case FRONT:
			if(axe==X || f==FRONT)
				rotInvert=1-rotInvert;
			break;
	}
	rot(rubik, crown+axe*w, 1+rotInvert*2);
	return 1;
}

bool dragClick3D(t_rubik *rubik, int faceN, t_vector3D vector, double margin)
{
	int crown, w=rubik->face->w, f=faceN/(w*w);
	bool invert;
	e_axeRot axe;

	if((axe=margeVect(rubik, vector, f, margin, &invert))==-1)
		return 0;
	if(axe==Y || axe==Z)
		crown=(faceN/w)%w;
	else if(axe==X)
		crown=faceN%w;
	switch(f)
	{
		case FRONT:
			invert=1-invert;
			break;
		case RIGHT:
		case LEFT:
			if(axe==Y)
			{
				if(f==RIGHT)
					invert=1-invert;
				crown=faceN%w;
			}
			else if(f==LEFT)
				invert=1-invert;
			break;
		case BOTTOM:
			invert=1-invert;
			break;
	}
	rot(rubik, crown+axe*w, 1+invert*2);
	return 1;
}

int margeVect(t_rubik *rubik, t_vector3D v, int faceN, double margin, bool *invert)
{
	t_triAxe triAxe={
		.x=vectDelZ(rubik->r_3D.triAxe.x), 
		.y=vectDelZ(rubik->r_3D.triAxe.y), 
		.z=vectDelZ(rubik->r_3D.triAxe.z)
	};
	*invert=FALSE;
	switch(faceN)
	{
		case BACK:
		case FRONT:
			if(fabs(angleVector(v, triAxe.x))<=margin)
				return Y;
			else if(fabs(angleVector(v, triAxe.y))<=margin)
				return X;
			else if(fabs(angleVector(vectorInvert(v), triAxe.x))<=margin)
			{
				*invert=TRUE;
				return Y;
			}
			else if(fabs(angleVector(vectorInvert(v), triAxe.y))<=margin)
			{
				*invert=TRUE;
				return X;
			}
			break;
		case RIGHT:
		case LEFT:
			if(fabs(angleVector(v, triAxe.z))<=margin)
				return Y;
			else if(fabs(angleVector(v, triAxe.y))<=margin)
				return Z;
			else if(fabs(angleVector(vectorInvert(v), triAxe.z))<=margin)
			{
				*invert=TRUE;
				return Y;
			}
			else if(fabs(angleVector(vectorInvert(v), triAxe.y))<=margin)
			{
				*invert=TRUE;
				return Z;
			}
			break;
		case TOP:
		case BOTTOM:
			if(fabs(angleVector(v, triAxe.x))<=margin)
				return Z;
			if(fabs(angleVector(v, triAxe.z))<=margin)
				return X;
			else if(fabs(angleVector(vectorInvert(v), triAxe.x))<=margin)
			{
				*invert=TRUE;
				return Z;
			}
			else if(fabs(angleVector(vectorInvert(v), triAxe.z))<=margin)
			{
				*invert=TRUE;
				return X;
			}
			break;
	}
	return -1;
}

bool inPoly3D(t_poly3D *poly, int x, int y, POINTORDER vOrder)
{
	int i;
	t_vertex3D p={.x=(double)x, .y=(double)y};
	if(!poly->nbVertex)
		return 0;
	for(i=0; i<poly->nbVertex; i++)
	{
		if(vOrder == CLOCKWISE && deltaRL(poly->vertex[i], poly->vertex[((i+1==poly->nbVertex)?0:i+1)], p)<0)
			return 0;
		else if(vOrder!=CLOCKWISE && deltaRL(poly->vertex[i], poly->vertex[((i+1==poly->nbVertex)?0:i+1)], p)>0)
			return 0;
	}
	return 1;
}

double deltaRL(t_vertex3D v1, t_vertex3D v2, t_vertex3D p)
{
	return (v2.x-v1.x)*(p.y-v1.y)-(v2.y-v1.y)*(p.x-v1.x);
}

t_vector3D vectDelZ(t_vector3D v)
{
	if(v.x==0.0 && v.y==0.0)
		v.z=0.0;
	else
	{
		v.z=0.0;
		v=normalize(v);
	}
	return v;
}
