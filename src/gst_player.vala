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
// sources: src/betaradio.vala src/json_soup.vala
// vapidirs: vapi

using Gst;

class GstPlayer : GLib.Object {

    private static GstPlayer instance = null;
    private dynamic Element player = null;
    private string title = null;
    private string uri = null;

    private GstPlayer(string name) {
        player = ElementFactory.make("playbin", name);
        player.get_bus().add_watch(ThreadPriority.NORMAL, bus_callback);
    }

    ~GstPlayer() {
        player = null;
    }

    public static GstPlayer get_instance() {
        if (instance == null) {
            instance = new GstPlayer("BetaRadio");
        }
        return instance;
    }

    public void play(string title, string url) {
        player.set_state(State.READY);
        player.uri = url;
        player.set_state(State.PLAYING);
        this.title = title;
        this.uri = url;
    }

    public void stop() {
        player.set_state(State.NULL);
    }

    private bool bus_callback(Gst.Bus bus, Gst.Message msg) {
        switch (msg.type) {
            case Gst.MessageType.ERROR:
                GLib.Error err;
                string debug;
                msg.parse_error (out err, out debug);
                warning("Error: %s", err.message);
                player.set_state(State.NULL);
                break;
            case Gst.MessageType.EOS:
                warning("end of stream");
                break;
            case Gst.MessageType.STREAM_START:
                message("Now playing '%s'.", this.title);
                break;
            case Gst.MessageType.ASYNC_DONE:
            case Gst.MessageType.BUFFERING:
            case Gst.MessageType.NEW_CLOCK:
            case Gst.MessageType.STATE_CHANGED:
            case Gst.MessageType.STREAM_STATUS:
            case Gst.MessageType.TAG:
                /* ignore */
                break;
            default:
                message("message type: %s", msg.type.get_name());
                break;
        }

        return true;
    }
}
