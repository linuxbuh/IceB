/*$d:$*/
/*17.10.2019	17.10.2019	Белых А.И.	zvb_elpay.cpp
Система нтернет ELPay 
 Імпорт зарплатної відомості виконується із зовнішнього файлу. 
Файл зарплатної відомості повинен бути формату CSV або MS Excel 2003. 
Сума вказується в копійках. 
1) Формат файлу CSV: текстовий, роздільник - крапка з комою, формат в таблиці 2. 
------------------------------------------------------------------------
№ поля        Поле          Формат      Приклад       Обов'язковість 
------------------------------------------------------------------------
1 ІПН                       NUMBER(10)    3041319127    Так
2 Номер карткового рахунку  NUMBER(14)    262521332     Так
3 Сума                      NUMBER (20)   123,45        Так
4 Валюта                    NUMBER (3)    980           Так
5 ЄДРПОУ                    VARCHAR (10)  33349855      Так
6 ПІБ                       VARCHAR (250) Балаганов А.С. Немає

Файл - довільне, розширення - csv, CHARACTER SET - WIN1251. 
Приклад вмісту файлу: 
3041319127;262521332;12345;980;33349855;Балаганов А.
6576586768;675446356;98720;980;33349855;Петров І.Н.
9879987989;908098678;86930;980;33349855;
8763457659;654768764;87666;980;33349855;Ніколаєнко С.З.




*/

#include        <errno.h>
#include <math.h>
#include    "buhg.h"


void zvb_elpay(short prn,const char *zapros,struct dirnupoi *poisk,const char *kod00)
{
char strsql[512];
char imaf_csv[64];
char imaf_lst[64];
char imaf_dbf[64];
int   kolstr;
FILE *ff_csv;
FILE *ff_lst;
FILE *ff_dbf;
SQLCURSOR cur;
SQLCURSOR cur1;
SQLCURSOR curr;
SQL_str row,row1;
class iceb_tu_str edrpou("");






if((kolstr=cur.make_cursor(&bd,zapros)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),zapros);
  return;
 }

if(zvb_elpay_start(imaf_lst,imaf_csv,imaf_dbf,&edrpou,kod00,&ff_csv,&ff_lst,&ff_dbf) != 0)
 return;


class iceb_tu_str kartshet("");
class iceb_tu_str inn("");
int kolstr1=0;
class iceb_tu_str fio("");
double suma=0.;
double sumai=0.;
int nomer_zap=0;

while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);
  if(poidirnu(row,poisk,&fio) != 0)
     continue;
  suma=atof(row[4]);

  if(fabs(suma) < 0.01)
   continue;

  if(prn == 2)
    suma*=(-1);   

    
  sprintf(strsql,"select fio,bankshet,inn from Kartb where tabn=%s",row[1]);
  if(sql_readkey(&bd,strsql,&row1,&cur1) == 1)
   {
    kartshet.new_plus(row1[1]);
    inn.new_plus(row1[2]);
   }
  else
   {
    kartshet.new_plus("");
    inn.new_plus("");
   }
  if(kartshet.ravno()[0] == '\0')
   {
    VVOD SOOB(1);
    SOOB.VVOD_SPISOK_add_MD(gettext("Не введён счёт !"));
    sprintf(strsql,"%s:%s %s",gettext("Табельный номер"),row[1],fio.ravno());
    SOOB.VVOD_SPISOK_add_MD(strsql);
    soobshw(&SOOB,stdscr,-1,-1,0,1);
    continue;
   }


  sumai+=suma;

  zvb_elpay_str(&nomer_zap,row[1],fio.ravno(),kartshet.ravno(),inn.ravno(),suma,edrpou.ravno(),ff_csv,ff_lst,ff_dbf);

 }

zvb_elpay_end(imaf_lst,imaf_csv,imaf_dbf,sumai,nomer_zap,ff_lst,ff_csv,ff_dbf);



class spis_oth oth;
oth.spis_imaf.plus(imaf_lst);
oth.spis_naim.plus(gettext("Распечатка документов для перечисления на карт-счета"));

oth.spis_imaf.plus(imaf_csv);
oth.spis_naim.plus(gettext("Документ в электронном формате csv"));

oth.spis_imaf.plus(imaf_dbf);
oth.spis_naim.plus(gettext("Документ в электронном формате dbf"));


iceb_t_rabfil(&oth,"");

}

