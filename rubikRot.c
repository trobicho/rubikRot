#include "rubikRot.h"

int rot(t_rubik *rubik, int crown, int rot)
{
	int i, a, v, s, b;

	for(b=0; b<rot; b++)
	{
		for(i=0; i<rubik->face[0].w; i++)
		{
			s=rubik->face[rubik->crown[crown].faceLine[0].face].tab[rubik->crown[crown].faceLine[0].line[i]];
			for(a=0; a<rubik->crown[crown].nbFace; a++)
			{
				v=s;
				s=rubik->face[rubik->crown[crown].faceLine[(a+1)%rubik->crown[crown].nbFace].face].tab\
				  [rubik->crown[crown].faceLine[(a+1)%rubik->crown[crown].nbFace].line[i]];
				rubik->face[rubik->crown[crown].faceLine[(a+1)%rubik->crown[crown].nbFace].face].tab\
					[rubik->crown[crown].faceLine[(a+1)%rubik->crown[crown].nbFace].line[i]]=v;
			}
		}
		if(rubik->crown[crown].faceAttached!=-1)
		{
			rotFace(&rubik->face[rubik->crown[crown].faceAttached], rubik->crown[crown].faceRotV);
		}
	}
}

void rotFace(r_face *face, int rot)
{
	int i, a, b, v, s, x, y, nbB=(face->w*face->h)/4;
	int tab[face->w*face->h];

	for(i=0; i<rot; i++)
	{
		for(a=0; a<face->nbCase; a++)
			tab[a]=face->tab[a];
		for(a=0; a<face->nbCase; a++)
		{
			face->tab[a]=tab[(a%face->w)*face->w+(face->w-(a/face->w+1))];
		}
	}
}
