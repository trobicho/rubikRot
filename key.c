#include "key.h"

t_key keyTab[30];
int keyTabLen=30;
int linkTab_alpha[26];
int linkSymMin, linkSymMax;

void initKeyTab_uk(void);
void initKeyTab_fr(void);
void keyTabLink(void);

int keyRot(t_rubik *rubik, char key, int rotV, bool ctrl)
{
	int i, crown, value=-1;
	e_axeRot axe;
	if((key>='a' && key<='z')||(key>='A' && key<='Z'))
	{
		if(key>='A' && key<='Z')
			key-='A';
		else
			key-='a';

		value=keyTab[linkTab_alpha[key]].value;
		axe=keyTab[linkTab_alpha[key]].axe;
	}
	else if((key>=32 && key<=47)||(key>=58 && key<=64)\
			||(key>=91 && key<=96)||(key>=123 && key<=126))
	{
		for(i=linkSymMin; i<=linkSymMax; i++)
		{
			if(key==keyTab[i].key)
			{
				value=keyTab[i].value;
				axe=keyTab[i].axe;
				break;
			}
		}
	}
	if(value<0 || value>=rubik->face[0].w)
		return -1;
	if(ctrl)
	{
		for(i=0; i<rubik->face->w; i++)
		{
			rot(rubik, i+axe*rubik->face->w, rotV);
		}
	}
	else
	{
		crown=value+axe*rubik->face[0].w;
		rot(rubik, crown, rotV);
	}
	return 0;
}
	
void initKeyTab(char *keyMap)
{
	if(strcmp(keyMap, "uk")==0)
		initKeyTab_uk();
	else if(strcmp(keyMap, "fr")==0)
		initKeyTab_fr();
	keyTabLink();
}

void keyTabLink(void)
{
	int i, c;
	bool symMin=FALSE, symMax=FALSE;
	for(i=0; i<keyTabLen; i++)
	{
		if((keyTab[i].key>=32 && keyTab[i].key<=47)\
				||(keyTab[i].key>=58 && keyTab[i].key<=64)\
				||(keyTab[i].key>=91 && keyTab[i].key<=96)\
				||(keyTab[i].key>=123 && keyTab[i].key<=126))
		{
			if(!symMin)
			{
				linkSymMin=i;
				symMin=TRUE;
			}
			else if(symMax)
				printf("multi plage of symbole\n");
		}
		else 
		{
			if(keyTab[i].key>=97 && keyTab[i].key<=122)
				linkTab_alpha[keyTab[i].key-97]=i;
			if(symMin && !symMax)
			{
				linkSymMax=i-1;
				symMax=TRUE;
			}
		}
	}
	if(symMin && !symMax)
		linkSymMax=keyTabLen-1;
}

void initKeyTab_uk(void)
{
	//axe X
	keyTab[0]=(t_key){.key='q', .value=0, .axe=X};
	keyTab[1]=(t_key){.key='w', .value=1, .axe=X};
	keyTab[2]=(t_key){.key='e', .value=2, .axe=X};
	keyTab[3]=(t_key){.key='r', .value=3, .axe=X};
	keyTab[4]=(t_key){.key='t', .value=4, .axe=X};
	keyTab[5]=(t_key){.key='y', .value=5, .axe=X};
	keyTab[6]=(t_key){.key='u', .value=6, .axe=X};
	keyTab[7]=(t_key){.key='i', .value=7, .axe=X};
	keyTab[8]=(t_key){.key='o', .value=8, .axe=X};
	keyTab[9]=(t_key){.key='p', .value=9, .axe=X};
	//axe Y
	keyTab[10]=(t_key){.key='a', .value=0, .axe=Y};
	keyTab[11]=(t_key){.key='s', .value=1, .axe=Y};
	keyTab[12]=(t_key){.key='d', .value=2, .axe=Y};
	keyTab[13]=(t_key){.key='f', .value=3, .axe=Y};
	keyTab[14]=(t_key){.key='g', .value=4, .axe=Y};
	keyTab[15]=(t_key){.key='h', .value=5, .axe=Y};
	keyTab[16]=(t_key){.key='j', .value=6, .axe=Y};
	keyTab[17]=(t_key){.key='k', .value=7, .axe=Y};
	keyTab[18]=(t_key){.key='l', .value=8, .axe=Y};
	keyTab[26]=(t_key){.key=';', .value=9, .axe=Y};
	//axe Z
	keyTab[19]=(t_key){.key='z', .value=0, .axe=Z};
	keyTab[20]=(t_key){.key='x', .value=1, .axe=Z};
	keyTab[21]=(t_key){.key='c', .value=2, .axe=Z};
	keyTab[22]=(t_key){.key='v', .value=3, .axe=Z};
	keyTab[23]=(t_key){.key='b', .value=4, .axe=Z};
	keyTab[24]=(t_key){.key='n', .value=5, .axe=Z};
	keyTab[25]=(t_key){.key='m', .value=6, .axe=Z};
	keyTab[27]=(t_key){.key=',', .value=7, .axe=Z};
	keyTab[28]=(t_key){.key='.', .value=8, .axe=Z};
	keyTab[29]=(t_key){.key='/', .value=9, .axe=Z};
}

void initKeyTab_fr(void)
{
	//axe X
	keyTab[0]=(t_key){.key='a', .value=0, .axe=X};
	keyTab[1]=(t_key){.key='z', .value=1, .axe=X};
	keyTab[2]=(t_key){.key='e', .value=2, .axe=X};
	keyTab[3]=(t_key){.key='r', .value=3, .axe=X};
	keyTab[4]=(t_key){.key='t', .value=4, .axe=X};
	keyTab[5]=(t_key){.key='y', .value=5, .axe=X};
	keyTab[6]=(t_key){.key='u', .value=6, .axe=X};
	keyTab[7]=(t_key){.key='i', .value=7, .axe=X};
	keyTab[8]=(t_key){.key='o', .value=8, .axe=X};
	keyTab[9]=(t_key){.key='p', .value=9, .axe=X};
	//axe Y
	keyTab[10]=(t_key){.key='q', .value=0, .axe=Y};
	keyTab[11]=(t_key){.key='s', .value=1, .axe=Y};
	keyTab[12]=(t_key){.key='d', .value=2, .axe=Y};
	keyTab[13]=(t_key){.key='f', .value=3, .axe=Y};
	keyTab[14]=(t_key){.key='g', .value=4, .axe=Y};
	keyTab[15]=(t_key){.key='h', .value=5, .axe=Y};
	keyTab[16]=(t_key){.key='j', .value=6, .axe=Y};
	keyTab[17]=(t_key){.key='k', .value=7, .axe=Y};
	keyTab[18]=(t_key){.key='l', .value=8, .axe=Y};
	keyTab[26]=(t_key){.key='m', .value=9, .axe=Y};
	//axe Z
	keyTab[19]=(t_key){.key='w', .value=0, .axe=Z};
	keyTab[20]=(t_key){.key='x', .value=1, .axe=Z};
	keyTab[21]=(t_key){.key='c', .value=2, .axe=Z};
	keyTab[22]=(t_key){.key='v', .value=3, .axe=Z};
	keyTab[23]=(t_key){.key='b', .value=4, .axe=Z};
	keyTab[24]=(t_key){.key='n', .value=5, .axe=Z};
	keyTab[25]=(t_key){.key=',', .value=6, .axe=Z};
	keyTab[27]=(t_key){.key=';', .value=7, .axe=Z};
	keyTab[28]=(t_key){.key=':', .value=8, .axe=Z};
	keyTab[29]=(t_key){.key='!', .value=9, .axe=Z};
}
