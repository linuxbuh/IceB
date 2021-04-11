/*$Id:$*/
/*15.04.2020	15.04.2020	Белых А.И.	vos_rka.cpp
Расчёт коэффициента амортизации
*/
#include "buhg.h"
#include "bsizt.h"

void vos_rka(long innom,short dr,short mr,short gr,const char *hnabu)
{
char strsql[1024];
SQL_str row;
class SQLCURSOR cur;
class iceb_tu_str bros("");

class VVOD PAR(2);

clear();

printw("%s %ld %s\n",gettext("Расчёт коэффициента амортизации"),innom,hnabu);
helstr(LINES-1,0," F10 ",gettext("выход"),NULL);

PAR.VVOD_delete();
PAR.VVOD_SPISOK_add_MD(gettext("Введите количество месяцев амортизации"));

if(vvod1(&bros,16,&PAR,NULL,stdscr,-1,-1) == FK10)
 return;

double kol_mes_amort_plan=bros.ravno_atof();
if(kol_mes_amort_plan == 0.)
  return;

int podr=0,kodotl=0;
int kod=0;
if((kod=poiin(innom,dr,mr,gr,&podr,&kodotl)) == 2)
 {
  iceb_t_soob(gettext("Уже списано!"));  
  return;
 }

clear();


class bsizw_data bal_st;

bsiz(innom,podr,dr,mr,gr,&bal_st,NULL);

int kol_mes_amort=0;


/*Узнаём количество месяцев уже выполненной амортизации*/

sprintf(strsql,"select suma from Uosamor1 where innom=%ld order by da desc",innom);
if((kol_mes_amort=readkey(strsql,&row,&cur)) <= 0)
 {
  iceb_t_soob(gettext("Не найдено записей амортизации"));
  return;
 }


double kol_ost_mes_amort=kol_mes_amort_plan-kol_mes_amort; /*Оставшееся количество месяцев амортизации*/
double suma_amort=bal_st.osbu/kol_ost_mes_amort;

/*Узнаём сумму амортизации без коэффициета*/
sprintf(strsql,"select kof from Uosgrup1 where kod='%s'",hnabu);
if(readkey(strsql,&row,&cur) != 1)
 {
  iceb_t_soob(gettext("Не найдена группа амортизации!"));
  return;
 }
double kof=atof(row[0]);
double suma_po_grup=bal_st.kbsbu*kof/100./12.;

double kof_amort=suma_amort/suma_po_grup;


printw("%s на %d.%d:%.2f\n",gettext("Балансовая стоимость на"),mr,gr,bal_st.kbsbu);
printw("%s %d.%d:%.2f\n",gettext("Остаточная балансовая стоимость на"),mr,gr,bal_st.osbu);
printw("%s:%.10g\n",gettext("Плановое количество месяцев амортизаци"),kol_mes_amort_plan);
printw("%s %s:%.10g\n",gettext("Процент амортизации по группе"),hnabu,kof);
printw("%s:%d\n",gettext("Количество месяцев уже выполненной амортизации"),kol_mes_amort);
printw("%s:%-.10g-%d=%-.10g\n",gettext("Количество месяцев оставшейся амортизации"),kol_mes_amort_plan,kol_mes_amort,kol_ost_mes_amort);
printw("%s:%.2f/%-.10g=%.2f\n",gettext("Cумма месячной амортизации на оставшиеся месяцы"),bal_st.osbu,kol_ost_mes_amort,suma_amort);
printw("%s %s:%.2f*%.10g/100/12=%-.10g\n",gettext("Месячная сумма амортизации по группе"),hnabu,bal_st.kbsbu,kof,suma_po_grup);
printw("%s:%-.10g/%-.10g=%-.10g\n",gettext("Коєффициент амортизации"),suma_amort,suma_po_grup,kof_amort);

OSTANOV();

}
