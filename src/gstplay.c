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
#include "gstplay.h"

typedef struct {
    GMainLoop*  loop;
    GstElement* bin;
    GstCallback func;
    GstStatus   state;
} GstData;

static gboolean gstBusCallback(GstBus*, GstMessage*, gpointer);
static void gstPlay(GstPlayer*, const char* const);
static void gstStop(GstPlayer*);
static void gstRegister(GstPlayer*, GstCallback);
static void gstRelease(GstPlayer*);

GstPlayer GstFunsTable = {
    .data     = NULL,
    .Play     = gstPlay,
    .Stop     = gstStop,
    .Register = gstRegister,
    .Release  = gstRelease,
};

GstPlayer* gstCreate(void)
{
    GstPlayer* gst = malloc(sizeof(GstPlayer));
    memset(gst, 0, sizeof(GstPlayer));
    memcpy(gst, &GstFunsTable, sizeof(GstPlayer));
    gst->data = malloc(sizeof(GstData));
    memset(gst->data, 0, sizeof(GstData));
    gst->self = &gst;
    return gst;
}

static void gstPlay(GstPlayer* gst, const char* const url)
{
    GstData* data = (GstData*) gst->data;
    GstBus*  bus  = NULL;

    if (data->state == GstPlay)
        gst->Stop(gst);

    gst_init (NULL, NULL);

    data->loop = g_main_loop_new(NULL, FALSE);

    data->bin= gst_element_factory_make("playbin", "BetaRadio");
    g_object_set(G_OBJECT(data->bin), "uri", url, NULL);

    bus = gst_pipeline_get_bus(GST_PIPELINE(data->bin));
    gst_bus_add_watch(bus, gstBusCallback, gst);
    gst_object_unref(bus);

    gst_element_set_state(data->bin, GST_STATE_PLAYING);

    data->state = GstPlay;

    if (data->func != NULL) {
        if (data->func(gst, data->state) != 0) {
            data->func = NULL;
        }
    }

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

        data->state = GstNull;

        if (data->func != NULL) {
            if (data->func(gst, data->state) != 0) {
                data->func = NULL;
            }
        }
    }

    return;
}

static void gstRegister(GstPlayer* gst, GstCallback func)
{
    GstData* data = (GstData*) gst->data;
    data->func = func;
    return;
}

static void gstRelease(GstPlayer* gst)
{
    GstData* data = (GstData*) gst->data;
    GstPlayer** self = gst->self;

    if (data->state == GstPlay)
        gstStop(gst);

    free(gst->data);
    free(gst);
    *self = NULL;

    return;
}

static gboolean gstBusCallback(GstBus *bus, GstMessage *message, gpointer pointer)
{
    GstPlayer* gst = (GstPlayer*) pointer;
    GstData* data = (GstData*) gst->data;

    switch (GST_MESSAGE_TYPE(message)) {
        case GST_MESSAGE_ERROR:
        case GST_MESSAGE_EOS:
            data->state = GstError;
            if (data->func != NULL) {
                if (data->func(gst, data->state) != 0) {
                    data->func = NULL;
                }
            }
            break;
        default:
            break;
    }

    return TRUE;
}
