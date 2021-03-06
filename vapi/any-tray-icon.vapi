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

[CCode (prefix = "AnyTray_", lower_case_cprefix = "any_tray_", cheader_filename = "any-tray-icon.h")]
namespace AnyTray
{
    public class Icon : GLib.Object {
        public Icon (string icon, string text);
        public void set_menu (Gtk.Menu menu);
        public void set_text (string text);
        public void set_visible (bool visible);
    }
}
/* vim: set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
