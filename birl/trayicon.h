/*
Birl is Ruijie Linux

作者：木瓜无衣 （1401570404@qq.com)

*/




#ifndef TRAYICON_H_INCLUDED
#define TRAYICON_H_INCLUDED

#include <gtk/gtk.h>
#include "common.h"

GtkStatusIcon *trayicon;
GtkWidget *trayicon_menu;
GtkWidget *trayicon_menuitem_logoff;
GtkWidget *trayicon_menuitem_exit;
extern GtkWidget *login_button;

void trayIconActivated();
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 active_time, gpointer popUpMenu);
void trayicon_init();

#endif // TRAYICON_H_INCLUDED
