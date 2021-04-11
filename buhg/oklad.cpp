/* $Id: oklad.c,v 5.19 2013/08/13 05:49:49 sasa Exp $ */
/*20.10.2020	01.08.2000	Белых А.И.	oklad.c
Найти в файле оклад на нужный табельный номер
*/
#include        <errno.h>
#include "buhg.h"


double oklad(int tabn,short mr,short gr,int *has)
/******************
      has=
        0 - оплата за месяц расчётная по отработанным дням
        1 - оплата за месяц расчётная по отработанным часам
        2 - оплата за месяц постоянная
        3 - оплата за час
        4 - оплата за день
        5 - оплата по тарифу (минимальная зарплата умноженная на тарифный коэффициент)
******************/

{
char strsql[512];
double oklad=0.;
class iceb_tu_str bros("");
SQL_str row_alx;
class SQLCURSOR cur_alx;
*has=0;


sprintf(strsql,"select sm,mt from Zarsdo where tn=%d and dt <= '%04d-%02d-01'  order by dt desc limit 1",tabn,gr,mr);
if(readkey(strsql,&row_alx,&cur_alx) != 1)
 return(0.);

oklad=atof(row_alx[0]);

*has=atoi(row_alx[1]);

if(*has == 5 || *has == 6)
  oklad=okladtr(mr,gr,oklad);



return(oklad);
}
/*************************************************/

double oklad(int tabn,int *has) 
/******************
    has=
        0 - оплата за месяц расчётная по отработанным дням
        1 - оплата за месяц расчётная по отработанным часам
        2 - оплата за месяц постоянная
        3 - оплата за час
        4 - оплата за день
        5 - оплата по тарифу (минимальная зарплата умноженная на тарифный коэффициент)
******************/

{
char strsql[512];
double oklad=0.;
class iceb_tu_str bros("");
SQL_str row_alx;
class SQLCURSOR cur_alx;
*has=0;
short dr=0,mr=0,gr=0;
poltekdat(&dr,&mr,&gr);

sprintf(strsql,"select sm,mt from Zarsdo where tn=%d and dt <= '%04d-%02d-01' limit 1",tabn,gr,mr);
if(readkey(strsql,&row_alx,&cur_alx) != 1)
 return(0.);

oklad=atof(row_alx[0]);

*has=atoi(row_alx[1]);

if(*has == 5 || *has == 6)
  oklad=okladtr(mr,gr,oklad);
return(oklad);
}
