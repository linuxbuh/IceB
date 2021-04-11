/*$Id: ud_simv.c,v 5.7 2013/05/17 14:56:43 sasa Exp $*/
/*16.02.2009	05.09.2005	Белых А.И.	ud_simv.c
Удаление из строки всех символов которые указаны во второй строке
*/
#include <string.h>

void ud_simv(char *stroka,const char *simvol)
{
if(stroka[0] == '\0')
 return;
int dlinna=strlen(stroka);
char buff[dlinna+1];
strcpy(buff,stroka);
memset(stroka,'\0',dlinna+1);
int nomer=0;
int metka;
for(int i=0; i < dlinna ; i++)
 {
  metka=0;
  for(int kk=0; kk < (int)strlen(simvol); kk++)
   if(buff[i] == simvol[kk]) 
    {
     metka=1;
     break;
    }
  if(metka == 1)
   continue;
  stroka[nomer++]=buff[i];
 }
}
