/*
 * Copyright (C) 2013 Lukasz Skalski <lukasz.skalski@op.pl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <stdlib.h>
#include <gio/gio.h>

gchar *testname = "org.freedesktop.systemd1";

/* Commandline options */
GOptionEntry entries[] =
{
  { "name", 'n', 0, G_OPTION_ARG_STRING, &testname, "valid dbus-name [default: org.freedesktop.systemd1]", NULL },
  { NULL }
};

int main (int argc, char** argv)
{
  GDBusConnection *connection = NULL;
  GOptionContext *context = NULL;
  GError *error = NULL;

  /* connect to the bus */
  connection = g_bus_get_sync (G_BUS_TYPE_MACHINE, NULL, &error);
  if (connection == NULL)
    {
      g_print ("Error connecting to D-Bus session bus: %s\n", error->message);
      g_error_free (error);
      exit (EXIT_FAILURE);
    }

  /* parse commandline options */
  context = g_option_context_new ("- GLib-kdbus new API test");
  g_option_context_add_main_entries (context, entries, NULL);
  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_print ("%s\n", error->message);
      g_error_free (error);
      exit (EXIT_FAILURE);
    }

  if (!g_dbus_is_name (testname))
    {
      g_print ("Please use valid d-bus name\n");
      exit (EXIT_FAILURE);
    }

 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_bus_id() function
  *
  * Returns: the unique ID of the bus or %NULL if @error is set.
  *          Free with g_free().
  *
  * -------------------------------------------------------------------------- */

  gchar *bus_id = g_dbus_get_bus_id (connection, &error);
  if (bus_id == NULL)
    {
      g_error ("g_dbus_get_bus_id(): %s\n", error->message);
      goto error;
    }
  else
    g_print ("\ng_dbus_get_bus_id() - bus_id: %s\n", bus_id);
  g_free (bus_id);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_list_names() fuction
  *
  * Returns: a list of all currently-owned names on the bus or %NULL if
  *          @error is set. Free with g_strfreev().
  *
  * -------------------------------------------------------------------------- */

  gchar **list_names = g_dbus_get_list_names (connection, &error);
  if (list_names == NULL)
    {
      g_error ("g_dbus_get_list_names(): %s\n", error->message);
      goto error;
    }
  else
    {
      int i=0;
      g_print ("\ng_dbus_get_list_names()\n");
      while (list_names[i])
        g_print ("   %s\n", list_names[i++]);
    }
  g_strfreev (list_names);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_list_activatable_names() function
  *
  * Returns: a list of all names that can be activated on the bus or %NULL if
  *          @error is set. Free with g_strfreev().
  *
  * -------------------------------------------------------------------------- */

  gchar **list_act_names = g_dbus_get_list_activatable_names (connection, &error);
  if (list_act_names == NULL)
    {
      g_error ("g_dbus_get_list_activatable_names(): %s\n", error->message);
      goto error;
    }
  else
    {
      int i = 0;
      g_print ("\ng_dbus_get_list_activatable_names()\n");
      while (list_act_names[i])
        g_print ("   %s\n", list_act_names[i++]);
    }
  g_strfreev (list_act_names);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_list_queued_owners() function
  *
  * Returns: the unique bus names of connections currently queued for
  *          the @name or %NULL if @error is set. Free with g_strfreev().
  *
  * -------------------------------------------------------------------------- */

  gchar **list_que_own = g_dbus_get_list_queued_owners (connection, testname, &error);
  if (list_que_own == NULL)
    {
      g_error ("g_dbus_get_list_queued_owners(): %s\n", error->message);
      goto error;
    }
  else
    {
      int i=0;
      g_print ("\ng_dbus_get_list_queued_owners\n");
      while (list_que_own[i])
        g_print ("   %s\n", list_que_own[i++]);
    }
  g_strfreev (list_que_own);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_name_owner() function
  *
  * Returns: the unique connection name of the primary owner of the
  *          name given. If the requested name doesn't have an owner,
  *          function returns %NULL and @error is set. Free with g_free().
  *
  * -------------------------------------------------------------------------- */

  gchar *owner = g_dbus_get_name_owner (connection, testname, &error);
  if (owner == NULL)
    {
      g_error ("g_dbus_get_name_owner(): %s\n", error->message);
      goto error;
    }
  else
    g_print ("\ng_dbus_get_name_owner() - name: %s, owner: %s\n", testname, owner);
  g_free (owner);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_name_has_owner() function
  *
  * Returns: %TRUE if specified name exists (currently has an owner)
  *          If the requested name doesn't have an owner or @error is
  *          set, function returns %FALSE.
  *
  * -------------------------------------------------------------------------- */

  gboolean has_owner = g_dbus_name_has_owner (connection, testname, &error);
  if (NULL)
    {
      g_error ("g_dbus_name_has_owner(): %s\n", error->message);
      goto error;
    }
  else
    g_print ("\ng_dbus_name_has_owner() - name: %s, has_owner: %d\n", testname, has_owner);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_get_connection_pid() function
  *
  * Returns: the Unix process ID of the process connected to the bus or
  *          (guint32) -1 if @error is set.
  *
  * -------------------------------------------------------------------------- */

  guint32 pid = g_dbus_get_connection_pid (connection, testname, &error);
  if (pid == -1)
    {
      g_error ("g_dbus_get_connection_pid(): %s\n", error->message);
      goto error;
    }
  else
    g_print ("\ng_dbus_get_connection_pid() - name: %s, PID: %d\n", testname, pid);


 /* --------------------------------------------------------------------------
  *  
  * Test g_dbus_name_has_owner() function
  *
  * Returns: the Unix user ID of the process connected to the bus or
  *          (guint32) -1 if @error is set.
  *
  * -------------------------------------------------------------------------- */

  guint32 uid = g_dbus_get_connection_uid (connection, testname, &error);
  if (uid == -1)
    {
      g_error ("error: g_dbus_get_connection_uid(): %s\n", error->message);
      goto error;
    }
  else
    g_print ("\ng_dbus_get_connection_uid() - name: %s, UID: %d\n", testname, uid);


  return EXIT_SUCCESS;
error:
  g_error_free (error);
  return EXIT_FAILURE;
}
