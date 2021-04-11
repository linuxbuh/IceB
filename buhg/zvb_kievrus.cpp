/*$Id: zvb_kievrus.c,v 5.5 2013/08/13 05:50:04 sasa Exp $*/
/*30.06.2013	11.10.2011	Белых А.И.	zvb_kievrus.c
Формирование файлов для зачисления на картсчета зарплаты

Бескревный Сергей Николаевич
начальник отдела ИТ
отделения "Винницкая региональная дирекция" ПАТ "Банк <<Киевская Русь>>"
тел.: (0432) 552813
e-mail: beskrevniy.s.n@kruss.kiev.ua
              vinnica@kruss.kiev.ua


Структура         DKF.dbf
SBK_NUM	S19			N карт.рах.
SBK_SUM	N12.2			Нараховано
SBK_INN	S10			Ідентифікаційний код
SBK_FIO	S50			Фамілія Ім'я по Батькові
*/
#include    "buhg.h"





void zvb_kievrus(short prn,const char *zapros,struct dirnupoi *poisk)
{
char strsql[512];
char imafr[64];
char imaf_dbf[64];
char imaf_dbf_tmp[64];

FILE  *ff_dbf_tmp;
FILE  *ffr;
int   kolstr;


SQLCURSOR cur;
SQLCURSOR cur1;
SQLCURSOR curr;
SQL_str row,row1;
double suma=0.;



if((kolstr=cur.make_cursor(&bd,zapros)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),zapros);
  return;
 }
class iceb_tu_str fio("");

if(zvb_kievrus_start(imafr,imaf_dbf,imaf_dbf_tmp,&ff_dbf_tmp,&ffr) != 0)
 return;


int kolstr1=0;

class iceb_tu_str kartshet("");
double sumai=0.;
class iceb_tu_str inn("");

cur.poz_cursor(0);
int nomer_zap=0;
while(cur.read_cursor(&row) != 0)
 {
  strzag(LINES-1,0,kolstr,++kolstr1);
  if(poidirnu(row,poisk,&fio) != 0)
     continue;

  suma=atof(row[4]);
  if(suma == 0.)
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
    sprintf(strsql,"%s\n%s:%s %s",gettext("Не введён счёт !"),gettext("Табельный номер"),row[1],fio.ravno());
    iceb_t_soob(strsql);
    continue;
   }


  sumai+=suma;

  zvb_kievrus_str(++nomer_zap,fio.ravno(),kartshet.ravno(),suma,inn.ravno(),row[1],ff_dbf_tmp,ffr);


    
 }

zvb_kievrus_end(imafr,imaf_dbf,imaf_dbf_tmp,sumai,nomer_zap,ffr,ff_dbf_tmp);

class spis_oth oth;
oth.spis_imaf.plus(imafr);
oth.spis_naim.plus(gettext("Распечатка документов для перечисления на карт-счета"));

oth.spis_imaf.plus(imaf_dbf);
oth.spis_naim.plus(gettext("Документ в электронном формате"));


iceb_t_rabfil(&oth,"");

}
