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

#ifndef __ANY_TRAY_ICON_H__
#define __ANY_TRAY_ICON_H__

#include <gtk/gtkmenu.h>

__BEGIN_DECLS

#define ANY_TRAY_TYPE_ICON            (any_tray_icon_get_type())
#define ANY_TRAY_ICON(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), ANY_TRAY_TYPE_ICON, AnyTrayIcon))
#define ANY_TRAY_IS_ICON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), ANY_TRAY_TYPE_ICON))
#define ANY_TRAY_ICON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST(klass), ANY_TRAY_TYPE_ICON, AnyTrayIconClass)
#define ANY_TRAY_IS_ICON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), ANY_TRAY_TYPE_ICON))
#define ANY_TRAY_ICON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), ANY_TRAY_TYPE_ICON, AnyTrayIconClass))

typedef struct _AnyTrayIcon        AnyTrayIcon;
typedef struct _AnyTrayIconClass   AnyTrayIconClass;
typedef struct _AnyTrayIconPrivate AnyTrayIconPrivate;

struct _AnyTrayIcon {
    GtkObject parent_instance;

    AnyTrayIconPrivate *GSEAL (priv);
};

struct _AnyTrayIconClass {
    GObjectClass parent_class;

    void (* set_menu)    (AnyTrayIcon *self,
                          GtkMenu     *menu);
    void (* set_text)    (AnyTrayIcon *self,
                          const gchar *text);
    void (* set_visible) (AnyTrayIcon *self,
                          gboolean     visible);
};

GType any_tray_icon_get_type (void) G_GNUC_CONST;

AnyTrayIcon *any_tray_icon_new         (const gchar *icon,
                                        const gchar *text);
void         any_tray_icon_set_menu    (AnyTrayIcon *self,
                                        GtkMenu     *menu);
void         any_tray_icon_set_text    (AnyTrayIcon *self,
                                        const gchar *text);
void         any_tray_icon_set_visible (AnyTrayIcon *self,
                                        gboolean     visible);

__END_DECLS
#endif // __ANY_TRAY_ICON_H__
/* vim: set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
