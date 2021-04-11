/*$Id: zvb_ibank2ua_start.c,v 5.16 2014/04/30 06:12:27 sasa Exp $*/
/*25.01.2020	18.09.2008	Белых А.И. 	zvb_ibank2ua_start.c
Начало формирования данных для зачисления на карт-счета для системы iBank2 UA
Если вернули 0-шапка сформирована
             1-нет
*/

#include        <errno.h>
#include    "buhg.h"



int zvb_ibank2ua_start(char *imafr,//имя файла распечатки
char *imaf, //имя файла в электронном виде
const char *kod00,
FILE **ff,  //Файл в электонном виде
FILE **ffr) //Файл распечатки
{

class zvb_menu_lst_r rm;

if(zvb_menu_lst(&rm) != 0)
 return(1);
 
sprintf(imaf,"ibank2ua.txt");
if((*ff = fopen(imaf,"w")) == NULL)
 {
  error_op_nfil(imaf,errno,"");
  return(1);
 }
sprintf(imafr,"ibank2ua%d.txt",getpid());
if((*ffr = fopen(imafr,"w")) == NULL)
 {
  error_op_nfil(imafr,errno,"");
  return(1);
 }

zvb_start_lst(&rm,*ffr);

class iceb_rnl_c rd;
zvb_head_lst(&rd,NULL,*ffr);


class iceb_tu_str naim_kontr("");
class iceb_tu_str mfo("");
class iceb_tu_str naim_bank("");
class iceb_tu_str shet_sp("");
SQL_str row;
class SQLCURSOR cur;
char strsql[512];
/*узнаём реквизиты организации*/
sprintf(strsql,"select naikon,naiban,mfo,nomsh from Kontragent where kodkon='%s'",kod00);
if(readkey(strsql,&row,&cur) == 1)
 {
  naim_kontr.new_plus(row[0]);
  naim_bank.new_plus(row[1]);
  mfo.new_plus(row[2]);
  shet_sp.new_plus(row[3]);
 }

fprintf(*ff,"\
Content-Type=doc/pay_sheet\n\n\
DATE_DOC=%02d.%02d.%04d\n\
NUM_DOC=%s\n\
CLN_NAME=%s\n\
PAYER_BANK_MFO=%s\n\
PAYER_BANK_NAME=%s\n\
PAYER_ACCOUNT=%s\n\
BEGIN_MONTH=%02d\n\
BEGIN_YEAR=%d\n\
ONFLOW_TYPE=%s\n",
rm.d_pp,rm.m_pp,rm.g_pp,
rm.nomer_pp.ravno(),
naim_kontr.ravno(),
mfo.ravno(),
naim_bank.ravno(),
shet_sp.ravno(),
rm.mz,
rm.gz,
rm.vid_nah.ravno());





return(0);
}
