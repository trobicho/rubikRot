#include "display.h"

void drawRubik(t_rubik *rubik)
{
	int i;
	if(rubik->r_3D.b_3D)
		draw3dRubik(rubik);
	else
	{
		for(i=0; i<rubik->nbFace; i++)
		{
			drawFace(rubik, &rubik->face[i]);
		}
	}
}

void faceSelected(t_rubik *rubik, int faceN)
{
	if(rubik->r_3D.b_3D)
		faceSelected_3D(rubik, faceN);
	else
		faceSelected_2D(rubik, faceN);
}
