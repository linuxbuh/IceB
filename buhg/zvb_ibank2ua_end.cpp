/*$Id: zvb_ibank2ua_end.c,v 5.7 2013/05/17 14:56:19 sasa Exp $*/
/*25.01.2020	18.09.2008	Белых А.И.	zvb_ibank2ua_end.c
Концовка формирования данных для зачисления на карт-счета для системы iBank2 UA
*/
#include "buhg.h"


void zvb_ibank2ua_end(const char *imaf_lst,double sumai,FILE *ffr,FILE *ff)
{
zvb_end_lst(imaf_lst,sumai,ffr);

class iceb_tu_str shet_bp("29242000000110");


fprintf(ff,"\
AMOUNT=%.2f\n\
PAYER_BANK_ACCOUNT=%s\n\
COMMISSION_PERCENT=0.00\n",
sumai,
shet_bp.ravno());

fclose(ff);


}
