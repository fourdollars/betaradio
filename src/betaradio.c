/**
 * Copyright (C) 2010 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gtk/gtk.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include <locale.h>
#include "gettext.h"
#define _(string) gettext(string)

#include "gstplay.h"
#include "json_cat.h"

static GstPlayer* gPlayer = NULL;
static int myCallback(GstPlayer*, GstStatus, void*);

void onQuit(GtkWidget* item, gpointer user_data)
{
    gPlayer->Release(gPlayer);
    gtk_main_quit();
}

void onStop(GtkWidget* item, gpointer user_data)
{
    GtkStatusIcon *tray_icon = (GtkStatusIcon*) user_data;

    if (GTK_CHECK_MENU_ITEM(item)->active) {
        gPlayer->Stop(gPlayer);
        gtk_status_icon_set_tooltip(tray_icon, _("BetaRadio Tuner"));
    }
}

static int myCallback(GstPlayer* player, GstStatus state, void* ptr)
{
    GtkStatusIcon *tray_icon = (GtkStatusIcon*) ptr;

    switch (state)
    {
        default:
        case GP_Null:
        case GP_Play:
            break;
        case GP_Error:
            player->Stop(player);
            return 1;
    }

    return 0;
}

gpointer onPlay(gpointer *data)
{
    gchar* type = g_object_get_data(G_OBJECT(data), "type");
    gchar* url = g_object_get_data(G_OBJECT(data), "url");

    g_debug("type:%s url:%s\n", type, url);

    if (url != NULL) {
        gPlayer->Play(gPlayer, type, url);
    }

    g_thread_exit(NULL);
}

void onMenu(GtkWidget* item, gpointer user_data)
{
    GtkStatusIcon* tray_icon = (GtkStatusIcon*) user_data;
    if (GTK_CHECK_MENU_ITEM(item)->active) {
        g_thread_create((GThreadFunc) onPlay, item, FALSE, NULL);
        gtk_status_icon_set_tooltip(tray_icon,
                g_strdup_printf("%s - %s", _("BetaRadio Tuner"), gtk_menu_item_get_label(GTK_MENU_ITEM(item))));
    }
}

gboolean onTrayEvent(GtkStatusIcon *status_icon, GdkEventButton *event, gpointer user_data)
{
    gtk_menu_popup(GTK_MENU(user_data), NULL, NULL, NULL, NULL, 0, 0);
    return FALSE;
}

GSList* menu_propagate(GtkWidget* menu, GSList* group, GtkStatusIcon* tray_icon)
{
    unsigned int k = 0;
    unsigned int length = 0;

    json_cat* source = json_cat_create();

    if (source->http(source, "http://betaradio.googlecode.com/svn/trunk/utils/list.json")
            ->isArray(source) == false) {
        source->destroy(source);
        return group;
    }

    length = source->length(source);

    for (k = 0; k < length; k++) {
        const char* property = NULL;
        const char* title = NULL;
        const char* url = NULL;

        GtkWidget *site_label = NULL;
        GtkWidget *site_menu = NULL;

        json_cat* json = json_cat_create();
        json->http(json, source->array(source, k)->getString(source));
/*        g_debug("%s\n", source->getString(source));*/

        if (json->object(json, "property")->isString(json) == false) {
            json->destroy(json);
            source->parent(source);
            continue;
        }

        property = json->getString(json);
        title = json->sibling(json, "title")->getString(json);
        url = json->sibling(json, "url")->getString(json);

        g_debug("%s - %s\n", title, url);

        site_label = gtk_menu_item_new_with_label(title);
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), site_label);
        site_menu = gtk_menu_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(site_label), site_menu);

        if (strcmp("category", property) == 0 &&
                json->sibling(json, "category")->isArray(json) == true) {
            unsigned int i = 0;
            unsigned int length = json->length(json);
            for (i = 0; i < length; i++) {
                unsigned int j = 0;
                unsigned int size = 0;
                GtkWidget *label = NULL;
                GtkWidget *sub_menu = NULL;

                json->array(json, i)->object(json, "title");
/*                g_debug("%s\n", json->getString(json));*/
                label = gtk_menu_item_new_with_label(json->getString(json));
                gtk_menu_shell_append(GTK_MENU_SHELL(site_menu), label);

                sub_menu = gtk_menu_new();
                gtk_menu_item_set_submenu(GTK_MENU_ITEM(label), sub_menu);

                size = json->sibling(json, "channel")->length(json);
                for (j = 0; j < size; j++) {
                    GtkWidget *menu_item = NULL;
                    json->array(json, j)->object(json, "title");
/*                    g_debug("\t%s\n", json->getString(json));*/
                    menu_item = gtk_radio_menu_item_new_with_label(group, json->getString(json));
                    group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_item));
                    g_object_set_data(G_OBJECT(menu_item), "type", (gpointer) json->sibling(json, "type")->getString(json));
/*                    g_debug("\t\t%s\n", json->getString(json));*/
                    g_object_set_data(G_OBJECT(menu_item), "url", (gpointer) json->sibling(json, "url")->getString(json));
/*                    g_debug("\t\t%s\n", json->getString(json));*/
                    g_signal_connect(G_OBJECT(menu_item), "toggled", G_CALLBACK(onMenu), tray_icon);
                    gtk_menu_shell_append(GTK_MENU_SHELL(sub_menu), menu_item);
                    json->grandparent(json);
                }
                json->grandparent(json);
            }
        } else if (strcmp("channel", property) == 0 &&
                json->sibling(json, "channel")->isArray(json) == true) {
            unsigned int j = 0;
            unsigned int size = json->length(json);
            for (j = 0; j < size; j++) {
                GtkWidget *menu_item = NULL;
                json->array(json, j)->object(json, "title");
                menu_item = gtk_radio_menu_item_new_with_label(group, json->getString(json));
                group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_item));
/*                g_debug("\t%s\n", json->getString(json));*/
                g_object_set_data(G_OBJECT(menu_item), "type", (gpointer) json->sibling(json, "type")->getString(json));
/*                g_debug("\t\t%s\n", json->getString(json));*/
                g_object_set_data(G_OBJECT(menu_item), "url", (gpointer) json->sibling(json, "url")->getString(json));
/*                g_debug("\t\t%s\n", json->getString(json));*/
                g_signal_connect(G_OBJECT(menu_item), "toggled", G_CALLBACK(onMenu), tray_icon);
                gtk_menu_shell_append(GTK_MENU_SHELL(site_menu), menu_item);
                json->grandparent(json);
            }
        }
        json->destroy(json);
        source->parent(source);
    }
    source->destroy(source);
    return group;
}

int main(int argc, char *argv[])
{
    GtkStatusIcon *tray_icon = NULL;
    GtkWidget *icon = NULL;
    GtkTooltips *tooltips = NULL;
    GtkWidget* menu = NULL;
    GSList* group = NULL;
    GtkWidget* menu_item = NULL;

    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(PACKAGE);

    g_thread_init(NULL);
    gtk_init(&argc, &argv);

    tooltips = gtk_tooltips_new();
    tray_icon = gtk_status_icon_new();

    if (g_file_test(DATADIR"/pixmaps/betaradio/betaradio.png", G_FILE_TEST_EXISTS) == TRUE) { /* System */
        gtk_status_icon_set_from_file(tray_icon, DATADIR"/pixmaps/betaradio/betaradio.png");
    } else if (g_file_test("data/betaradio.png", G_FILE_TEST_EXISTS) == TRUE) { /* Develop */
        gtk_status_icon_set_from_file(tray_icon, "data/betaradio.png");
    } else {
        gtk_status_icon_set_from_stock(tray_icon, GTK_STOCK_MISSING_IMAGE);
    }
    gtk_status_icon_set_tooltip(tray_icon, _("BetaRadio Tuner"));
    gtk_status_icon_set_visible(tray_icon, TRUE);

    gPlayer = GstPlayerCreate();
    gPlayer->Register(gPlayer, myCallback, tray_icon);

    menu = gtk_menu_new();

    menu_item = gtk_radio_menu_item_new_with_label(group, _("Stop"));
    group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(menu_item));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(G_OBJECT(menu_item), "toggled", G_CALLBACK(onStop), tray_icon);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    group = menu_propagate(menu, group, tray_icon);

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());

    menu_item = gtk_radio_menu_item_new_with_label(group, _("Quit"));
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(onQuit), NULL);

    gtk_widget_show_all(GTK_WIDGET(menu));

    g_signal_connect(G_OBJECT(tray_icon), "button-release-event", G_CALLBACK(onTrayEvent), menu);

    gtk_main();

    return 0;
}
