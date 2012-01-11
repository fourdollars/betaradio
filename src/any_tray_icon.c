/* -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*- */
/**
 * Copyright (C) 2012 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
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

#include <stdlib.h>
#include <ltdl.h>
#include <gtk/gtkstatusicon.h>

#include "any_tray_icon.h"

enum
{
    PROP_0,
    PROP_ICON,
    PROP_TEXT
};

enum
{
    ACTIVATE_SIGNAL,
    POPUP_MENU_SIGNAL
};

struct _AnyTrayIconPrivate {
    gchar         *icon;
    gchar         *text;
    GtkMenu       *menu;
    GtkStatusIcon *status_icon;
    void          *indicator;
    void          *indicator_icon;
    gboolean       visible;
};

static GObject* any_tray_icon_constructor  (GType                  type,
                                            guint                  n_construct_properties,
                                            GObjectConstructParam *construct_params);
static void     any_tray_icon_set_property (GObject               *object,
                                            guint                  prop_id,
                                            const GValue          *value,
                                            GParamSpec            *pspec);
static void     any_tray_icon_get_property (GObject               *object, 
                                            guint                  prop_id,
                                            GValue                *value,
                                            GParamSpec            *pspec);
static void     any_tray_icon_dispose      (GObject               *object);
static void     any_tray_icon_finalize     (GObject               *object);

G_DEFINE_TYPE (AnyTrayIcon, any_tray_icon, G_TYPE_OBJECT)

static gboolean
any_tray_check_unity (void)
{
    return TRUE;
}

static void
any_tray_icon_class_init (AnyTrayIconClass *class)
{
    GObjectClass *gobject_class = (GObjectClass *) class;

    gobject_class->constructor  = any_tray_icon_constructor;
    gobject_class->set_property = any_tray_icon_set_property;
    gobject_class->get_property = any_tray_icon_get_property;
    gobject_class->dispose      = any_tray_icon_dispose;
    gobject_class->finalize     = any_tray_icon_finalize;

    g_object_class_install_property (gobject_class,
                                     PROP_ICON,
                                     g_param_spec_string ("icon",
                                                          "icon",
                                                          "File of this icon",
                                                          NULL,
                                                          G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));
    g_object_class_install_property (gobject_class,
                                     PROP_TEXT,
                                     g_param_spec_string ("text",
                                                          "text",
                                                          "Text of this icon",
                                                          NULL,
                                                          G_PARAM_READWRITE|G_PARAM_STATIC_NAME|G_PARAM_STATIC_NICK|G_PARAM_STATIC_BLURB));
    g_type_class_add_private (class, sizeof (AnyTrayIconPrivate));

    lt_dlinit();

    return;
}

static void
any_tray_icon_init (AnyTrayIcon *icon)
{
    AnyTrayIconPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE (icon, ANY_TRAY_TYPE_ICON, AnyTrayIconPrivate);
    gchar* desktop = getenv("XDG_CURRENT_DESKTOP");

    icon->priv = priv;

    priv->icon        = NULL;
    priv->text        = NULL;
    priv->menu        = NULL;
    priv->status_icon = NULL;
    priv->indicator   = NULL;
    priv->visible     = TRUE;

    if (desktop != NULL && strcmp("Unity", desktop) == 0) {
        priv->indicator = lt_dlopen("libappindicator.so.1");
    }

    return;
}

static GObject*
any_tray_icon_constructor (GType                  type,
                           guint                  n_construct_properties,
                           GObjectConstructParam *construct_params)
{
    GObject *object = G_OBJECT_CLASS (any_tray_icon_parent_class)->constructor (type, n_construct_properties, construct_params);
    return object;
}

static void
any_tray_icon_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
    g_return_if_fail (ANY_TRAY_ICON (object));

    AnyTrayIcon        *icon = ANY_TRAY_ICON (object);
    AnyTrayIconPrivate *priv = icon->priv;

    switch (prop_id) {
        default:
            break;
        case PROP_ICON:
            g_free(priv->icon);
            priv->icon = g_strdup (g_value_get_string (value));
            break;
        case PROP_TEXT:
            g_free(priv->text);
            priv->text = g_strdup (g_value_get_string (value));
            break;
    }
}
static void
any_tray_icon_get_property (GObject    *object, 
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
    g_return_if_fail (ANY_TRAY_ICON (object));

    AnyTrayIcon        *icon = ANY_TRAY_ICON (object);
    AnyTrayIconPrivate *priv = icon->priv;

    switch (prop_id) {
        default:
            break;
        case PROP_ICON:
            g_value_set_string (value, priv->icon);
            break;
        case PROP_TEXT:
            g_value_set_string (value, priv->text);
            break;
    }
}

static void
any_tray_icon_dispose (GObject *object)
{
    AnyTrayIcon        *icon = ANY_TRAY_ICON (object);
    AnyTrayIconPrivate *priv = icon->priv;

    if (priv->status_icon)
        g_object_unref (priv->status_icon);

    if (priv->menu)
        g_object_unref (priv->menu);

    if (priv->indicator)
        lt_dlclose (priv->indicator);
}

static void
any_tray_icon_finalize (GObject *object)
{
    AnyTrayIcon        *icon = ANY_TRAY_ICON (object);
    AnyTrayIconPrivate *priv = icon->priv;

    if (priv->icon)
        g_free(priv->icon);

    if (priv->text)
        g_free(priv->text);

    G_OBJECT_CLASS (any_tray_icon_parent_class)->finalize (object);
}

AnyTrayIcon *
any_tray_icon_new (const gchar *icon,
                   const gchar *text)
{
    AnyTrayIcon        *object = ANY_TRAY_ICON (g_object_new (ANY_TRAY_TYPE_ICON, "icon", icon, "text", text, NULL));
    AnyTrayIconPrivate *priv   = object->priv;

    if (priv->indicator) {
        void*(*app_indicator_new)(const gchar*, const gchar*, gint) =
            (void*(*)(const gchar*, const gchar*, gint)) lt_dlsym(priv->indicator, "app_indicator_new");
        priv->indicator_icon = app_indicator_new(text, icon, 0);
    }
    else {
        priv->status_icon = gtk_status_icon_new_from_file(icon);
        gtk_status_icon_set_tooltip_text (priv->status_icon, text);
    }

    return object;
}

static gboolean
gtk_status_icon_button_release_event (GtkStatusIcon* sender, GdkEventButton* event, gpointer self)
{
    AnyTrayIcon        *icon = ANY_TRAY_ICON (self);
    AnyTrayIconPrivate *priv = icon->priv;

    g_return_val_if_fail (event != NULL, FALSE);
    gtk_menu_popup (priv->menu, NULL, NULL, NULL, NULL, event->button, event->time);
    return TRUE;
}

void
any_tray_icon_set_menu (AnyTrayIcon *self,
                        GtkMenu     *menu)
{
    g_return_if_fail (ANY_TRAY_ICON (self));

    AnyTrayIcon        *icon = ANY_TRAY_ICON (self);
    AnyTrayIconPrivate *priv = icon->priv;

    if (priv->menu)
        g_object_unref (priv->menu);

    priv->menu = g_object_ref (menu);

    if (priv->indicator) {
        void (*app_indicator_set_menu)(void*, GtkMenu *) =
            (void (*)(void*, GtkMenu *)) lt_dlsym(priv->indicator, "app_indicator_set_menu");
        void (*app_indicator_set_status)(void*, gint) =
            (void (*)(void*, gint)) lt_dlsym(priv->indicator, "app_indicator_set_status");
        app_indicator_set_menu(priv->indicator_icon, priv->menu);
        app_indicator_set_status (priv->indicator_icon, priv->visible);
    }
    else {
        g_signal_connect_object (priv->status_icon, "button-release-event", (GCallback) gtk_status_icon_button_release_event, self, 0);
    }
}

void
any_tray_icon_set_visible (AnyTrayIcon *self,
                           gboolean     visible)
{
    g_return_if_fail (ANY_TRAY_ICON (self));

    AnyTrayIcon        *icon = ANY_TRAY_ICON (self);
    AnyTrayIconPrivate *priv = icon->priv;

    priv->visible = visible;

    if (priv->indicator) {
        void (*app_indicator_set_status)(void*, gint) =
            (void (*)(void*, gint)) lt_dlsym(priv->indicator, "app_indicator_set_status");
        app_indicator_set_status (priv->indicator_icon, priv->visible);
    }
    else {
        gtk_status_icon_set_visible (priv->status_icon, priv->visible);
    }
}

void
any_tray_icon_set_text (AnyTrayIcon *self,
                        const gchar *text)
{
    g_return_if_fail (ANY_TRAY_ICON (self));

    AnyTrayIcon        *icon = ANY_TRAY_ICON (self);
    AnyTrayIconPrivate *priv = icon->priv;

    if (priv->text)
        g_free (priv->text);

    priv->text = g_strdup (text);

    if (priv->status_icon)
        gtk_status_icon_set_tooltip_text (priv->status_icon, priv->text);
}

/* vim: set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
