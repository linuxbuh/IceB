/*$Id: iceb_tu_str.h,v 5.24 2013/11/24 08:23:08 sasa Exp $*/
/*03.03.2017	03.12.2003  	Белых А.И.	iceb_tu_str.h
Динамическая символьная строка
*/

class iceb_tu_str
 {
  private:
  char *stroka;
  char *stroka_filtr; //Строка в которую записывется отфильтрованная или перекодированная "stroka"
  int  dlinna;
  public:

  iceb_tu_str(); //Конструктор
  iceb_tu_str(const char*); //Конструктор c записью строки
  iceb_tu_str(int); //Конструктор c записью числа
  ~iceb_tu_str(); //Деструктор

  void free_class();     //Освоботить память (сделать free)
  int  ins(char simv,int poz); /*вставить символ в строку*/
  int  plus(const char*);  //Записать с увеличением размера
  int  plus(const char*,const char*);  //Записать с увеличением размера
  int  plus(const char*,int);  //Записать только указанное количество символов
  int  z_plus(const char*);  //Записать с увеличением размера, вставить зяпятую перед строкой если запись не первая
  int  z_plus(int);  //Записать с увеличением размера, вставить зяпятую перед строкой если запись не первая
  int  plus_ps(const char*); //Записать с символом перевода строки в конце
  int  plus(double); //Записать число в символьном виде
  int  plus(double,int kolznakpz); //Записать число в символьном виде с указанным количеством знаков после запятой
  int  plus(int); //Записать число в символьном виде
  int  ps_plus(const char*); //Записать с символом перевода строки в начале
  int  ps_plus(int); //Записать с символом перевода строки в начале
  void plus_sqldata(const char*); //Записать дату преобразовав её из SQL формата в нормальное представление
  void plus_tek_dat(); //Записать текущюю дату
  int  plus_v_spisok(const char*); //Записать с предварительной вставкой перевода строки все записи кроме первой
  void plus_data(short,short,short); //Записать дату в символьном виде
  void new_plus_data(short,short,short); //Записать дату в символьном виде удалив предыдущюю строку
  void new_plus_sqldata(const char*); //Записать дату, предварительно удалив предыдущее значение,преобразовав её из SQL формата в нормальное представление
  void new_plus_tek_dat(); //Записать текущюю дату предватительно удалив предыдущюю информацию если она была введена
  int  new_plus(const char*);   //Освободить ранее выделенную память перед записью нового значения  
  int  new_plus(const char*,int);   //Освободить ранее выделенную память перед записью нового значения  записать только указанное количество символов
  int  new_plus_ps(const char*);   //Освободить ранее выделенную память перед записью нового значения и добавить перевод строки в конце
  int  new_plus(int); //Удалить предыдущее , Записать число в символьном виде
  int  new_plus(double); //Удалить предыдущее ,Записать число в символьном виде
  int  new_plus(double,int kolznakpz); //Удалить предыдущее ,Записать число в символьном виде с указанным количеством знаков после запятой
  const char *ravno();       //Получить адрес строки 
  const char *ravno_filtr();       //Получить адрес отфильтованной строки (для фильтрации SQL-запросов к базе данных)
  const char *ravno_filtr_dpp();       //Получить адрес отфильтованной строки (для фильтрации SQL-запросов к базе данных) до первого пробела в строке
  const char *ravno_dpp();       //Получить адрес до первого пробела в строке
  const char *ravno_filtr_xml(); /*Отфильтровать строку для xml файла*/
  const char *ravno_shell_quote(); /*Квотирует строку так чтобы оболочка (/bin/sh) интерпретировала её как unquoted_string.*/

  int  ravno_pr();        //Если первый символ '+' вернуть 1, если '-' вернуть 2 , иначе возвращаем 0
  double ravno_atof(); //Получить в виде double
  const char *ravno_dtczp(); /*получить число в виде символьной строки с запятоай вместо точки*/
  int  ravno_atoi(); //Получить в виде int
  short ravno_god(); //Получить год если введена дата
  const char *ravno_mes_god(); //Получить строку даты мм.гг
  long int  ravno_atol(); //Получить в виде long int
  const char *ravno_sqldata(); //Получить дату преобразовав дд.мм.гггг -> гггг-мм-дд (если строка содержит дату)
  const char *ravno_fsqldata(); //Получить дату преобразовав  SQL DATE - гггг-мм-дд -> дд.мм.гггг (если строка содержит дату)
  const char *ravno_xmldata(); //Получить дату преобразовав дд.мм.гггг -> ддммгггг
  const char *ravno_time(); //Получить строку времени в формате час:минута:секунда
  const char *ravno_time_end(); //Получить строку времени в формате час:минута:секунда. Если строка пустая то выдать 24:00:00
  const char *ravno_udsimv(const char *strsimv); /*Получить строку с удалением указанных символов- символы будут удалены гдебы они не находились */
  void udsimv(const char *strsimv); /*удалить символы в строке */
  void ud_lp(); /*Удалить лидирующие пробелы*/
  const char *ravno_tolower(); /*Получить строку преобразованную в маленькие буквы*/
  const char *ravno_toupper(); /*Получить строку преобразованную в большие буквы*/
//  const char *ravno_imaf(); //Получить строку в виде имени для файла (заменить все русские буквы на латинские и исключить все спец символы)
  int getdlinna();  //Получить количество байт в строке "stroka"
  int prov_dat(); //Проверить правильность ввода даты д.м.г
  int prov_dat1(); //Проверить правильность ввода даты м.г
  int find(char*,char); //Вернуть номер в списке разделённых заданным символом
  int find(int,char); //Вернуть номер в списке разделённых заданным символом
 };








