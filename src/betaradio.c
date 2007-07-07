/**
 * Copyright (c) 2007 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include "eggtrayicon.h"

#include "gstplay.h"
#include "channel.h"
#include "sitelist.h"

static const char* current = NULL;

void onQuit( GtkWidget* item, gpointer user_data )
{
	//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	gstStop();
	gtk_main_quit();
}

void onStop( GtkWidget* item, gpointer user_data )
{
    if (GTK_CHECK_MENU_ITEM(item)->active) {
		//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		gstStop();
		current = NULL;
	}
}

gpointer onPlay(gpointer *data)
{
		gchar* url = get_channel_url_by_id((gchar*) data);
		if (url != NULL) {
			current = (gchar*) data;
			//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
			gstPlay(url);
			//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		}
		g_free(url);
		g_thread_exit(NULL);
}

void onMenu( GtkWidget* item, gpointer user_data )
{
	if (GTK_CHECK_MENU_ITEM(item)->active) {
		//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		gchar* id = g_object_get_data(G_OBJECT(item), "id");
		g_thread_create(onPlay, id, FALSE, NULL);
	}
}

void appendSubMenu( const char *name, GtkWidget* menu, GSList *group, char **site_list )
{
	//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	GtkWidget *label = NULL;
	GtkWidget *sub_menu = NULL;
	GtkWidget *menu_item = NULL;

	label = gtk_menu_item_new_with_label(name);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), label);
	sub_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(label), sub_menu);

	do {
		menu_item = gtk_radio_menu_item_new_with_label( group, *site_list );
		group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(menu_item) );
		g_object_set_data( G_OBJECT(menu_item), "id", *(site_list + 1) );
		if( current && 0 == strcmp( *(site_list + 1), current ) )
			gtk_check_menu_item_set_active( GTK_CHECK_MENU_ITEM(menu_item), TRUE );
		g_signal_connect( G_OBJECT(menu_item), "toggled", G_CALLBACK(onMenu), NULL );
		gtk_menu_shell_append( GTK_MENU_SHELL(sub_menu), menu_item );
	} while (*(site_list += 2));
}

void onEggTrayEvent( GtkWidget* widget, GdkEventButton* evt, gpointer user_data )
{
	//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	GSList* group = NULL;
	GtkWidget* menu_item = NULL;
	GtkWidget* menu = gtk_menu_new();

	menu_item = gtk_radio_menu_item_new_with_label( group, "停止播放" );
	group = gtk_radio_menu_item_get_group( GTK_RADIO_MENU_ITEM(menu_item) );
	gtk_menu_shell_append( GTK_MENU_SHELL(menu), menu_item );
	g_signal_connect( G_OBJECT(menu_item), "toggled", G_CALLBACK(onStop), NULL );

	gtk_menu_shell_append( GTK_MENU_SHELL(menu), gtk_separator_menu_item_new() );

	appendSubMenu("音樂", menu, group, music_site_list);
	appendSubMenu("生活資訊", menu, group, life_site_list);
	appendSubMenu("新聞", menu, group, news_site_list);
	appendSubMenu("綜合", menu, group, others_site_list);
	appendSubMenu("外語", menu, group, foreign_site_list);
	appendSubMenu("多元文化", menu, group, culture_site_list);
	appendSubMenu("交通", menu, group, traffic_site_list);

	gtk_menu_shell_append( GTK_MENU_SHELL(menu), gtk_separator_menu_item_new() );

	menu_item = gtk_image_menu_item_new_from_stock( GTK_STOCK_QUIT, NULL );
	gtk_menu_shell_append( GTK_MENU_SHELL(menu), menu_item );
	g_signal_connect( G_OBJECT(menu_item), "activate", G_CALLBACK(onQuit), NULL );

	/* g_signal_connect( G_OBJECT(menu), "selection-done", G_CALLBACK(gtk_widget_destroy), NULL ); */
	gtk_widget_show_all( GTK_WIDGET(menu) );
	gtk_menu_popup( GTK_MENU(menu), NULL, NULL, NULL, NULL, evt->button, evt->time );
}

int main( int argc, char** argv )
{
	GtkWidget *icon;
	GtkWidget *evt_box;
	GtkTooltips *tooltips;
	EggTrayIcon *tray_icon;

	g_thread_init(NULL);
	gtk_init( &argc, &argv );
	
	tooltips = gtk_tooltips_new ();
	tray_icon = egg_tray_icon_new ("Beta Radio");
	evt_box = gtk_event_box_new ();
	gtk_container_add (GTK_CONTAINER (tray_icon), evt_box);
	g_signal_connect (G_OBJECT(evt_box), "button-press-event", G_CALLBACK(onEggTrayEvent), NULL);
	gtk_tooltips_set_tip (GTK_TOOLTIPS(tooltips), evt_box, "Beta Radio", NULL);

	icon = gtk_image_new_from_stock( GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_MENU );
	gtk_container_add (GTK_CONTAINER (evt_box), icon);

	gtk_widget_show_all (GTK_WIDGET (tray_icon));
	gtk_main();
	
	return 0;
}
