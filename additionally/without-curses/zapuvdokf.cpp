/* $Id: zapuvdok.c,v 5.21 2013/05/17 14:56:16 sasa Exp $ */
/*09.11.2008	19.04.2000	Белых А.И.	zapuvdok.c
Запис в документ услуг
Если вернули 0 -записали
             1 -нет
*/
#include        <time.h>
#include	"bezcurses.h"
#include <unistd.h>

extern double	okrcn;  /*Округление цены*/

int zapuvdok(int tp, //1-приход 2-расход
short dd,short md,short gd, //Дата документа
const char *nomd,
int metka,int kodzap,double kolih,double cena,const char *ei,const char *shetu,
int podr,
const char *shet_sp, //счёт списания/приобретения
const char *dop_naim) //дополнение к наименованию услуги
{
time_t 		vrem;
char		strsql[1024];

cena=okrug(cena,okrcn);
time(&vrem);
int nomzap=0;
for(;;)
 {
  sprintf(strsql,"select tp from Usldokum1 where datd='%04d-%02d-%02d' \
and podr=%d and tp=%d and nomd='%s' and metka=%d and kodzap=%d and \
nz=%d",gd,md,dd,podr,tp,nomd,metka,kodzap,nomzap);   
//printw("%s\n",strsql);
//refresh();
//    OSTANOV();

  if(readkey(strsql) >= 1)
   {
//printw("%s\n",strsql);
//refresh();
    
      nomzap++;
      continue;
//    OSTANOV();
   }   
  sprintf(strsql,"insert into Usldokum1 \
  values (%d,'%04d-%02d-%02d','%s',%d,%d,%.10g,%.10g,'%s','%s',%d,%d,%ld,'%s',%d,'%s')",
  tp,gd,md,dd,nomd,metka,kodzap,kolih,cena,ei,shetu,podr,iceb_t_getuid(),vrem,dop_naim,nomzap,shet_sp);
//printw("%s\n",strsql);
//refresh();
  if(sql_zap(&bd,strsql) != 0)
   {
    if(sql_nerror(&bd) == ER_DUP_ENTRY) //Запись уже есть
     {
      printf("%s-%s\n",__FUNCTION__,gettext("Такая запись уже есть !"));
     }
    else
      msql_error(&bd,__FUNCTION__,strsql);
    return(1);
   }
  break;
 }
return(0);
}
