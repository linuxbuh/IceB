/*$Id: dirdm.c,v 5.16 2013/09/26 09:43:29 sasa Exp $*/
/*13.07.2015	10.02.2009	Белых А.И.	dirdm.c
Ввод и корректировка списка драгоценных металлов
*/
#include        <errno.h>
#include        "buhg.h"

void  dirdm_kod(const char *kods,const char *kodn);
void	dirdm_ras(SQLCURSOR *cur);
int dirdm_pu(const char *kodavt);
void dirdm_sap(int metkar,const char *naimpoi);
void dirdm_vvod(const char *kodm);
int   dirdm_zap(VVOD *VV,const char *kodm);



int dirdm(int metkar, //0-работа 1-выбор из списка с корректировкой 2-чистый выбор
class iceb_tu_str *kodm,class iceb_tu_str *naim)
{
char		strsql[1024];
SQL_str		row;
int		kolstr=0;
int		K=0;
int		pozz=0,pozz1=0;
int		i=0,kom=0;
int		mspz=0;
int		prc;
int             Y=3;
int             X=0;
class iceb_tu_str koment("");
class iceb_t_mstr spis_menu(0);
class iceb_tu_str naimpoi("");
class iceb_tu_str kodvib("");
class iceb_tu_str naimvib("");

SQLCURSOR curr;
naimpoi.new_plus(naim->ravno());

if(metkar == 2)
 {
  Y=-1;
  X=-1;
  koment.new_plus(gettext("Выберите нужное и нажмите Enter"));
 }

//Создаем класс меню
class VVOD DANET(1);
class VVOD VVOD1(2);

naz1:;

short           kls=LINES-6;

sprintf(strsql,"select * from Uosdm");


class SQLCURSOR cur;

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

if(kolstr == 0)
 {
lll:;
  dirdm_sap(metkar,naimpoi.ravno());
  VVOD SOOB(1);
  SOOB.VVOD_SPISOK_add_MD(gettext("Не найдено ни одной записи !"));
  K=soobshw(&SOOB,stdscr,-1,-1,1,1);

  if(K == SFK2 || K == ENTER || K == FK3 || K == FK5 || K == FK6)
    goto lll;

  if(K == KEY_RESIZE)
   goto naz1;   

  goto vper;
 }

if(kolstr != 0 && pozz > kolstr)
  pozz = kolstr-1;

naz:;



cur.poz_cursor(pozz);
mspz=2;
pozz1=0;

spis_menu.spis_str.free_class();
for(i=0;i<=kls-1;)
 {
  if(cur.read_cursor(&row) == 0)
    break;
  pozz1++;

  if(naimpoi.getdlinna() > 1 && strstrm(row[1],naimpoi.ravno()) == 0)
   continue;
  
  sprintf(strsql,"%-3s|%-*.*s|%s",
  row[0],
  iceb_tu_kolbait(30,row[1]),iceb_tu_kolbait(30,row[1]),row[1],
  row[2]);
  
  i++;
  spis_menu.spis_str.plus(strsql);
  
  if(pozz1 == 1) /*Первая запись удовлетворяет условиям поиска*/
   mspz=1;

 }

if(i == 0)
 {
  if(kolstr != 0 && pozz > 0)
   {
    pozz--;
    goto naz;
   }
  goto lll;
 }  
if(kom >= i)
  kom=i-1;

if(kom < 0)
 kom=0;

if(i <=kls-1)
  spis_menu.spis_str.plus(gettext("Конец списка"));

dirdm_sap(metkar,naimpoi.ravno());

prc=(pozz+pozz1)*100/kolstr;
memset(strsql,'\0',sizeof(strsql));

kom=spis_menu.menu(kom,Y,X,NULL,strsql,prc,&K,koment.ravno(),COLS-21);


if(kom == -7) //Изменение границы экрана
  goto naz1;

if(kom == -3) /*Стрелка вниз*/
 {
  if(i <kls)
   {
    beep();
    goto naz;
   }
  for(i=0; i< mspz   ; )
   {
    pozz++;
    if(pozz >= kolstr)
     {
      pozz=kolstr-1;
      break;
     }
    cur.poz_cursor(pozz);
    if(cur.read_cursor(&row) == 0)
      break;
    if(naimpoi.getdlinna() > 1 && strstrm(row[1],naimpoi.ravno()) == 0)
     continue;
    i++;
   }
  kom=kls-1;
  goto naz;
 }
if(kom == -2) /*Стрелка вверх*/
 {
  for(i=0; i < 1 ; )
   {

    pozz--;

    if(pozz <= 0)
     {
      beep();
      pozz=0;
      goto naz;
     }

    cur.poz_cursor(pozz);
    cur.read_cursor(&row);
    if(naimpoi.getdlinna() > 1 && strstrm(row[1],naimpoi.ravno()) == 0)
     continue;
    i++;
   }

  goto naz;
 }
 
if(kom == -5) /*Страница вниз*/
 {
  if(pozz + kls == kolstr)
   {
    pozz=kolstr-1;
    beep();
    goto naz;  
   }
  if(i == kls)
    pozz+=pozz1;
  else
    beep();
  if(pozz >= kolstr)
   {
    pozz=kolstr-pozz1;
    beep();
    goto naz;  
   }
/*  
  printw("\n вперед pozz=%d kolstr=%ld\n",pozz,kolstr);
  OSTANOV();
*/
  goto naz;

 }

if(kom == -4) /*Страница вверх*/
 {
/*
  printw("\npozz=%d\n",pozz);
  refresh();
*/
  for(i=0; i<kls ; )
   {
    pozz--;

    if(pozz <= 0)
     {
      beep();
      pozz=0;
      goto naz;
     }

    cur.poz_cursor(pozz);
    if(cur.read_cursor(&row) == 0)
      break;
    if(naimpoi.getdlinna() > 1 && strstrm(row[1],naimpoi.ravno()) == 0)
     continue;
    i++;
   }
  goto naz;
 }

if(kom >= 0)
 {
  class iceb_tu_str str_vib(spis_menu.spis_str.ravno(kom));
  if(SRAV(gettext("Конец списка"),str_vib.ravno(),0) == 0)
     goto naz;

  if(pozz+kom >= kolstr)
   goto naz; /*Стали на конец списка*/

  polen(str_vib.ravno(),&kodvib,1,'|');
  polen(str_vib.ravno(),&naimvib,2,'|');

 }

vper:;

switch (K)
 {
  case FK10: /*Выход*/
  case ESC : /*Выход*/
     return(1);
     break;

  case FK1:  /*Получить подсказку*/
    GDITE();
    iceb_t_pdoc("uos5_10.txt");
    goto naz;

  case ENTER:
    if(metkar != 0)
     {
      kodm->new_plus(kodvib.ravno());
      naim->new_plus(naimvib.ravno());
      return(0);
     }      

  case SFK2:   /*Корректировать*/
    dirdm_vvod(kodvib.ravno());
    goto naz1;

  case FK2:  /*Ввести запись*/
  case PLU:
    dirdm_vvod("");
    goto naz1;

  case FK3:       /*Удалить запись*/

    DANET.VVOD_delete();
    DANET.VVOD_SPISOK_add_MD(gettext("Удалить ? Вы уверены ?"));

    if(danet(&DANET,2,stdscr) == 1)
     {
      if(dirdm_pu(kodvib.ravno()) != 0)
        goto naz;
      sprintf(strsql,"delete from Uosdm where kod=%s",kodvib.ravno());
      if(sql_zapis(strsql,0,0) == 0)
        goto naz1;
     }
    goto naz;

  case FK4:       /*Поиск*/

     helstr(LINES-1,0," F10 ",gettext("выход"),NULL);

     VVOD1.VVOD_delete();
     VVOD1.VVOD_SPISOK_add_MD(gettext("Введите образец для поиска"));
     
     naimpoi.new_plus("");
     
     vvod1(&naimpoi,40,&VVOD1,NULL,stdscr,-1,-1);

     GDITE();

     clear();
     kom=pozz=0;
     goto naz;

  case FK5:    //Распечатать
    dirdm_ras(&cur);
    goto naz;


  default:
    goto naz;
    break;
 }

return(1);


}
/**********************/
/*Ввод и корректировки*/
/**********************/

void dirdm_vvod(const char *kodm)
{
char		strsql[512];
SQL_str		row;
time_t		vremz=0;
int		ktozap=0;
int		N=1,K=0;
SQLCURSOR curr;

class VVOD VV(0);

VV.VVOD_SPISOK_add_data(10);//0
VV.VVOD_SPISOK_add_data(100);//1
VV.VVOD_SPISOK_add_data(20);//2


if(kodm[0] != '\0')
 {

  sprintf(strsql,"select * from Uosdm where kod=%s",kodm);
  if(sql_readkey(&bd,strsql,&row,&curr) != 1)
   {
    iceb_t_soob(gettext("Не найдена запись для корректировки!"));
    return;   
   }
  VV.VVOD_SPISOK_zapis_data(0,row[0]);
  VV.VVOD_SPISOK_zapis_data(1,row[1]);
  VV.VVOD_SPISOK_zapis_data(2,row[2]);

  ktozap=atoi(row[3]);
  vremz=atol(row[4]);

 }
else
 {
  sprintf(strsql,"%d",oprsk("Uosdm"));
  VV.VVOD_SPISOK_zapis_data(0,strsql);
 }


VV.VVOD_SPISOK_add_MD(gettext("Код драг-металла........"));//0
VV.VVOD_SPISOK_add_MD(gettext("Наименование............"));//1
VV.VVOD_SPISOK_add_MD(gettext("Единица измерения......."));//2


naz:;

VV.VVOD_delete_ZAG();
if(kodm[0] == '\0')
  VV.VVOD_SPISOK_add_ZAG(gettext("Ввод новой записи"));
else
  VV.VVOD_SPISOK_add_ZAG(gettext("Корректировка записи"));


kzvz(ktozap,vremz,&VV);

 helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("запись"),
"F3",gettext("реквизиты"),
"F10",gettext("выход"),NULL);


VV.vramka(-1,-1,0);

for(;;)
 {

  attron(VV.VVOD_return_cs(iceb_CFS)); 
  VV.vprintw(N,stdscr);
  
  K=getch();


  attroff(VV.VVOD_return_cs(iceb_CFS));
  attron(VV.VVOD_return_cs(iceb_CFM));

  VV.vprintw(N,stdscr);

  if(K == KEY_RESIZE)
    goto naz;


  if(K == KEY_MOUSE)
    K=mouseklic(&N,VV.PY,VV.PX,VV.MDLS,VV.KLST,stdscr);

   
  if(K == ENTER)
   {
    attroff(VV.VVOD_return_cs(iceb_CFM));
    attron(VV.VVOD_return_cs(iceb_CFV));
    VV.vprintw_clear(N,stdscr);

    VV.vgetstr(N,stdscr);
    
    attroff(VV.VVOD_return_cs(iceb_CFV));
    attron(VV.VVOD_return_cs(iceb_CFM));
/*
    if(N == 1) 
     {

     }
*/
    if(N == 0 && VV.VVOD_SPISOK_return_data(N)[0] != '\0')
     {
      if(SRAV(kodm,VV.data_ravno(N),0) != 0)
       {      
        sprintf(strsql,"select kod from Uosdm where kod=%d",VV.data_ravno_atoi(N));
        if(readkey(strsql) > 0)
         {
          iceb_t_soob(gettext("Такая запись уже есть!"));
         }
       }
     }

    attroff(VV.VVOD_return_cs(iceb_CFV));
    attron(VV.VVOD_return_cs(iceb_CFM));

    VV.vprintw(N,stdscr);
    N++;
   }
  if(K==SDN) /*Стрелка вниз*/
   {
    N++;
   }
  if(K==SUP) /*Стрелка вверх*/
   {
    N--;
   }
  if(N > VV.KLST-1)
   {
    N=0;
   }
  if(N < 0)
   {
    N=VV.KLST-1;
   }

  if(K == ESC || K == FK10)
   {
    attroff(VV.VVOD_return_cs(iceb_CFS));
    attron(VV.VVOD_return_cs(iceb_CFM));
    N=-1;
    break;
   }

  if(K == FK1) //Помощь
   {
    attroff(VV.VVOD_return_cs(iceb_CFS));
    GDITE();
//    prosf(strsql);
    clear();
    goto naz;
   }

  if(K == FK2 || K == PLU) //Записываем
   {
    if(dirdm_zap(&VV,kodm) != 0)
     goto naz;
    break;
   }

  if(K == FK3) //Просмотр единиц измерения
   {
    attroff(VV.VVOD_return_cs(iceb_CFS));
    class iceb_tu_str kod("");
    if(vibrek("Edizmer",&kod) == 0)
     {
      VV.VVOD_SPISOK_zapis_data(2,kod.ravno());
     } 

    goto naz;
   }
 }
attroff(VV.VVOD_return_cs(iceb_CFS));
attron(VV.VVOD_return_cs(iceb_CB));
clear();
refresh();
}

/*********************/
/*Запись             */
/*********************/
//Если вернули 0 - записали 1-нет

int   dirdm_zap(VVOD *VV,const char *kodm)
{
char	strsql[1024];

attroff(VV->VVOD_return_cs(iceb_CFS));
attron(VV->VVOD_return_cs(iceb_CFM));

if(VV->VVOD_SPISOK_return_data(0)[0] == '\0' ||\
   VV->VVOD_SPISOK_return_data(1)[0] == '\0')
 {
  iceb_t_soob(gettext("Не введены все обязательные реквизиты !"));
  return(1);
 }       
/*проверяем единицу измерения*/
if(VV->data_ravno(2)[0] != '\0')
 {
  sprintf(strsql,"select kod from Edizmer where kod='%s'",VV->data_ravno(2));
  if(readkey(strsql) <= 0)
   {
    sprintf(strsql,"%s %s!",gettext("Не найдена единица измерения"),VV->data_ravno(2));
    iceb_t_soob(strsql);
    return(1);
   }
 }

if(SRAV(kodm,VV->VVOD_SPISOK_return_data(0),0) != 0)
 {
  sprintf(strsql,"select kod from Uosdm where kod=%s",VV->VVOD_SPISOK_return_data(0));
  if(readkey(strsql) > 0)
   {
    iceb_t_soob(gettext("Такая запись уже есть !"));
    return(1);
   }

  if(kodm[0] != '\0')
   {
    VVOD DANET(1);
    DANET.VVOD_SPISOK_add_MD(gettext("Корректировать код ? Вы уверены ?"));

    if(danet(&DANET,2,stdscr) == 2)
      return(1);
   }

 }

time_t vrem;
time(&vrem);

if(kodm[0] == '\0') //Ввод новой записи
  sprintf(strsql,"insert into Uosdm values(%s,'%s','%s',%d,%ld)",
  VV->VVOD_SPISOK_return_data(0),  
  VV->data_ravno_filtr(1),  
  VV->data_ravno_filtr(2),
  iceb_t_getuid(),vrem);
else
  sprintf(strsql,"replace into Uosdm values(%s,'%s','%s',%d,%ld)",
  VV->VVOD_SPISOK_return_data(0),  
  VV->data_ravno_filtr(1),  
  VV->data_ravno_filtr(2),
  iceb_t_getuid(),vrem);


if(sql_zapis(strsql,0,0) != 0)
 return(1);

if(kodm[0] != '\0' && SRAV(kodm,VV->VVOD_SPISOK_return_data(0),0) != 0)
 dirdm_kod(kodm,VV->VVOD_SPISOK_return_data(0)); //Корректировка кода таблицах


return(0);


}


/**************************/
/*Шапка                   */
/**************************/

void dirdm_sap(int metkar,const char *naimpoi)
{
short		y=2;
short           x=COLS-20;



if(metkar != 2)
 {
  clear();
  printw(gettext("Ввод и корректировка драгоценных металлов"));
 }
if(metkar == 1)
 {
  move(1,0);
  printw(gettext("Найдите нужное и нажмите \"Enter\""));
 }

if(metkar < 2)
 {
  mvprintw(2,0,"   1              2                   3");
  mvprintw(y++,x,gettext("Наименование полей:"));
  mvprintw(y++,x,"1-%s",gettext("код"));
  mvprintw(y++,x,"2-%s",gettext("наименование"));
  mvprintw(y++,x,"3-%s",gettext("ед.измерения"));

 if(naimpoi[0] != '\0')
  {
   y++;
   attron(A_BLINK);
   mvprintw(y++,x,gettext("Поиск:"));
   mvprintw(y++,x,"%s",naimpoi);
   attroff(A_BLINK);
   
  }
  helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("запись"),
"F3",gettext("удалить"),
"F4",gettext("поиск"),
"F5",gettext("печать"),
"F10",gettext("выход"),NULL);
 }
 
if(metkar == 2)
  helstr(LINES-1,0,"Enter",gettext("выбор нужного"),
  "F10",gettext("выход"),NULL);

}

/********************************/
/*Проверка возможности удаления*/
/********************************/
//Если вернули 0-можно удалять 1-нет
int dirdm_pu(const char *kodavt)
{
char	strsql[512];
class SQLCURSOR cur;
SQL_str row;

sprintf(strsql,"select innom from Uosindm where kod=%s limit 1",kodavt);

if(readkey(strsql,&row,&cur) > 0)
 {
  sprintf(strsql,"%s %s!",gettext("Материал введён в инвентарный номер"),row[0]);
  iceb_t_soob(strsql);
  return(1);
 }


return(0);


}
/********************/
/*Распечатать       */
/********************/

void	dirdm_ras(SQLCURSOR *cur)
{
char	imaf[64];
FILE	*ff;
SQL_str row;

sprintf(imaf,"dmet%d.lst",getpid());

if((ff = fopen(imaf,"w")) == NULL)
 {
  error_op_nfil(imaf,errno,"");
  return;
 }
iceb_t_zagolov(gettext("Распечатка списка драгоценных металлов"),0,0,0,0,0,0,ff);


fprintf(ff,"\
-------------------------------------------------\n");
fprintf(ff,"\
Код |       Наименование           |Единица изм.|\n");
/*
1234 1234567890 1234567890 1234567890 1234567 1234567 123
*/

fprintf(ff,"\
-------------------------------------------------\n");

cur->poz_cursor(0);

while(cur->read_cursor(&row) != 0)
 {
  fprintf(ff,"%-4s %-*s %s\n",
  row[0],
  iceb_tu_kolbait(30,row[1]),row[1],
  row[2]);
 }
fprintf(ff,"\
-------------------------------------------------\n");

podpis(ff);
fclose(ff);

class spis_oth oth;
oth.spis_imaf.plus(imaf);
oth.spis_naim.plus(gettext("Распечатка списка драгоценных металлов"));
iceb_t_ustpeh(imaf,3);
iceb_t_rabfil(&oth,"");
}
/**************************/
/*Корректировка кода      */
/**************************/
void  dirdm_kod(const char *kods,const char *kodn)
{
char		strsql[512];

VVOD SOOBN(1);
SOOBN.VVOD_SPISOK_add_MD(gettext("Выполняется каскадная корректировка кода в таблицах !"));
soobsh(&SOOBN,-1,-1,1);

GDITE();

sprintf(strsql,"update Uosindm set kod=%s where kod=%s",kodn,kods);
if(sql_zapis(strsql,0,0) != 0)
 {
  msql_error(&bd,gettext("Ошибка корректировки записи !"),strsql);
  return;
 }

//Удаляем запись которую откорректировали
sprintf(strsql,"delete from Uosdm where kod=%s",kods);
sql_zapis(strsql,1,0);

}
