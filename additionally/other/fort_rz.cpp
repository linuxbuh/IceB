/*11.03.2020	27.11.2016	Белых А.И.	fort_rz.c
отчёт
*/
#include <buhl.h>

class fort_rz_rp
 {
  public:
   class iceb_tu_str datn;
   class iceb_tu_str datk;
   class iceb_tu_str pod;
   class iceb_tu_str hz;

  fort_rz_rp()
   {
    clear();
   }

  void clear()
   {
    hz.new_plus("");
    datn.new_plus("");
    datk.new_plus("");
    pod.new_plus("");
    
   }   
 };

void fort_rz_rm(class masiv_din_int *podr,class masiv_din_int *hz,class masiv_din_double *podr_hz_v,class masiv_din_double *podr_hz_sum,FILE *ff);
 

int fort_rz(class spis_oth *oth)
{
char strsql[1024];
int kom=0;
class fort_rz_rp rp;


class VVOD VV(0);
VV.VVOD_SPISOK_add_ZAG(gettext("Введите нужные реквизиты"));

VV.VVOD_SPISOK_add_MD(gettext("Дата начала.......(м.г).."));//0
VV.VVOD_SPISOK_add_MD(gettext("Дата конца........(м.г).."));//1
VV.VVOD_SPISOK_add_MD(gettext("Код подразделения..(,,).."));//2
VV.VVOD_SPISOK_add_MD(gettext("Шифр затрат........(,,).."));//3

VV.VVOD_SPISOK_add_data(rp.datn.ravno(),8);
VV.VVOD_SPISOK_add_data(rp.datk.ravno(),8);
VV.VVOD_SPISOK_add_data(rp.pod.ravno(),128);
VV.VVOD_SPISOK_add_data(rp.hz.ravno(),128);

naz:;

clear();
 
helstr(LINES-1,0,
//"F1",gettext("помощь"),
"F2/+",gettext("расчитать"),
"F4",gettext("очистить"),
"F10",gettext("выход"),NULL);

kom=VV.vvod(0,1,1,-1,-1);

rp.datn.new_plus(VV.data_ravno(0));
rp.datk.new_plus(VV.data_ravno(1));
rp.pod.new_plus(VV.data_ravno(2));
rp.hz.new_plus(VV.data_ravno(3));
switch(kom)
 {
  case FK1:   /*Помощь*/
   GDITE();
//   iceb_t_pdoc("buhg5_1.txt");
   goto naz;

  case ESC:
  case FK10:
   return(1);


  case FK2:
  case PLU:
     break;


  case FK4: /*Очистить меню*/
    VV.VVOD_clear_data();  
    goto naz;

  default:
     goto naz;
 }

if(rp.datn.prov_dat1() != 0)
 {
  iceb_t_soob(gettext("Не правильно введена дата начала!"));
  goto naz;
 }

if(rp.datk.getdlinna() <=1)
 {
  rp.datk.new_plus(rp.datn.ravno());
 }

if(rp.datk.prov_dat1() != 0)
 {
  iceb_t_soob(gettext("Не правильно введена дата конца!"));
  goto naz;
 }
 
SQL_str row,row1;
class SQLCURSOR cur,cur1;
int kolstr=0;
class masiv_din_int podr;
class masiv_din_int hz;

class masiv_din_double podr_hz_v;
class masiv_din_double podr_hz_sum;

class masiv_din_double podr_hz_sum_kf;
/*Узнаём подразделения*/
sprintf(strsql,"select distinct pd from Fortzn where datan >= '%s' and datan <= '%s'",rp.datn.ravno_sqldata(),rp.datk.ravno_sqldata());

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,__FUNCTION__,strsql);
 }

if(kolstr == 0)
 {
  iceb_t_soob(gettext("Не найдено ни одной записи!"));
  goto naz;
 }
 
while(cur.read_cursor(&row) != 0)
 {

  if(proverka(rp.pod.ravno(),row[0],0,0) != 0)
   continue;
   
  podr.plus(atoi(row[0]));
 }

/*Узнаём шифры затрат*/
sprintf(strsql,"select distinct hz from Fortzn where datan >= '%s' and datan <= '%s'",rp.datn.ravno_sqldata(),rp.datk.ravno_sqldata());

if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,__FUNCTION__,strsql);
 }

while(cur.read_cursor(&row) != 0)
 {
  if(proverka(rp.hz.ravno(),row[0],0,0) != 0)
   continue;
  hz.plus(atoi(row[0]));
 }

/*создаём массив*/
//sprintf(strsql,"select hz,pd,nv,rc,kol  from Fortzn where datan >= '%s' and datan <= '%s'",rp.datn.ravno_sqldata(),rp.datk.ravno_sqldata());
sprintf(strsql,"select datan,nn,pd,hz,pd,nv,rc,kol  from Fortzn where datan >= '%s' and datan <= '%s'\
 order by datan asc,nn asc,pd asc",rp.datn.ravno_sqldata(),rp.datk.ravno_sqldata());
if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,__FUNCTION__,strsql);
 }

podr_hz_v.make_class(podr.kolih()*hz.kolih());
podr_hz_sum.make_class(podr.kolih()*hz.kolih());

podr_hz_sum_kf.make_class(podr.kolih()*hz.kolih());

double vrem=0.;
double suma=0.;
int nomer_pod=0;
int nomer_hz=0;
int kol_podr=podr.kolih();
float kf=0.;
char st_dok[128];
memset(st_dok,'\0',sizeof(st_dok));
int kol_kf0=0.;

while(cur.read_cursor(&row) != 0)
 {
  if(proverka(rp.pod.ravno(),row[4],0,0) != 0)
   continue;
  if(proverka(rp.hz.ravno(),row[3],0,0) != 0)
   continue;
  
  
  vrem=atof(row[5])*atof(row[7])/60.;
  suma=atof(row[6])*atof(row[7])/100.;          

  nomer_pod=podr.find(row[4]);  
  nomer_hz=hz.find(row[3]);
  
  podr_hz_v.plus(vrem,nomer_hz*kol_podr+nomer_pod);
  podr_hz_sum.plus(suma,nomer_hz*kol_podr+nomer_pod);
  
  
  sprintf(strsql,"%s|%s|%s",row[0],row[1],row[2]);

  if(SRAV(st_dok,strsql,0) != 0)
   {
    /*узнаём коэффициент*/
    sprintf(strsql,"select kf from Forthn where datan='%s' and nn='%s' and pd='%s'",  row[0],row[1],row[2]);
    if(readkey(strsql,&row1,&cur1) == 1)
     kf=atof(row1[0]);
    else
     {
      kf=0.;
     }  

    if(kf == 0.)
     {
      printw("Коефіцієнт дорівнює нулю. Документ %s, дата %s, підрозділ %s\n",
      row[1],iceb_tu_datzap(row[0]),row[2]);
      kf=1.;
      kol_kf0++;
     }
   }
  podr_hz_sum_kf.plus(suma*kf,nomer_hz*kol_podr+nomer_pod);

  sprintf(st_dok,"%s|%s|%s",row[0],row[1],row[2]);
 }

char imaf[64];
sprintf(imaf,"fort%d.lst",getpid());

class iceb_fopen fil;
if(fil.start(imaf,"w") != 0)
 return(1);

char imafkf[64];
sprintf(imafkf,"fortkf%d.lst",getpid());
class iceb_fopen filkf;
if(filkf.start(imafkf,"w") != 0)
 return(1);
 
iceb_t_zagolov("Зведення по підрозділам и шифрам витрат",rp.datn.ravno(),rp.datk.ravno(),fil.ff);
fprintf(filkf.ff,"Розрахунок без використання коефіціентів.\n");

if(rp.pod.getdlinna() > 1)
 fprintf(fil.ff,"Підрозділ:%s\n",rp.pod.ravno());

if(rp.hz.getdlinna() > 1)
 fprintf(fil.ff,"Шифр витрат:%s\n",rp.hz.ravno());


fort_rz_rm(&podr,&hz,&podr_hz_v,&podr_hz_sum,fil.ff);



iceb_t_zagolov("Зведення по підрозділам и шифрам витрат",rp.datn.ravno(),rp.datk.ravno(),filkf.ff);
fprintf(filkf.ff,"Розрахунок з використанням коефіціентов.\n");

if(rp.pod.getdlinna() > 1)
 fprintf(filkf.ff,"Підрозділ:%s\n",rp.pod.ravno());

if(rp.hz.getdlinna() > 1)
 fprintf(filkf.ff,"Шифр витрат:%s\n",rp.hz.ravno());

fort_rz_rm(&podr,&hz,&podr_hz_v,&podr_hz_sum_kf,filkf.ff);


podpis(fil.ff);
fil.end();

podpis(filkf.ff);
filkf.end();


oth->spis_imaf.plus(imafkf);
oth->spis_naim.plus("Розрахунок з використанням коефіцієнтів");

oth->spis_imaf.plus(imaf);
oth->spis_naim.plus("Розрахунок без використання коефіцієнтів");

for(int nom=0; nom < oth->spis_imaf.kolih(); nom++)
  iceb_t_ustpeh(oth->spis_imaf.ravno(nom),3);

if(kol_kf0 > 0)
 OSTANOV();

return(0);
}
/*********************************************/
void fort_rz_rm(class masiv_din_int *podr,
class masiv_din_int *hz,
class masiv_din_double *podr_hz_v,
class masiv_din_double *podr_hz_sum,
FILE *ff)
{

int kol_podr=podr->kolih();

class iceb_tu_str liniq("-------");
for(int nom=0; nom <= kol_podr; nom++)
 liniq.plus("----------------------");

class iceb_tu_str liniq1;
for(int nom=0; nom <= kol_podr; nom++)
 liniq1.plus("----------------------");

fprintf(ff,"%s\n",liniq.ravno());
 
fprintf(ff," Шифр |");

for(int nom=0; nom < kol_podr;nom++)
  fprintf(ff,"%21d|",podr->ravno(nom));

fprintf(ff,"        Разом        |\n");

fprintf(ff,"витрат|%s\n",liniq1.ravno());

fprintf(ff,"      |");
for(int nom=0; nom <= kol_podr;nom++)
  fprintf(ff,"Норма часу|   Сума   |");

fprintf(ff,"\n");

fprintf(ff,"%s\n",liniq.ravno());

double vrem=0.;
double suma=0.;
double itogo[2];

for(int nom_hz=0; nom_hz < hz->kolih(); nom_hz++)
 {
  itogo[0]=itogo[1]=0.;
  
  fprintf(ff,"%6d|",hz->ravno(nom_hz));
  for(int nom_pod=0; nom_pod < kol_podr; nom_pod++)
   {
    vrem=podr_hz_v->ravno(nom_hz*kol_podr+nom_pod);
    suma=podr_hz_sum->ravno(nom_hz*kol_podr+nom_pod);
 
    if(suma != 0. || vrem != 0.)
     fprintf(ff,"%10.2f|%10.2f|",vrem,suma);
    else
     fprintf(ff,"%10s|%10s|","","");

    itogo[0]+=vrem;
    itogo[1]+=suma;
   }  
  fprintf(ff,"%10.2f|%10.2f|\n",itogo[0],itogo[1]);
 }

fprintf(ff,"%s\n",liniq.ravno());

fprintf(ff,"%*s|",iceb_tu_kolbait(6,"Разом"),"Разом");
for(int nom=0; nom < kol_podr; nom++)
 {
  fprintf(ff,"%10.2f|%10.2f|",podr_hz_v->suma_kol(hz->kolih(),kol_podr,nom),podr_hz_sum->suma_kol(hz->kolih(),kol_podr,nom));
 }
fprintf(ff,"%10.2f|%10.2f|\n",podr_hz_v->suma(),podr_hz_sum->suma());
fprintf(ff,"%s\n",liniq.ravno());


}


