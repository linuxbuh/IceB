/*$Id: sprtab.c,v 5.31 2013/09/26 09:43:41 sasa Exp $*/
/*13.01.2020	01.01.2003	Белых А.И.	sprtabotp_m.c
Меню для получения справок по табельному номеру
*/
#include <errno.h>
#include        "buhg.h"

class sprtabotp_poi
 {
  public:
  class iceb_tu_str dat1,dat2;
  short mn,gn;
  short dk,mk,gk;
  class iceb_tu_str tabnom;
  class iceb_tu_str vid_otp;
  class iceb_tu_str prikaz;
  class iceb_tu_str period;
  class iceb_tu_str kol_dnei;
  class iceb_tu_str kod_podr;
  int metka_ri;  /*0-расчёт с использванием дожностных окладов 1-расчёт по коэффициентам введённым в файл*/


  class iceb_tu_str kodnvr; //Коды не входящие в расчёт отпускных
  class iceb_tu_str kodnvrind; //Коды не входящие в расчёт индексации отпускных
  class iceb_tu_str kodt_nvr; //Коды табеля не входящие в расчёт
  class iceb_tu_str kodnvr_d; //Дополнительные коды не входящие в расчёт
  class iceb_tu_str kodn_d_dk; //Коды начислений для дополнительных кодов
  int kolmes; /*количесво месяцев в заданном периоде*/
  
  sprtabotp_poi()
   {
    clear();
   }

  void clear()
   {
    tabnom.new_plus("");
    dat1.new_plus("");
    dat2.new_plus("");
    vid_otp.new_plus("");
    prikaz.new_plus("");
    period.new_plus("");
    kol_dnei.new_plus("");
    kod_podr.new_plus("");
    dk=mn=gn=mk=gk=0;
    metka_ri=0;

   
    kodnvr.new_plus(""); //Коды не входящие в расчёт отпускных
    kodnvrind.new_plus(""); //Коды не входящие в расчёт индексации отпускных
    kodt_nvr.new_plus(""); //Коды табеля не входящие в расчёт
    kodnvr_d.new_plus(""); //Дополнительные коды не входящие в расчёт
    kodn_d_dk.new_plus(""); //Коды начислений для дополнительных кодов
    kolmes=0;  
   }
 };

class sprtabotp_dat
 {
  public:
   class masiv_din_double sumanmes;
   class SPISOK DATAI; //Список дат м.год индексации
   class masiv_din_double KOF; //Коффициенты индексации по датам
   class masiv_din_double sumanvr; //Суммы не входящие в расчёт
   class masiv_din_double sumanvrxr; //Суммы не входящие в расчёт хозрасчёт
   class masiv_din_double sumanmesxr; //Сумма начисленная хозрасчёт
   class masiv_din_double sumanvi; //Сумма не входящая в расчёт индексации отпускных
   class masiv_din_double sumanvixr; //Сумма не входящая в расчёт индексации отпускных хозрасчёта
   class masiv_din_double sumanmesbu;
   class masiv_din_double sumanvrbu; //Суммы не входящие в расчёт бюджет
   class masiv_din_double sumanvibu; //Сумма не входящая в расчёт индексации отпускных бюджета
   class masiv_din_int  kolkd; //Количество календарных дней в месяце
   class masiv_din_int  kol_pd; //Количество праздничных дней в месяце
   class masiv_din_int  kolkd_vdr; //Количество календарных дней взятых для расчёта
   class masiv_din_int skodnvr; /*список кодов начислений вошедших в расчёт*/
   class masiv_din_double sum_vr;

   class iceb_tu_str fio;
   class iceb_tu_str dolg;
   class iceb_tu_str kodpodr;     
   class iceb_tu_str naipod;
 };
 

void readki(SPISOK*,masiv_din_double*);
void sprtabotp_rnvvr(int tabn,short mn,short gn,short dk,short mk,short gk,class masiv_din_int *skodnvr,class masiv_din_double *sum_vr,FILE *ff);
double sprtabotp_koef(int tabnom,short mr,short gr,short mk,short gk,FILE *ff);


double sprtabotp_po(int tabn,class sprtabotp_poi *rp,const char *kod_otp,int metka_mes,int metka_b,FILE *ff_prot);
int sprtabotp_rntn(class sprtabotp_poi *rp,class spis_oth *oth);


extern char	*shetb; /*Бюджетные счета начислений*/

int sprtabotp_r(class sprtabotp_poi *rp,class spis_oth*);

void sprtabotp_m()
{
static class sprtabotp_poi rp;
class iceb_tu_str fam("");
class iceb_tu_str inn("");
class iceb_tu_str datnr("");
class iceb_tu_str indeks(gettext("Расчёт с использованием должностных окладов"));
char		strsql[4096];
SQL_str		row;
SQLCURSOR curr;
int		ttn1=0;
int		N=0,K=0;
int		konec=0;
int kom=0,kom1=0;
class iceb_tu_str fiov("");
class iceb_tu_str kod("");
class iceb_tu_str naikod("");

class VVOD MENU(3);

VVOD VV(0);
if(rp.tabnom.ravno_atoi() == 0)
 VV.VVOD_SPISOK_add_data(128);
else
 {
  sprintf(strsql,"%d",rp.tabnom.ravno_atoi());
  VV.VVOD_SPISOK_add_data(strsql,128);

  sprintf(strsql,"select fio from Kartb where tabn=%d",rp.tabnom.ravno_atoi());
  if(sql_readkey(&bd,strsql,&row,&curr) != 1)
   {
    sprintf(strsql,"%s %d !",gettext("Не найден табельный номер"),rp.tabnom.ravno_atoi());
    iceb_t_soob(strsql);
   }
  else
   {
    fam.new_plus(row[0]);
   }
 }

VV.VVOD_SPISOK_add_data(rp.dat1.ravno(),8);
VV.VVOD_SPISOK_add_data(rp.dat2.ravno(),8);
VV.VVOD_SPISOK_add_data(rp.vid_otp.ravno(),128);
VV.VVOD_SPISOK_add_data(rp.prikaz.ravno(),128);
VV.VVOD_SPISOK_add_data(rp.period.ravno(),128);
VV.VVOD_SPISOK_add_data(rp.kol_dnei.ravno(),20);
VV.VVOD_SPISOK_add_data(indeks.ravno(),100);
VV.VVOD_SPISOK_add_data(rp.kod_podr.ravno(),128);

VV.VVOD_SPISOK_add_MD(gettext("Табельный номер.........(,,)."));//0
VV.VVOD_SPISOK_add_MD(gettext("Дата начала.(м.г)............"));//1
VV.VVOD_SPISOK_add_MD(gettext("Дата конца..(м.г)............"));//2
VV.VVOD_SPISOK_add_MD(gettext("Вид отпуска.................."));//3
VV.VVOD_SPISOK_add_MD(gettext("Приказ......................."));//4
VV.VVOD_SPISOK_add_MD(gettext("Период......................."));//5
VV.VVOD_SPISOK_add_MD(gettext("Количество дней отпуска.(,,)."));//6
VV.VVOD_SPISOK_add_MD(gettext("Способ расчёта индексации...."));//7
VV.VVOD_SPISOK_add_MD(gettext("Код подразделения.......(,,)."));//8

naz:;


clear();

if(rp.metka_ri == 0)
 VV.data_plus(7,gettext("Расчёт с использованием должностных окладов"));
else
 VV.data_plus(7,gettext("Расчёт с использованием коэффициентов"));

VV.VVOD_delete_ZAG();

VV.VVOD_SPISOK_add_ZAG(gettext("Распечатать расчёт отпускных"));

if(fam.ravno()[0] != '\0')
 {
  VV.VVOD_SPISOK_add_ZAG(fam.ravno());
 }

if(datnr.getdlinna() > 1)
 {
  sprintf(strsql,"%s:%s",gettext("Дата начала работы"),datnr.ravno());
  VV.VVOD_SPISOK_add_ZAG(strsql);
 } 

 helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("расчёт"),
"F3",gettext("таб.номер"),
"F4",gettext("очистить"),
"F5",gettext("настройка"),
"F10",gettext("выход"),NULL);

VV.vramka(-1,-1,0);


while(konec == 0)
 {
  if(N > VV.KLST-1)
    N=0;

  if(N < 0)
    N=VV.KLST-1;


  attron(VV.VVOD_return_cs(iceb_CFS)); 

  VV.vprintw(N,stdscr);




  
  K=getch();


  attroff(VV.VVOD_return_cs(iceb_CFS));
  attron(VV.VVOD_return_cs(iceb_CFM));


  VV.vprintw(N,stdscr);

  if(K == KEY_MOUSE)
    K=mouseklic(&N,VV.PY,VV.PX,VV.MDLS,VV.KLST,stdscr);



  switch(K)
   {

    case KEY_RESIZE: //Изменение размеров экрана
      goto naz;

    case ENTER:

      attroff(VV.VVOD_return_cs(iceb_CFM));
      attron(VV.VVOD_return_cs(iceb_CFV));
      VV.vprintw_clear(N,stdscr);

      if(N == 7)
       {
         MENU.VVOD_delete();

        MENU.VVOD_SPISOK_add_MD(gettext("Расчёт с использованием должностных окладов"));
        MENU.VVOD_SPISOK_add_MD(gettext("Расчёт с использованием коэффициентов"));

        clearstr(LINES-1,0);
        
        while(menu3w(stdscr,&MENU,&rp.metka_ri,-1,-1,0) != 0);
//        N=0;
        goto naz;
       }

      VV.vgetstr(N,stdscr);
      
      attroff(VV.VVOD_return_cs(iceb_CFV));
      attron(VV.VVOD_return_cs(iceb_CFM));

      if(N == 0 && VV.VVOD_SPISOK_return_data(N)[0] != '\0') //Табельный номер
       {

        sprintf(strsql,"select fio,datn from Kartb where tabn=%s",VV.VVOD_SPISOK_return_data(N));
        if(sql_readkey(&bd,strsql,&row,&curr) != 1)
         {
          sprintf(strsql,"%s %s !",gettext("Не найден табельный номер"),VV.VVOD_SPISOK_return_data(N));
          iceb_t_soob(strsql);
          fam.new_plus("");
          datnr.new_plus("");
         }
        else
         {
          fam.new_plus(row[0]);
          datnr.new_plus(iceb_tu_datzap(row[1]));
         }

        N++;
        goto naz;
       }

      if(N == 1 && VV.VVOD_SPISOK_return_data(N)[0] != '\0') //Дата начала
       {
        if(rsdat1(&rp.mn,&rp.gn,VV.VVOD_SPISOK_return_data(N)) != 0)
         {
          iceb_t_soob(gettext("Не верно введена дата начала !"));
          goto naz;
         }
       }
      if(N == 2 && VV.VVOD_SPISOK_return_data(N)[0] != '\0') //Дата начала
       {
        if(rsdat1(&rp.mk,&rp.gk,VV.VVOD_SPISOK_return_data(N)) != 0)
         {
          iceb_t_soob(gettext("Не верно введена дата конца !"));
          goto naz;
         }
       }


      VV.vprintw(N,stdscr);
      N++;

      break;
      
    case SDN: //Стрелка вниз
     N++;
     break;

    case SUP:  //Стрелка вверх
      N--;
      break;

    case FK1:
      GDITE();
      sprintf(strsql,"%s","zarp4_3_2_6.txt");

      iceb_t_pdoc(strsql);
      goto naz;

    case FK2:
    case PLU:
     konec=1;
     break;

    case FK3:
       
    MENU.VVOD_delete();
    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка табельных номеров"));//0
    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка подразделений"));//1
    MENU.VVOD_SPISOK_add_MD(gettext("Выход"));//2

    clearstr(LINES-1,0);
    
    kom1=0;
    while(menu3w(stdscr,&MENU,&kom1,-1,-1,0) != 0);

    kod.new_plus("");
    naikod.new_plus("");

    switch (kom1)
     {
      case 2 :
      case -1:
       goto naz;


      case 0 :
       clear();
       ttn1=0;
       fiov.new_plus("");
       if(dirtab(&ttn1,&fiov,0,1) == 0)
        {
         rp.tabnom.z_plus(ttn1);
         sprintf(strsql,"%d",ttn1);
         VV.VVOD_SPISOK_zapis_data(0,rp.tabnom.ravno());
         fam.new_plus(fiov.ravno());

         sprintf(strsql,"select datn from Kartb where tabn=%d",ttn1);
         if(readkey(strsql,&row,&curr) == 1)
           datnr.new_plus(iceb_tu_datzap(row[0]));

        }
        goto naz;

      case 1 :
        if(dirzarpodr(1,&kod,&naikod) == 0)
         {
          rp.kod_podr.z_plus(kod.ravno());
          VV.VVOD_SPISOK_zapis_data(8,rp.kod_podr.ravno());
          
         }
        goto naz;

     }

     goto naz;

    case FK5:
      MENU.VVOD_delete();

      MENU.VVOD_SPISOK_add_MD(gettext("Работа с файлом настройки"));
      MENU.VVOD_SPISOK_add_MD(gettext("Работа со справочником должностных окладов"));
      MENU.VVOD_SPISOK_add_MD(gettext("Выход"));

      clearstr(LINES-1,0);
      kom=0;      
      while(menu3w(stdscr,&MENU,&kom,-1,-1,0) != 0);
      N=0;
      switch(kom)
       {
        case 0:
         iceb_redfil("zarotp.alx",0);
         break;
        case 1:
          dirzarsdo(VV.data_ravno_atoi(0));
        default:
         break;
       }


      goto naz;

    case FK4:
     VV.VVOD_clear_data();
     fam.new_plus("");
     goto naz;

    case FK10:
     return;

   }

 }

rp.tabnom.new_plus(VV.data_ravno(0));
rp.dat1.new_plus(VV.data_ravno(1));
rp.dat2.new_plus(VV.data_ravno(2));
rp.vid_otp.new_plus(VV.data_ravno(3));
rp.prikaz.new_plus(VV.data_ravno(4));
rp.period.new_plus(VV.data_ravno(5));
rp.kol_dnei.new_plus(VV.data_ravno(6));
rp.kod_podr.new_plus(VV.data_ravno(8));

konec=0; //Чтобы не зациклило


if(rsdat1(&rp.mn,&rp.gn,VV.VVOD_SPISOK_return_data(1)) != 0)
 {
  iceb_t_soob(gettext("Не верно введена дата начала !"));
  goto naz;
 }

if(VV.VVOD_SPISOK_return_data(2)[0] != '\0')
 {
  if(rsdat1(&rp.mk,&rp.gk,VV.VVOD_SPISOK_return_data(2)) != 0)
   {
    iceb_t_soob(gettext("Не верно введена дата конца !"));
    goto naz;
   }
 }
else
 {
  rp.mk=rp.mn;
  rp.gk=rp.gn;
 } 

if(rp.tabnom.getdlinna() > 1 && pole2(rp.tabnom.ravno(),',') == 0)
 {
  sprintf(strsql,"select fio,inn from Kartb where tabn=%d",rp.tabnom.ravno_atoi());
  if(sql_readkey(&bd,strsql,&row,&curr) != 1)
   {
    sprintf(strsql,"%s %d !",gettext("Не найден табельный номер"),rp.tabnom.ravno_atoi());
    iceb_t_soob(strsql);
    goto naz;
   }
 }
 
rp.kolmes=period(1,rp.mn,rp.gn,rp.dk,rp.mk,rp.gk,1);

iceb_t_poldan("Коды не входящие в расчёт отпускных",&rp.kodnvr,"zarotp.alx");
iceb_t_poldan("Коды не входящие в расчёт индексации отпускных",&rp.kodnvrind,"zarotp.alx");
iceb_t_poldan("Коды табеля не входящие в расчёт",&rp.kodt_nvr,"zarotp.alx");
iceb_t_poldan("Дополнительные коды не входящие в расчёт",&rp.kodnvr_d,"zarotp.alx");
iceb_t_poldan("Коды начислений для дополнительных кодов",&rp.kodn_d_dk,"zarotp.alx");

class spis_oth oth;


dpm(&rp.dk,&rp.mk,&rp.gk,5);


if(rp.tabnom.getdlinna() <= 1 || pole2(rp.tabnom.ravno(),',') > 0)
 {
  /*расчёт отпускных по нескольким табельных номерам*/
  if(sprtabotp_rntn(&rp,&oth) == 0)
   iceb_t_rabfil(&oth,"");
 }
else
 {
  /*расчёт отпускных по одному табельному номеру*/
  if(sprtabotp_r(&rp,&oth) == 0)
   iceb_t_rabfil(&oth,"");
 }

}
/********************************/
/* Распечатка результата        */
/*возвращает сумму последней колонки в таблице*/
/********************************/
double rasrez(int tabn,
class sprtabotp_poi *rp,
class sprtabotp_dat *dt,
class masiv_din_double *sumanmes,
class masiv_din_double *sumanvr,
int metka, //0-общий 1-бюджет 2-хозрасчёт
class masiv_din_double *sumanvi, //Массив сумм не входящих в расчёт индексации
FILE *ff,
FILE *ff_prot)
{
int nomkod=0;
int nommes=0;
char strsql[2048];
short d,m,g;
SQL_str row;
int kolkdi=0;
char bros[1024];
double kof=0.;
double  sumaindekc=0.;
SQLCURSOR curr;
//Вычисляем количество колонок
int kolkol=0;
int kolkodnvr=pole2(rp->kodnvr.ravno(),',');

for(nomkod=0; nomkod < kolkodnvr ; nomkod++)
 if(sumanvr->suma_kol(rp->kolmes,kolkodnvr,nomkod) != 0.)
//  if(itogvert(kolkodnvr,rp->kolmes,nomkod,sumanvr) != 0.)
   kolkol++;
int kolkodnvrind=pole2(rp->kodnvrind.ravno(),',');
for(nomkod=0; nomkod < kolkodnvrind ; nomkod++)
 if(sumanvi->suma_kol(rp->kolmes,kolkodnvrind,nomkod) != 0.)
//  if(itogvert(kolkodnvrind,rp->kolmes,nomkod,sumanvi) != 0.)
   kolkol++;

char stroka[51+(kolkol+1)*11+1];
memset(stroka,'\0',sizeof(stroka));
memset(stroka,'-',sizeof(stroka)-1);

class iceb_tu_str kod_otp("");
iceb_t_poldan("Коды начислений отпускных",&kod_otp,"zarotp.alx");

time_t  tmm;
time(&tmm);
iceb_t_zagolov(gettext("Расчётный лист на оплату отпускных"),1,rp->mn,rp->gn,rp->dk,rp->mk,rp->gk,ff);

fprintf(ff,"\n%s:%d\n",gettext("Табельный номер"),tabn);
fprintf(ff,"%s:%s\n",gettext("Фамилия Имя Отчество"),dt->fio.ravno());
fprintf(ff,"%s:%s\n",gettext("Должность"),dt->dolg.ravno());
fprintf(ff,"%s:%s %s\n",gettext("Подразделение"),dt->kodpodr.ravno(),dt->naipod.ravno());
fprintf(ff,"%s:%s\n",gettext("Вид отпуска"),rp->vid_otp.ravno());
fprintf(ff,"%s:%s\n",gettext("Приказ"),rp->prikaz.ravno());
fprintf(ff,"%s:%s\n",gettext("Период з"),rp->period.ravno());
fprintf(ff,"%s:%s\n",gettext("Количество дней отпуска"),rp->kol_dnei.ravno());
fprintf(ff,"\n");

if(rp->kodt_nvr.ravno()[0] != '\0')
 fprintf(ff,"%s:%s\n",gettext("Коды табеля не входящие в расчёт"),rp->kodt_nvr.ravno());
 
if(metka == 1)
  fprintf(ff,"%s.\n",gettext("Бюджет"));

if(metka == 2)
  fprintf(ff,"%s.\n",gettext("Хозрасчёт"));

int metka_pk=iceb_t_poldan_vkl("Печать наименований кодов не входящих в расчёт","zarotp.alx");

if(metka_pk == 1)
 {
  fprintf(ff,"%s:%.*s\n",
  gettext("Коды, которые не вошли в расчёт"),
  iceb_tu_kolbait(60,rp->kodnvr.ravno()),
  rp->kodnvr.ravno());

  for(int nom=60; nom < iceb_tu_strlen(rp->kodnvr.ravno()); nom+=90)
   fprintf(ff,"%.*s\n",
   iceb_tu_kolbait(90,iceb_tu_adrsimv(nom,rp->kodnvr.ravno())),
   iceb_tu_adrsimv(nom,rp->kodnvr.ravno()));
      
 }
else
 {
  if(kolkodnvr != 0)
   {
    class iceb_tu_str kod("");
    class iceb_tu_str naikod("");
    fprintf(ff,"%s:\n",gettext("Коды, которые не вошли в расчёт"));
    short shethik=0;
    char str_prom[1024];
    char str2[1024];
    
    for(nomkod=0; nomkod < kolkodnvr ; nomkod++)
     {
      if(polen(rp->kodnvr.ravno(),&kod,nomkod+1,',') != 0)
        kod.new_plus(rp->kodnvr.ravno());
      sprintf(strsql,"select naik from Nash where kod=%s",kod.ravno());
      if(sql_readkey(&bd,strsql,&row,&curr) == 1)
       naikod.new_plus(row[0]);    
      else
       naikod.new_plus("");  
      if(shethik == 0)
       {
        memset(str_prom,'\0',sizeof(str_prom));
        sprintf(str_prom,"%2s %-*.*s",
        kod.ravno(),
        iceb_tu_kolbait(45,naikod.ravno()),iceb_tu_kolbait(45,naikod.ravno()),naikod.ravno());
       }

      if(shethik == 1)
       {
        sprintf(str2,"  %2s %-*.*s",
        kod.ravno(),
        iceb_tu_kolbait(45,naikod.ravno()),iceb_tu_kolbait(45,naikod.ravno()),naikod.ravno());
        strcat(str_prom,str2);
       }


      
      shethik++;
      if(shethik == 2)
       {
        fprintf(ff,"%s\n",str_prom);
        shethik=0;
       }
    
    
     }
    if(shethik != 0)
        fprintf(ff,"%s\n",str_prom);

   }
 }
 
if(kolkodnvrind != 0)
 {
  class iceb_tu_str kod("");
  class iceb_tu_str naikod("");
  fprintf(ff,"%s:\n",gettext("Коды, которые не вошли в расчёт индексации"));
  for(nomkod=0; nomkod < kolkodnvrind ; nomkod++)
   {
    if(polen(rp->kodnvrind.ravno(),&kod,nomkod+1,',') != 0)
      kod.new_plus(rp->kodnvrind.ravno());

    sprintf(strsql,"select naik from Nash where kod=%s",kod.ravno());
    if(sql_readkey(&bd,strsql,&row,&curr) == 1)
     naikod.new_plus(row[0]);    
    else
     naikod.new_plus("");
    fprintf(ff,"%s %s\n",kod.ravno(),naikod.ravno());
   }

 }

fprintf(ff,"\n");

fprintf(ff,"%s\n",stroka);

fprintf(ff,gettext(" Дата  |К.д|П.д|В.р|Начислено |"));
//Печатаем коды не входящие в расчёт
for(nomkod=0; nomkod < kolkodnvr ; nomkod++)
 {
  if(sumanvr->suma_kol(rp->kolmes,kolkodnvr,nomkod) == 0.)
//  if(itogvert(kolkodnvr,rp->kolmes,nomkod,sumanvr) == 0.)
    continue;
    
  if(polen(rp->kodnvr.ravno(),bros,sizeof(bros),nomkod+1,',') != 0)
    strcpy(bros,rp->kodnvr.ravno());

  sprintf(strsql,"select naik from Nash where kod=%d",atoi(bros));
  if(readkey(strsql,&row,&curr) == 1)
   sprintf(strsql,"%s %s",bros,row[0]);
  else
   strcpy(strsql,bros);

  fprintf(ff,"%*.*s|",
  iceb_tu_kolbait(10,strsql),
  iceb_tu_kolbait(10,strsql),
  strsql);
 }

fprintf(ff,"%*.*s|",
iceb_tu_kolbait(10,gettext("В расчёт")),iceb_tu_kolbait(10,gettext("В расчёт")),gettext("В расчёт"));

//Печатаем коды не входящие в расчёт индексации
for(nomkod=0; nomkod < kolkodnvrind ; nomkod++)
 {
//  if(itogvert(kolkodnvrind,rp->kolmes,nomkod,sumanvi) == 0.)
  if(sumanvi->suma_kol(rp->kolmes,kolkodnvrind,nomkod) == 0.)
    continue;
    
  if(polen(rp->kodnvrind.ravno(),bros,sizeof(bros),nomkod+1,',') != 0)
    strcpy(bros,rp->kodnvrind.ravno());

  sprintf(strsql,"select naik from Nash where kod=%d",atoi(bros));
  if(readkey(strsql,&row,&curr) == 1)
   sprintf(strsql,"%s %s",bros,row[0]);
  else
   strcpy(strsql,bros);

  fprintf(ff,"%*.*s|",
  iceb_tu_kolbait(10,strsql),
  iceb_tu_kolbait(10,strsql),
  strsql);


 }


fprintf(ff,"%-*.*s|%-*.*s|\n",
iceb_tu_kolbait(8,gettext("Коэф.")),iceb_tu_kolbait(8,gettext("Коэф.")),gettext("Коэф."),
iceb_tu_kolbait(10,gettext("Итого")),iceb_tu_kolbait(10,gettext("Итого")),gettext("Итого"));

fprintf(ff,"%s\n",stroka);

d=1; m=rp->mn; g=rp->gn;
double itog=0.;
double itog1=0.;
double itogind=0.;
int kold_vdr=0;
int itogo_dni[3];
double v_rashet=0.;
double itogo_v_rashet=0.;
memset(itogo_dni,'\0',sizeof(itogo_dni));

for(nommes=0; nommes < rp->kolmes; nommes++)
 {

  fprintf(ff,"%02d.%04d|%3d|%3d|%3d|%10.2f|",m,g,dt->kolkd.ravno(nommes),dt->kol_pd.ravno(nommes),dt->kolkd_vdr.ravno(nommes)-dt->kol_pd.ravno(nommes),sumanmes->ravno(nommes));
  kolkdi+=dt->kolkd.ravno(nommes);
  kold_vdr+=dt->kolkd_vdr.ravno(nommes);
  itog1+=sumanmes->ravno(nommes);
  itog=0.;
  itogo_dni[0]+=dt->kolkd.ravno(nommes);
  itogo_dni[1]+=dt->kol_pd.ravno(nommes);
  itogo_dni[2]+=dt->kolkd_vdr.ravno(nommes)-dt->kol_pd.ravno(nommes);


  for(nomkod=0; nomkod < kolkodnvr ; nomkod++)
   {
    //if(itogvert(kolkodnvr,rp->kolmes,nomkod,sumanvr) == 0.)
    if(sumanvr->suma_kol(rp->kolmes,kolkodnvr,nomkod) == 0.)
      continue;
    double sss=sumanvr->ravno(nommes*kolkodnvr+nomkod);
    if(sss != 0.)
     fprintf(ff,"%10.2f|",sss);
    else
     fprintf(ff,"%10s|","");

    itog+=sss;
   }

  double otp=0.;

  if(nommes == 0 || nommes == 1)/*прибавляем сумму отпускных в счёт этого месяца*/
   {

    otp=sprtabotp_po(tabn,rp,kod_otp.ravno(),nommes,metka,ff_prot);
    
   }

  /*печатаем колонку В расчёт*/
  v_rashet=sumanmes->ravno(nommes)-itog+otp;
  itogo_v_rashet+=v_rashet;
  fprintf(ff,"%10.2f|",v_rashet);

  itogind=0.;
  for(nomkod=0; nomkod < kolkodnvrind ; nomkod++)
   {
//    if(itogvert(kolkodnvrind,rp->kolmes,nomkod,sumanvi) == 0.)
    if(sumanvi->suma_kol(rp->kolmes,kolkodnvrind,nomkod) == 0.)
      continue;

    fprintf(ff,"%10.2f|",sumanvi->ravno(nommes*kolkodnvrind+nomkod));

    itogind+=sumanvi->ravno(nommes*kolkodnvrind+nomkod);
   }

  if(rp->metka_ri == 1)
   {
    int nomer=0;
    sprintf(bros,"%d.%d",m,g);
    if((nomer=dt->DATAI.find(bros)) >= 0)
     {
      kof=dt->KOF.ravno(nomer);
      fprintf(ff,"%8.8g|%10.2f|",kof,kof*(v_rashet-itogind));
     }  
    else
     {
      kof=1.;
      fprintf(ff,"%8s|%10.2f|","",kof*(v_rashet-itogind));
     }
   }
  else
   {
    kof=sprtabotp_koef(tabn,m,g,rp->mk,rp->gk,ff_prot);
    fprintf(ff,"%8.8g|%10.2f|",kof,kof*(v_rashet-itogind));
   }

  sumaindekc+=kof*(v_rashet-itogind);
  if(otp != 0.)
   fprintf(ff," %s %.2f",gettext("отпускные"),otp);
  fprintf(ff,"\n");
  
  dpm(&d,&m,&g,3);
 }
fprintf(ff,"%s\n",stroka);

//Печатаем итоговую строку
fprintf(ff,"%*s:%3d %3d %3d %10.2f",iceb_tu_kolbait(7,gettext("Итого")),gettext("Итого"),itogo_dni[0],itogo_dni[1],itogo_dni[2],itog1);
for(nomkod=0; nomkod < kolkodnvr ; nomkod++)
 {
//  if(itogvert(kolkodnvr,rp->kolmes,nomkod,sumanvr) == 0.)
   if(sumanvr->suma_kol(rp->kolmes,kolkodnvr,nomkod) == 0.)
    continue;
  itog=0.;
  for(nommes=0; nommes < rp->kolmes; nommes++)
    itog+=sumanvr->ravno(nommes*kolkodnvr+nomkod);
  fprintf(ff," %10.2f",itog);
  itog1-=itog;
 }

itogind=0.;
for(nomkod=0; nomkod < kolkodnvrind ; nomkod++)
 {
//  if(itogvert(kolkodnvrind,rp->kolmes,nomkod,sumanvi) == 0.)
  if(sumanvi->suma_kol(rp->kolmes,kolkodnvrind,nomkod) == 0.)
    continue;
  itog=0.;
  for(nommes=0; nommes < rp->kolmes; nommes++)
    itog+=sumanvi->ravno(nommes*kolkodnvrind+nomkod);
  fprintf(ff," %10.2f",itog);
  itogind+=itog;
 }

fprintf(ff," %10.2f",itogo_v_rashet);


fprintf(ff," %8s %10.2f"," ",sumaindekc);
fprintf(ff,"\n");

if(itogind != 0.)
 {
  fprintf(ff,"\n%.2f+%.2f=%.2f\n",itogind,sumaindekc,itogind+sumaindekc);
 }
double suma_otp=0.;
class iceb_tu_str kol_d(""); 

int kol_pol=pole2(rp->kol_dnei.ravno(),',');

if(kol_pol > 0)
 {
  fprintf(ff,"\n");
  for(int nom=1;nom <= kol_pol ; nom++)
   {
    polen(rp->kol_dnei.ravno(),&kol_d,nom,',');
    if(kol_d.ravno_atoi() == 0)
     continue;
    suma_otp=sumaindekc/itogo_dni[2]*kol_d.ravno_atoi();
    fprintf(ff,"%.2f/%d*%d=%.2f\n",sumaindekc,itogo_dni[2],kol_d.ravno_atoi(),suma_otp);
    
   }
 }
else
 if(rp->kol_dnei.getdlinna() > 1)
  {
   suma_otp=sumaindekc/itogo_dni[2]*rp->kol_dnei.ravno_atoi();
   fprintf(ff,"\n%.2f/%d*%d=%.2f\n",sumaindekc,itogo_dni[2],rp->kol_dnei.ravno_atoi(),suma_otp);
  }
fprintf(ff,"\n-----------------------------------------------------\n");
fprintf(ff,"-----------------------------------------------------\n");
fprintf(ff,"-----------------------------------------------------\n");
fprintf(ff,"-----------------------------------------------------\n");
fprintf(ff,"-----------------------------------------------------\n");


return(sumaindekc);

}
/*********************************************/
/*расчёт одного табельного номера*/
/**********************************************/
double sprtabotp_r1tn(int tabn,class sprtabotp_poi *rp,
class sprtabotp_dat *dt,
class iceb_zarrsov *svod_otvr, /*свод по отработанному времени*/
FILE *ff,
FILE *ff_prot)
{
char strsql[1024];
int kolstr=0;
class SQLCURSOR cur,cur1,curr;
SQL_str row,row1;


int metka_dkn=0; /*метка присутствия начислений для дополнительных кодов*/



sprintf(strsql,"select fio,dolg,podr,datn from Kartb where tabn=%d",tabn);
if(sql_readkey(&bd,strsql,&row,&curr) != 1)
 {
  sprintf(strsql,"%s %d !",gettext("Не найден табельный номер"),tabn);
  iceb_t_soob(strsql);
  return(-1.);
 }
dt->fio.new_plus(row[0]);
dt->dolg.new_plus(row[1]);
dt->kodpodr.new_plus(row[2]);

//Читаем наименование подразделения
sprintf(strsql,"select naik from Podr where kod=%s",dt->kodpodr.ravno());
if(sql_readkey(&bd,strsql,&row,&curr) == 1)
 dt->naipod.new_plus(row[0]);
else
 dt->naipod.new_plus("");



int kol_kd=pole2(rp->kodn_d_dk.ravno(),',');
if(kol_kd == 0 && rp->kodn_d_dk.ravno()[0] != '\0')
  kol_kd=1;
/*опрeделяем есть ли начисления для дополнительных кодов*/
for(int nom=0; nom < kol_kd; nom++)
 {
  int kdn=0;
  
  if(kol_kd == 1)
   kdn=rp->kodn_d_dk.ravno_atoi();
  else
   polen(rp->kodn_d_dk.ravno(),&kdn,nom+1,',');
    
  sprintf(strsql,"select datz,knah,suma,shet from Zarp where tabn=%d \
and datz >= '%04d-%d-%d' and datz <='%04d-%d-%d' and prn='1' and suma <> 0. and knah=%d limit 1",
  tabn,rp->gn,rp->mn,1,rp->gk,rp->mk,rp->dk,kdn);
  if(readkey(strsql) == 1)
   {
    metka_dkn=1;
    break;
   }
 }

if(metka_dkn == 1)
 {
  rp->kodnvr.z_plus(rp->kodnvr_d.ravno());
 }



int kolkodnvr=pole2(rp->kodnvr.ravno(),',');
if(kolkodnvr == 0 && rp->kodnvr.ravno()[0] != '\0')
  kolkodnvr=1;

int kolkodnvrind=pole2(rp->kodnvrind.ravno(),',');
if(kolkodnvrind == 0 && rp->kodnvrind.ravno()[0] != '\0')
  kolkodnvrind=1;
  

//Читаем коэффициенты индексации

readki(&dt->DATAI,&dt->KOF);


//double sumanmes[kolmes]; //Сумма начисленная
//memset(&sumanmes,'\0',sizeof(sumanmes));
dt->sumanmes.make_class(rp->kolmes);

//double sumanvr[kolmes*kolkodnvr]; //Суммы не входящие в расчёт
//memset(&sumanvr,'\0',sizeof(sumanvr));
dt->sumanvr.make_class(rp->kolmes*kolkodnvr);

//double sumanmesxr[rp->kolmes]; //Сумма начисленная хозрасчёт
//memset(&sumanmesxr,'\0',sizeof(sumanmesxr));
dt->sumanmesxr.make_class(rp->kolmes);

//double sumanvrxr[rp->kolmes*kolkodnvr]; //Суммы не входящие в расчёт хозрасчёт
//memset(&sumanvrxr,'\0',sizeof(sumanvrxr));
dt->sumanvrxr.make_class(rp->kolmes*kolkodnvr);


//double sumanmesbu[rp->kolmes]; //Сумма начисленная бюджет
//memset(&sumanmesbu,'\0',sizeof(sumanmesbu));
dt->sumanmesbu.make_class(rp->kolmes);

//double sumanvrbu[rp->kolmes*kolkodnvr]; //Суммы не входящие в расчёт бюджет
//memset(&sumanvrbu,'\0',sizeof(sumanvrbu));
dt->sumanvrbu.make_class(rp->kolmes*kolkodnvr);

//double sumanvi[rp->kolmes*kolkodnvrind]; //Сумма не входящая в расчёт индексации отпускных
//memset(sumanvi,'\0',sizeof(sumanvi));
dt->sumanvi.make_class(rp->kolmes*kolkodnvrind);

//double sumanvixr[rp->kolmes*kolkodnvrind]; //Сумма не входящая в расчёт индексации отпускных хозрасчёта
//memset(sumanvixr,'\0',sizeof(sumanvixr));
dt->sumanvixr.make_class(rp->kolmes*kolkodnvrind);

// double sumanvibu[rp->kolmes*kolkodnvrind]; //Сумма не входящая в расчёт индексации отпускных бюджета
//memset(sumanvibu,'\0',sizeof(sumanvibu));
dt->sumanvibu.make_class(rp->kolmes*kolkodnvrind);

dt->kolkd.make_class(rp->kolmes);

dt->kol_pd.make_class(rp->kolmes);

dt->kolkd_vdr.make_class(rp->kolmes);




sprintf(strsql,"select datz,knah,suma,shet from Zarp where tabn=%d \
and datz >= '%04d-%d-%d' and datz <='%04d-%d-%d' and prn='1' and suma <> 0.",
tabn,rp->gn,rp->mn,1,rp->gk,rp->mk,rp->dk);

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
  iceb_t_soob(gettext("Не найдено ни одной записи !"));
  return(1);
 }
int kolstr1=0;
short d=0,m=0,g=0;
short dnr=0,mnr=0,gnr=0;
double suma=0.;

while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);

  rsdat(&d,&m,&g,row[0],2);
  if(dnr != 0 && dnr != 1) //Если первый месяц отработан не полностью в расчёт его не берем
    if(mnr == m && gnr == g)
      continue;

  suma=atof(row[2]);

  int nommes=period(1,rp->mn,rp->gn,d,m,g,1)-1;    

//  sumanmes[nommes]+=suma;
  dt->sumanmes.plus(suma,nommes);

  if(proverka(shetb,row[3],0,1) == 0)
    dt->sumanmesbu.plus(suma,nommes);
  else 
    dt->sumanmesxr.plus(suma,nommes);
  int nomkod=0;
  if(proverka(rp->kodnvr.ravno(),row[1],0,1) == 0)
   {
    pole1(rp->kodnvr.ravno(),row[1],',',0,&nomkod);
//    sumanvr[nommes*kolkodnvr+nomkod]+=suma;
    dt->sumanvr.plus(suma,nommes*kolkodnvr+nomkod);
    if(proverka(shetb,row[3],0,1) == 0)
      dt->sumanvrbu.plus(suma,nommes*kolkodnvr+nomkod);
    else 
      dt->sumanvrxr.plus(suma,nommes*kolkodnvr+nomkod);
//      sumanvrxr[nommes*kolkodnvr+nomkod]+=suma;

   }
  else
   {
    if(dt->skodnvr.find(row[1]) < 0)
     dt->skodnvr.plus(row[1]);
   }

  if(proverka(rp->kodnvrind.ravno(),row[1],0,1) == 0)
   {
    pole1(rp->kodnvrind.ravno(),row[1],',',0,&nomkod);
    dt->sumanvi.plus(suma,nommes*kolkodnvrind+nomkod);
    if(proverka(shetb,row[3],0,1) == 0)
      dt->sumanvibu.plus(suma,nommes*kolkodnvrind+nomkod);
    else 
      dt->sumanvixr.plus(suma,nommes*kolkodnvrind+nomkod);
//      sumanvixr[nommes*kolkodnvrind+nomkod]+=suma;

   }
 }
/*Создаём масив для сумм начислений вошедших в расчёт*/
int kolih_kod=dt->skodnvr.kolih();
dt->sum_vr.make_class(kolih_kod*rp->kolmes);
kolstr1=0;
cur.poz_cursor(0);
int nommes=0,nomkod=0;
while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);

  rsdat(&d,&m,&g,row[0],2);
  if(dnr != 0 && dnr != 1) //Если первый месяц отработан не полностью в расчёт его не берем
    if(mnr == m && gnr == g)
      continue;

  suma=atof(row[2]);

  nommes=period(1,rp->mn,rp->gn,d,m,g,1)-1;    
  if((nomkod=dt->skodnvr.find(row[1])) < 0)
   continue;
  dt->sum_vr.plus(suma,nommes*kolih_kod+nomkod);
 }

 
//Узнаем количество календарных дней
if(rp->gn != rp->gk)
  sprintf(strsql,"select kodt,god,mes,dnei,kdnei,has from Ztab where tabn=%d and \
((god > %d and god < %d) \
or (god=%d and mes >= %d) \
or (god=%d and mes <= %d)) order by god asc,mes asc",
  tabn,rp->gn,rp->gk,rp->gn,rp->mn,rp->gk,rp->mk);
else
  sprintf(strsql,"select kodt,god,mes,dnei,kdnei,has from Ztab where tabn=%d and \
god=%d and mes >= %d and mes <= %d order by god asc,mes asc",
  tabn,rp->gn,rp->mn,rp->mk);

//printw("%s\n",strsql);

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
kolstr1=0;
d=1;
int nomvs=0;
int mes_z=0;
if(kolstr > 0)
while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);
  
  g=atoi(row[1]);
  m=atoi(row[2]);
  nommes=period(1,rp->mn,rp->gn,d,m,g,1)-1;    
  dt->kolkd.plus(atoi(row[4]),nommes);

  if(proverka(rp->kodt_nvr.ravno(),row[0],0,1) != 0)
   dt->kolkd_vdr.plus(atoi(row[4]),nommes);

  /*узнаём количество праздничных дней в месяце*/
  if(mes_z != m)
   {
    sprintf(strsql,"select kpd from Zarskrdh where data='%04d-%02d-01'",g,m);
    if(readkey(strsql,&row1,&cur1) == 1)
     dt->kol_pd.plus(atoi(row1[0]),nommes);
    mes_z=m;
   }

  if(svod_otvr->kod.find(row[0]) < 0)
     svod_otvr->kod.plus(row[0]);

  sprintf(strsql,"%s|%02d.%04d",row[0],m,g);

  if((nomvs=svod_otvr->kod_mes_god.find(strsql)) < 0)
   svod_otvr->kod_mes_god.plus(strsql);

  svod_otvr->kolrd.plus(row[3],nomvs);    
  svod_otvr->kolkd.plus(row[4],nomvs);    
  svod_otvr->kolhs.plus(atof(row[5]),nomvs);    

 }

//GDITE();


//Распечатываем общий итог
double sumai=rasrez(tabn,rp,dt,&dt->sumanmes,&dt->sumanvr,
0,&dt->sumanvi,ff,ff_prot);

/***********
rasrez(rp,&dt,fio.ravno(),dolg.ravno(),kodpodr.ravno(),naipod.ravno(),&dt.sumanmesbu,&dt.sumanvrbu,
fil.ff,fil_prot.ff,1,&dt.sumanvibu);
rasrez(rp,&dt,fio.ravno(),dolg.ravno(),kodpodr.ravno(),naipod.ravno(),&dt.sumanmesxr,&dt.sumanvrxr,
fil.ff,fil_prot.ff,2,&dt.sumanvixr);
***************/
return(sumai);
}
/************************************************/

/***********************************************/
int sprtabotp_r(class sprtabotp_poi *rp,
class spis_oth *oth)
{
char		imaf[64],imafbu[64],imafxr[64];
class SQLCURSOR cur,cur1;
short		dk=31;
class SQLCURSOR curr;
clear();
GDITE();

class sprtabotp_dat dt;


dpm(&rp->dk,&rp->mk,&rp->gk,5); //получить последний день месяца

printw("%d %s\n",rp->tabnom.ravno_atoi(),dt.fio.ravno());
printw("%d.%d.%d => %d.%d.%d\n",1,rp->mn,rp->gn,rp->dk,rp->mk,rp->gk);
refresh();


class iceb_fopen fil_prot;
char imaf_prot[64];
sprintf(imaf_prot,"otpp%d.lst",getpid());
if(fil_prot.start(imaf_prot,"w") != 0)
 return(1);

iceb_t_zagolov(gettext("Протокол хода расчёта отпускных"),1,rp->mn,rp->gn,rp->dk,rp->mk,rp->gk,fil_prot.ff);


/********************************************************/
class iceb_fopen fil;
sprintf(imaf,"otp%d.lst",getpid());
if(fil.start(imaf,"w") != 0)
 return(1);

fprintf(fil.ff,"\x1B\x33%c\n",30); /*Уменьшаем межстрочный интервал*/


class iceb_zarrsov svod_otvr; /*свод по отработанному времени*/


sprtabotp_r1tn(rp->tabnom.ravno_atoi(),rp,&dt,&svod_otvr,fil.ff,fil_prot.ff);


podpis(fil.ff);
fil.end();


char imaf_sn[64];
sprintf(imaf_sn,"otpn%d.lst",getpid());
class iceb_fopen fil_rsvod;
if(fil_rsvod.start(imaf_sn,"w") != 0)
 return(1);
 
sprtabotp_rnvvr(rp->tabnom.ravno_atoi(),rp->mn,rp->gn,dk,rp->mk,rp->gk,&dt.skodnvr,&dt.sum_vr,fil_rsvod.ff);
zarrsov(rp->mn,rp->gn,rp->mk,rp->gk,rp->tabnom.ravno_atoi(),&svod_otvr,fil_rsvod.ff);

podpis(fil_rsvod.ff);
fil_rsvod.end();

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus(gettext("Расчёт отпускных"));
oth->spis_imaf.plus(imaf_sn);
oth->spis_naim.plus(gettext("Свод начислений и отработанного времени"));

if(shetb != NULL)
 {
  sprintf(imafbu,"otpbu%d.lst",getpid());

  if(fil.start(imafbu,"w") != 0)
   return(1);


  rasrez(rp->tabnom.ravno_atoi(),rp,&dt,&dt.sumanmesbu,&dt.sumanvrbu,
  1,&dt.sumanvibu,fil.ff,fil_prot.ff);

  podpis(fil.ff);
  fil.end();


  sprintf(imafxr,"otpxr%d.lst",getpid());
  if(fil.start(imafxr,"w") != 0)
   return(1);

  rasrez(rp->tabnom.ravno_atoi(),rp,&dt,&dt.sumanmesxr,&dt.sumanvrxr,
  2,&dt.sumanvixr,fil.ff,fil_prot.ff);
  podpis(fil.ff);

  fil.end();

  
  oth->spis_imaf.plus(imafbu);
  oth->spis_naim.plus(gettext("Расчёт бюджетных отпускных"));
  
  oth->spis_imaf.plus(imafxr);
  oth->spis_naim.plus(gettext("Расчёт хозрасчётных отпускных"));
 }
podpis(fil_prot.ff);
fil_prot.end();

oth->spis_imaf.plus(imaf_prot);
oth->spis_naim.plus(gettext("Протокол хода расчёта"));


for(int nom=0; nom < oth->spis_imaf.kolih(); nom++)
 iceb_t_ustpeh(oth->spis_imaf.ravno(nom),3);

 
return(0);

}

/*********************************/
/*Чтение коєффициентов индексации*/
/*********************************/
void readki(SPISOK *DATAI,masiv_din_double *KOF)
{
class iceb_tu_str bros("");
class iceb_tu_str bros1("");
short m,g;
int   nomer;
double kof=0.;
SQL_str row_alx;
class SQLCURSOR cur_alx;
int kolstr=0;
char strsql[512];

sprintf(strsql,"select str from Alx where fil='zarotp.alx' order by ns asc");
if((kolstr=cur_alx.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return;
 }

if(kolstr == 0)
 {
  sprintf(strsql,"%s %s!",gettext("Не найдена настройка"),"zarotp.alx");
  iceb_t_soob(strsql);
  return;
 }

while(cur_alx.read_cursor(&row_alx) != 0)
 {
  if(row_alx[0][0] == '#')
   continue;
  if(polen(row_alx[0],&bros,1,'|') != 0)
    continue;
  if(polen(bros.ravno(),&bros1,1,'.') != 0)
    continue;
  m=atoi(bros.ravno());

  if(polen(bros.ravno(),&bros1,2,'.') != 0)
    continue;
  g=bros1.ravno_atoi();  

  if(polen(row_alx[0],&bros1,2,'|') != 0)
    continue;
  kof=bros1.ravno_atof();  

  sprintf(strsql,"%d.%d",m,g);
  if((nomer=DATAI->find(strsql)) < 0)
    DATAI->plus(strsql); 

  KOF->plus(kof,nomer);
//  printw("%s**%s\n",row_alx[0],bros);    

 }


}
/******************************************/
/*Распечатка начислений вошедших в расчёт*/
/******************************************/
void sprtabotp_rnvvr(int tabn,short mn,short gn,
short dk,short mk,short gk,
class masiv_din_int *skodnvr,
class masiv_din_double *sum_vr,FILE *ff)
{
SQL_str row;
class SQLCURSOR cur;
class iceb_tu_str naik;
char strsql[512];
double itogo_gor=0.;
double suma=0.;
int kolmes=period(1,mn,gn,dk,mk,gk,1);

short d=1,m=mn,g=gn;

iceb_t_zagolov(gettext("Начисления взятые в расчёт отпускных"),1,mn,gn,dk,mk,gk,ff);

int kolih_kod=skodnvr->kolih();
class masiv_din_double itogo_kol;

itogo_kol.make_class(kolmes);
sprintf(strsql,"select fio from Kartb where tabn=%d",tabn);
naik.new_plus("");
if(readkey(strsql,&row,&cur) == 1)
 naik.new_plus(row[0]);

fprintf(ff,"%d %s\n",tabn,naik.ravno());


class iceb_tu_str liniq;
liniq.plus("-------------------------------------");

for(int nom=0; nom <= kolmes; nom++)
 liniq.plus("-----------");

fprintf(ff,"%s\n",liniq.ravno());
fprintf(ff," Код |   Наименование начисления    |");
/*          12345 123456789012345678901234567890*/

for(int nom_mes=0; nom_mes < kolmes; nom_mes++)
 {
  fprintf(ff," %02d.%04d  |",m,g);
  dpm(&d,&m,&g,3);
 }

fprintf(ff,"%-*.*s|\n",
iceb_tu_kolbait(10,gettext("Итого")),
iceb_tu_kolbait(10,gettext("Итого")),
gettext("Итого"));
fprintf(ff,"%s\n",liniq.ravno());

for(int nom_kod=0; nom_kod < kolih_kod; nom_kod++)
 {
  naik.new_plus("");
  
  sprintf(strsql,"select naik from Nash where kod=%d",skodnvr->ravno(nom_kod));
  if(readkey(strsql,&row,&cur) == 1)
   naik.new_plus(row[0]);

  fprintf(ff,"%5d %-*.*s|",skodnvr->ravno(nom_kod),
  iceb_tu_kolbait(30,naik.ravno()),
  iceb_tu_kolbait(30,naik.ravno()),
  naik.ravno());
  itogo_gor=0.;
  for(int nom_mes=0; nom_mes < kolmes; nom_mes++)
   {
    suma=sum_vr->ravno(nom_mes*kolih_kod+nom_kod);
    if(suma != 0.)
     {
      fprintf(ff,"%10.2f|",suma);
      itogo_gor+=suma;    
      itogo_kol.plus(suma,nom_mes);
     }
    else
      fprintf(ff,"%10s|","");
     
   }
  fprintf(ff,"%10.2f|\n",itogo_gor);
  dpm(&d,&m,&g,3);

 }
fprintf(ff,"%s\n",liniq.ravno());
fprintf(ff,"%*.*s|",
iceb_tu_kolbait(36,gettext("Итого")),
iceb_tu_kolbait(36,gettext("Итого")),
gettext("Итого"));

for(int nom_mes=0; nom_mes < kolmes; nom_mes++)
 fprintf(ff,"%10.2f|",itogo_kol.ravno(nom_mes));
fprintf(ff,"%10.2f|\n",itogo_kol.suma());


}
/*************************/
/*определение коэффициента*/
/**************************/
double sprtabotp_koef(int tabnom,short mr,short gr,short mk,short gk,FILE *ff)
{
char strsql[1024];
SQL_str row;
class SQLCURSOR cur;
double oklad1=0.;
double oklad2=0.;
if(ff != NULL)
 {
  fprintf(ff,"\n%s-Расчёт коэффициента %d %02d.%04d\n",__FUNCTION__,tabnom,mr,gr);
  fprintf(ff,"%s-Ищем оклады меньше этой даты или равной\n",__FUNCTION__);
 }
 
sprintf(strsql,"select dt,sm,mt from Zarsdo where tn=%d and dt <= '%04d-%02d-01' and km <> '-' order by dt desc limit 1",tabnom,gr,mr);

if(readkey(strsql,&row,&cur) == 0)
 {
  if(ff != NULL)
   fprintf(ff,"%s-Не нашли коэффициент 1\n",__FUNCTION__);
  return(1.);
 }

if(ff != NULL)
 fprintf(ff,"%s-%s %10s %s\n",__FUNCTION__,iceb_tu_datzap_mg(row[0]),row[1],row[2]);

oklad1=atof(row[1]);
int metka=atoi(row[2]);
if(metka == 5 || metka == 6)
  oklad1=okladtr(mr,gr,oklad1);

if(ff != NULL)
 fprintf(ff,"%s-Ищем последее повышение зарплаты\n",__FUNCTION__);

//sprintf(strsql,"select dt,sm,mt from Zarsdo where tn=%d and dt > '%04d-%02d-01' and km <> '-' order by dt desc limit 1",tabnom,gr,mr);
sprintf(strsql,"select dt,sm,mt from Zarsdo where tn=%d and dt <= '%04d-%02d-01' and km <> '-' order by dt desc limit 1",tabnom,gk,mk);
if(readkey(strsql,&row,&cur) == 0)
 {
  if(ff != NULL)
   fprintf(ff,"%s-Не нашли коэффициент 1\n",__FUNCTION__);
  return(1.);
 }
if(ff != NULL)
 fprintf(ff,"%s-%s %10s %s\n",__FUNCTION__,iceb_tu_datzap_mg(row[0]),row[1],row[2]);

oklad2=atof(row[1]);
metka=atoi(row[2]);
if(metka == 5 || metka == 6)
  oklad2=okladtr(mr,gr,oklad2);


double koef=oklad2/oklad1;
koef=okrug(koef,0.000001);
if(ff != NULL)
 fprintf(ff,"%s-Расчитываем коэффициент %.2f/%.2f=%f\n",__FUNCTION__,oklad2,oklad1,koef);

return(koef);

}
/*************************************/
/*поиск в месяце предыдущем началу расчёта отпускных в счёт месяца начала расчёта*/
/*********************************/

double sprtabotp_po(int tabn,class sprtabotp_poi *rp,const char *kod_otp,int metka_mes,
int metka_b, //0-общий 1-бюджет 2-хозрасчёт
FILE *ff_prot)
{
double suma_otp=0.;
char strsql[1024];
int kolstr=0;
SQL_str row;
class SQLCURSOR cur;
short dp=1,mp=rp->mn,gp=rp->gn;
short den=1,mes=rp->mn,god=rp->gn;

dpm(&dp,&mp,&gp,4);
if(metka_mes == 1)
 dpm(&den,&mes,&god,3);

sprintf(strsql,"select knah,suma,shet from Zarp where tabn=%d \
and datz >= '%04d-%d-%d' and datz <='%04d-%d-%d' and prn='1' and suma <> 0.\
 and godn=%d and mesn=%d",
tabn,gp,mp,1,gp,mp,31,god,mes);

//printw("%s-%s\n",__FUNCTION__,strsql);
//OSTANOV();

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
  return(0.);
 }

while(cur.read_cursor(&row) != 0)
 {
  if(metka_b == 1)
   if(proverka(shetb,row[2],0,1) != 0)
    continue;
  if(metka_b == 2)
   if(proverka(shetb,row[2],0,1) == 0)
    continue;
          
  if(proverka(row[0],kod_otp,0,1) == 0)
   {
    if(ff_prot != NULL)
      fprintf(ff_prot,"%s-Сумма отпускных предыдущего месяца=%s\n",__FUNCTION__,row[1]);

    suma_otp+=atof(row[1]);
   }

 }
 
return(suma_otp);

}

/**************************************************/
/*расчёт по нескольким табельным номерам*/
/***********************************************/
int sprtabotp_rntn(class sprtabotp_poi *rp,
class spis_oth *oth)
{
char strsql[1024];
class SQLCURSOR cur,cur1;
SQL_str row,row1;
int kolstr=0;

clear();

sprintf(strsql,"select distinct tabn from Zarp where datz >= '%04d-%02d-01' and \
datz <= '%04d-%02d-%02d' and prn = '1' and suma <> 0. order by tabn asc",
rp->gn,rp->mn,
rp->gk,rp->mk,rp->dk);

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
  iceb_t_soob(gettext("Не найдено ни одной записи !"));
  return(1);
 }
int kolstr1=0;


class iceb_fopen fil_prot;
char imaf_prot[64];
sprintf(imaf_prot,"otpp%d.lst",getpid());
if(fil_prot.start(imaf_prot,"w") != 0)
 return(1);

iceb_t_zagolov(gettext("Протокол хода расчёта отпускных"),1,rp->mn,rp->gn,rp->dk,rp->mk,rp->gk,fil_prot.ff);

class iceb_fopen fil_sp;
char imaf_sp[64];
sprintf(imaf_sp,"otsp%d.lst",getpid());
if(fil_sp.start(imaf_sp,"w") != 0)
 return(1);

iceb_t_zagolov(gettext("Расчёт отпускных"),1,rp->mn,rp->gn,rp->dk,rp->mk,rp->gk,fil_sp.ff);


if(rp->metka_ri == 0)
 fprintf(fil_sp.ff,"%s\n",gettext("Расчёт с использованием должностных окладов"));
if(rp->metka_ri == 1)
 fprintf(fil_sp.ff,"%s\n",gettext("Расчёт с использованием коэффициентов"));

if(rp->tabnom.getdlinna() > 1)
 fprintf(fil_sp.ff,"%s:%s\n",gettext("Табельный номер"),rp->tabnom.ravno());

if(rp->kod_podr.getdlinna() > 1)
 fprintf(fil_sp.ff,"%s:%s\n",gettext("Код подразделения"),rp->kod_podr.ravno());
 
class sprtabotp_dat dt;

class iceb_fopen fil;
char imaf[64];
sprintf(imaf,"otp%d.lst",getpid());
if(fil.start(imaf,"w") != 0)
 return(1);

fprintf(fil.ff,"\x1B\x33%c\n",30); /*Уменьшаем межстрочный интервал*/


double itogo=0.;
while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);
  
  if(proverka(rp->tabnom.ravno(),row[0],0,0) != 0)
   continue;

  sprintf(strsql,"select podr from Kartb where tabn=%d",atoi(row[0]));
  if(sql_readkey(&bd,strsql,&row1,&cur1) != 1)
   {
    sprintf(strsql,"%s-%s %d !",__FUNCTION__,gettext("Не найден табельный номер"),atoi(row[0]));
    iceb_t_soob(strsql);
   }
  else
   {
    if(proverka(rp->kod_podr.ravno(),row1[0],0,0) != 0)
     continue;
   }

  class sprtabotp_dat dt;
  
  class iceb_zarrsov svod_otvr; /*свод по отработанному времени*/

  if(kolstr1 > 1)
   fprintf(fil.ff,"\f");
   
  double suma=sprtabotp_r1tn(atoi(row[0]),rp,&dt,&svod_otvr,fil.ff,fil_prot.ff);
  itogo+=suma;
  
  printw("%6s %-*s %12.2f \n",row[0],
  iceb_tu_kolbait(40,dt.fio.ravno()),
  dt.fio.ravno(),suma);
  
  refresh();
  
  fprintf(fil_sp.ff,"%6s %-*s %12.2f\n",row[0],
  iceb_tu_kolbait(40,dt.fio.ravno()),
  dt.fio.ravno(),suma);
 }
printw("%*s:%12.2f\n",
iceb_tu_kolbait(47,gettext("Итого")),
gettext("Итого"),itogo);

fprintf(fil_sp.ff,"%*s:%12.2f\n",
iceb_tu_kolbait(47,gettext("Итого")),
gettext("Итого"),itogo);


podpis(fil.ff);
fil.end();

podpis(fil_prot.ff);
fil_prot.end();

podpis(fil_sp.ff);
fil_sp.end();

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus(gettext("Расчёт отпускных"));
oth->spis_imaf.plus(imaf_sp);
oth->spis_naim.plus(gettext("Итоговый список"));
oth->spis_imaf.plus(imaf_prot);
oth->spis_naim.plus(gettext("Протокол хода расчёта"));

for(int nom=0; nom < oth->spis_imaf.kolih(); nom++)
 iceb_t_ustpeh(oth->spis_imaf.ravno(nom),3);

OSTANOV();
 
return(0);

}
