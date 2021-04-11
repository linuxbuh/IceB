/*$Id: zvb_ibank2ua_start.c,v 5.16 2014/04/30 06:12:27 sasa Exp $*/
/*25.10.2019	17.10.2019	Белых А.И. 	zvb_elpay_start.c
Начало формирования данных для зачисления на карт-счета для системы нтернет ELPay 
Если вернули 0-шапка сформирована
             1-нет
*/

#include <errno.h>
#include "buhg.h"



int zvb_elpay_start(char *imaf_lst,//имя файла распечатки
char *imaf, //имя файла в электронном виде
char *imaf_dbf,
class iceb_tu_str *edrpou, /*ЕДРПОУ*/
const char *kod00,
FILE **ff_csv,  //Файл в электонном виде
FILE **ff_lst, //Файл распечатки
FILE **ff_dbf)
{
class zvb_menu_lst_r rm;

if(zvb_menu_lst(&rm) != 0)
 return(1);

sprintf(imaf,"elpay.csv");
if((*ff_csv = fopen(imaf,"w")) == NULL)
 {
  error_op_nfil(imaf,errno,"");
  return(1);
 }

sprintf(imaf_lst,"elpayua%d.txt",getpid());
if((*ff_lst = fopen(imaf_lst,"w")) == NULL)
 {
  error_op_nfil(imaf_lst,errno,"");
  return(1);
 }

sprintf(imaf_dbf,"elpayua.dbf");
if((*ff_dbf = fopen(imaf_dbf,"w")) == NULL)
 {
  error_op_nfil(imaf_dbf,errno,"");
  return(1);
 }

zvb_start_lst(&rm,*ff_lst);
class iceb_rnl_c rd;
zvb_head_lst(&rd,NULL,*ff_lst);


class iceb_tu_str shet_sp("");
SQL_str row;
class SQLCURSOR cur;
char strsql[512];
/*узнаём реквизиты организации*/
sprintf(strsql,"select kod from Kontragent where kodkon='%s'",kod00);
if(readkey(strsql,&row,&cur) == 1)
 {
  edrpou->new_plus(row[0]);
 }


return(0);
}
