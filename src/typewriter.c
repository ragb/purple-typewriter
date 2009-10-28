/*
 * typewriter plugin
 * (c) 2009 - Rui Batista <ruiandrebatista@gmail.com>
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

#define PLUGIN_ID "core-ragb-typewriter"
#define PLUGIN_NAME "typewriter"


/* Purple headers */
#include <account.h>
#include <conversation.h>
#include <debug.h>
#include         <plugin.h>
#include <prefs.h>
#include <sound.h>
#include <version.h>



#define TIMEOUT_ID_KEY PLUGIN_ID "timeout-id"

#define SOUND_FILE_PREF_NAME "/" PLUGIN_ID "/sound_file"
#define INTERVAL_SECONDS_PREF_NAME "/" PLUGIN_ID "/interval_seconds"

static PurplePlugin *plugin_handle = NULL;

static gboolean
play_typewriter_sound (void *data)
{
  PurpleAccount *account;
  PurpleConversation *conv;

  conv = (PurpleConversation *) data;
  account = purple_conversation_get_account (conv);

  purple_sound_play_file (purple_prefs_get_path (SOUND_FILE_PREF_NAME),
			  account);
  return TRUE;
}


static void
buddy_typing_cb (PurpleAccount * account, const char *name, void *unused)
{
  PurpleConversation *conv;
  gint timeout_id;
  void *data;

  /* find conversation */
  conv =
    purple_find_conversation_with_account (PURPLE_CONV_TYPE_IM, name,
					   account);

  if (!conv)
    {
      purple_debug_warning (PLUGIN_ID,
			    "Can't find conversation with name %s", name);
      return;
    }

  /* see if we have a timeout running, if no create one */
  data = purple_conversation_get_data (conv, TIMEOUT_ID_KEY);
  if (!data)
    {
      /* create timeout */
      timeout_id =
	g_timeout_add_seconds (purple_prefs_get_int
			       (INTERVAL_SECONDS_PREF_NAME),
			       play_typewriter_sound, conv);

      purple_conversation_set_data (conv, TIMEOUT_ID_KEY,
				    (void *) timeout_id);
    }

}


static void
buddy_typing_stopped_cb (PurpleAccount *
			 account, const char *name, void *unused)
{
  PurpleConversation *conv;
  void *data;
  gint timeout_id;
  conv =
    purple_find_conversation_with_account
    (PURPLE_CONV_TYPE_IM, name, account);

  if (!conv)
    {
      purple_debug_warning (PLUGIN_ID,
			    "Can't find conversation %s, on buddy stopped typing",
			    name);
      return;
    }

  data = purple_conversation_get_data (conv, TIMEOUT_ID_KEY);
  if (!data)
    {
      purple_debug_warning (PLUGIN_ID,
			    "Can't find timeout id in conversation data");
      return;
    }

  timeout_id = (gint) data;
  g_source_remove (timeout_id);
  purple_conversation_set_data (conv, TIMEOUT_ID_KEY, NULL);
}

static gboolean
plugin_load (PurplePlugin * plugin)
{
  purple_debug_info (PLUGIN_ID, "Loading plugin");
  purple_signal_connect (purple_conversations_get_handle
			 (), "buddy-typing", plugin,
			 PURPLE_CALLBACK (buddy_typing_cb), NULL);

  purple_signal_connect (purple_conversations_get_handle (),
			 "buddy-typing-stopped", plugin,
			 PURPLE_CALLBACK (buddy_typing_stopped_cb), NULL);

  /* store reference for the plugin handle */
  plugin_handle = plugin;
  return TRUE;
}

/* For specific notes on the meanings of each of these members, consult the C Plugin Howto
 * on the website. */
static PurplePluginInfo info = {
  PURPLE_PLUGIN_MAGIC, PURPLE_MAJOR_VERSION, PURPLE_MINOR_VERSION, PURPLE_PLUGIN_STANDARD, NULL, 0, NULL, PURPLE_PRIORITY_DEFAULT, PLUGIN_ID, NULL, PACKAGE_VERSION, NULL, NULL, "ruiandrebatista@gmail.com",	/* correct author */
  "http://outputstream.wordpress.com", plugin_load, NULL, NULL, NULL, NULL, NULL, NULL,	/* this tells libpurple the address of the function to call
											   to get the list of plugin actions. */
  NULL,
  NULL,
  NULL,
  NULL
};

static void
init_plugin (PurplePlugin * plugin)
{

  info.name = _("Typewriter");
  info.summary = _("Typewriter plugin");
  info.description =
    _("Typewriter plugin, plays sounds when buddies are typing");

  /* Create preferences */
  purple_prefs_add_none ("/" PLUGIN_ID);

  /* Sound interval */
  purple_prefs_add_int (INTERVAL_SECONDS_PREF_NAME, 2);

  /* Sound file path */
  purple_prefs_add_path (SOUND_FILE_PREF_NAME,
			 PURPLE_SOUNDSDIR "/typewriter.wav");
}

PURPLE_INIT_PLUGIN (typewriter, init_plugin, info)
