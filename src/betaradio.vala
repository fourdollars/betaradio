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

// modules: any-tray-icon config x11 gtk+-3.0 gstreamer-1.0 json-glib-1.0 libsoup-3.0
// sources: src/json_soup.vala src/gst_player.vala
// vapidirs: vapi

using X;
using Gtk;
using Gst;

class BetaRadio : GLib.Object {
    private Gtk.Menu menu = null;
    private AnyTray.Icon icon = null;

    public static int main (string[] args) {
        X.init_threads();
        Gtk.init(ref args);
        Gst.init(ref args);

        Intl.bindtextdomain( Config.PACKAGE_NAME, Config.LOCALEDIR );
        Intl.bind_textdomain_codeset( Config.PACKAGE_NAME, "UTF-8" );
        Intl.textdomain( Config.PACKAGE_NAME );

        if (Thread.supported () == false) {
            stderr.printf("Threads are not supported!\n");
            return -1;
        }

        var app = new GLib.Application("com.google.code.p.betaradio", GLib.ApplicationFlags.FLAGS_NONE);

        bool is_running = false;
        app.activate.connect(() => {
            if (is_running) return;
            is_running = true;

            var instance = new BetaRadio();
            Gtk.main();
            instance = null;
        });

        return app.run();
    }

    public BetaRadio () {
        icon = new AnyTray.Icon(Config.PACKAGE_NAME, _("BetaRadio Tuner"));
        icon.set_text(_("Data Synchronizing ..."));

        try {
            var thread = new Thread<void*>("menu", () => {
                menu = new Gtk.Menu();
                unowned SList<Gtk.RadioMenuItem> group = null;

                var stop = new Gtk.RadioMenuItem.with_label(group, _("Stop"));
                group = stop.get_group();
                menu.append(stop);
                stop.toggled.connect((e) => {
                    if (e.get_active() == true) {
                        GstPlayer.get_instance().stop();
                        icon.set_text(_("BetaRadio Tuner"));
                    }
                });

                menu.append(new Gtk.SeparatorMenuItem());

                group = getMenu(menu, group);

                menu.append(new Gtk.SeparatorMenuItem());

                var quit = new Gtk.RadioMenuItem.with_label(group, _("Quit"));
                group = quit.get_group();
                menu.append(quit);
                quit.toggled.connect((e) => {
                    if (e.get_active() == true) {
                        GstPlayer.get_instance().stop();
                        icon.set_text(_("BetaRadio Tuner"));
                        Gtk.main_quit();
                    }
                });

                menu.show_all();

                icon.set_menu(menu);

                icon.set_text(_("BetaRadio Tuner"));

                return null;
            });
        } catch(GLib.ThreadError e) {
            debug("%s", e.message);
        }
    }

    private unowned SList<Gtk.RadioMenuItem> getMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group) {
        var list = new JsonSoup.http("https://raw.githubusercontent.com/fourdollars/betaradio/master/utils/list.json");
        if (list.is_array() == false) {
            var conn_err = new Gtk.MenuItem.with_label(_("Connection failed. Please restart this program."));
            menu.append(conn_err);
            return group;
        }
        int length = list.length();
        for (int i = 0; i < length; i++) {
            string feed = list.array(i).get_string();
            var json = new JsonSoup.http(feed);
            if (json.object("property").is_string() == false) {
                continue;
            }
            string title = json.sibling("title").get_string();
            var item = new Gtk.MenuItem.with_label(title);
            menu.append(item);
            var submenu = new Gtk.Menu();
            item.set_submenu(submenu);
            string property = json.sibling("property").get_string();
            if (property == "category" && json.sibling("category").is_array() == true) {
                group = getCategoryMenu(submenu, group, json);
            } else if (property == "channel" && json.sibling("channel").is_array() == true) {
                group = getChannelMenu(submenu, group, json);
            }
            list.parent();
        }
        return group;
    }

    private unowned SList<Gtk.RadioMenuItem> getCategoryMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group, JsonSoup json) {
        int length = json.length();
        for (int i = 0; i < length; i++) {
            string category = json.array(i).object("title").get_string();
            var item = new Gtk.MenuItem.with_label(category);
            var submenu = new Gtk.Menu();
            menu.append(item);
            item.set_submenu(submenu);
            int size = json.sibling("channel").length();
            for (int j = 0; j < size; j++) {
                string title = json.array(j).object("title").get_string();
                string type = json.sibling("type").get_string();
                string url = filter_url(json.sibling("url").get_string(), type);
                var radio = new Gtk.RadioMenuItem.with_label(group, title);
                group = radio.get_group();
                submenu.append(radio);
                radio.toggled.connect( (e) => {
                    if (e.get_active() == true) {
                        GstPlayer.get_instance().play(title, url);
                        icon.set_text(title);
                    }
                });
                json.grandparent();
            }
            json.grandparent();
        }
        return group;
    }

    private unowned SList<Gtk.RadioMenuItem> getChannelMenu(Gtk.Menu menu, SList<Gtk.RadioMenuItem> group, JsonSoup json) {
        int length = json.length();
        for (int i = 0; i < length; i++) {
            string title = json.array(i).object("title").get_string();
            string type = json.sibling("type").get_string();
            string url = filter_url(json.sibling("url").get_string(), type);
            var radio = new Gtk.RadioMenuItem.with_label(group, title);
            group = radio.get_group();
            menu.append(radio);
            radio.toggled.connect( (e) => {
                if (e.get_active() == true) {
                    GstPlayer.get_instance().play(title, url);
                    icon.set_text(title);
                }
            });
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
