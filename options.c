/***************************************************************************
        xbindkeys : a program to bind keys to commands under X11.
                           -------------------
    begin                : Sat Oct 13 14:11:34 CEST 2001
    copyright            : (C) 2001 by Philippe Brochard
    email                : hocwp@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include <ctype.h>
#include "options.h"
#include "xbindkeys.h"
#include "keys.h"
#include "grab_key.h"

#ifdef GUILE_FLAG
#include <libguile.h>
#endif

char *display_name = NULL;

char rc_file[512];
#ifdef GUILE_FLAG
char rc_guile_file[512];
#endif

int verbose = 0;
int poll_rc = 0;
int have_to_show_binding = 0;
int have_to_get_binding = 0;
int have_to_start_as_daemon = 1;
int detectable_ar = 0;

char *geom = NULL;


static void show_version (void);
static void show_help (void);
static void show_defaults_rc (void);
#ifdef GUILE_FLAG
static void show_defaults_guile_rc (void);
int init_xbk_guile_fns (void);
SCM set_numlock_wrapper (SCM x);
SCM set_scrolllock_wrapper (SCM x);
SCM set_capslock_wrapper (SCM x);
SCM xbindkey_wrapper(SCM key, SCM cmd);
SCM xbindkey_function_wrapper(SCM key, SCM fun);
SCM remove_xbindkey_wrapper(SCM key);
SCM run_command_wrapper (SCM command);
SCM grab_all_keys_wrapper (void);
SCM ungrab_all_keys_wrapper (void);
SCM remove_all_keys_wrapper (void);
SCM debug_info_wrapper (void);
#endif



void
get_options (int argc, char **argv)
{
  int i;
  char *home;

  strncpy (rc_file, "", sizeof(rc_file));
#ifdef GUILE_FLAG
  strncpy (rc_guile_file, "", sizeof (rc_guile_file));
#endif

  verbose = 0;
  have_to_show_binding = 0;
  have_to_get_binding = 0;
  have_to_start_as_daemon = 1;


  for (i = 1; i < argc; i++)
    {
      if (strcmp (argv[i], "-V") == 0 || strcmp (argv[i], "--version") == 0)
	{
	  show_version ();
	  exit (1);
	}
      else if ((strcmp (argv[i], "-X") == 0
		|| strcmp (argv[i], "--display") == 0) && i + 1 < argc)
	{
	  display_name = argv[++i];
	}
      else if ((strcmp (argv[i], "-f") == 0
		|| strcmp (argv[i], "--file") == 0) && i + 1 < argc)
	{
	  strncpy (rc_file, argv[++i], sizeof (rc_file) - 1);
	}
#ifdef GUILE_FLAG
      else if ((strcmp (argv[i], "-fg") == 0
		|| strcmp (argv[i], "--guile-file") == 0) && i + 1 < argc)
	{
	  strncpy (rc_guile_file, argv[++i], sizeof (rc_guile_file) - 1);
	}
#endif
      else if (strcmp (argv[i], "-p") == 0 || strcmp (argv[i], "--poll-rc") == 0)
	{
	  poll_rc = 1;
	}
      else if (strcmp (argv[i], "-s") == 0 || strcmp (argv[i], "--show") == 0)
	{
	  have_to_show_binding = 1;
	}
      else if (strcmp (argv[i], "-k") == 0 || strcmp (argv[i], "--key") == 0)
	{
	  have_to_get_binding = 1;
	}
      else if (strcmp (argv[i], "-mk") == 0
	       || strcmp (argv[i], "--multikey") == 0)
	{
	  have_to_get_binding = 2;
	}
      else if (strcmp (argv[i], "-v") == 0
	       || strcmp (argv[i], "--verbose") == 0)
	{
	  verbose = 1;
	  have_to_start_as_daemon = 0;
	}
      else if (strcmp (argv[i], "-d") == 0
	       || strcmp (argv[i], "--defaults") == 0)
	{
	  show_defaults_rc ();
	}
#ifdef GUILE_FLAG
      else if (strcmp (argv[i], "-dg") == 0
	       || strcmp (argv[i], "--defaults-guile") == 0)
	{
	  show_defaults_guile_rc ();
	}
#endif
      else if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "--help") == 0)
	{
	  show_help ();
	  exit (1);
	}
      else if ((strcmp (argv[i], "-g") == 0
		|| strcmp (argv[i], "--geometry") == 0) && i + 1 < argc)
	{
	  geom = argv[++i];
	}
      else if (strcmp (argv[i], "-n") == 0
	       || strcmp (argv[i], "--nodaemon") == 0)
	{
	  have_to_start_as_daemon = 0;
	}
      else if (strcmp (argv[i], "-sd") == 0
	       || strcmp (argv[i], "--detectable-ar") == 0)
	{
	  detectable_ar = 1;
	}
      else
	{
	  show_help ();
	  exit (1);
	}
    }

  if (strcmp (rc_file, "") == 0)
    {
      home = getenv ("HOME");

      if (rc_file != NULL)
	{
	  strncpy (rc_file, home, sizeof (rc_file) - 20);
	  strncat (rc_file, "/.xbindkeysrc", sizeof (rc_file));
	}
    }

#ifdef GUILE_FLAG
  if (strcmp (rc_guile_file, "") == 0)
    {
      home = getenv ("HOME");

      if (rc_guile_file != NULL)
	{
	  strncpy (rc_guile_file, home, sizeof (rc_guile_file) - 20);
	  strncat (rc_guile_file, "/.xbindkeysrc.scm", sizeof (rc_guile_file));
	}
    }
#endif
}

void
show_options (void)
{
  if (verbose)
    {
      printf ("displayName = %s\n", display_name);
      printf ("rc file = %s\n", rc_file);
#ifdef GUILE_FLAG
      printf ("rc guile file = %s\n", rc_guile_file);
#endif
    }
}


static void
show_version (void)
{
  fprintf (stderr, "xbindkeys %s by Philippe Brochard\n", PACKAGE_VERSION);
}

static void
show_help (void)
{
  show_version ();

  fprintf (stderr, "usage: xbindkeys [options]\n");
  fprintf (stderr, "  where options are:\n");

  fprintf (stderr, "  -V, --version           Print version and exit\n");
  fprintf (stderr, "  -d, --defaults          Print a default rc file\n");
#ifdef GUILE_FLAG
  fprintf (stderr, " -dg, --defaults-guile    Print a default guile configuration file\n");
#endif
  fprintf (stderr, "  -f, --file              Use an alternative rc file\n");
#ifdef GUILE_FLAG
  fprintf (stderr, " -fg, --file-guile        Use an alternative guile configuration file\n");
#endif
  fprintf (stderr, "  -p, --poll-rc           Poll the rc/guile configs for updates\n");
  fprintf (stderr, "  -h, --help              This help!\n");
  fprintf (stderr, "  -X, --display           Set X display to use\n");
  fprintf (stderr,
	   "  -v, --verbose           More information on xbindkeys when it run\n");
  fprintf (stderr, "  -s, --show              Show the actual keybinding\n");
  fprintf (stderr, "  -k, --key               Identify one key pressed\n");
  fprintf (stderr, " -mk, --multikey          Identify multi key pressed\n");
  fprintf (stderr,
	   "  -g, --geometry          size and position of window open with -k|-mk option\n");
  fprintf (stderr, "  -n, --nodaemon          don't start as daemon\n");
}


static void
show_defaults_rc (void)
{
  printf ("# For the benefit of emacs users: -*- shell-script -*-\n");
  printf ("###########################\n");
  printf ("# xbindkeys configuration #\n");
  printf ("###########################\n");
  printf ("#\n");
  printf ("# Version: %s\n", PACKAGE_VERSION);
  printf ("#\n");
  printf ("# If you edit this file, do not forget to uncomment any lines\n");
  printf ("# that you change.\n");
  printf ("# The pound(#) symbol may be used anywhere for comments.\n");
  printf ("#\n");

  printf ("# To specify a key, you can use 'xbindkeys --key' or\n");
  printf
    ("# 'xbindkeys --multikey' and put one of the two lines in this file.\n");
  printf ("#\n");
  printf ("# The format of a command line is:\n");
  printf ("#    \"command to start\"\n");
  printf ("#       associated key\n");
  printf ("#\n");
  printf ("#\n");
  printf ("# A list of keys is in /usr/include/X11/keysym.h and in\n");
  printf ("# /usr/include/X11/keysymdef.h\n");
  printf ("# The XK_ is not needed.\n");
  printf ("#\n");
  printf ("# List of modifier:\n");
  printf ("#   Release, Control, Shift, Mod1 (Alt), Mod2 (NumLock),\n");
  printf ("#   Mod3 (CapsLock), Mod4, Mod5 (Scroll).\n");
  printf ("#\n");
  printf ("\n");
  printf ("# The release modifier is not a standard X modifier, but you can\n");
  printf ("# use it if you want to catch release events instead of press events\n");
  printf ("\n");
  printf
    ("# By defaults, xbindkeys does not pay attention with the modifiers\n");
  printf ("# NumLock, CapsLock and ScrollLock.\n");
  printf
    ("# Uncomment the lines above if you want to pay attention to them.\n");
  printf ("\n");
  printf ("#keystate_numlock = enable\n");
  printf ("#keystate_capslock = enable\n");
  printf ("#keystate_scrolllock= enable\n");
  printf ("\n");
  printf ("# Examples of commands:\n");
  printf ("\n");

  printf ("\"xbindkeys_show\" \n");
  printf ("  control+shift + q\n");
  printf ("\n");
  printf ("# set directly keycode (here control + f with my keyboard)\n");
  printf ("\"xterm\"\n");
  printf ("  c:41 + m:0x4\n");
  printf ("\n");
  printf ("# specify a mouse button\n");
  printf ("\"xterm\"\n");
  printf ("  control + b:2\n");
  printf ("\n");
  printf ("#\"xterm -geom 50x20+20+20\"\n");
  printf ("#   Shift+Mod2+alt + s\n");
  printf ("#\n");
  printf
    ("## set directly keycode (here control+alt+mod2 + f with my keyboard)\n");
  printf ("#\"xterm\"\n");
  printf ("#  alt + c:0x29 + m:4 + mod2\n");
  printf ("#\n");
  printf ("## Control+Shift+a  release event starts rxvt\n");
  printf ("#\"rxvt\"\n");
  printf ("#  release+control+shift + a\n");
  printf ("#\n");
  printf ("## Control + mouse button 2 release event starts rxvt\n");
  printf ("#\"rxvt\"\n");
  printf ("#  Control + b:2 + Release\n");

  printf ("\n");
  printf ("##################################\n");
  printf ("# End of xbindkeys configuration #\n");
  printf ("##################################\n");

  exit (1);
}


#ifdef GUILE_FLAG
static void
show_defaults_guile_rc (void)
{
  printf (";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
  printf (";; Start of xbindkeys guile configuration ;;\n");
  printf (";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
  printf (";; This configuration is guile based.\n");
  printf (";;   http://www.gnu.org/software/guile/guile.html\n");
  printf (";; any functions that work in guile will work here.\n");
  printf (";; see EXTRA FUNCTIONS:\n");
  printf ("\n");
  printf (";; Version: %s\n", PACKAGE_VERSION);
  printf ("\n");
  printf (";; If you edit this file, do not forget to uncomment any lines\n");
  printf (";; that you change.\n");
  printf (";; The semicolon(;) symbol may be used anywhere for comments.\n");
  printf ("\n");
  printf (";; To specify a key, you can use 'xbindkeys --key' or\n");
  printf (";; 'xbindkeys --multikey' and put one of the two lines in this file.\n");
  printf ("\n");
  printf (";; A list of keys is in /usr/include/X11/keysym.h and in\n");
  printf (";; /usr/include/X11/keysymdef.h\n");
  printf (";; The XK_ is not needed.\n");
  printf ("\n");
  printf (";; List of modifier:\n");
  printf (";;   Release, Control, Shift, Mod1 (Alt), Mod2 (NumLock),\n");
  printf (";;   Mod3 (CapsLock), Mod4, Mod5 (Scroll).\n");
  printf ("\n");
  printf ("\n");
  printf (";; The release modifier is not a standard X modifier, but you can\n");
  printf (";; use it if you want to catch release instead of press events\n");
  printf ("\n");
  printf (";; By defaults, xbindkeys does not pay attention to modifiers\n");
  printf (";; NumLock, CapsLock and ScrollLock.\n");
  printf (";; Uncomment the lines below if you want to use them.\n");
  printf (";; To dissable them, call the functions with #f\n");
  printf ("\n");
  printf ("\n");
  printf (";;;;EXTRA FUNCTIONS: Enable numlock, scrolllock or capslock usage\n");
  printf (";;(set-numlock! #t)\n");
  printf (";;(set-scrolllock! #t)\n");
  printf (";;(set-capslock! #t)\n");
  printf ("\n");
  printf (";;;;; Scheme API reference\n");
  printf (";;;;\n");
  printf (";; Optional modifier state:\n");
  printf (";; (set-numlock! #f or #t)\n");
  printf (";; (set-scrolllock! #f or #t)\n");
  printf (";; (set-capslock! #f or #t)\n");
  printf (";; \n");
  printf (";; Shell command key:\n");
  printf (";; (xbindkey key \"foo-bar-command [args]\")\n");
  printf (";; (xbindkey '(modifier* key) \"foo-bar-command [args]\")\n");
  printf (";; \n");
  printf (";; Scheme function key:\n");
  printf (";; (xbindkey-function key function-name-or-lambda-function)\n");
  printf (";; (xbindkey-function '(modifier* key) function-name-or-lambda-function)\n");
  printf (";; \n");
  printf (";; Other functions:\n");
  printf (";; (remove-xbindkey key)\n");
  printf (";; (run-command \"foo-bar-command [args]\")\n");
  printf (";; (grab-all-keys)\n");
  printf (";; (ungrab-all-keys)\n");
  printf (";; (remove-all-keys)\n");
  printf (";; (debug)\n");
  printf ("\n");
  printf ("\n");
  printf (";; Examples of commands:\n");
  printf ("\n");
  printf ("(xbindkey '(control shift q) \"xbindkeys_show\")\n");
  printf ("\n");
  printf (";; set directly keycode (here control + f with my keyboard)\n");
  printf ("(xbindkey '(\"m:0x4\" \"c:41\") \"xterm\")\n");
  printf ("\n");
  printf (";; specify a mouse button\n");
  printf ("(xbindkey '(control \"b:2\") \"xterm\")\n");
  printf ("\n");
  printf (";;(xbindkey '(shift mod2 alt s) \"xterm -geom 50x20+20+20\")\n");
  printf ("\n");
  printf (";; set directly keycode (control+alt+mod2 + f with my keyboard)\n");
  printf ("(xbindkey '(alt \"m:4\" mod2 \"c:0x29\") \"xterm\")\n");
  printf ("\n");
  printf (";; Control+Shift+a  release event starts rxvt\n");
  printf (";;(xbindkey '(release control shift a) \"rxvt\")\n");
  printf ("\n");
  printf (";; Control + mouse button 2 release event starts rxvt\n");
  printf (";;(xbindkey '(releace control \"b:2\") \"rxvt\")\n");
  printf ("\n");
  printf ("\n");
  printf (";; Extra features\n");
  printf ("(xbindkey-function '(control a)\n");
  printf ("		   (lambda ()\n");
  printf ("		     (display \"Hello from Scheme!\")\n");
  printf ("		     (newline)))\n");
  printf ("\n");
  printf ("(xbindkey-function '(shift p)\n");
  printf ("		   (lambda ()\n");
  printf ("		     (run-command \"xterm\")))\n");
  printf ("\n");
  printf ("\n");
  printf (";; Double click test\n");
  printf ("(xbindkey-function '(control w)\n");
  printf ("		   (let ((count 0))\n");
  printf ("		     (lambda ()\n");
  printf ("		       (set! count (+ count 1))\n");
  printf ("		       (if (> count 1)\n");
  printf ("			   (begin\n");
  printf ("			    (set! count 0)\n");
  printf ("			    (run-command \"xterm\"))))))\n");
  printf ("\n");
  printf (";; Time double click test:\n");
  printf (";;  - short double click -> run an xterm\n");
  printf (";;  - long  double click -> run an rxvt\n");
  printf ("(xbindkey-function '(shift w)\n");
  printf ("		   (let ((time (current-time))\n");
  printf ("			 (count 0))\n");
  printf ("		     (lambda ()\n");
  printf ("		       (set! count (+ count 1))\n");
  printf ("		       (if (> count 1)\n");
  printf ("			   (begin\n");
  printf ("			    (if (< (- (current-time) time) 1)\n");
  printf ("				(run-command \"xterm\")\n");
  printf ("				(run-command \"rxvt\"))\n");
  printf ("			    (set! count 0)))\n");
  printf ("		       (set! time (current-time)))))\n");
  printf ("\n");
  printf ("\n");
  printf (";; Chording keys test: Start differents program if only one key is\n");
  printf (";; pressed or another if two keys are pressed.\n");
  printf (";; If key1 is pressed start cmd-k1\n");
  printf (";; If key2 is pressed start cmd-k2\n");
  printf (";; If both are pressed start cmd-k1-k2 or cmd-k2-k1 following the\n");
  printf (";;   release order\n");
  printf ("(define (define-chord-keys key1 key2 cmd-k1 cmd-k2 cmd-k1-k2 cmd-k2-k1)\n");
  printf ("    \"Define chording keys\"\n");
  printf ("  (let ((k1 #f) (k2 #f))\n");
  printf ("    (xbindkey-function key1 (lambda () (set! k1 #t)))\n");
  printf ("    (xbindkey-function key2 (lambda () (set! k2 #t)))\n");
  printf ("    (xbindkey-function (cons 'release key1)\n");
  printf ("		       (lambda ()\n");
  printf ("			 (if (and k1 k2)\n");
  printf ("			     (run-command cmd-k1-k2)\n");
  printf ("			     (if k1 (run-command cmd-k1)))\n");
  printf ("			 (set! k1 #f) (set! k2 #f)))\n");
  printf ("    (xbindkey-function (cons 'release key2)\n");
  printf ("		       (lambda ()\n");
  printf ("			 (if (and k1 k2)\n");
  printf ("			     (run-command cmd-k2-k1)\n");
  printf ("			     (if k2 (run-command cmd-k2)))\n");
  printf ("			 (set! k1 #f) (set! k2 #f)))))\n");
  printf ("\n");
  printf ("\n");
  printf (";; Example:\n");
  printf (";;   Shift + b:1                   start an xterm\n");
  printf (";;   Shift + b:3                   start an rxvt\n");
  printf (";;   Shift + b:1 then Shift + b:3  start gv\n");
  printf (";;   Shift + b:3 then Shift + b:1  start xpdf\n");
  printf ("\n");
  printf ("(define-chord-keys '(shift \"b:1\") '(shift \"b:3\")\n");
  printf ("  \"xterm\" \"rxvt\" \"gv\" \"xpdf\")\n");
  printf ("\n");
  printf (";; Here the release order have no importance\n");
  printf (";; (the same program is started in both case)\n");
  printf ("(define-chord-keys '(alt \"b:1\") '(alt \"b:3\")\n");
  printf ("  \"gv\" \"xpdf\" \"xterm\" \"xterm\")\n");
  printf ("\n");
  printf ("\n");
  printf (";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");
  printf (";; End of xbindkeys guile configuration ;;\n");
  printf (";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n");

  exit (1);
}
#endif



static int
file_exist (char * filename)
{
  FILE * stream;

  if ((stream = fopen (filename, "r")) == NULL)
      return 0;

  fclose (stream);
  return 1;
}


int
rc_file_exist (void)
{
#ifdef GUILE_FLAG
  if (file_exist (rc_guile_file))
    return 1;
#endif

  if (!file_exist (rc_file))
    {
      fprintf (stderr, "Error : %s not found or reading not allowed.\n",
	       rc_file);
      fprintf (stderr,
	       "please, create one with 'xbindkeys --defaults > %s'.\n",
	       rc_file);
#ifdef GUILE_FLAG
      fprintf (stderr,
	       "or, if you want scheme configuration style,\n");
      fprintf (stderr,
	       "with 'xbindkeys --defaults-guile > %s'.\n",
	       rc_guile_file);
#endif
      return 0;
    }

  return 1;
}





int
get_rc_file (void)
{
  char line[1024];
  char line2[1024];
  char command[1024];
  KeyType_t type;
  EventType_t event_type;
  KeySym keysym;
  KeyCode keycode;
  unsigned int button;
  unsigned int modifier;
  FILE *stream = NULL;
  char *pos1;
  char *pos2;
  char *p;
  int i;


  if (init_keys () != 0)
    return (-1);

  /* Open RC File */
  if ((stream = fopen (rc_file, "r")) == NULL)
    {
      fprintf (stderr, "Error : %s not found or reading not allowed.\n",
	       rc_file);
      fprintf (stderr,
	       "please, create one with 'xbindkeys --defaults > %s'.\n",
	       rc_file);
      return (-1);
    }


  /* Read RC file */
  while (fgets (line, sizeof (line), stream))
    {
      /* search for keystate options */
      if (strstr (line, "keystate_numlock") != NULL)
	{
	  /* search for comment or command line */
	  pos1 = strchr (line, '"');
	  pos2 = strchr (line, '#');
	  if (!pos1 && !pos2)
	    {
	      if (strstr (line, "enable") != NULL)
		{
		  numlock_mask = 0;
		}
	      if (verbose)
		printf ("keystate_numlock: %s\n",
			numlock_mask == 0 ? "Enabled" : "Disabled");

	      continue;
	    }
	}

      if (strstr (line, "keystate_capslock") != NULL)
	{
	  /* search for comment or command line */
	  pos1 = strchr (line, '"');
	  pos2 = strchr (line, '#');
	  if (!pos1 && !pos2)
	    {
	      if (strstr (line, "enable") != NULL)
		{
		  capslock_mask = 0;
		}
	      if (verbose)
		printf ("keystate_capslock: %s\n",
			capslock_mask == 0 ? "Enabled" : "Disabled");

	      continue;
	    }
	}

      if (strstr (line, "keystate_scrolllock") != NULL)
	{
	  /* search for comment or command line */
	  pos1 = strchr (line, '"');
	  pos2 = strchr (line, '#');
	  if (!pos1 && !pos2)
	    {
	      if (strstr (line, "enable") != NULL)
		{
		  scrolllock_mask = 0;
		}
	      if (verbose)
		printf ("keystate_scrolllock: %s\n",
			scrolllock_mask == 0 ? "Enabled" : "Disabled");

	      continue;
	    }
	}


      pos1 = strchr (line, '"');
      if (pos1)
	{
	  pos2 = strchr (line, '#');

	  if (!pos2 || pos2 > pos1)
	    {
	      /* search for command line */
	      pos2 = strrchr (line, '"');
	      if (pos2 && pos1 < pos2)
		{
		  command[0] = '\0';
		  type = SYM;
		  event_type = PRESS;
		  keysym = 0;
		  keycode = 0;
		  button = 0;
		  modifier = 0;

		  for (p = pos1 + 1, i = 0;
		       p < pos2 && i < sizeof (command); p++, i++)
		    {
		      command[i] = *p;
		    }
		  command[i] = '\0';

		  /* get associated keys */
		  if (fgets (line, sizeof (line), stream))
		    {
		      pos1 = line;

		      while (*pos1 != '\n')
			{
			  /* jump space */
			  for (; *pos1 == '+' || *pos1 == ' '
			       || *pos1 == '\t'; pos1++);

			  /* find corresponding + or \n */
			  pos2 = strchr (pos1, '+');
			  if (!pos2)
			    {
			      for (pos2 = pos1; *pos2 != '\n'; pos2++);
			    }

			  /* copy string in line2 */
			  for (p = pos1, i = 0;
			       p < pos2 && i < sizeof (line2)
			       && *p != '+' && *p != ' ' && *p != '\t';
			       p++, i++)
			    {
			      line2[i] = *p;
			    }
			  line2[i] = '\0';

			  /* is a numeric keycode (c:nnn) ? */
			  if (line2[0] == 'c' && line2[1] == ':')
			    {
			      if (isdigit (line2[2]))
				{
				  type = CODE;
				  keycode = strtol (line2+2, (char **) NULL, 0);
				}
			      else
				{
				  keysym = 0;
				  keycode = 0;
				  button = 0;
				  break;
				}
			    }
			  else
			    /* is a numeric modifier (m:nnn) ? */
			  if (line2[0] == 'm' && line2[1] == ':')
			    {
			      if (isdigit (line2[2]))
				{
				  modifier |= strtol (line2+2, (char **) NULL, 0);
				}
			      else
				{
				  keysym = 0;
				  keycode = 0;
				  button = 0;
				  break;
				}
			    }
			  else
			    /* is a mouse button (b:nnn) ? */
			  if (line2[0] == 'b' && line2[1] == ':')
			    {
			      if (isdigit (line2[2]))
				{
				  type = BUTTON;
				  button = strtol (line2+2, (char **) NULL, 0);
				}
			      else
				{
				  keysym = 0;
				  keycode = 0;
				  button = 0;
				  break;
				}
			    }
			  else
			    {
			      /* apply to modifier, release/press or key */
			      if (strcasecmp (line2, "control") == 0)
				modifier |= ControlMask;
			      else if (strcasecmp (line2, "shift") == 0)
				modifier |= ShiftMask;
			      else if (strcasecmp (line2, "mod1") == 0
				       || strcasecmp (line2, "alt") == 0)
				modifier |= Mod1Mask;
			      else if (strcasecmp (line2, "mod2") == 0)
				modifier |= Mod2Mask;
			      else if (strcasecmp (line2, "mod3") == 0)
				modifier |= Mod3Mask;
			      else if (strcasecmp (line2, "mod4") == 0)
				modifier |= Mod4Mask;
			      else if (strcasecmp (line2, "mod5") == 0)
				modifier |= Mod5Mask;
			      else if (strcasecmp (line2, "release") == 0)
				event_type = RELEASE;
			      else
				{
				  type = SYM;
				  keysym = XStringToKeysym (line2);
				  if (keysym == 0)
				    break;
				}
			    }

			  pos1 = pos2;
			}
		    }

		  if (add_key (type, event_type, keysym, keycode,
			       button, modifier, command, 0) != 0)
		    break;
		}
	    }
	}
    }


  /* Close RC File */
  if (stream != NULL)
    fclose (stream);

  if (keys == NULL)
    {
      fprintf (stderr, "Error in alocation of keys\n");
      return (-1);
    }

  /* Verify if all is good (like my english) */
  for (i = 0; i < nb_keys; i++)
    {
      if (keys[i].key.sym == 0 || keys[i].command == NULL)
	{
	  fprintf (stderr, "Error in RC file : %s\n", rc_file);
	  return (-1);
	}
    }

  if (verbose)
    printf ("%d keys in %s\n", nb_keys, rc_file);

  return (0);
}



//Everything from here on out has been changed by MMH
#ifdef GUILE_FLAG
int
init_xbk_guile_fns (void)
{
  if (verbose)
    printf("initializing guile fns...\n");
  scm_c_define_gsubr("set-numlock!", 1, 0, 0, set_numlock_wrapper);
  scm_c_define_gsubr("set-scrolllock!", 1, 0, 0, set_scrolllock_wrapper);
  scm_c_define_gsubr("set-capslock!", 1, 0, 0, set_capslock_wrapper);
  scm_c_define_gsubr("xbindkey", 2, 0, 0, xbindkey_wrapper);
  scm_c_define_gsubr("xbindkey-function", 2, 0, 0, xbindkey_function_wrapper);
  scm_c_define_gsubr("remove-xbindkey", 1, 0, 0, remove_xbindkey_wrapper);
  scm_c_define_gsubr("run-command", 1, 0, 0, run_command_wrapper);
  scm_c_define_gsubr("grab-all-keys", 0, 0, 0, grab_all_keys_wrapper);
  scm_c_define_gsubr("ungrab-all-keys", 0, 0, 0, ungrab_all_keys_wrapper);
  scm_c_define_gsubr("remove-all-keys", 0, 0, 0, remove_all_keys_wrapper);
  scm_c_define_gsubr("debug", 0, 0, 0, debug_info_wrapper);
  return 0;
}

int
get_rc_guile_file (void)
{
  FILE *stream;

  if (verbose)
    printf("getting rc guile file %s.\n", rc_guile_file);

  if (init_keys () != 0)
    return (-1);

  /* Open RC File */
  if ((stream = fopen (rc_guile_file, "r")) == NULL)
    {
      if (verbose)
	fprintf (stderr, "WARNING : %s not found or reading not allowed.\n",
		 rc_guile_file);
      return (-1);
    }
  fclose (stream);

  init_xbk_guile_fns();
  scm_primitive_load(scm_from_locale_string(rc_guile_file));
  return 0;
}

#define MAKE_MASK_WRAPPER(name, mask_name) \
SCM name (SCM val) \
{ \
  if (verbose) \
    printf("Running mask cmd!\n"); \
  mask_name = SCM_FALSEP(val); \
  return SCM_UNSPECIFIED; \
}

MAKE_MASK_WRAPPER(set_numlock_wrapper, numlock_mask);
MAKE_MASK_WRAPPER(set_scrolllock_wrapper, scrolllock_mask);
MAKE_MASK_WRAPPER(set_capslock_wrapper, capslock_mask);



SCM extract_key (SCM key, KeyType_t *type, EventType_t *event_type,
		 KeySym *keysym, KeyCode *keycode,
		 unsigned int *button, unsigned int *modifier)
{
  char *str;
  int len;

  while(SCM_CONSP(key)){ //Iterate through the list (If it is a list)
    if(!SCM_CONSP(SCM_CDR(key))){ //if this is the last item
      key = SCM_CAR(key);  //go to that
      break; //and continue
    }
    //Otherwise, this is a modifier.

    //So copy it:
    //Guile strings are not \0 terminated. hence we must copy.
    if (scm_is_true(scm_symbol_p(SCM_CAR(key)))) {
      SCM newkey = scm_symbol_to_string(SCM_CAR(key));
      str = scm_to_locale_string(newkey);
    } else {
      str = scm_to_locale_string(SCM_CAR(key));
    }
    len = strlen(str);


    /*str = scm_to_locale_string(SCM_CAR(key));*/


    if(verbose) //extra verbosity here.
      printf("xbindkey_wrapper debug: modifier = %s.\n", str);

    //copied directly with some substitutions. ie. line2 -> str
    //Do whatever needs to be done with modifiers.
    if (strncasecmp (str, "control", len) == 0)
      *modifier |= ControlMask;
    else if (strncasecmp (str, "shift", len) == 0)
      *modifier |= ShiftMask;
    else if (strncasecmp (str, "mod1", len) == 0
             || strncasecmp (str, "alt", len) == 0)
      *modifier |= Mod1Mask;
    else if (strncasecmp (str, "mod2", len) == 0)
      *modifier |= Mod2Mask;
    else if (strncasecmp (str, "mod3", len) == 0)
      *modifier |= Mod3Mask;
    else if (strncasecmp (str, "mod4", len) == 0)
      *modifier |= Mod4Mask;
    else if (strncasecmp (str, "mod5", len) == 0)
      *modifier |= Mod5Mask;
    else if (strncasecmp (str, "release", len) == 0)
      *event_type = RELEASE;
    else if(strlen (str) > 2 && str[0] == 'm' && str[1] == ':'){
      *modifier |= strtol (str+2, (char **) NULL, 0);
      //this break have nothing to do here!
      //break
    }else{
      printf("Bad modifier:\n%s\n", str); //or error
      return SCM_BOOL_F; //and return false
    }
    free(str); //we copied, so we must destroy this
    str=NULL;
    key = SCM_CDR(key); //and go a step down the list
  }
  //So this was either the only or last item of the 1st arg
  //Hence it is the key

  //So copy it:
  //Guile strings are not \0 terminated. hence we must copy.
  if (scm_is_true(scm_symbol_p(key))) {
    SCM newkey = scm_symbol_to_string(key);
    str = scm_to_locale_string(newkey);
  } else {
    str = scm_to_locale_string(key);
  }
  len = strlen(str);

  if(verbose)
    printf("xbindkey_wrapper debug: key = %s\n", str);

  //Check for special numeric stuff.
  //This way is really far nicer looking and more efficient than
  //having three copies of the code.
  if(strlen (str) > 2 && str[1] == ':' && isdigit (str[2]))
    {
      switch (str[0])
        {
	case 'b':
	  *type = BUTTON;
	  *button = strtol (str+2, (char **) NULL, 0);
	  break;
	case 'c':
	  *type = CODE;
	  *keycode = strtol (str+2, (char **) NULL, 0);
	  break;
	case 'm': //is a modifier so it is in the other part.
	  printf("bad modifyer: %s.", str);
	  printf("m: modifiers need be applied to keys\n");
	  return SCM_BOOL_F;
	default:
	  printf("bad modifyer: %c: shoud be b:, c: or m: .\n", str[0]);
	  return SCM_BOOL_F;
        }
    }
  else //regular key
    {
      *type = SYM;
      *keysym = XStringToKeysym (str);
      if (*keysym == 0){
        printf("No keysym for key: %s\n", str);
  	return SCM_BOOL_F;
      }
    }

  free(str); //these were used by add key and copied.

  return SCM_BOOL_T;
}



SCM xbindkey_wrapper(SCM key, SCM cmd)
{
  KeyType_t type = SYM;
  EventType_t event_type = PRESS;
  KeySym keysym = 0;
  KeyCode keycode = 0;
  unsigned int button = 0;
  unsigned int modifier = 0;
  char *cmdstr;

  //Guile strings are not \0 terminated. hence we must copy.
  cmdstr = scm_to_locale_string(cmd);
  if(verbose)
    printf("xbindkey_wrapper debug: cmd=%s.\n", cmdstr);

  if (extract_key (key, &type, &event_type, &keysym, &keycode,
		   &button, &modifier) == SCM_BOOL_F)
    {
      return SCM_BOOL_F;
    }

  if (add_key (type, event_type, keysym, keycode,
       	button, modifier, cmdstr, 0) != 0)
    {
      printf("add_key didn't return 0!!!\n");
      return SCM_BOOL_F;
    }

  free(cmdstr); //we may get rid of them!

  return SCM_UNSPECIFIED;
}


SCM tab_scm[2];

SCM xbindkey_function_wrapper (SCM key, SCM fun)
{
  KeyType_t type = SYM;
  EventType_t event_type = PRESS;
  KeySym keysym = 0;
  KeyCode keycode = 0;
  unsigned int button = 0;
  unsigned int modifier = 0;

  if (extract_key (key, &type, &event_type, &keysym, &keycode,
		   &button, &modifier) == SCM_BOOL_F)
    {
      return SCM_BOOL_F;
    }

  tab_scm[0] = fun;

  if (add_key (type, event_type, keysym, keycode,
	       button, modifier, NULL, tab_scm[0]) != 0)
    {
      printf("add_key didn't return 0!!!\n");
      return SCM_BOOL_F;
    }
  else {
    printf ("add_key ok!!!  fun=%d\n", (scm_procedure_p (fun) == SCM_BOOL_T));
  }

  //scm_permanent_object (tab_scm[0]);
  scm_remember_upto_here_1 (tab_scm[0]);

  return SCM_UNSPECIFIED;
}



SCM remove_xbindkey_wrapper (SCM key)
{
  KeyType_t type = SYM;
  EventType_t event_type = PRESS;
  KeySym keysym = 0;
  KeyCode keycode = 0;
  unsigned int button = 0;
  unsigned int modifier = 0;

  if (extract_key (key, &type, &event_type, &keysym, &keycode,
		   &button, &modifier) == SCM_BOOL_F)
    {
      return SCM_BOOL_F;
    }

  if (remove_key (type, event_type, keysym, keycode, button, modifier) != 0)
    {
      printf("remove_key didn't return 0!!!\n");
      return SCM_BOOL_F;
    }


  return SCM_UNSPECIFIED;
}


SCM run_command_wrapper (SCM command)
{
  char *cmdstr;

  cmdstr = scm_to_locale_string(command);

  run_command (cmdstr);

  free(cmdstr);

  return SCM_UNSPECIFIED;
}

SCM grab_all_keys_wrapper (void)
{
  grab_keys (current_display);

  return SCM_UNSPECIFIED;
}


SCM ungrab_all_keys_wrapper (void)
{
  ungrab_all_keys (current_display);

  return SCM_UNSPECIFIED;
}

SCM remove_all_keys_wrapper (void)
{
  close_keys ();

  return SCM_UNSPECIFIED;
}


SCM debug_info_wrapper (void)
{
  printf ("\nKeys = %p\n", keys);
  printf ("nb_keys = %d\n", nb_keys);

  return SCM_UNSPECIFIED;
}

#endif
