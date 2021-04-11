/* $Id: iceb_t_sozmas.c,v 5.3 2013/05/17 14:56:29 sasa Exp $ */
/*13.12.2010	13.12.2010	Белых А.И.	iceb_t_sozmas.c
Выделить память и зарядить данные
В нулевом елементе количество кодов
*/
#include "buhl.h"

void iceb_t_sozmas(short **kod,char *st1,class SQLCURSOR *cur)
{
short		i,i1;
class iceb_tu_str  str("");
char            bros[1024];

if(polen(st1,&str,2,'|') != 0)
  str.new_plus(st1);
class iceb_tu_str spisok_kodov("");

iceb_t_fplus(str.ravno(),&spisok_kodov,cur);

if(spisok_kodov.getdlinna() <= 1)
 return;  

i=pole2(spisok_kodov.ravno(),',');
if(i == 0 && spisok_kodov.getdlinna() > 1)
  i=1;

if((*kod=new short[i+1]) == NULL)
 {
  printw("%s-Не могу выделить память для kod !!!\n",__FUNCTION__);
  OSTANOV();
  return;
 }

(*kod)[0]=i;

for(i1=1; i1 < *kod[0]+1; i1++)
 {
  memset(bros,'\0',sizeof(bros));
  if(i == 1)
   strncpy(bros,spisok_kodov.ravno(),sizeof(bros)-1);
  else        
    polen(spisok_kodov.ravno(),bros,sizeof(bros),i1,',');
  (*kod)[i1]=(short)atoi(bros);
 }

}
