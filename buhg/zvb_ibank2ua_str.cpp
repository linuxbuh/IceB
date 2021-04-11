/*$Id: zvb_ibank2ua_str.c,v 5.7 2013/08/13 05:50:04 sasa Exp $*/
/*22.10.2019	18.09.2008	Белых А.И.	zvb_ibank2ua_str.c
Строка записи перечисленой на карт-счёт зарплаты для системы iBank2 UA
*/
#include "buhg.h"

void zvb_ibank2ua_str(int *nomer_zap,
const char *tabnom,
const char *fio,
const char *shet_b,
const char *inn,
double suma,
FILE *ff,FILE *ffr)
{
*nomer_zap+=1;
/*****************
fprintf(ffr,"%5d %6s %-*.*s %10s %-29s %10.2f\n",
*nomer_zap,
tabnom,
iceb_tu_kolbait(30,fio),iceb_tu_kolbait(30,fio),fio,
inn,shet_b,suma);
*****************/
zvb_str_lst(*nomer_zap,fio,shet_b,suma,inn,tabnom,ffr);


fprintf(ff,"\
CARD_HOLDERS.%d.CARD_NUM=%s\n\
CARD_HOLDERS.%d.CARD_HOLDER=%s\n\
CARD_HOLDERS.%d.STATUS=0\n\
CARD_HOLDERS.%d.CARD_HOLDER_INN=%s\n\
CARD_HOLDERS.%d.AMOUNT=%.2f\n\
CARD_HOLDERS.%d.STAFF_ID=%s\n\
CARD_HOLDERS.%d.RESIDENT_FLAG=1\n",
*nomer_zap-1,
shet_b,
*nomer_zap-1,
fio,
*nomer_zap-1,
*nomer_zap-1,
inn,
*nomer_zap-1,
suma,
*nomer_zap-1,
shet_b,
*nomer_zap-1);

}
