/* -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*- */
/* vim:set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
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

using Gtk;
using Gst;

class BetaRadio : GLib.Object {
    private Gtk.StatusIcon icon = null;
    private Gtk.Menu menu = null;
    private GstPlayer player = null;

    public static void main (string[] args) {
        Gst.init(ref args);
        Gtk.init(ref args);
        var app = new BetaRadio();
        Gtk.main();
    }

    public BetaRadio () {
        icon = new Gtk.StatusIcon.from_stock(Gtk.STOCK_MISSING_IMAGE);
        menu = new Gtk.Menu();
        unowned SList<Gtk.RadioMenuItem> group = null;

        var stop = new Gtk.RadioMenuItem.with_label(group, "Stop");
        group = stop.get_group();
        menu.append(stop);
        stop.activate.connect((e) => {
            if (player != null) {
                player.stop();
                player = null;
            }
        });

        menu.append(new Gtk.SeparatorMenuItem());

        group = getMenu(menu, group);

        menu.append(new Gtk.SeparatorMenuItem());

        var quit = new Gtk.RadioMenuItem.with_label(group, "Quit");
        group = stop.get_group();
        menu.append(quit);
        quit.activate.connect((e) => {
            if (player != null) {
                player.stop();
                player = null;
            }
            Gtk.main_quit();
        });

        menu.show_all();

        icon.button_release_event.connect((e) => {
            menu.popup(null, null, null, e.button, e.time);
            return true;
        });
    }

    private unowned SList<Gtk.RadioMenuItem> getMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group) {
        var list = new JsonSoup.http("http://betaradio.googlecode.com/svn/trunk/utils/list.json");
        if (list.isArray() == false) {
            return group;
        }
        int length = list.length();
        for (int i = 0; i < length; i++) {
            string feed = list.array(i).getString();
            var json = new JsonSoup.http(feed);
            if (json.object("property").isString() == false) {
                continue;
            }
            string title = json.sibling("title").getString();
            var item = new Gtk.MenuItem.with_label(title);
            menu.append(item);
            var submenu = new Gtk.Menu();
            item.set_submenu(submenu);
            string property = json.sibling("property").getString();
            if (property == "category" && json.sibling("category").isArray() == true) {
                group = getCategoryMenu(submenu, group, json);
            } else if (property == "channel" && json.sibling("channel").isArray() == true) {
                group = getChannelMenu(submenu, group, json);
            }
            list.parent();
        }
        return group;
    }

    private unowned SList<Gtk.RadioMenuItem> getCategoryMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group, JsonSoup json) {
        int length = json.length();
        for (int i = 0; i < length; i++) {
            string category = json.array(i).object("title").getString();
            var item = new Gtk.MenuItem.with_label(category);
            var submenu = new Gtk.Menu();
            menu.append(item);
            item.set_submenu(submenu);
            int size = json.sibling("channel").length();
            for (int j = 0; j < size; j++) {
                string title = json.array(j).object("title").getString();
                string type = json.sibling("type").getString();
                string url = filter_url(json.sibling("url").getString(), type);
                var radio = new Gtk.RadioMenuItem.with_label(group, title);
                group = radio.get_group();
                radio.toggled.connect( (e) => {
                    if (player != null) {
                        player.stop();
                    }
                    player = new GstPlayer("BetaRadio", url);
                    player.play();
                });
                submenu.append(radio);
                json.grandparent();
            }
            json.grandparent();
        }
        return group;
    }

    private unowned SList<Gtk.RadioMenuItem> getChannelMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group, JsonSoup json) {
        int length = json.length();
        for (int i = 0; i < length; i++) {
            string title = json.array(i).object("title").getString();
            string type = json.sibling("type").getString();
            string url = filter_url(json.sibling("url").getString(), type);
            var radio = new Gtk.RadioMenuItem.with_label(group, title);
            group = radio.get_group();
            radio.toggled.connect( (e) => {
                if (player != null) {
                    player.stop();
                }
                player = new GstPlayer("BetaRadio", url);
                player.play();
            });
            menu.append(radio);
            json.grandparent();
        }
        return group;
    }

    private string filter_url(string url, string type) {
        /* http://bcr.media.hinet.net/RA000042 */
        /* mmsh://bcr.media.hinet.net/RA000042\?MSWMExt\=.asf */
        if (type == "mms" && url.has_prefix("http://") == true) {
            return url.replace("http", "mmsh").concat("\\?MSWMExt\\=.asf");
        }
        return url;
    }
}
