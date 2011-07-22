/* betaradio.c generated by valac 0.12.0, the Vala compiler
 * generated from betaradio.vala, do not modify */

/* -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*- */
/* vim:set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n-lib.h>
#include <config.h>
#include <gst/gst.h>
#include <glib/gstdio.h>
#include <gdk/gdk.h>


#define TYPE_BETA_RADIO (beta_radio_get_type ())
#define BETA_RADIO(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_BETA_RADIO, BetaRadio))
#define BETA_RADIO_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_BETA_RADIO, BetaRadioClass))
#define IS_BETA_RADIO(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_BETA_RADIO))
#define IS_BETA_RADIO_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_BETA_RADIO))
#define BETA_RADIO_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_BETA_RADIO, BetaRadioClass))

typedef struct _BetaRadio BetaRadio;
typedef struct _BetaRadioClass BetaRadioClass;
typedef struct _BetaRadioPrivate BetaRadioPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))

#define TYPE_GST_PLAYER (gst_player_get_type ())
#define GST_PLAYER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_GST_PLAYER, GstPlayer))
#define GST_PLAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_GST_PLAYER, GstPlayerClass))
#define IS_GST_PLAYER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_GST_PLAYER))
#define IS_GST_PLAYER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_GST_PLAYER))
#define GST_PLAYER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_GST_PLAYER, GstPlayerClass))

typedef struct _GstPlayer GstPlayer;
typedef struct _GstPlayerClass GstPlayerClass;

#define TYPE_JSON_SOUP (json_soup_get_type ())
#define JSON_SOUP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_JSON_SOUP, JsonSoup))
#define JSON_SOUP_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_JSON_SOUP, JsonSoupClass))
#define IS_JSON_SOUP(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_JSON_SOUP))
#define IS_JSON_SOUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_JSON_SOUP))
#define JSON_SOUP_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_JSON_SOUP, JsonSoupClass))

typedef struct _JsonSoup JsonSoup;
typedef struct _JsonSoupClass JsonSoupClass;
#define _g_free0(var) (var = (g_free (var), NULL))
typedef struct _Block1Data Block1Data;
typedef struct _Block2Data Block2Data;
#define _g_regex_unref0(var) ((var == NULL) ? NULL : (var = (g_regex_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))

struct _BetaRadio {
	GObject parent_instance;
	BetaRadioPrivate * priv;
};

struct _BetaRadioClass {
	GObjectClass parent_class;
};

struct _BetaRadioPrivate {
	GtkStatusIcon* icon;
	GtkMenu* menu;
};

struct _Block1Data {
	int _ref_count_;
	BetaRadio * self;
	gchar* title;
	gchar* url;
};

struct _Block2Data {
	int _ref_count_;
	BetaRadio * self;
	gchar* title;
	gchar* url;
};


static gpointer beta_radio_parent_class = NULL;

GType beta_radio_get_type (void) G_GNUC_CONST;
#define BETA_RADIO_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_BETA_RADIO, BetaRadioPrivate))
enum  {
	BETA_RADIO_DUMMY_PROPERTY
};
void beta_radio_main (gchar** args, int args_length1);
BetaRadio* beta_radio_new (void);
BetaRadio* beta_radio_construct (GType object_type);
static void _lambda0_ (GtkCheckMenuItem* e, BetaRadio* self);
GType gst_player_get_type (void) G_GNUC_CONST;
GstPlayer* gst_player_get_instance (void);
void gst_player_stop (GstPlayer* self);
static void __lambda0__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self);
static GSList* beta_radio_getMenu (BetaRadio* self, GtkMenu* menu, GSList* group);
static void _lambda3_ (GtkCheckMenuItem* e, BetaRadio* self);
static void __lambda3__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self);
static gboolean _lambda4_ (GdkEventButton* e, BetaRadio* self);
static gboolean __lambda4__gtk_status_icon_button_release_event (GtkStatusIcon* _sender, GdkEventButton* event, gpointer self);
JsonSoup* json_soup_new_http (const gchar* url);
JsonSoup* json_soup_construct_http (GType object_type, const gchar* url);
GType json_soup_get_type (void) G_GNUC_CONST;
gboolean json_soup_is_array (JsonSoup* self);
gint json_soup_length (JsonSoup* self);
JsonSoup* json_soup_array (JsonSoup* self, gint idx);
gchar* json_soup_get_string (JsonSoup* self);
JsonSoup* json_soup_object (JsonSoup* self, const gchar* name);
gboolean json_soup_is_string (JsonSoup* self);
JsonSoup* json_soup_sibling (JsonSoup* self, const gchar* name);
static GSList* beta_radio_getCategoryMenu (BetaRadio* self, GtkMenu* menu, GSList* group, JsonSoup* json);
static GSList* beta_radio_getChannelMenu (BetaRadio* self, GtkMenu* menu, GSList* group, JsonSoup* json);
JsonSoup* json_soup_parent (JsonSoup* self);
static Block1Data* block1_data_ref (Block1Data* _data1_);
static void block1_data_unref (Block1Data* _data1_);
static gchar* beta_radio_filter_url (BetaRadio* self, const gchar* url, const gchar* type);
static void _lambda1_ (GtkCheckMenuItem* e, Block1Data* _data1_);
void gst_player_play (GstPlayer* self, const gchar* url);
static void __lambda1__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self);
JsonSoup* json_soup_grandparent (JsonSoup* self);
static Block2Data* block2_data_ref (Block2Data* _data2_);
static void block2_data_unref (Block2Data* _data2_);
static void _lambda2_ (GtkCheckMenuItem* e, Block2Data* _data2_);
static void __lambda2__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self);
static void beta_radio_finalize (GObject* obj);


void beta_radio_main (gchar** args, int args_length1) {
	BetaRadio* _tmp0_ = NULL;
	BetaRadio* app;
	bindtextdomain (PACKAGE_NAME, LOCALEDIR);
	bind_textdomain_codeset (PACKAGE_NAME, "UTF-8");
	textdomain (PACKAGE_NAME);
	gst_init (&args_length1, &args);
	gtk_init (&args_length1, &args);
	_tmp0_ = beta_radio_new ();
	app = _tmp0_;
	g_message ("betaradio.vala:34: Running");
	gtk_main ();
	g_mem_profile ();
	_g_object_unref0 (app);
}


int main (int argc, char ** argv) {
	g_mem_set_vtable (glib_mem_profiler_table);
	g_type_init ();
	beta_radio_main (argv, argc);
	return 0;
}


static void _lambda0_ (GtkCheckMenuItem* e, BetaRadio* self) {
	gboolean _tmp0_;
	g_return_if_fail (e != NULL);
	_tmp0_ = gtk_check_menu_item_get_active (e);
	if (_tmp0_ == TRUE) {
		GstPlayer* _tmp1_ = NULL;
		GstPlayer* _tmp2_;
		const gchar* _tmp3_ = NULL;
		_tmp1_ = gst_player_get_instance ();
		_tmp2_ = _tmp1_;
		gst_player_stop (_tmp2_);
		_g_object_unref0 (_tmp2_);
		_tmp3_ = _ ("BetaRadio Tuner");
		gtk_status_icon_set_tooltip_text (self->priv->icon, _tmp3_);
	}
}


static void __lambda0__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self) {
	_lambda0_ (_sender, self);
}


static void _lambda3_ (GtkCheckMenuItem* e, BetaRadio* self) {
	gboolean _tmp0_;
	g_return_if_fail (e != NULL);
	_tmp0_ = gtk_check_menu_item_get_active (e);
	if (_tmp0_ == TRUE) {
		GstPlayer* _tmp1_ = NULL;
		GstPlayer* _tmp2_;
		const gchar* _tmp3_ = NULL;
		_tmp1_ = gst_player_get_instance ();
		_tmp2_ = _tmp1_;
		gst_player_stop (_tmp2_);
		_g_object_unref0 (_tmp2_);
		_tmp3_ = _ ("BetaRadio Tuner");
		gtk_status_icon_set_tooltip_text (self->priv->icon, _tmp3_);
		gtk_main_quit ();
	}
}


static void __lambda3__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self) {
	_lambda3_ (_sender, self);
}


static gboolean _lambda4_ (GdkEventButton* e, BetaRadio* self) {
	gboolean result = FALSE;
	gtk_menu_popup (self->priv->menu, NULL, NULL, NULL, NULL, (*e).button, (*e).time);
	result = TRUE;
	return result;
}


static gboolean __lambda4__gtk_status_icon_button_release_event (GtkStatusIcon* _sender, GdkEventButton* event, gpointer self) {
	gboolean result;
	result = _lambda4_ (event, self);
	return result;
}


BetaRadio* beta_radio_construct (GType object_type) {
	BetaRadio * self = NULL;
	gboolean _tmp0_;
	const gchar* _tmp5_ = NULL;
	GtkMenu* _tmp6_ = NULL;
	GSList* group;
	const gchar* _tmp7_ = NULL;
	GtkRadioMenuItem* _tmp8_ = NULL;
	GtkRadioMenuItem* stop;
	GSList* _tmp9_ = NULL;
	GtkSeparatorMenuItem* _tmp10_ = NULL;
	GtkSeparatorMenuItem* _tmp11_;
	GSList* _tmp12_ = NULL;
	GtkSeparatorMenuItem* _tmp13_ = NULL;
	GtkSeparatorMenuItem* _tmp14_;
	const gchar* _tmp15_ = NULL;
	GtkRadioMenuItem* _tmp16_ = NULL;
	GtkRadioMenuItem* quit;
	GSList* _tmp17_ = NULL;
	self = (BetaRadio*) g_object_new (object_type, NULL);
	_tmp0_ = g_file_test (DATADIR "/pixmaps/betaradio/betaradio.png", G_FILE_TEST_IS_REGULAR);
	if (_tmp0_ == TRUE) {
		GtkStatusIcon* _tmp1_ = NULL;
		_tmp1_ = gtk_status_icon_new_from_file (DATADIR "/pixmaps/betaradio/betaradio.png");
		_g_object_unref0 (self->priv->icon);
		self->priv->icon = _tmp1_;
	} else {
		gboolean _tmp2_;
		_tmp2_ = g_file_test ("data/betaradio.png", G_FILE_TEST_IS_REGULAR);
		if (_tmp2_ == TRUE) {
			GtkStatusIcon* _tmp3_ = NULL;
			_tmp3_ = gtk_status_icon_new_from_file ("data/betaradio.png");
			_g_object_unref0 (self->priv->icon);
			self->priv->icon = _tmp3_;
		} else {
			GtkStatusIcon* _tmp4_ = NULL;
			_tmp4_ = gtk_status_icon_new_from_stock (GTK_STOCK_MISSING_IMAGE);
			_g_object_unref0 (self->priv->icon);
			self->priv->icon = _tmp4_;
		}
	}
	_tmp5_ = _ ("BetaRadio Tuner");
	gtk_status_icon_set_tooltip_text (self->priv->icon, _tmp5_);
	_tmp6_ = (GtkMenu*) gtk_menu_new ();
	_g_object_unref0 (self->priv->menu);
	self->priv->menu = g_object_ref_sink (_tmp6_);
	group = NULL;
	_tmp7_ = _ ("Stop");
	_tmp8_ = (GtkRadioMenuItem*) gtk_radio_menu_item_new_with_label (group, _tmp7_);
	stop = g_object_ref_sink (_tmp8_);
	_tmp9_ = gtk_radio_menu_item_get_group (stop);
	group = _tmp9_;
	gtk_menu_shell_append ((GtkMenuShell*) self->priv->menu, (GtkWidget*) ((GtkMenuItem*) stop));
	g_signal_connect_object ((GtkCheckMenuItem*) stop, "toggled", (GCallback) __lambda0__gtk_check_menu_item_toggled, self, 0);
	_tmp10_ = (GtkSeparatorMenuItem*) gtk_separator_menu_item_new ();
	_tmp11_ = g_object_ref_sink (_tmp10_);
	gtk_menu_shell_append ((GtkMenuShell*) self->priv->menu, (GtkWidget*) ((GtkMenuItem*) _tmp11_));
	_g_object_unref0 (_tmp11_);
	_tmp12_ = beta_radio_getMenu (self, self->priv->menu, group);
	group = _tmp12_;
	_tmp13_ = (GtkSeparatorMenuItem*) gtk_separator_menu_item_new ();
	_tmp14_ = g_object_ref_sink (_tmp13_);
	gtk_menu_shell_append ((GtkMenuShell*) self->priv->menu, (GtkWidget*) ((GtkMenuItem*) _tmp14_));
	_g_object_unref0 (_tmp14_);
	_tmp15_ = _ ("Quit");
	_tmp16_ = (GtkRadioMenuItem*) gtk_radio_menu_item_new_with_label (group, _tmp15_);
	quit = g_object_ref_sink (_tmp16_);
	_tmp17_ = gtk_radio_menu_item_get_group (quit);
	group = _tmp17_;
	gtk_menu_shell_append ((GtkMenuShell*) self->priv->menu, (GtkWidget*) ((GtkMenuItem*) quit));
	g_signal_connect_object ((GtkCheckMenuItem*) quit, "toggled", (GCallback) __lambda3__gtk_check_menu_item_toggled, self, 0);
	gtk_widget_show_all ((GtkWidget*) self->priv->menu);
	g_signal_connect_object (self->priv->icon, "button-release-event", (GCallback) __lambda4__gtk_status_icon_button_release_event, self, 0);
	_g_object_unref0 (quit);
	_g_object_unref0 (stop);
	return self;
}


BetaRadio* beta_radio_new (void) {
	return beta_radio_construct (TYPE_BETA_RADIO);
}


static GSList* beta_radio_getMenu (BetaRadio* self, GtkMenu* menu, GSList* group) {
	GSList* result = NULL;
	JsonSoup* _tmp0_ = NULL;
	JsonSoup* list;
	gboolean _tmp1_;
	gint _tmp2_;
	gint length;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (menu != NULL, NULL);
	_tmp0_ = json_soup_new_http ("http://betaradio.googlecode.com/svn/trunk/utils/list.json");
	list = _tmp0_;
	_tmp1_ = json_soup_is_array (list);
	if (_tmp1_ == FALSE) {
		result = group;
		_g_object_unref0 (list);
		return result;
	}
	_tmp2_ = json_soup_length (list);
	length = _tmp2_;
	{
		gint i;
		i = 0;
		{
			gboolean _tmp3_;
			_tmp3_ = TRUE;
			while (TRUE) {
				JsonSoup* _tmp4_ = NULL;
				gchar* _tmp5_ = NULL;
				gchar* feed;
				JsonSoup* _tmp6_ = NULL;
				JsonSoup* json;
				JsonSoup* _tmp7_ = NULL;
				gboolean _tmp8_;
				JsonSoup* _tmp9_ = NULL;
				gchar* _tmp10_ = NULL;
				gchar* title;
				GtkMenuItem* _tmp11_ = NULL;
				GtkMenuItem* item;
				GtkMenu* _tmp12_ = NULL;
				GtkMenu* submenu;
				JsonSoup* _tmp13_ = NULL;
				gchar* _tmp14_ = NULL;
				gchar* property;
				gboolean _tmp15_ = FALSE;
				if (!_tmp3_) {
					i++;
				}
				_tmp3_ = FALSE;
				if (!(i < length)) {
					break;
				}
				_tmp4_ = json_soup_array (list, i);
				_tmp5_ = json_soup_get_string (_tmp4_);
				feed = _tmp5_;
				_tmp6_ = json_soup_new_http (feed);
				json = _tmp6_;
				_tmp7_ = json_soup_object (json, "property");
				_tmp8_ = json_soup_is_string (_tmp7_);
				if (_tmp8_ == FALSE) {
					_g_object_unref0 (json);
					_g_free0 (feed);
					continue;
				}
				_tmp9_ = json_soup_sibling (json, "title");
				_tmp10_ = json_soup_get_string (_tmp9_);
				title = _tmp10_;
				_tmp11_ = (GtkMenuItem*) gtk_menu_item_new_with_label (title);
				item = g_object_ref_sink (_tmp11_);
				gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) item);
				_tmp12_ = (GtkMenu*) gtk_menu_new ();
				submenu = g_object_ref_sink (_tmp12_);
				gtk_menu_item_set_submenu (item, (GtkWidget*) submenu);
				_tmp13_ = json_soup_sibling (json, "property");
				_tmp14_ = json_soup_get_string (_tmp13_);
				property = _tmp14_;
				if (g_strcmp0 (property, "category") == 0) {
					JsonSoup* _tmp16_ = NULL;
					gboolean _tmp17_;
					_tmp16_ = json_soup_sibling (json, "category");
					_tmp17_ = json_soup_is_array (_tmp16_);
					_tmp15_ = _tmp17_ == TRUE;
				} else {
					_tmp15_ = FALSE;
				}
				if (_tmp15_) {
					GSList* _tmp18_ = NULL;
					_tmp18_ = beta_radio_getCategoryMenu (self, submenu, group, json);
					group = _tmp18_;
				} else {
					gboolean _tmp19_ = FALSE;
					if (g_strcmp0 (property, "channel") == 0) {
						JsonSoup* _tmp20_ = NULL;
						gboolean _tmp21_;
						_tmp20_ = json_soup_sibling (json, "channel");
						_tmp21_ = json_soup_is_array (_tmp20_);
						_tmp19_ = _tmp21_ == TRUE;
					} else {
						_tmp19_ = FALSE;
					}
					if (_tmp19_) {
						GSList* _tmp22_ = NULL;
						_tmp22_ = beta_radio_getChannelMenu (self, submenu, group, json);
						group = _tmp22_;
					}
				}
				json_soup_parent (list);
				_g_free0 (property);
				_g_object_unref0 (submenu);
				_g_object_unref0 (item);
				_g_free0 (title);
				_g_object_unref0 (json);
				_g_free0 (feed);
			}
		}
	}
	result = group;
	_g_object_unref0 (list);
	return result;
}


static Block1Data* block1_data_ref (Block1Data* _data1_) {
	g_atomic_int_inc (&_data1_->_ref_count_);
	return _data1_;
}


static void block1_data_unref (Block1Data* _data1_) {
	if (g_atomic_int_dec_and_test (&_data1_->_ref_count_)) {
		_g_object_unref0 (_data1_->self);
		_g_free0 (_data1_->url);
		_g_free0 (_data1_->title);
		g_slice_free (Block1Data, _data1_);
	}
}


static void _lambda1_ (GtkCheckMenuItem* e, Block1Data* _data1_) {
	BetaRadio * self;
	gboolean _tmp0_;
	self = _data1_->self;
	g_return_if_fail (e != NULL);
	_tmp0_ = gtk_check_menu_item_get_active (e);
	if (_tmp0_ == TRUE) {
		GstPlayer* _tmp1_ = NULL;
		GstPlayer* _tmp2_;
		_tmp1_ = gst_player_get_instance ();
		_tmp2_ = _tmp1_;
		gst_player_play (_tmp2_, _data1_->url);
		_g_object_unref0 (_tmp2_);
		gtk_status_icon_set_tooltip_text (self->priv->icon, _data1_->title);
	}
}


static void __lambda1__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self) {
	_lambda1_ (_sender, self);
}


static GSList* beta_radio_getCategoryMenu (BetaRadio* self, GtkMenu* menu, GSList* group, JsonSoup* json) {
	GSList* result = NULL;
	gint _tmp0_;
	gint length;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (menu != NULL, NULL);
	g_return_val_if_fail (json != NULL, NULL);
	_tmp0_ = json_soup_length (json);
	length = _tmp0_;
	{
		gint i;
		i = 0;
		{
			gboolean _tmp1_;
			_tmp1_ = TRUE;
			while (TRUE) {
				JsonSoup* _tmp2_ = NULL;
				JsonSoup* _tmp3_ = NULL;
				gchar* _tmp4_ = NULL;
				gchar* category;
				GtkMenuItem* _tmp5_ = NULL;
				GtkMenuItem* item;
				GtkMenu* _tmp6_ = NULL;
				GtkMenu* submenu;
				JsonSoup* _tmp7_ = NULL;
				gint _tmp8_;
				gint size;
				if (!_tmp1_) {
					i++;
				}
				_tmp1_ = FALSE;
				if (!(i < length)) {
					break;
				}
				_tmp2_ = json_soup_array (json, i);
				_tmp3_ = json_soup_object (_tmp2_, "title");
				_tmp4_ = json_soup_get_string (_tmp3_);
				category = _tmp4_;
				_tmp5_ = (GtkMenuItem*) gtk_menu_item_new_with_label (category);
				item = g_object_ref_sink (_tmp5_);
				_tmp6_ = (GtkMenu*) gtk_menu_new ();
				submenu = g_object_ref_sink (_tmp6_);
				gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) item);
				gtk_menu_item_set_submenu (item, (GtkWidget*) submenu);
				_tmp7_ = json_soup_sibling (json, "channel");
				_tmp8_ = json_soup_length (_tmp7_);
				size = _tmp8_;
				{
					gint j;
					j = 0;
					{
						gboolean _tmp9_;
						_tmp9_ = TRUE;
						while (TRUE) {
							Block1Data* _data1_;
							JsonSoup* _tmp10_ = NULL;
							JsonSoup* _tmp11_ = NULL;
							gchar* _tmp12_ = NULL;
							JsonSoup* _tmp13_ = NULL;
							gchar* _tmp14_ = NULL;
							gchar* type;
							JsonSoup* _tmp15_ = NULL;
							gchar* _tmp16_ = NULL;
							gchar* _tmp17_;
							gchar* _tmp18_ = NULL;
							gchar* _tmp19_;
							GtkRadioMenuItem* _tmp20_ = NULL;
							GtkRadioMenuItem* radio;
							GSList* _tmp21_ = NULL;
							_data1_ = g_slice_new0 (Block1Data);
							_data1_->_ref_count_ = 1;
							_data1_->self = g_object_ref (self);
							if (!_tmp9_) {
								j++;
							}
							_tmp9_ = FALSE;
							if (!(j < size)) {
								block1_data_unref (_data1_);
								_data1_ = NULL;
								break;
							}
							_tmp10_ = json_soup_array (json, j);
							_tmp11_ = json_soup_object (_tmp10_, "title");
							_tmp12_ = json_soup_get_string (_tmp11_);
							_data1_->title = _tmp12_;
							_tmp13_ = json_soup_sibling (json, "type");
							_tmp14_ = json_soup_get_string (_tmp13_);
							type = _tmp14_;
							_tmp15_ = json_soup_sibling (json, "url");
							_tmp16_ = json_soup_get_string (_tmp15_);
							_tmp17_ = _tmp16_;
							_tmp18_ = beta_radio_filter_url (self, _tmp17_, type);
							_tmp19_ = _tmp18_;
							_g_free0 (_tmp17_);
							_data1_->url = _tmp19_;
							_tmp20_ = (GtkRadioMenuItem*) gtk_radio_menu_item_new_with_label (group, _data1_->title);
							radio = g_object_ref_sink (_tmp20_);
							_tmp21_ = gtk_radio_menu_item_get_group (radio);
							group = _tmp21_;
							gtk_menu_shell_append ((GtkMenuShell*) submenu, (GtkWidget*) ((GtkMenuItem*) radio));
							g_signal_connect_data ((GtkCheckMenuItem*) radio, "toggled", (GCallback) __lambda1__gtk_check_menu_item_toggled, block1_data_ref (_data1_), (GClosureNotify) block1_data_unref, 0);
							json_soup_grandparent (json);
							_g_object_unref0 (radio);
							_g_free0 (type);
							block1_data_unref (_data1_);
							_data1_ = NULL;
						}
					}
				}
				json_soup_grandparent (json);
				_g_object_unref0 (submenu);
				_g_object_unref0 (item);
				_g_free0 (category);
			}
		}
	}
	result = group;
	return result;
}


static Block2Data* block2_data_ref (Block2Data* _data2_) {
	g_atomic_int_inc (&_data2_->_ref_count_);
	return _data2_;
}


static void block2_data_unref (Block2Data* _data2_) {
	if (g_atomic_int_dec_and_test (&_data2_->_ref_count_)) {
		_g_object_unref0 (_data2_->self);
		_g_free0 (_data2_->url);
		_g_free0 (_data2_->title);
		g_slice_free (Block2Data, _data2_);
	}
}


static void _lambda2_ (GtkCheckMenuItem* e, Block2Data* _data2_) {
	BetaRadio * self;
	gboolean _tmp0_;
	self = _data2_->self;
	g_return_if_fail (e != NULL);
	_tmp0_ = gtk_check_menu_item_get_active (e);
	if (_tmp0_ == TRUE) {
		GstPlayer* _tmp1_ = NULL;
		GstPlayer* _tmp2_;
		_tmp1_ = gst_player_get_instance ();
		_tmp2_ = _tmp1_;
		gst_player_play (_tmp2_, _data2_->url);
		_g_object_unref0 (_tmp2_);
		gtk_status_icon_set_tooltip_text (self->priv->icon, _data2_->title);
	}
}


static void __lambda2__gtk_check_menu_item_toggled (GtkCheckMenuItem* _sender, gpointer self) {
	_lambda2_ (_sender, self);
}


static GSList* beta_radio_getChannelMenu (BetaRadio* self, GtkMenu* menu, GSList* group, JsonSoup* json) {
	GSList* result = NULL;
	gint _tmp0_;
	gint length;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (menu != NULL, NULL);
	g_return_val_if_fail (json != NULL, NULL);
	_tmp0_ = json_soup_length (json);
	length = _tmp0_;
	{
		gint i;
		i = 0;
		{
			gboolean _tmp1_;
			_tmp1_ = TRUE;
			while (TRUE) {
				Block2Data* _data2_;
				JsonSoup* _tmp2_ = NULL;
				JsonSoup* _tmp3_ = NULL;
				gchar* _tmp4_ = NULL;
				JsonSoup* _tmp5_ = NULL;
				gchar* _tmp6_ = NULL;
				gchar* type;
				JsonSoup* _tmp7_ = NULL;
				gchar* _tmp8_ = NULL;
				gchar* _tmp9_;
				gchar* _tmp10_ = NULL;
				gchar* _tmp11_;
				GtkRadioMenuItem* _tmp12_ = NULL;
				GtkRadioMenuItem* radio;
				GSList* _tmp13_ = NULL;
				_data2_ = g_slice_new0 (Block2Data);
				_data2_->_ref_count_ = 1;
				_data2_->self = g_object_ref (self);
				if (!_tmp1_) {
					i++;
				}
				_tmp1_ = FALSE;
				if (!(i < length)) {
					block2_data_unref (_data2_);
					_data2_ = NULL;
					break;
				}
				_tmp2_ = json_soup_array (json, i);
				_tmp3_ = json_soup_object (_tmp2_, "title");
				_tmp4_ = json_soup_get_string (_tmp3_);
				_data2_->title = _tmp4_;
				_tmp5_ = json_soup_sibling (json, "type");
				_tmp6_ = json_soup_get_string (_tmp5_);
				type = _tmp6_;
				_tmp7_ = json_soup_sibling (json, "url");
				_tmp8_ = json_soup_get_string (_tmp7_);
				_tmp9_ = _tmp8_;
				_tmp10_ = beta_radio_filter_url (self, _tmp9_, type);
				_tmp11_ = _tmp10_;
				_g_free0 (_tmp9_);
				_data2_->url = _tmp11_;
				_tmp12_ = (GtkRadioMenuItem*) gtk_radio_menu_item_new_with_label (group, _data2_->title);
				radio = g_object_ref_sink (_tmp12_);
				_tmp13_ = gtk_radio_menu_item_get_group (radio);
				group = _tmp13_;
				gtk_menu_shell_append ((GtkMenuShell*) menu, (GtkWidget*) ((GtkMenuItem*) radio));
				g_signal_connect_data ((GtkCheckMenuItem*) radio, "toggled", (GCallback) __lambda2__gtk_check_menu_item_toggled, block2_data_ref (_data2_), (GClosureNotify) block2_data_unref, 0);
				json_soup_grandparent (json);
				_g_object_unref0 (radio);
				_g_free0 (type);
				block2_data_unref (_data2_);
				_data2_ = NULL;
			}
		}
	}
	result = group;
	return result;
}


static gchar* string_replace (const gchar* self, const gchar* old, const gchar* replacement) {
	gchar* result = NULL;
	gchar* _tmp0_ = NULL;
	gchar* _tmp1_;
	GRegex* _tmp2_ = NULL;
	GRegex* _tmp3_;
	GRegex* regex;
	gchar* _tmp4_ = NULL;
	gchar* _tmp5_;
	GError * _inner_error_ = NULL;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (old != NULL, NULL);
	g_return_val_if_fail (replacement != NULL, NULL);
	_tmp0_ = g_regex_escape_string (old, -1);
	_tmp1_ = _tmp0_;
	_tmp2_ = g_regex_new (_tmp1_, 0, 0, &_inner_error_);
	_tmp3_ = _tmp2_;
	_g_free0 (_tmp1_);
	regex = _tmp3_;
	if (_inner_error_ != NULL) {
		if (_inner_error_->domain == G_REGEX_ERROR) {
			goto __catch0_g_regex_error;
		}
		g_critical ("file %s: line %d: unexpected error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return NULL;
	}
	_tmp4_ = g_regex_replace_literal (regex, self, (gssize) (-1), 0, replacement, 0, &_inner_error_);
	_tmp5_ = _tmp4_;
	if (_inner_error_ != NULL) {
		_g_regex_unref0 (regex);
		if (_inner_error_->domain == G_REGEX_ERROR) {
			goto __catch0_g_regex_error;
		}
		_g_regex_unref0 (regex);
		g_critical ("file %s: line %d: unexpected error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return NULL;
	}
	result = _tmp5_;
	_g_regex_unref0 (regex);
	return result;
	_g_regex_unref0 (regex);
	goto __finally0;
	__catch0_g_regex_error:
	{
		GError * e;
		e = _inner_error_;
		_inner_error_ = NULL;
		g_assert_not_reached ();
		_g_error_free0 (e);
	}
	__finally0:
	if (_inner_error_ != NULL) {
		g_critical ("file %s: line %d: uncaught error: %s (%s, %d)", __FILE__, __LINE__, _inner_error_->message, g_quark_to_string (_inner_error_->domain), _inner_error_->code);
		g_clear_error (&_inner_error_);
		return NULL;
	}
}


static gchar* beta_radio_filter_url (BetaRadio* self, const gchar* url, const gchar* type) {
	gchar* result = NULL;
	gboolean _tmp0_ = FALSE;
	gchar* _tmp6_;
	g_return_val_if_fail (self != NULL, NULL);
	g_return_val_if_fail (url != NULL, NULL);
	g_return_val_if_fail (type != NULL, NULL);
	if (g_strcmp0 (type, "mms") == 0) {
		gboolean _tmp1_;
		_tmp1_ = g_str_has_prefix (url, "http://");
		_tmp0_ = _tmp1_ == TRUE;
	} else {
		_tmp0_ = FALSE;
	}
	if (_tmp0_) {
		gchar* _tmp2_ = NULL;
		gchar* _tmp3_;
		gchar* _tmp4_ = NULL;
		gchar* _tmp5_;
		_tmp2_ = string_replace (url, "http", "mmsh");
		_tmp3_ = _tmp2_;
		_tmp4_ = g_strconcat (_tmp3_, "\\?MSWMExt\\=.asf", NULL);
		_tmp5_ = _tmp4_;
		_g_free0 (_tmp3_);
		result = _tmp5_;
		return result;
	}
	_tmp6_ = g_strdup (url);
	result = _tmp6_;
	return result;
}


static void beta_radio_class_init (BetaRadioClass * klass) {
	beta_radio_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (BetaRadioPrivate));
	G_OBJECT_CLASS (klass)->finalize = beta_radio_finalize;
}


static void beta_radio_instance_init (BetaRadio * self) {
	self->priv = BETA_RADIO_GET_PRIVATE (self);
	self->priv->icon = NULL;
	self->priv->menu = NULL;
}


static void beta_radio_finalize (GObject* obj) {
	BetaRadio * self;
	self = BETA_RADIO (obj);
	_g_object_unref0 (self->priv->icon);
	_g_object_unref0 (self->priv->menu);
	G_OBJECT_CLASS (beta_radio_parent_class)->finalize (obj);
}


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
GType beta_radio_get_type (void) {
	static volatile gsize beta_radio_type_id__volatile = 0;
	if (g_once_init_enter (&beta_radio_type_id__volatile)) {
		static const GTypeInfo g_define_type_info = { sizeof (BetaRadioClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) beta_radio_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (BetaRadio), 0, (GInstanceInitFunc) beta_radio_instance_init, NULL };
		GType beta_radio_type_id;
		beta_radio_type_id = g_type_register_static (G_TYPE_OBJECT, "BetaRadio", &g_define_type_info, 0);
		g_once_init_leave (&beta_radio_type_id__volatile, beta_radio_type_id);
	}
	return beta_radio_type_id__volatile;
}



