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
#include <gst/gst.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include <locale.h>
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "gettext.h"
#define _(string) gettext(string)

#include "eggtrayicon.h"

#include "gstplay.h"
#include "channel.h"
#include "sitelist.h"

static GstPlayer* gstPlayer = NULL;
static const char* current = NULL;
static gboolean g_bRadioStatus = FALSE;
static int myGstCallback(GstPlayer*, GstStatus, void*);

typedef struct {
    GtkWidget *m_pIcon;
    gchar *m_pcRadioOn;
    gchar *m_pcRadioOff;
} radio_icon_t;

gboolean updateRadioStatus(gpointer user_data)
{
    static gboolean s_bFlash = TRUE;
    radio_icon_t *t_pRadioIcon = (radio_icon_t*) user_data;

    if (g_bRadioStatus == TRUE && s_bFlash == TRUE) {
        gtk_image_set_from_file(GTK_IMAGE(t_pRadioIcon->m_pIcon), t_pRadioIcon->m_pcRadioOn);
    } else {
        gtk_image_set_from_file(GTK_IMAGE(t_pRadioIcon->m_pIcon), t_pRadioIcon->m_pcRadioOff);
    }

    s_bFlash = s_bFlash == TRUE ? FALSE : TRUE;

    return TRUE;
}

void onQuit(GtkWidget* item, gpointer user_data)
{
    g_bRadioStatus = FALSE;
    gstPlayer->Release(gstPlayer);
    gtk_main_quit();
}

void onStop(GtkWidget* item, gpointer user_data)
{
    if (GTK_CHECK_MENU_ITEM(item)->active) {
        gstPlayer->Stop(gstPlayer);
        g_bRadioStatus = FALSE;
        current = NULL;
    }
}

static int myGstCallback(GstPlayer* gst, GstStatus state, void* ptr)
{
    const char* const str = (const char* const) ptr;

    g_print(" - %s %d\n", str, state);

    switch (state)
    {
        default:
        case GstNull:
        case GstPlay:
            g_bRadioStatus = TRUE;
            break;
        case GstError:
            gst->Stop(gst);
            g_bRadioStatus = FALSE;
            current = NULL;
            return 1;
    }

    return 0;
}

gpointer onPlay(gpointer *data)
{
    gchar* id = g_object_get_data(G_OBJECT(data), "id");
    gchar* live = g_object_get_data(G_OBJECT(data), "live");
    gchar* url = get_channel_url_by_id((gchar*) id, live ? 1 : 0);
    if (url != NULL) {
        current = (gchar*) id;
        gstPlayer->Play(gstPlayer, url);
        g_free(url);
    }
    g_thread_exit(NULL);
}

void onMenu(GtkWidget* item, gpointer user_data)
{
    if (GTK_CHECK_MENU_ITEM(item)->active) {
        g_bRadioStatus = FALSE;
        g_thread_create((GThreadFunc) onPlay, item, FALSE, NULL);
    }
}

GSList* appendMenu(const char *name, GtkWidget* menu, GSList *group, char **site_list, gboolean bLive)
{
    GtkWidget *label = NULL;
    GtkWidget *sub_menu = NULL;
    GtkWidget *menu_item = NULL;

    label = gtk_menu_item_new_with_label(name);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), label);
    sub_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(label), sub_menu);

    do {
        menu_item = gtk_radio_menu_item_new_with_label(group, *site_list);
        group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_item));
        g_object_set_data(G_OBJECT(menu_item), "id", *(site_list + 1));
        g_object_set_data(G_OBJECT(menu_item), "live", bLive ? *(site_list + 1) : NULL);
        g_signal_connect(G_OBJECT(menu_item), "toggled", G_CALLBACK(onMenu), NULL);
        gtk_menu_shell_append(GTK_MENU_SHELL(sub_menu), menu_item);
    } while (*(site_list += 2));

    return group;
}

gboolean onEggTrayEvent(GtkWidget* widget, GdkEventButton* evt, gpointer user_data)
{
    if (widget != NULL) {
        gtk_menu_popup(GTK_MENU(user_data), NULL, NULL, NULL, NULL, evt->button, evt->time);
        return TRUE;
    }
    return FALSE;
}

int main(int argc, char *argv[])
{
    GtkWidget *icon = NULL;
    GtkWidget *evt_box = NULL;
    GtkTooltips *tooltips = NULL;
    EggTrayIcon *tray_icon = NULL;
    GtkWidget* menu = NULL;
    GSList* group = NULL;
    GtkWidget* menu_item = NULL;
    radio_icon_t t_RadioIcon = {NULL, NULL, NULL};

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    g_thread_init(NULL);
    gtk_init(&argc, &argv);

    gstPlayer = gstCreate();
    gstPlayer->Register(gstPlayer, myGstCallback, "BetaRadio");

    tooltips = gtk_tooltips_new();
    tray_icon = egg_tray_icon_new("BetaRadio");
    evt_box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(tray_icon), evt_box);

    menu = gtk_menu_new();

    menu_item = gtk_radio_menu_item_new_with_label(group, _("Stop"));
    group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_item));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(G_OBJECT(menu_item), "toggled", G_CALLBACK(onStop), NULL);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    group = appendMenu(_("Music"), menu, group, music_site_list, 0);
    group = appendMenu(_("Life"), menu, group, life_site_list, 0);
    group = appendMenu(_("News"), menu, group, news_site_list, 0);
    group = appendMenu(_("Others"), menu, group, others_site_list, 0);
    group = appendMenu(_("Foreign"), menu, group, foreign_site_list, 0);
    group = appendMenu(_("Culture"), menu, group, culture_site_list, 0);
    group = appendMenu(_("Traffic"), menu, group, traffic_site_list, 0);
#ifdef TV
    group = appendMenu(_("Free TV"), menu, group, live_site_list, 1);
#endif

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(onQuit), NULL);

    gtk_widget_show_all(GTK_WIDGET(menu));

    g_signal_connect(G_OBJECT(evt_box), "button-press-event", G_CALLBACK(onEggTrayEvent), menu);

    gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips), evt_box, _("BetaRadio Tuner"), NULL);

    t_RadioIcon.m_pcRadioOn = g_build_filename(DATADIR, "pixmaps", "betaradio", "radio-on.png", NULL);
    t_RadioIcon.m_pcRadioOff = g_build_filename(DATADIR, "pixmaps", "betaradio", "radio-off.png", NULL);

    t_RadioIcon.m_pIcon = gtk_image_new_from_file(t_RadioIcon.m_pcRadioOff);
    gtk_container_add(GTK_CONTAINER(evt_box), t_RadioIcon.m_pIcon);

    g_timeout_add(500, updateRadioStatus, &t_RadioIcon);

    gtk_widget_show_all(GTK_WIDGET(tray_icon));
    gtk_main();

    return 0;
}
