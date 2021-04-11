/*$Id:$*/
/*23.10.2019	22.10.2019	Белых А.И.	zvb_end_lst.cpp
Коцовка распечатки перечисления на карт счета в банк
*/
#include "buhg.h"

void zvb_end_lst(const char *imaf,double sumai,FILE *ff)
{
class iceb_t_fioruk_rk ruk;
class iceb_t_fioruk_rk glbuh;

iceb_t_fioruk(1,&ruk);
iceb_t_fioruk(2,&glbuh);

fprintf(ff,"ICEB_LST_END\n");
fprintf(ff,"\
------------------------------------------------------------------------------------------------------------\n");
fprintf(ff,"%*s:%10.2f\n",iceb_tu_kolbait(96,"Разом"),"Разом",sumai);


fprintf(ff,"\n\
%-*s_____________________ %s\n\n\
%-*s_____________________ %s\n",
iceb_tu_kolbait(20,gettext("Руководитель")),
gettext("Руководитель"),
ruk.fio.ravno(),
iceb_tu_kolbait(20,gettext("Главный бухгалтер")),
gettext("Главный бухгалтер"),
glbuh.fio.ravno());

podpis(ff);
fclose(ff);

class iceb_rnl_c rh;
iceb_t_ustpeh(imaf,3,&rh.orient);
iceb_t_rnl(imaf,&rh,&zvb_head_lst);

}
