#include "pyramix.h"

int getFV(int face, int w, int v);
int getVH(int w, int *v);
int sigma(int w, int a);

void pyramix3Dinit(t_form *form, t_rubik *rubik)
{
	double matrix[4][4];
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
	pyramix3Dcreate(form, rubik);
	rubik->r_3D.b_3DInit=TRUE;
	rubik->r_3D.center=(t_vertex3D){.x=0.5, .y=(sqrt(3)/6), .z=sqrt(2.0/3)/3};
	rubik->r_3D.cam=(t_camera){.pos=(t_vertex3D){.x=0.5, .y=0.5, .z=0}, .targetVector=(t_vertex3D){.x=0, .y=0, .z=1}, .angle=0};
	t_vertex3D min={.x=rubik->surface->w/10, .y=rubik->surface->h/10, .z=10.0};
	t_vertex3D max={.x=rubik->surface->w-rubik->surface->w/10, .y=rubik->surface->h-rubik->surface->h/10, .z=1000};
	defineMatrice(matrix, 3.14/2, 1.0, 0.0, 0.0);
	rotation(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
	set_perspective(0);
	set_min(-1, -1, 0);
	set_max(2, 2, 2);
	set_screen(rubik->surface);
	t_fitSpaceToScreen(1);
}

void pyramix3Dcreate(t_form *pyramix, t_rubik *rubik)
{
	int i;
	double min=0.0, max=1.0;
	int w=rubik->face->w;
	int x=0, y=0, z=0;
	int facePoly = calc_nbPolyFace(w);
	double Hz=sqrt(2.0/3), Hy=sqrt(3.0)/2.0, My=sqrt(3.0)/6.0;
	pyramix->nbPoly=4*facePoly;
	pyramix->poly=malloc(sizeof(t_formPoly3D)*pyramix->nbPoly);
	pyramix->nbVertex=sigma(w, 3) + sigma(w-2, 1) + 1;
	pyramix->vertex=malloc(sizeof(t_vertex3D)*pyramix->nbVertex);
	printf("nbV = %d\n", pyramix->nbVertex);
	for(i=0; i<pyramix->nbVertex; i++, x++)
	{
		if(((y==0 || z==0) && x>(w-(z+y))) || (y>0 && z>0 && x>1))
		{
			y++;
			x=0;
		}
		if((y==(w-z) && x>0) || y>(w-z))
		{
			z++;
			y=0;
			x=0;
		}
		if(y>0 && y<w && z>0 && z<w)
			pyramix->vertex[i].x=(1.0/w) * (x? ((double)w - (y/2.0 + z/2.0)) : (y/2.0 + z/2.0));
		else
			pyramix->vertex[i].x=(1.0/w)*((double)x + y/2.0 + z/2.0);
		pyramix->vertex[i].y=(Hy/w)*y + (My/w)*z;
		pyramix->vertex[i].z=(Hz/w)*z;
		printf("[%d](%lf, %lf, %lf)\n", i, pyramix->vertex[i].x*10, pyramix->vertex[i].y*10, pyramix->vertex[i].z*10);

	}
	int i2;
	int cw;
	int ai2;
	for(i=0, i2=0; i<pyramix->nbPoly; i++, i2++)
	{
		if(i%facePoly == 0)
		{
			i2 = 0;
			cw = w;
			ai2 = 0;
		}
		pyramix->poly[i].nbVertex=3;
		pyramix->poly[i].vertex=malloc(sizeof(t_vertex3D**)*3);
		pyramix->poly[i].texture_type=COLOR;
		pyramix->poly[i].doubleNormal=0;
		if(i/facePoly > 3)
		{
			pyramix->poly[i].vertex[0]=&pyramix->vertex[0];
			pyramix->poly[i].vertex[1]=&pyramix->vertex[1];
			pyramix->poly[i].vertex[2]=&pyramix->vertex[2];
		}
		else
		{
			int iw = cw*2-1;
			if(i2 >= iw)
			{
				i2 = 0;
				ai2 += cw + 1;
				cw--;
			}
			if(i2%iw >= cw)
			{
				printf("%d (%d): %d, %d\n", facePoly, i2, iw, w);
				pyramix->poly[i].vertex[0]=pyramix->poly[i-cw].vertex[2];
				pyramix->poly[i].vertex[1]=pyramix->poly[i-cw].vertex[1];
				pyramix->poly[i].vertex[2]=pyramix->poly[(i-cw)+1].vertex[2];
				if((i/facePoly)%2 == 0)
					pyramix->poly[i].vertexOrder=CLOCKWISE;
				else
					pyramix->poly[i].vertexOrder=COUNTERCLOCKWISE;
			}
			else
			{
				pyramix->poly[i].vertex[0]=&pyramix->vertex[getFV(i/facePoly, w, ai2 + i2)];
				pyramix->poly[i].vertex[1]=&pyramix->vertex[getFV(i/facePoly, w, ai2 + i2+1)];
				pyramix->poly[i].vertex[2]=&pyramix->vertex[getFV(i/facePoly, w, ai2 + i2+cw+1)];
				if((i/facePoly)%2 == 0)
					pyramix->poly[i].vertexOrder=CLOCKWISE;
				else
					pyramix->poly[i].vertexOrder=COUNTERCLOCKWISE;
			}
		}
	}
}

int getFV(int face, int w, int v)
{
	if(face == 0)
		return v;
	int fV = sigma(w+1, 1); //first face vertex
	int vH = getVH(w+1, &v);
	int v2=0;

	printf("getFV: face(%d) v(%d) vH(%d)\n", face, v, vH);
	if(vH == 0)
	{
		if(face == 1)
			return v;
		else
		{
			int i;
			v2=0;
			for(i=0; i<v; i++)
			{
				v2+=(w+1)-i;
			}
			return ((face==2) ? v2 : v2+(w-v));
			//return v2;
		}
	}
	v2=fV;
	int h;
	for(h=1; h<vH; h++)
	{
		v2+=(w-h)*3;
	}
	if(face > 1)
	{
		if(v>0)
			v2+=(w+1)-vH;
		for(h=1; h<v; h++)
		{
			v2+=2;
		}
		if(face == 3 && v < w-vH && v)
			v2+=1;
		else if(face == 3 && !v)
		{
			v2+=w-vH;
		}
	}
	//v += ((face-1)*(w-vH));
	printf("%d\n", v2);
	return (face == 1 ? v2+v : v2);
}

int getVH(int w, int *v)
{
	int h;
	for(h=-1; *v>=0; h++)
	{
		*v-=w;
		w--;
	}
	*v+=w+1;
	return h;
}

int calc_nbPolyFace(int w)
{
	int i;
	int n=1, r=1;
	for(i = 1; i < w; i++)
	{
		r+=2;
		n+=r;
	}
	return n;
}

int sigma(int w, int a)
{
	int i, r=0;
	for(i=0; i<w; i++)
	{
		r+=(i+1)*a;
	}
	return r;
}
