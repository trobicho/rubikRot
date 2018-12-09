#include "human.h"

bool t_humanVector(t_vector3D *vector, SDL_Rect pos1, SDL_Rect pos2, int distMin);

void t_human(t_rubik *rubik)
{
	SDL_Event event;
	srand(time(NULL));
	Uint8 *keystates = SDL_GetKeyState(NULL);
	int stop=0, pause=1, next=0, random=0;
	int nbAlgo=0, algoPas=1;
	int i;
	char key;
	bool repeat=0;

	SDL_EnableKeyRepeat(100, 10);
	while(!stop)
	{
		if(pause)
			SDL_WaitEvent(&event);
		else
			SDL_PollEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				stop=1;
				break;
			
			case SDL_VIDEORESIZE:
				rubik->surface=SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_RESIZABLE);
				SDL_FillRect(rubik->surface, NULL, rubik->display.backgroundColor);
				set_screen(rubik->surface);
				t_fitSpaceToScreen(0);
				centerRubik(rubik);
				drawRubik(rubik);
				SDL_Flip(rubik->surface);
				break;

			case SDL_MOUSEBUTTONDOWN:
				humanClick(rubik, &event);
				break;

			case SDL_KEYDOWN:
				if(event.key.keysym.sym==SDLK_ESCAPE)
					stop=1;
				else if(rubik->r_3D.b_3D && human3D(rubik, &event, keystates))
					break;
				else
					SDL_EnableKeyRepeat(100, 10);
				if(event.key.keysym.sym==SDLK_SPACE)
					pause=1-pause;
				else if(keystates[SDLK_LALT])
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_r:
							for(i=0; i<rubik->nbFace; i++)
							{
								faceInit(&rubik->face[i], i);
							}
							drawRubik(rubik);
							SDL_Flip(rubik->surface);
							break;

						case SDLK_s:
							pause=1;
							next=1;
							break;
						
						case SDLK_m:
							rubikMix(rubik, 21);
							drawRubik(rubik);
							SDL_Flip(rubik->surface);
							break;

						case SDLK_v:
							rubik->r_3D.b_3D=1-rubik->r_3D.b_3D;
							SDL_FillRect(rubik->surface, NULL, rubik->display.backgroundColor);
							drawRubik(rubik);
							SDL_Flip(rubik->surface);
							break;

						case SDLK_p:
							set_perspective(1-get_perspective());
							SDL_FillRect(rubik->surface, NULL, rubik->display.backgroundColor);
							drawRubik(rubik);
							SDL_Flip(rubik->surface);
							break;
						
						case SDLK_c:
							rubik->r_3D.b_color=1-rubik->r_3D.b_color;
							SDL_FillRect(rubik->surface, NULL, rubik->display.backgroundColor);
							drawRubik(rubik);
							SDL_Flip(rubik->surface);
							break;
					}
				}
				else
				{
					key=event.key.keysym.sym;
					if(keyRot(rubik, key, ((keystates[SDLK_LSHIFT])?3:1), keystates[SDLK_LCTRL])!=-1)
					{
						drawRubik(rubik);
						SDL_Flip(rubik->surface);
					}
				}
				break;
		}
		if(!pause || next)
		{
			if(random)
			{
				rot(rubik, rand()%(rubik->face[0].w*3-1), 1);
				drawRubik(rubik);
				SDL_Flip(rubik->surface);
				usleep(1000000);
			}
			else
			{
				nbAlgo++;
				if(nbAlgo%algoPas==0)
				{
					drawRubik(rubik);
					SDL_Flip(rubik->surface);
				}
				if(rubikSolve(rubik))
				{
					printf("end: %d\n", nbAlgo);
					nbAlgo=0;
					pause=1;
					drawRubik(rubik);
					SDL_Flip(rubik->surface);
				}
			}
			next=0;
		}
	}
}

void humanClick(t_rubik *rubik, SDL_Event *event)
{
	int faceN, distMin=20;
	bool b_click=FALSE;
	double vectMarg=0.2;
	SDL_Rect click, motion;
	t_vector3D vect;
	if(event->type==SDL_MOUSEBUTTONDOWN && event->button.button==SDL_BUTTON_LEFT)
	{
		if((faceN=t_click(rubik, event->button))!=-1)
		{
			faceSelected(rubik, faceN);
			click.x=event->button.x;
			click.y=event->button.y;
			b_click=TRUE;
			SDL_Flip(rubik->surface);
		}
	}
	while(b_click)
	{
		SDL_WaitEvent(event);
		switch(event->type)
		{
			case SDL_MOUSEMOTION:
				motion.x=event->button.x;
				motion.y=event->button.y;
				if(t_humanVector(&vect, click, motion, distMin) && dragClick(rubik, faceN, vect, vectMarg))
				{
					b_click=FALSE;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(event->button.button==SDL_BUTTON_LEFT)
				{
					b_click=FALSE;
				}
				break;
		}
	}
	drawRubik(rubik);
	SDL_Flip(rubik->surface);
}

bool human3D(t_rubik *rubik, SDL_Event *event, Uint8 *keystates)
{
	double matrix[4][4];
	bool redraw=0;
	switch(event->type)
	{
		case SDL_KEYDOWN:
			switch(event->key.keysym.sym)
			{
				case SDLK_UP:
					defineMatrice(matrix, -0.1, 1.0, 0.0, 0.0);
					rotation(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					rotationTriAxe(&rubik->r_3D.triAxe, matrix);
					redraw=1;
					break;
				case SDLK_DOWN:
					defineMatrice(matrix, 0.1, 1.0, 0.0, 0.0);
					rotation(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					rotationTriAxe(&rubik->r_3D.triAxe, matrix);
					redraw=1;
					break;
				case SDLK_LEFT:
					if(keystates[SDLK_LCTRL])
						defineMatrice(matrix, -0.1, 0.0, 0.0, 1.0);
					else
						defineMatrice(matrix, 0.1, 0.0, 1.0, 0.0);
					rotation(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					rotationTriAxe(&rubik->r_3D.triAxe, matrix);
					redraw=1;
					break;
				case SDLK_RIGHT:
					if(keystates[SDLK_LCTRL])
						defineMatrice(matrix, 0.1, 0.0, 0.0, 1.0);
					else
						defineMatrice(matrix, -0.1, 0.0, 1.0, 0.0);
					rotation(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					rotationTriAxe(&rubik->r_3D.triAxe, matrix);
					redraw=1;
					break;
				case SDLK_PLUS:
				case SDLK_KP_PLUS:
					defineMatrice(matrix, 90.0, 1.02, 1.02, 1.02);
					scale(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					redraw=1;
					break;
				case SDLK_MINUS:
				case SDLK_KP_MINUS:
					defineMatrice(matrix, 90.0, 0.98, 0.98, 0.98);
					scale(&rubik->r_3D.form, rubik->r_3D.center, matrix, 1);
					redraw=1;
					break;
				default:
					return 0;
			}
	}
	if(redraw)
	{
		SDL_FillRect(rubik->surface, NULL, rubik->display.backgroundColor);
		drawRubik(rubik);
		SDL_Flip(rubik->surface);
	}
	return 1;
	return 0;
}

bool t_humanVector(t_vector3D *vector, SDL_Rect pos1, SDL_Rect pos2, int distMin)
{
	int distX=pos2.x-pos1.x, distY=pos2.y-pos1.y;
	double dist=sqrt((double)distX*distX+distY*distY);
	if(dist>=distMin)
	{
		vector->x=distX/dist;
		vector->y=distY/dist;
		vector->z=0.0;
		return 1;
	}
	return 0;
}
