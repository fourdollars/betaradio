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

#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include "gstplay.h"

typedef struct GstData GstData;

struct GstData {
    GMainLoop*  loop;
    GstElement* bin;
    GstCallback func;
    GstStatus   state;
    void*       ptr;
};

static void gstChangeStatus(GstPlayer*, GstStatus);
static void gstPlay(GstPlayer*, const char*, const char*);
static void gstStop(GstPlayer*);
static void gstRegister(GstPlayer*, GstCallback, void*);
static void gstRelease(GstPlayer*);

static gboolean gstBusCallback(GstBus*, GstMessage*, gpointer);

GstPlayer GstFunsTable = {
    .data     = NULL,
    .Play     = gstPlay,
    .Stop     = gstStop,
    .Register = gstRegister,
    .Release  = gstRelease,
};

GstPlayer* GstPlayerCreate(void)
{
    GstPlayer* gst = malloc(sizeof(GstPlayer));
    memcpy(gst, &GstFunsTable, sizeof(GstPlayer));
    gst->data = malloc(sizeof(GstData));
    memset(gst->data, 0, sizeof(GstData));
    return gst;
}

static void gstChangeStatus(GstPlayer* gst, GstStatus state)
{
    GstData* data = (GstData*) gst->data;
    data->state = state;

    if (data->func != NULL) {
        if (data->func(gst, data->state, data->ptr) != 0) {
            data->func = NULL;
        }
    }

    return;
}

static void on_pad_added (GstElement *element, GstPad *pad, gpointer data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;
    sinkpad = gst_element_get_static_pad(decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);
}

static void gstPlay(GstPlayer* gst, const char* type, const char* url)
{
    GstData* data = (GstData*) gst->data;
    GstBus*  bus  = NULL;

    if (data->state == GP_Play)
        gst->Stop(gst);

    gst_init (NULL, NULL);

    data->loop = g_main_loop_new(NULL, FALSE);

    data->bin = gst_element_factory_make("playbin2", "BetaRadio");
    g_object_set(G_OBJECT (data->bin), "uri", url, NULL);

    gst_element_set_state(data->bin, GST_STATE_PLAYING);

    bus = gst_pipeline_get_bus(GST_PIPELINE(data->bin));
    gst_bus_add_watch(bus, gstBusCallback, gst);
    gst_object_unref(bus);

    gst_element_set_state(data->bin, GST_STATE_PLAYING);

    g_main_loop_run(data->loop);

    return;
}

static void gstStop(GstPlayer* gst)
{
    GstData* data = (GstData*) gst->data;

    if (data->loop != NULL) {
        gst_element_set_state(data->bin, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(data->bin));
        data->bin = NULL;

        g_main_loop_quit(data->loop);
        g_main_loop_unref(data->loop);
        data->loop = NULL;

        gstChangeStatus(gst, GP_Null);
    }

    return;
}

static void gstRegister(GstPlayer* gst, GstCallback func, void* ptr)
{
    GstData* data = (GstData*) gst->data;
    data->func = func;
    data->ptr = ptr;
    return;
}

static void gstRelease(GstPlayer* gst)
{
    GstData* data = (GstData*) gst->data;

    if (data->state == GP_Play)
        gstStop(gst);

    free(gst->data);
    free(gst);

    return;
}

static gboolean gstBusCallback(GstBus* bus, GstMessage* message, gpointer pointer)
{
    GstPlayer* gst = (GstPlayer*) pointer;
    GstData* data = (GstData*) gst->data;

/*    g_debug("%s %s\n", GST_MESSAGE_SRC(message)->name, GST_MESSAGE_TYPE_NAME(message));*/

    switch (GST_MESSAGE_TYPE(message)) {
        default:
            break;
        case GST_MESSAGE_NEW_CLOCK:
            gstChangeStatus(gst, GP_Play);
            break;
        case GST_MESSAGE_ERROR:
        case GST_MESSAGE_EOS:
            gstChangeStatus(gst, GP_Error);
            break;
    }

    return TRUE;
}
