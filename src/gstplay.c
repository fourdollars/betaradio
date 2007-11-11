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

#include <gst/gst.h>
#include "gstplay.h"

static GMainLoop *g_loop = NULL;

static gboolean my_bus_callback (GstBus *bus, GstMessage *message, gpointer data)
{
	GMainLoop *loop = data;

	switch (GST_MESSAGE_TYPE (message)) {
		case GST_MESSAGE_ERROR:
			{
				GError *err;
				gchar *debug;
				gst_message_parse_error (message, &err, &debug);
				g_print ("Error: %s\n", err->message);
				g_error_free (err);
				g_free (debug);
				//g_debug("%s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, GST_MESSAGE_TYPE_NAME(message));
				g_main_loop_quit (loop);
				break;
			}
		case GST_MESSAGE_EOS:
			/* end-of-stream */
			//g_debug("%s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, GST_MESSAGE_TYPE_NAME(message));
			g_main_loop_quit (loop);
			break;
		case GST_MESSAGE_STATE_CHANGED:
			// be quite.
			break;
		default:
			/* unhandled message */
			//g_debug("%s %s %d %s\n", __FILE__, __FUNCTION__, __LINE__, GST_MESSAGE_TYPE_NAME(message));
			break;
	}

	/* remove message from the queue */
	return TRUE;
}

gboolean gstStop(void)
{
	return gstPlay(NULL);
}

gboolean gstPlay(gchar *uri)
{
	//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	GMainLoop *loop;
	GstElement *play;
	GstBus *bus;

	/* stop the previous player */
	if (g_loop != NULL) {
		//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		g_main_loop_quit(g_loop);
		g_loop = NULL;
	}

	if (uri == NULL) {
		//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		return 0;
	}

	/* init GStreamer */
	gst_init (NULL, NULL);
	g_loop = loop = g_main_loop_new (NULL, FALSE);
	//g_debug("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* set up */
	play = gst_element_factory_make ("playbin", "play");
	g_object_set (G_OBJECT (play), "uri", uri, NULL);

	bus = gst_pipeline_get_bus (GST_PIPELINE (play));
	gst_bus_add_watch (bus, my_bus_callback, loop);
	gst_object_unref (bus);

	gst_element_set_state (play, GST_STATE_PLAYING);

	/* now run */
	g_main_loop_run (loop);

	/* also clean up */
	gst_element_set_state (play, GST_STATE_NULL);
	gst_object_unref (GST_OBJECT (play));

	return 0;
}
