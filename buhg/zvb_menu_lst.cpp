/*$Id:$*/
/*24.10.2019	23.10.2019	Белых А.И.	zvb_menu_lst.cpp
Меню для вводa данных которые печатаются в заголовке распечатки 
*/
#include "buhg.h"

int zvb_menu_lst(class zvb_menu_lst_r *rm)
{
static class iceb_tu_str data_pp("");
static class iceb_tu_str nomer_pp("");
static class iceb_tu_str data_zp("");
static class iceb_tu_str vid_nah(gettext("Заработная плата и авансы"));


VVOD VV(0);
VV.VVOD_SPISOK_add_ZAG(gettext("Формирование файла для импорта в систему \"Клиент Банк\""));

VV.VVOD_SPISOK_add_MD(gettext("Дата документа.....(д.м.г)."));
VV.VVOD_SPISOK_add_MD(gettext("Номер документа............"));
VV.VVOD_SPISOK_add_MD(gettext("Заработная плата за (м.г).."));
VV.VVOD_SPISOK_add_MD(gettext("Вид начисления............."));

VV.VVOD_SPISOK_add_data(data_pp.ravno(),11);
VV.VVOD_SPISOK_add_data(nomer_pp.ravno(),16);
VV.VVOD_SPISOK_add_data(data_zp.ravno(),8);
VV.VVOD_SPISOK_add_data(vid_nah.ravno(),64);


naz:;

helstr(LINES-1,0,\
//"F1",gettext("помощь"),
"F2/+",gettext("расчитать"),
"F4",gettext("очистить"),
"F10",gettext("выход"),NULL);


int kom=VV.vvod(0,1,1,-1,-1);

move(LINES-1,0);

data_pp.new_plus(VV.VVOD_SPISOK_return_data(0));
nomer_pp.new_plus(VV.VVOD_SPISOK_return_data(1));
data_zp.new_plus(VV.VVOD_SPISOK_return_data(2));
vid_nah.new_plus(VV.VVOD_SPISOK_return_data(3));

rm->vid_nah.new_plus(vid_nah.ravno());
rm->nomer_pp.new_plus(nomer_pp.ravno());
switch(kom)
 {
  case FK1:   /*Помощь*/
   GDITE();
//   prosf(strsql);
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

if(VV.data_ravno(0)[0] != '\0')
if(rsdat(&rm->d_pp,&rm->m_pp,&rm->g_pp,VV.data_ravno(0),1) != 0)
 {
  iceb_t_soob(gettext("Не правильно введена дата документа!"));
  goto naz;
   
 }

if(VV.data_ravno(2)[0] != '\0')
if(rsdat1(&rm->mz,&rm->gz,VV.data_ravno(2)) != 0)
 {
  iceb_t_soob("Не правильно введена дата зарплаты!");
  goto naz;
   
 }

return(0);
}

