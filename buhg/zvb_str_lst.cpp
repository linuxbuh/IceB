/*$Id:$*/
/*22.10.2019	22.10.2019	Белых А.И.	zvb_str_lst.c
*/
#include "buhg.h"



void zvb_str_lst(int nomerpp,const char *fio,
const char *nomersh,
double suma,
const char *inn,
const char *tabnom,
FILE *ff)
{

fprintf(ff,"\
%6d %-*s %-*.*s %-*s %-*s %11.2f\n",
nomerpp,
iceb_tu_kolbait(6,tabnom),tabnom,
iceb_tu_kolbait(40,fio),
iceb_tu_kolbait(40,fio),
fio,
iceb_tu_kolbait(29,nomersh),nomersh,
iceb_tu_kolbait(10,inn),inn,
suma);



}
