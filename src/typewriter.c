/*
 * typewriter plugin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 *
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <glib/gi18n-lib.h>


/* config.h may define PURPLE_PLUGINS; protect the definition here so that we
 * don't get complaints about redefinition when it's not necessary. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#define PLUGIN_ID "gtk-ragb-typewriter"
#define PLUGIN_NAME "typewriter"


/* Purple headers */
#include <account.h>
#include <conversation.h>
#include <debug.h>
#include         <plugin.h>
#include <sound.h>
#include <version.h>

static PurplePlugin *plugin_handle = NULL;

static void boddy_typing_cb(PurpleAccount * account, const char *name,
			    void *data)
{
    purple_debug_misc(PLUGIN_ID, "Boddy %s is typing, making sound", name);
    purple_sound_play_file("/usr/share/sounds/question.wav", account);
    g_print("Called callback");
}


static gboolean plugin_load(PurplePlugin * plugin)
{
    purple_debug_info(PLUGIN_ID, "Loading plugin");

    purple_signal_connect(purple_conversations_get_handle(),
			  "boddy-typing", plugin,
			  PURPLE_CALLBACK(boddy_typing_cb), NULL);

    /* store reference for the plugin handle */
    plugin_handle = plugin;
    return TRUE;
}

/* For specific notes on the meanings of each of these members, consult the C Plugin Howto
 * on the website. */
static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    PLUGIN_ID,
    NULL,
    PACKAGE_VERSION,

    NULL,
    NULL,
    "ruiandrebatista@gmail.com",	/* correct author */
    "http://outputstream.wordpress.com",


    plugin_load,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
    NULL,			/* this tells libpurple the address of the function to call
				   to get the list of plugin actions. */
    NULL,
    NULL,
    NULL,
    NULL
};

static void init_plugin(PurplePlugin * plugin)
{

    info.name = _("Typewriter");
    info.summary = _("Typewriter plugin");
    info.description =
	_("Typewriter plugin, plays sounds when boddies are typing");
}

PURPLE_INIT_PLUGIN(typewriter, init_plugin, info)
