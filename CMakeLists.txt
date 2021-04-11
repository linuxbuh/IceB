#cборка iceB

project(iceB)

cmake_minimum_required(VERSION 2.6)



message ("Начали построение")


#Используйте пакет pkg-config для обнаружения GTK + заголовки / файлы
#библиотеки
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0 ncursesw)

#Установка CMake использовать glib-2.0, указать компилятору, где искать
#заголовки и компоновщику где искать библиотеки
include_directories(${GLIB2_INCLUDE_DIRS})
link_directories(${GLIB2_LIBRARY_DIRS})

#Добавьте другие флаги компилятора
add_definitions(${GLIB2_CFLAGS_OTHER})


#set(MYSQLCLIENT_LIB mysqlclient) #расположение библиотеки mysqlclient
set(MYSQLCLIENT_LIB mariadbclient) #расположение библиотеки mariadb
set(MYSQLCLIENT_LIB mariadb) #расположение библиотеки mariadb
set(NCURSESW_LIB ncursesw)        #расположение библиотеки ncursesw


include_directories(./ buhg libbuh libutil libscr libsql /usr/include/mysql /usr/include/mariadb) # Расположение заголовочных файлов
#include_directories(./ buhg libbuh libutil libscr libsql) # Расположение заголовочных файлов


add_definitions(-Wall -O2 ) #добавить директивы компилятора. Здесь мы включаем оптимизацию O2 и разрешаем все предупреждающие сообщения.

add_subdirectory (./libscr)
add_subdirectory (./libsql)
add_subdirectory (./libutil)
add_subdirectory (./libbuh)
add_subdirectory (./buhg)
add_subdirectory (./util)
add_subdirectory (./additionally/without-curses)
add_subdirectory (./additionally/other)

#записываем программы в каталог откуда они будут запускаться
install(TARGETS i_admint startxbu xbu xdk xkas xmu xrnn xud xukr xuos xupl xusl xzp DESTINATION /usr/local/bin)

install(TARGETS iceb_ps DESTINATION /usr/lib/cups/filter)

#переписываем настроечные файлы 
install(DIRECTORY ./buhg/alx ./buhg/doc DESTINATION /usr/share/iceB)

install(FILES ./util/cups/iceb.convs ./util/cups/iceb.types DESTINATION /etc/cups)


install(FILES ./util/terminfo/xterm DESTINATION /lib/terminfo/x)
