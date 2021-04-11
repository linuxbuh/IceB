/*$Id: zvb_kievrus_start.c,v 5.6 2013/09/26 09:43:46 sasa Exp $*/
/*21.10.2019	15.10.2009	Белых А.И.	zvb_kievrus_start.c
Меню для начала 
*/

#include        <errno.h>
#include    "buhg.h"

int zvb_kievrus_start(char *imaf_lst,//имя файла распечатки
char *imaf_dbf, 
char *imaf_dbf_tmp, //имя файла в электронном виде
FILE **ff,  //Файл в электонном виде
FILE **ff_lst) //Файл распечатки
{

class zvb_menu_lst_r rm;

if(zvb_menu_lst(&rm) != 0)
 return(1);

sprintf(imaf_dbf,"kievrusbank.dbf");
sprintf(imaf_dbf_tmp,"kievrus%d.tmp",getpid());

if((*ff = fopen(imaf_dbf_tmp,"w")) == NULL)
 {
  error_op_nfil(imaf_dbf_tmp,errno,"");
  return(1);
 }

sprintf(imaf_lst,"kievrus%d.txt",getpid());
if((*ff_lst = fopen(imaf_lst,"w")) == NULL)
 {
  error_op_nfil(imaf_lst,errno,"");
  return(1);
 }


zvb_start_lst(&rm,*ff_lst);
class iceb_rnl_c rd;
zvb_head_lst(&rd,NULL,*ff_lst);

return(0);

}

