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

#include <linux/stddef.h>
#include <stdlib.h>
#include <string.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "json_cat.h"

typedef struct json_cat_private json_cat_private;

static json_cat* json_cat_load(json_cat* cat, const char* file);
static json_cat* json_cat_feed(json_cat* cat, const char* fish);
static json_cat* json_cat_http(json_cat* cat, const char* url);
static json_cat* json_cat_object(json_cat* cat, const char* string);
static json_cat* json_cat_sibling(json_cat* cat, const char* string);
static json_cat* json_cat_array(json_cat* cat, unsigned int index);
static json_cat* json_cat_parent(json_cat* cat);
static json_cat* json_cat_grandparent(json_cat* cat);
static json_cat* json_cat_reset(json_cat* cat);

static bool json_cat_valid(json_cat* cat);
static bool json_cat_valid_value(json_cat* cat);

static bool json_cat_isObject(json_cat* cat);
static bool json_cat_isArray(json_cat* cat);

static bool json_cat_isString(json_cat* cat);
static bool json_cat_isInt(json_cat* cat);
static bool json_cat_isDouble(json_cat* cat);
static bool json_cat_isBool(json_cat* cat);

static void json_cat_destroy(json_cat* cat);

static const char* json_cat_getString(json_cat* cat);
static int json_cat_getInt(json_cat* cat);
static double json_cat_getDouble(json_cat* cat);
static bool json_cat_getBool(json_cat* cat);
static unsigned int json_cat_length(json_cat* cat);

static json_cat_private* get_json_cat_private(json_cat* cat);

struct json_cat_private {
    bool isFailed;
    JsonParser* parser;
    JsonNode* node;
};

static const json_cat json_cat_template = {
    .load = json_cat_load,
    .feed = json_cat_feed,
    .http = json_cat_http,
    .object = json_cat_object,
    .sibling = json_cat_sibling,
    .array = json_cat_array,
    .parent = json_cat_parent,
    .grandparent = json_cat_grandparent,
    .reset = json_cat_reset,
    .isObject = json_cat_isObject,
    .isArray = json_cat_isArray,
    .isString = json_cat_isString,
    .isInt = json_cat_isInt,
    .isDouble = json_cat_isDouble,
    .isBool = json_cat_isBool,
    .destroy = json_cat_destroy,
    .getString = json_cat_getString,
    .getInt = json_cat_getInt,
    .getDouble = json_cat_getDouble,
    .getBool = json_cat_getBool,
    .length = json_cat_length,
};

json_cat* json_cat_create(void)
{
    json_cat* cat = g_malloc(sizeof(json_cat));
    if (cat == NULL) {
        g_warning("No enough memory.");
        return NULL;
    }
    memset(cat, 0, sizeof(json_cat));
    memcpy(cat, &json_cat_template, sizeof(json_cat));
    cat->_priv = malloc(sizeof(json_cat_private));
    if (cat->_priv == NULL) {
        g_warning("No enough memory.");
        g_free(cat);
        return NULL;
    }
    memset(cat->_priv, 0, sizeof(json_cat_private));
    g_type_init();
    return cat;
}

static json_cat* json_cat_load(json_cat* cat, const char* file)
{
    GError* error = NULL;
    json_cat_private* priv = get_json_cat_private(cat);
    if (priv->parser != NULL) {
        g_object_unref(priv->parser);
    }
    priv->parser = json_parser_new();
    json_parser_load_from_file(priv->parser, file, &error);
    if (error) {
        g_print("Unable to parse `%s': %s\n", file, error->message);
        g_error_free(error);
        g_object_unref(priv->parser);
        priv->parser = NULL;
        return cat;
    }
    priv->node = json_parser_get_root(priv->parser);
    return cat;
}

static json_cat* json_cat_feed(json_cat* cat, const char* fish)
{
    GError* error = NULL;
    json_cat_private* priv = get_json_cat_private(cat);
    if (priv->parser != NULL) {
        g_object_unref(priv->parser);
    }
    priv->parser = json_parser_new();
    json_parser_load_from_data(priv->parser, fish, -1, &error);
    if (error) {
        g_print("Unable to parse buffer: %s\n", error->message);
        g_error_free(error);
        g_object_unref(priv->parser);
        priv->parser = NULL;
        return cat;
    }
    priv->node = json_parser_get_root(priv->parser);
    return cat;
}

static json_cat* json_cat_http(json_cat* cat, const char* url)
{
    GError* error = NULL;
    json_cat_private* priv = get_json_cat_private(cat);
    SoupSession* session = soup_session_sync_new();
    SoupMessage* msg = soup_message_new("GET", url);

    if (soup_session_send_message(session, msg) != 200) {
        g_print("Unable to get data from: %s\n", url);
        return cat;
    }

    if (priv->parser != NULL) {
        g_object_unref(priv->parser);
    }

    priv->parser = json_parser_new();
    json_parser_load_from_data(priv->parser, msg->response_body->data, msg->response_body->length, &error);

    if (error) {
        g_print("Unable to parse buffer: %s\n", error->message);
        g_error_free(error);
        g_object_unref(priv->parser);
        priv->parser = NULL;
        return cat;
    }

    priv->node = json_parser_get_root(priv->parser);

    return cat;
}

static json_cat* json_cat_object(json_cat* cat, const char* string)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (priv->parser == NULL || priv->isFailed == true) {
        return cat;
    }
    if (json_node_get_node_type(priv->node) == JSON_NODE_OBJECT) {
        JsonObject* object = json_node_get_object(priv->node);
        if (json_object_has_member(object, string) == TRUE) {
            priv->node = json_object_get_member(object, string);
        } else {
            g_warning("Get '%s' from object failed\n", string);
            priv->isFailed = true;
        }
    } else {
        g_warning("Get from object failed\n");
        priv->isFailed = true;
    }
    return cat;
}

static json_cat* json_cat_sibling(json_cat* cat, const char* string)
{
    json_cat_parent(cat);
    json_cat_object(cat, string);
    return cat;
}

static json_cat* json_cat_array(json_cat* cat, unsigned int index)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (priv->parser == NULL || priv->isFailed == true) {
        return cat;
    }
    if (json_node_get_node_type(priv->node) == JSON_NODE_ARRAY) {
        JsonArray* array = json_node_get_array(priv->node);
        if (json_array_get_length(array) > index) {
            priv->node = json_array_get_element(array, index);
        } else {
            g_warning("Get %u from array failed\n", index);
            priv->isFailed = true;
        }
    } else {
        g_warning("Get from array failed\n");
        priv->isFailed = true;
    }
    return cat;
}

static json_cat* json_cat_parent(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    JsonNode* node = NULL;
    if (priv->parser == NULL || priv->isFailed == true) {
        return cat;
    }
    node = json_node_get_parent(priv->node);
    if (node == NULL) {
        g_warning("Already be root.");
    } else {
        priv->node = node;
    }
    return cat;
}

static json_cat* json_cat_grandparent(json_cat* cat)
{
    json_cat_parent(cat);
    json_cat_parent(cat);
    return cat;
}

static json_cat* json_cat_reset(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);

    if (priv->parser != NULL) {
        priv->node = json_parser_get_root(priv->parser);
        priv->isFailed = false;
    } else {
        g_warning("There is no parser.");
    }

    return cat;
}

static bool json_cat_valid(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    return (priv->isFailed == false);
}

static bool json_cat_valid_value(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid(cat) == false) {
        return false;
    }
    if (json_node_get_node_type(priv->node) != JSON_NODE_VALUE) {
        g_warning("Not json value.");
        return false;
    }
    return true;
}

static bool json_cat_isObject(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid(cat) == false) {
        return false;
    }
    return (json_node_get_node_type(priv->node) == JSON_NODE_OBJECT);
}

static bool json_cat_isArray(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid(cat) == false) {
        return false;
    }
    return (json_node_get_node_type(priv->node) == JSON_NODE_ARRAY);
}

static bool json_cat_isString(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid_value(cat) == false) {
        return false;
    }
    if (json_node_get_value_type(priv->node) != G_TYPE_STRING) {
        return false;
    }
    return true;
}

static bool json_cat_isInt(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid_value(cat) == false) {
        return false;
    }
    if (json_node_get_value_type(priv->node) != G_TYPE_INT64) {
        return false;
    }
    return true;
}

static bool json_cat_isDouble(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid_value(cat) == false) {
        return false;
    }
    if (json_node_get_value_type(priv->node) != G_TYPE_DOUBLE) {
        return false;
    }
    return true;
}

static bool json_cat_isBool(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (json_cat_valid_value(cat) == false) {
        return false;
    }
    if (json_node_get_value_type(priv->node) != G_TYPE_BOOLEAN) {
        return false;
    }
    return true;
}

static void json_cat_destroy(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    g_free(priv);
    g_free(cat);
    return;
}

static const char* json_cat_getString(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    return json_node_get_string(priv->node);
}

static int json_cat_getInt(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    return json_node_get_int(priv->node);
}

static double json_cat_getDouble(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    return json_node_get_double(priv->node);
}

static bool json_cat_getBool(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    return json_node_get_boolean(priv->node);
}

static unsigned int json_cat_length(json_cat* cat)
{
    json_cat_private* priv = get_json_cat_private(cat);
    if (priv->parser == NULL || priv->isFailed == true) {
        return 0;
    }
    if (json_node_get_node_type(priv->node) == JSON_NODE_ARRAY) {
        JsonArray* array = json_node_get_array(priv->node);
        return json_array_get_length(array);
    }
    return 0;
}

static json_cat_private* get_json_cat_private(json_cat* cat)
{
    return (json_cat_private*) cat->_priv;
}
