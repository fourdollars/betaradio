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

using Json;
using Soup;

class JsonSoup : GLib.Object {
    private Json.Parser parser = null;
    private unowned Json.Node node = null;

    /* Constructor */
    public JsonSoup.buffer(string buffer) {
        parser = new Parser();
        try {
            parser.load_from_data(buffer);
            node = parser.get_root();
        } catch (Error e) {
            warning("%s", e.message);
        }
    }
    public JsonSoup.file(string file) {
        parser = new Parser();
        try {
            parser.load_from_file(file);
            node = parser.get_root();
        } catch (Error e) {
            warning("%s", e.message);
        }
    }
    public JsonSoup.http(string url) {
        var session = new SessionAsync();
        var message = new Message.from_uri("GET", new URI(url));
        if (session.send_message(message) != 200) {
            warning("Can not connect to %s", url);
        }
        parser = new Parser();
        try {
            parser.load_from_data((string) message.response_body.data);
            node = parser.get_root();
        } catch (Error e) {
            warning("%s", e.message);
        }
    }

    /* Destructor */
    ~JsonSoup() {
        node = null;
        parser = null;
    }

    /* Movement */
    public JsonSoup object(string name) {
        if (node.get_node_type() != NodeType.OBJECT) {
            warning("This is not a object.");
            return this;
        }
        var object = node.get_object();
        if (object.has_member(name) == false) {
            warning("There is no such member as %s.", name);
            return this;
        }
        node = object.get_member(name);
        return this;
    }
    public JsonSoup sibling(string name) {
        parent();
        object(name);
        return this;
    }
    public JsonSoup array(int idx) {
        if (node.get_node_type() != NodeType.ARRAY) {
            warning("This is not a array.");
            return this;
        }
        var array = node.get_array();
        if (array.get_length() > idx || idx < 0) {
            warning("Out of index. %d", idx);
            return this;
        }
        node = array.get_element(idx);
        return this;
    }
    public JsonSoup parent() {
        unowned Json.Node parent_node = node.get_parent();
        if (parent_node == null) {
            warning("Already be root.");
            return this;
        }
        node = parent_node;
        return this;
    }
    public JsonSoup grandparent() {
        parent();
        parent();
        return this;
    }
    public JsonSoup reset() {
        node = parser.get_root();
        return this;
    }

    /* Type Checking */
    private bool isValue() {
        if (node.get_node_type() == NodeType.VALUE) {
            return true;
        } else {
            return false;
        }
    }
    public bool isObject() {
        return (node.get_node_type() == NodeType.OBJECT);
    }
    public bool isArray() {
        return (node.get_node_type() == NodeType.ARRAY);
    }
    public bool isString() {
        if (isValue() == false) {
            return false;
        }
        return (node.get_value_type() == typeof(string));
    }
    public bool isInt() {
        if (isValue() == false) {
            return false;
        }
        return (node.get_value_type() == typeof(int64));
    }
    public bool isDouble() {
        if (isValue() == false) {
            return false;
        }
        return (node.get_value_type() == typeof(double));
    }
    public bool isBool() {
        if (isValue() == false) {
            return false;
        }
        return (node.get_value_type() == typeof(bool));
    }

    /* Fetch Data */
    public string getString() {
        return node.get_string();
    }
    public int64 getInt() {
        return node.get_int();
    }
    public double getDouble() {
        return node.get_double();
    }
    public bool getBool() {
        return node.get_boolean();
    }
    public uint length() {
        if (isArray() == false) {
            return 0;
        }
        var array = node.get_array();
        return array.get_length();
    }
}
