/*$Id:$*/
/*03.04.2020	15.09.2019	Белых А.И.	zarkmd_m.cpp
Меню запуска расчёта командировочных расходов
*/

#include        "buhg.h"

extern FILE *zar_pr_ffp;

int zarkmd_m(class spis_oth *oth)
{
static class iceb_tu_str datr("");
static class iceb_tu_str tabn("");
static class iceb_tu_str kodp("");
short mr=0,gr=0;
int kom=0,kom1=0;
int tabnom=0;

class VVOD MENU(3);
class VVOD VV(0);
VV.VVOD_SPISOK_add_ZAG(gettext("Расчёт командировочных расходов"));

VV.VVOD_SPISOK_add_MD(gettext("Дата расчёта......(м.г)."));
VV.VVOD_SPISOK_add_MD(gettext("Табельный номер....(,,)."));
VV.VVOD_SPISOK_add_MD(gettext("Код подразделения..(,,)."));

VV.VVOD_SPISOK_add_data(datr.ravno(),8);
VV.VVOD_SPISOK_add_data(tabn.ravno(),128);
VV.VVOD_SPISOK_add_data(kodp.ravno(),128);

naz:;

clear();
 
helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("расчитать"),
"F3",gettext("реквизиты"),
"F4",gettext("очистить"),
"F10",gettext("выход"),NULL);

kom=VV.vvod(0,1,1,-1,-1);

datr.new_plus(VV.VVOD_SPISOK_return_data(0));
tabn.new_plus(VV.VVOD_SPISOK_return_data(1));
kodp.new_plus(VV.VVOD_SPISOK_return_data(2));

class iceb_tu_str kod("");
class iceb_tu_str naim("");

switch(kom)
 {
  case FK1:   /*Помощь*/
   GDITE();
//   iceb_t_pdoc("buhg5_1.txt");
   goto naz;

  case ESC:
  case FK10:
   return(1);


  case FK2:
  case PLU:
     break;


  case FK3:
    MENU.VVOD_delete();

    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка табельных номеров"));
    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка кодов подразделений"));
    MENU.VVOD_SPISOK_add_MD(gettext("Выход"));

    clearstr(LINES-1,0);
    
    kom1=0;
    while(menu3w(stdscr,&MENU,&kom1,-1,-1,0) != 0);

    switch (kom1)
     {
      case 2 :
      case -1:
        goto naz;
    
      case 0 :
        if(dirtab(&tabnom,&naim,0,1) == 0)
          VV.data_z_plus(1,tabnom);
        break;

      case 1 :
        if(dirzarpodr(1,&kod,&naim) == 0)
           VV.data_z_plus(2,kod.ravno());
                 
        break;
     }
    goto naz;

  case FK4: /*Очистить меню*/
    VV.VVOD_clear_data();  
    goto naz;


  default:
     goto naz;
 }


if(rsdat1(&mr,&gr,datr.ravno()) != 0)
 goto naz;

char strsql[1024];

char imaf[64];
sprintf(imaf,"rkr%d.csv",getpid());

class iceb_fopen fil_kr;
if(fil_kr.start(imaf,"w") != 0)
 return(1);
 
char imaf_prot[64];
sprintf(imaf_prot,"prkr%d.lst",getpid());
SQL_str row;
class SQLCURSOR cur;

int kolstr=0;
zar_pr_start(imaf_prot); /*открытие файла для протокола хода расчёта*/



sprintf(strsql,"select tabn,fio,podr from Kartb where datk='0000-00-00' order by tabn asc");
if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
  iceb_t_soob(gettext("Нет ни одной карточки работника!"));
  return(1);
 }

iceb_t_zagolov("Расчёт командировочных расходов",0,0,0,0,mr,gr,fil_kr.ff);
iceb_t_zagolov("Протокол хода расчёта командировочных расходов",0,0,0,0,mr,gr,zar_pr_ffp);

if(tabn.getdlinna() > 1)
 fprintf(fil_kr.ff,"%s:%s\n",gettext("Табельный номер"),tabn.ravno());
if(kodp.getdlinna() > 1)
 fprintf(fil_kr.ff,"%s:%s\n",gettext("Код подразделения"),kodp.ravno());

//fprintf(fil_kr.ff,"------------------------------------------------------------\n");
fprintf(fil_kr.ff,"\
 Т.н. |Фамилия имя отчество          |За пред 2м|За тек. м.|\n");
//fprintf(fil_kr.ff,"------------------------------------------------------------\n");
int kolstr1=0;
while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);
  if(proverka(tabn.ravno(),row[0],0,0) != 0)
   continue;
  if(proverka(kodp.ravno(),row[2],0,0) != 0)
   continue;

  sprintf(strsql,"\n%s %s\n",row[0],row[1]);
  zar_pr_ins(strsql);  

  zarkmd(atoi(row[0]),mr,gr,atoi(row[1]),"",1,fil_kr.ff);

 }

//fprintf(fil_kr.ff,"------------------------------------------------------------\n");
 
zar_pr_end(); /*закрытие файла для потокола хода расчёта*/
podpis(fil_kr.ff);
fil_kr.end();

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus(gettext("Расчёт командировочных расходов"));

oth->spis_imaf.plus(imaf_prot);
oth->spis_naim.plus(gettext("Протокол хода расчёта"));

for(int nom=0; nom < oth->spis_imaf.kolih(); nom++)
  iceb_t_ustpeh(oth->spis_imaf.ravno(nom),3);

return(0);

}
