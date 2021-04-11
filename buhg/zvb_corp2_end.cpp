/*$Id: zvb_corp2_end.c,v 5.1 2014/08/31 06:18:19 sasa Exp $*/
/*20.11.2019	18.08.2014	Белых А.И.	zvb_corp2_end.c
Концовки файлов

"Державний ощадний банк України"
тел. 55-45-21 Алексей

Ім’я поля 	Тип		Довжина	Найменування реквізиту
NSC		Char		29	рахунок отримувача
SUMMA		Numeric		20.2	сума платежу (в грн)
FIO		Character	38	найменування отримувача
ID_KOD		Numeric		14	ідентифікатор код отримувача

*/
#include <errno.h>
#include "buhg.h"

void corp2_h(const char *imaf,long kolz);

void zvb_corp2_end(const char *imaf_dbf,const char *imaf_dbf_tmp,const char *imaf_lst,double sumai,int kolzap,FILE *ffr,FILE *ff_dbf_tmp)
{
class iceb_t_fioruk_rk ruk;
class iceb_t_fioruk_rk glbuh;

iceb_t_fioruk(1,&ruk);
iceb_t_fioruk(2,&glbuh);

fputc(26, ff_dbf_tmp);


zvb_end_lst(imaf_lst,sumai,ffr);
#if 0
##################################################3
fprintf(ffr,"\
------------------------------------------------------------------------------------------------------------\n");
fprintf(ffr,"%*s:%10.2f\n",iceb_tu_kolbait(95,"Разом"),"Разом",sumai);


fprintf(ffr,"\n\
%-*s_____________________ %s\n\n\
%-*s_____________________ %s\n",
iceb_tu_kolbait(20,gettext("Руководитель")),
gettext("Руководитель"),
ruk.fio.ravno(),
iceb_tu_kolbait(20,gettext("Главный бухгалтер")),
gettext("Главный бухгалтер"),
glbuh.fio.ravno());

podpis(ffr); 

fclose(ffr);

##############################################################
#endif
fclose(ff_dbf_tmp);

perecod(2,imaf_dbf_tmp); /*Перекодировка*/

corp2_h(imaf_dbf,kolzap);
iceb_t_cat(imaf_dbf,imaf_dbf_tmp);

unlink(imaf_dbf_tmp);

}

/******************************/
/*Создаем заголовок файла dbf*/
/******************************/
void corp2_f(DBASE_FIELD *f,const char *fn,char  ft,int l1,int l2,
int *header_len,int *rec_len)
{

strncpy(f->name, fn,sizeof(f->name)-1);
f->type = ft;
f->length = l1;
f->dec_point = l2;
*header_len=*header_len+sizeof(DBASE_FIELD);
*rec_len=*rec_len+l1;
}

/***********************************************/
void corp2_h(const char *imaf,long kolz)
{
time_t		tmm;
struct  tm      *bf;
FILE		*ff;
#define		kolpol 4 //Количество колонок (полей) в dbf файле
DBASE_FIELD f[kolpol];
DBASE_HEAD  h;
int fd;
int i;
int header_len, rec_len;

if((ff = fopen(imaf,"w")) == NULL)
 {
  error_op_nfil(imaf,errno,"");
  return;
 }

memset(&h,'\0',sizeof(h));
memset(&f, '\0', sizeof(f));

h.version = 3;

time(&tmm);
bf=localtime(&tmm);

h.l_update[0] = bf->tm_year;       /* yymmdd for last update*/
h.l_update[1] = bf->tm_mon+1;       /* yymmdd for last update*/
h.l_update[2] = bf->tm_mday;       /* yymmdd for last update*/

h.count = kolz;              /* number of records in file*/

header_len = sizeof(h);
rec_len = 0;
int shetshik=0;
corp2_f(&f[shetshik++],"NSC", 'C', 29, 0,&header_len,&rec_len);
corp2_f(&f[shetshik++],"SUMMA", 'N', 20, 2,&header_len,&rec_len);
corp2_f(&f[shetshik++],"FIO", 'C', 38, 0,&header_len,&rec_len);
corp2_f(&f[shetshik++],"ID_KOD", 'N', 14, 0,&header_len,&rec_len);


h.header = header_len + 1;/* length of the header
                           * includes the \r at end
                           */
h.lrecl= rec_len + 1;     /* length of a record
                           * includes the delete
                           * byte
                          */
/*
 printw("h.header=%d h.lrecl=%d\n",h.header,h.lrecl);
*/


fd = fileno(ff);

if(write(fd, &h, sizeof(h)) < 0)
 {
  printw("\n%s-%s\n",__FUNCTION__,strerror(errno));
  OSTANOV();
 }
for(i=0; i < kolpol; i++) 
 {
  if(write(fd, &f[i], sizeof(DBASE_FIELD)) < 0)
   {
    printw("\n%s-%s\n",__FUNCTION__,strerror(errno));
    OSTANOV();
   }
 }
fputc('\r', ff);

fclose(ff);

}
