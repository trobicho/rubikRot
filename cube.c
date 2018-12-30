#include "cube.h"

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
