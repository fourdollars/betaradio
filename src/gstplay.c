/**
 * Copyright (c) 2007 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include "streamplayer.h"

typedef struct StreamData StreamData;

struct StreamData {
    GMainLoop*     loop;
    GstElement*    bin;
    StreamCallback func;
    StreamStatus   state;
    void*          ptr;
};

static void gstChangeStatus(StreamPlayer*, StreamStatus);
static void gstPlay(StreamPlayer*, const char* const);
static void gstStop(StreamPlayer*);
static void gstRegister(StreamPlayer*, StreamCallback, void*);
static void gstRelease(StreamPlayer*);

static gboolean gstBusCallback(GstBus*, GstMessage*, gpointer);

StreamPlayer GstFunsTable = {
    .data     = NULL,
    .Play     = gstPlay,
    .Stop     = gstStop,
    .Register = gstRegister,
    .Release  = gstRelease,
};

StreamPlayer* StreamPlayerCreate(void)
{
    StreamPlayer* gst = malloc(sizeof(StreamPlayer));
    memcpy(gst, &GstFunsTable, sizeof(StreamPlayer));
    gst->data = malloc(sizeof(StreamData));
    memset(gst->data, 0, sizeof(StreamData));
    return gst;
}

static void gstChangeStatus(StreamPlayer* gst, StreamStatus state)
{
    StreamData* data = (StreamData*) gst->data;
    data->state = state;

    if (data->func != NULL) {
        if (data->func(gst, data->state, data->ptr) != 0) {
            data->func = NULL;
        }
    }

    return;
}

static void gstPlay(StreamPlayer* gst, const char* const url)
{
    StreamData* data = (StreamData*) gst->data;
    GstBus*  bus  = NULL;

    if (data->state == SP_Play)
        gst->Stop(gst);

    gst_init (NULL, NULL);

    data->loop = g_main_loop_new(NULL, FALSE);

    data->bin= gst_element_factory_make("playbin", "BetaRadio");
    g_object_set(G_OBJECT(data->bin), "uri", url, NULL);

    bus = gst_pipeline_get_bus(GST_PIPELINE(data->bin));
    gst_bus_add_watch(bus, gstBusCallback, gst);
    gst_object_unref(bus);

    gst_element_set_state(data->bin, GST_STATE_PLAYING);

    g_main_loop_run(data->loop);

    return;
}

static void gstStop(StreamPlayer* gst)
{
    StreamData* data = (StreamData*) gst->data;

    if (data->loop != NULL) {
        gst_element_set_state(data->bin, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(data->bin));
        data->bin = NULL;

        g_main_loop_quit(data->loop);
        g_main_loop_unref(data->loop);
        data->loop = NULL;

        gstChangeStatus(gst, SP_Null);
    }

    return;
}

static void gstRegister(StreamPlayer* gst, StreamCallback func, void* ptr)
{
    StreamData* data = (StreamData*) gst->data;
    data->func = func;
    data->ptr = ptr;
    return;
}

static void gstRelease(StreamPlayer* gst)
{
    StreamData* data = (StreamData*) gst->data;

    if (data->state == SP_Play)
        gstStop(gst);

    free(gst->data);
    free(gst);

    return;
}

static gboolean gstBusCallback(GstBus* bus, GstMessage* message, gpointer pointer)
{
    StreamPlayer* gst = (StreamPlayer*) pointer;
    StreamData* data = (StreamData*) gst->data;

    g_print("%s %s\n", GST_MESSAGE_SRC(message)->name, GST_MESSAGE_TYPE_NAME(message));

    switch (GST_MESSAGE_TYPE(message)) {
        default:
            break;
        case GST_MESSAGE_NEW_CLOCK:
            gstChangeStatus(gst, SP_Play);
            break;
        case GST_MESSAGE_ERROR:
        case GST_MESSAGE_EOS:
            gstChangeStatus(gst, SP_Error);
            break;
    }

    return TRUE;
}
