/*$Id:$*/
/*08.02.2020	08.02.2020	Белых А.И.	zarspbol.cpp
Получение списка работников имеющих больничный
*/
#include        "buhg.h"

void zarspbol_hap(int nomstr,int *kolstr,FILE *ff);



int zarspbol(class spis_oth *oth)
{
int kom=0,kom1=0;
static class iceb_tu_str datan("");
static class iceb_tu_str datak("");
short mn=0,gn=0,mk=0,gk=0;

class VVOD MENU(3);
class VVOD VV(0);

VV.VVOD_SPISOK_add_data(datan.ravno(),8);
VV.VVOD_SPISOK_add_data(datak.ravno(),8);


VV.VVOD_SPISOK_add_ZAG(gettext("Введите даты для расчёта"));
VV.VVOD_SPISOK_add_MD(gettext("Дата начала.(м.г)..."));
VV.VVOD_SPISOK_add_MD(gettext("Дата конца..(м.г)..."));

naz:;


clear();



helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("расчёт"),
//"F3",gettext("таб.номер"),
"F4",gettext("очистить"),
"F5",gettext("настройка"),
"F10",gettext("выход"),NULL);



kom=VV.vvod(0,1,1,-1,-1);

datan.new_plus(VV.VVOD_SPISOK_return_data(0));
datak.new_plus(VV.VVOD_SPISOK_return_data(1));

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

  case FK4: /*Очистить меню*/
    VV.VVOD_clear_data();  
    goto naz;

  case FK5:
    MENU.VVOD_delete();

    MENU.VVOD_SPISOK_add_MD(gettext("Редактирование заголовка документа"));
    MENU.VVOD_SPISOK_add_MD(gettext("Редактирование концовки документа"));
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
        iceb_redfil("zarspbolz.alx",0);
        break;

      case 1 :
                 
        iceb_redfil("zarspbole.alx",0);
        break;
     }
    goto naz;

  default:
     goto naz;
 }


if(iceb_t_rsdatp1(&mn,&gn,datan.ravno(),&mk,&gk,datak.ravno()) != 0)
  goto naz;


class iceb_tu_str kodbol("");

if(iceb_t_poldan("Код больничного",&kodbol,"zarnast.alx",0) != 0)
 return(1);

char strsql[1024];
SQL_str row,row1;
class SQLCURSOR cur,cur1;

sprintf(strsql,"select tabn,knah from Zarp where datz >= '%04d-%02d-01' and datz <= '%04d-%02d-31' and suma <> 0. and prn='1'",gn,mn,gk,mk);
int kolstr=0;

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

class SPISOK sptabn;

while(cur.read_cursor(&row) != 0)
 {
  if(proverka(kodbol.ravno(),row[1],0,0) != 0)
   continue;
  if(sptabn.find(row[0],0) != 0)
   sptabn.plus(row[0]);
  
 }

char imaf[64];
sprintf(imaf,"zarspbol%d.lst",getpid());
class iceb_fopen fil;
if(fil.start(imaf,"w") != 0)
 {
  return(1);
 }

iceb_t_insfil("zarspbolz.alx",fil.ff);

zarspbol_hap(1,NULL,fil.ff);

int nz=0;
class iceb_tu_str fio("");
class iceb_tu_str inn("");
int tabn=0;
for(int nom=0; nom < sptabn.kolih(); nom++)
 {
  tabn=sptabn.ravno_atoi(nom);
  sprintf(strsql,"select fio,inn from Kartb where tabn=%d",tabn);
  if(readkey(strsql,&row1,&cur1) != 0)
   {
    fio.new_plus(row1[0]);
    inn.new_plus(row1[1]);
   }  
  else
   {
    fio.new_plus("");
    inn.new_plus("");
   }
  fprintf(fil.ff,"%3d|%-*.*s|%-*.*s",++nz,
  iceb_tu_kolbait(40,fio.ravno()),
  iceb_tu_kolbait(40,fio.ravno()),
  fio.ravno(),
  iceb_tu_kolbait(10,inn.ravno()),
  iceb_tu_kolbait(10,inn.ravno()),
  inn.ravno());    
  fprintf(fil.ff,"|              |                         |      |          |          |     |     |     |\n");
 }

fprintf(fil.ff,"ICEB_LST_END\n");
fprintf(fil.ff,"------------------------------------------------------------------------------------------------------------------------------------------------\n");
 
iceb_t_insfil("zarspbole.alx",fil.ff);

podpis(fil.ff);
fil.end();

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus(gettext("Протокол №12"));


int orient=iceb_t_ustpeh(oth->spis_imaf.ravno(0),3);

iceb_t_rnl(oth->spis_imaf.ravno(0),orient,NULL,&zarspbol_hap);


return(0);
}
/***************************************************/
/*Распечатка шапки*/
/**************************************************/

void zarspbol_hap(int nomstr,int *kolstr,FILE *ff)
{

int ks=iceb_t_insfil("zarspbolh.alx",ff);

if(kolstr != NULL)
 *kolstr+=ks;

}
