#include "load.h"

int lsearch_c(char *line, int i, char c, int len);
int lget_n(char *line, int i, int len);
int lget_h(char *line, int i, int len);

int load_color(t_rubik *rubik, char *fileWay)
{
	FILE *file;
	char line[1000];
	int c, n;
	if((file=fopen(fileWay, "r"))==NULL)
		return 0;
	while(fgets(line, 1000, file)!=NULL)
	{
		if(line[0]!='#')
		{
			if(!strncmp(line, "NB_COLOR", 8) && !rubik->nbColor)
			{
				rubik->nbColor=lget_n(line, 8, strlen(line));
				rubik->color=malloc(sizeof(Uint32)*rubik->nbColor);
			}
			else if(!strncmp(line, "COLOR", 5))
			{
				if((c=lsearch_c(line, 5, '_', strlen(line)))!=-1)
				{
					n=lget_n(line, c+1, strlen(line));
					if(n<rubik->nbColor && n>=0 && (c=lsearch_c(line, 5, '#', strlen(line)))!=-1)
					{
						rubik->color[n]=lget_h(line, c+1, strlen(line));
					}
				}
			}
			else if(!strncmp(line, "SCREEN_COLOR", 5))
			{
				if((c=lsearch_c(line, 12, '#', strlen(line)))!=-1)
				{
					rubik->display.backgroundColor=lget_h(line, c+1, strlen(line));
				}
			}
		}
	}
	fclose(file);
	return 0;
}

int lsearch_c(char *line, int i, char c, int len)
{
	for(; i<len && line[i]!=c; i++);
	if(i>=len)
		return -1;
	return i;
}

int lget_n(char *line, int i, int len)
{
	int n=0, teste=0;
	for(; i<len&&(line[i]=='\t' || line[i]==' '); i++);
	for(; line[i]>='0' && line[i]<='9'; i++)
	{
		teste=1;
		n=n*10+line[i]-'0';
	}	
	if(!teste)
		return -1;
	return n;
}

int lget_h(char *line, int i, int len)
{
	int n=0, teste=0;
	for(; i<len&&(line[i]=='\t' || line[i]==' '); i++);
	for(;; i++)
	{
		if(line[i]>='0' && line[i]<='9')
			n=n*16+line[i]-'0';
		else if(line[i]>='A' && line[i]<='F')
			n=n*16+line[i]-('A'-10);
		else if(line[i]>='a' && line[i]<='f')
			n=n*16+line[i]-('a'-10);
		else
			break;
		teste=1;
	}
	if(!teste)
		return -1;
	return n;
}
