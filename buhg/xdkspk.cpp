/*$Id:$*/
/*02.09.2020	01.09.2020	Белых А.И.	xdkspk.cpp
Распечатка свода платежей по контрагентам
*/
#include        <errno.h>
#include        "buhg.h"

void xdkspk_hap(int nomstr,int *kolstr,FILE *ff);
void xdkspk_hap1(int nomstr,int *kolstr,FILE *ff);


int xdkspk(class spis_oth *oth)
{
short		kom;
short		dn=0,mn=0,gn=0;
short		dk=0,mk=0,gk=0;
char		imaf[64],imafi[64];
static class iceb_tu_str kor("");
long		kolstr=0,kolstr1=0;
SQL_str         row;
char		strsql[1024];
double		sum;
static class iceb_tu_str dat1("");
static class iceb_tu_str dat2("");
static class iceb_tu_str kodop("");
int kom1=0;
class iceb_tu_str kod("");

VVOD VV(0);
VVOD MENU(3);

VV.VVOD_SPISOK_add_MD(gettext("Дата начала..(д.м.г)..."));
VV.VVOD_SPISOK_add_MD(gettext("Дата конца...(д.м.г)..."));
VV.VVOD_SPISOK_add_MD(gettext("Код контрагента (,,)..."));
VV.VVOD_SPISOK_add_MD(gettext("Код операции....(,,)..."));

VV.VVOD_SPISOK_add_ZAG(gettext("Распечатка журнала платёжных документов"));

VV.VVOD_SPISOK_add_data(dat1.ravno(),11);
VV.VVOD_SPISOK_add_data(dat2.ravno(),11);
VV.VVOD_SPISOK_add_data(kor.ravno(),128);
VV.VVOD_SPISOK_add_data(kodop.ravno(),128);

naz:;

clear();

helstr(LINES-1,0,"F1",gettext("помощь"),
"F2/+",gettext("поиск"),
"F3",gettext("реквизиты"),
"F10",gettext("выход"),NULL);

kom=VV.vvod(0,1,1,-1,-1);

dat1.new_plus(VV.data_ravno(0));
dat2.new_plus(VV.data_ravno(1));
kor.new_plus(VV.data_ravno(2));
kodop.new_plus(VV.data_ravno(3));

switch(kom)
 {
  case FK1:   /*Помощь*/
   GDITE();
//   iceb_t_pdoc("dokum2_5.txt");
   goto naz;

  case FK10:
  case ESC:
    return(1);

  case FK3:
    MENU.VVOD_delete();
    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка контрагентов"));
    MENU.VVOD_SPISOK_add_MD(gettext("Просмотр списка операций"));
    MENU.VVOD_SPISOK_add_MD(gettext("Выход"));

    clearstr(LINES-1,0);
    kom1=0;        
    while(menu3w(stdscr,&MENU,&kom1,-1,-1,0) != 0);

    switch (kom1)
     {
      case 3 :
      case -1:
       goto naz;

      case 0 :
        clear();
        kod.new_plus(VV.data_ravno(2));
        if(vibrek("Kontragent",&kod) == 0)
         VV.data_plus(2,kod.ravno());
 
        break;

      case 1 :
        clear();
        kod.new_plus(VV.data_ravno(3));
        if(vibrek("Opltp",&kod) == 0)
         VV.data_plus(3,kod.ravno());
        break;

     }

    clear();
    goto naz;


  case FK2:
  case PLU:
    break;
  default:
    goto naz;
 }

if(rsdatp(&dn,&mn,&gn,VV.VVOD_SPISOK_return_data(0),&dk,&mk,&gk,VV.VVOD_SPISOK_return_data(1)) != 0)
  goto naz;
  
move(LINES-1,0);
GDITE();


class iceb_tu_str edrpou("");
class iceb_tu_str kod_kontr("");
class iceb_tu_str kod_kontr_pred("");
class iceb_tu_str naim_kontr("");

class SQLCURSOR cur;
class SQLCURSOR cur1;

sprintf(strsql,"select datd,nomd,suma,polu from Pltp where \
datd >= '%04d-%02d-%02d' and datd <= '%04d-%02d-%02d' order by plat asc",
//datd >= '%04d-%02d-%02d' and datd <= '%04d-%02d-%02d' order by plat asc,datd asc",
gn,mn,dn,gk,mk,dk);
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

class iceb_t_tmptab tabtmp;
const char *imatmptab={"xdkspk"};
char zaprostmp[512];
memset(zaprostmp,'\0',sizeof(zaprostmp));

sprintf(zaprostmp,"CREATE TEMPORARY TABLE %s (\
kk CHAR(10) NOT NULL DEFAULT '',\
datd DATE NOT NULL DEFAULT '0000-00-00',\
edrpou CHAR(20) NOT NULL DEFAULT '',\
nd CHAR(20) NOT NULL DEFAULT '',\
sum double(12,2) NOT NULL DEFAULT 0.,\
nk CHAR(255) NOT NULL DEFAULT '',\
INDEX(kk,datd)) ENGINE = MYISAM",imatmptab);

/*
0 kk     код контрагента
1 datd   дата плятёжки
2 edrpou ЕДРПОУ
3 nd     номер платёжки
4 sum    сумма платежа
5 nk     наименование контрагента
*/

if(tabtmp.create_tab(imatmptab,zaprostmp) != 0)
 {
  return(1);
 }  

while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);   
  
  if(proverka(kodop.ravno(),row[5],0,0) != 0)
    continue;

  polen(row[3],&kod_kontr,1,'#');
  if(proverka(kor.ravno(),kod_kontr.ravno(),0,0) != 0)
    continue;

  sum=atof(row[2]);
  
  polen(row[3],&naim_kontr,2,'#');

  polen(row[3],&edrpou,4,'#');

  sprintf(strsql,"insert into %s values ('%s','%s','%s','%s',%.2f,'%s')",
  imatmptab,
  kod_kontr.ravno(),
  row[0],
  edrpou.ravno(),
  row[1],
  sum,
  iceb_tu_sqlfiltr(naim_kontr.ravno()));
  
  sql_zapis(strsql,1,0);    

 }
 
sprintf(strsql,"select * from %s order by kk asc,datd asc",imatmptab);
if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return(1);
 }

sprintf(imaf,"spk%d.lst",getpid());
class iceb_fopen fil;
if(fil.start(imaf,"w") != 0)
 return(1);

sprintf(imafi,"spki%d.lst",getpid());
class iceb_fopen fili;
if(fili.start(imafi,"w") != 0)
 return(1);

iceb_t_zagolov(gettext("Свод по контрагентам"),1,mn,gn,dk,mk,gk,fil.ff);
iceb_t_zagolov(gettext("Итоговый свод по контрагентам"),1,mn,gn,dk,mk,gk,fili.ff);
if(kodop.ravno()[0] != '\0')
 {
  fprintf(fil.ff,"%s:%s\n",gettext("Код операции"),kodop.ravno());
  fprintf(fili.ff,"%s:%s\n",gettext("Код операции"),kodop.ravno());
 }
if(kor.ravno()[0] != '\0')
 {
  fprintf(fil.ff,"%s:%s\n",gettext("Код контрагента"),kor.ravno());
  fprintf(fili.ff,"%s:%s\n",gettext("Код контрагента"),kor.ravno());
 } 







int kol=0;

xdkspk_hap(1,&kol,fil.ff);
xdkspk_hap1(1,&kol,fili.ff);

double suma_po_kontr=0.;
double itogo=0.;
kolstr1=0;

while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);   
  
  
  
  if(SRAV(kod_kontr_pred.ravno(),row[0],0) != 0)
    {
     if(kod_kontr_pred.ravno()[0] != '\0')
      {
       fprintf(fil.ff,"%*s|%11.2f|\n",
       iceb_tu_kolbait(39,gettext("Итого по контрагенту")),
       gettext("Итого по контрагенту"),
       suma_po_kontr);

       fprintf(fili.ff,"%*.*s|%-*.*s|%11.2f|%s\n",
       iceb_tu_kolbait(10,kod_kontr_pred.ravno()),
       iceb_tu_kolbait(10,kod_kontr_pred.ravno()),
       kod_kontr_pred.ravno(),
       iceb_tu_kolbait(10,edrpou.ravno()),
       iceb_tu_kolbait(10,edrpou.ravno()),
       edrpou.ravno(),    
       suma_po_kontr,
       naim_kontr.ravno());
      }
     suma_po_kontr=0;
     kod_kontr_pred.new_plus(row[0]);
    }

  sum=atof(row[4]);
  suma_po_kontr+=sum;
  itogo+=sum;  
  edrpou.new_plus(row[2]);
  naim_kontr.new_plus(row[5]);
    
  fprintf(fil.ff,"%*.*s|%-*.*s|%s|%-*s|%11.2f|%s\n",
  iceb_tu_kolbait(10,row[0]),
  iceb_tu_kolbait(10,row[0]),
  row[0],
  iceb_tu_kolbait(10,row[2]),
  iceb_tu_kolbait(10,row[2]),
  row[2],    
  iceb_tu_datzap(row[1]),
  iceb_tu_kolbait(6,row[3]),row[3],
  sum,
  row[5]);


 }
fprintf(fili.ff,"%*.*s|%-*.*s|%11.2f|%s\n",
iceb_tu_kolbait(10,kod_kontr_pred.ravno()),
iceb_tu_kolbait(10,kod_kontr_pred.ravno()),
kod_kontr_pred.ravno(),
iceb_tu_kolbait(10,edrpou.ravno()),
iceb_tu_kolbait(10,edrpou.ravno()),
edrpou.ravno(),    
suma_po_kontr,
naim_kontr.ravno());

fprintf(fil.ff,"%*s|%11.2f|\n",
iceb_tu_kolbait(39,gettext("Итого по контрагенту")),
gettext("Итого по контрагенту"),
suma_po_kontr);

fprintf(fil.ff,"%*s|%11.2f|\n",
iceb_tu_kolbait(39,gettext("Общий итог")),
gettext("Общий итог"),
itogo);

fprintf(fili.ff,"%*s|%11.2f|\n",
iceb_tu_kolbait(21,gettext("Общий итог")),
gettext("Общий итог"),
itogo);

podpis(fil.ff);
fil.end();

podpis(fili.ff);
fili.end();

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus(gettext("Свод по контрагентам"));
oth->spis_imaf.plus(imafi);
oth->spis_naim.plus(gettext("Итоговый свод по контрагентам"));

kol=iceb_t_ustpeh(oth->spis_imaf.ravno(0),3);
iceb_t_rnl(oth->spis_imaf.ravno(0),kol,NULL,&xdkspk_hap);

kol=iceb_t_ustpeh(oth->spis_imaf.ravno(1),3);
iceb_t_rnl(oth->spis_imaf.ravno(1),kol,NULL,&xdkspk_hap1);

//OSTANOV();


return(0);
}
/*********************************************/
void xdkspk_hap(int nomstr,int *kolstr,FILE *ff)
{
*kolstr=4;
fprintf(ff,"%50s %s N%d\n","",gettext("Лист"),nomstr);
fprintf(ff,"\
-----------------------------------------------------------------------\n");
fprintf(ff,"%s\n",gettext("Код контр.|  EГРПОУ  |   Дата   |Ном.д.|   Cумма   |        Контрагент"));
/*
1234567890 1234567890 1234567890 123456 1234567890
*/

fprintf(ff,"\
-----------------------------------------------------------------------\n");
}
/*********************************************/
void xdkspk_hap1(int nomstr,int *kolstr,FILE *ff)
{
*kolstr=4;
fprintf(ff,"%50s %s N%d\n","",gettext("Лист"),nomstr);
fprintf(ff,"\
-----------------------------------------------------------------------\n");
fprintf(ff,"%s\n",gettext("Код контр.|  EГРПОУ  |   Cумма   |        Контрагент"));
/*
1234567890 1234567890 1234567890 123456 1234567890
*/

fprintf(ff,"\
-----------------------------------------------------------------------\n");
}
