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

using Gst;

class GstPlayer : Window {
    private dynamic Element player = null;
    public GstPlayer(string name, string url) {
        player = ElementFactory.make("playbin2", name);
        player.uri = url;
        //player.get_bus().add_watch(bus_callback);
    }
    ~GstPlayer() {
        player = null;
    }
    public void play() {
        player.set_state(State.PLAYING);
    }
    public void stop() {
        player.set_state(State.NULL);
    }
    private bool bus_callback(Gst.Bus bus, Gst.Message message) {
        switch (message.type) {
            case Gst.MessageType.ERROR:
                GLib.Error err;
                string debug;
                message.parse_error (out err, out debug);
                error("Error: %s\n", err.message);
                break;
            case Gst.MessageType.EOS:
                warning("end of stream\n");
                break;
            case Gst.MessageType.STATE_CHANGED:
                Gst.State oldstate;
                Gst.State newstate;
                Gst.State pending;
                message.parse_state_changed (out oldstate, out newstate,
                        out pending);
                GLib.stdout.printf ("state changed: %s->%s:%s\n",
                        oldstate.to_string (), newstate.to_string (),
                        pending.to_string ());
                break;
            default:
                break;
        }

        return true;
    }
}
