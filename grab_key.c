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

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include "xbindkeys.h"
#include "grab_key.h"
#include "options.h"
#include "keys.h"


unsigned int numlock_mask = 0;
unsigned int scrolllock_mask = 0;
unsigned int capslock_mask = 0;



static void my_grab_key (Display * dpy, KeyCode keycode,
			 unsigned int modifier, Window win);



void
get_offending_modifiers (Display * dpy)
{
  int i;
  XModifierKeymap *modmap;
  KeyCode nlock, slock;
  static int mask_table[8] = {
    ShiftMask, LockMask, ControlMask, Mod1Mask,
    Mod2Mask, Mod3Mask, Mod4Mask, Mod5Mask
  };

  nlock = XKeysymToKeycode (dpy, XK_Num_Lock);
  slock = XKeysymToKeycode (dpy, XK_Scroll_Lock);

  /*
   * Find out the masks for the NumLock and ScrollLock modifiers,
   * so that we can bind the grabs for when they are enabled too.
   */
  modmap = XGetModifierMapping (dpy);

  if (modmap != NULL && modmap->max_keypermod > 0)
    {
      for (i = 0; i < 8 * modmap->max_keypermod; i++)
	{
	  if (modmap->modifiermap[i] == nlock && nlock != 0)
	    numlock_mask = mask_table[i / modmap->max_keypermod];
	  else if (modmap->modifiermap[i] == slock && slock != 0)
	    scrolllock_mask = mask_table[i / modmap->max_keypermod];
	}
    }

  capslock_mask = LockMask;

  if (modmap)
    XFreeModifiermap (modmap);
}


static void
my_grab_key (Display * dpy, KeyCode keycode, unsigned int modifier,
	     Window win)
{
  modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);


  XGrabKey (dpy, keycode, modifier, (win ? win : DefaultRootWindow (dpy)),
	    False, GrabModeAsync, GrabModeAsync);

  if (modifier == AnyModifier)
    return;

  if (numlock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (capslock_mask)
    XGrabKey (dpy, keycode, modifier | capslock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && capslock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask | capslock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | numlock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (capslock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode, modifier | capslock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)),
	      False, GrabModeAsync, GrabModeAsync);

  if (numlock_mask && capslock_mask && scrolllock_mask)
    XGrabKey (dpy, keycode,
	      modifier | numlock_mask | capslock_mask | scrolllock_mask,
	      (win ? win : DefaultRootWindow (dpy)), False, GrabModeAsync,
	      GrabModeAsync);

}


static void
my_grab_button (Display * dpy, unsigned int button, unsigned int modifier,
		Window win)
{
  modifier &= ~(numlock_mask | capslock_mask | scrolllock_mask);

  XGrabButton (dpy, button, modifier, (win ? win : DefaultRootWindow (dpy)),
	       False, ButtonPressMask | ButtonReleaseMask,
	       GrabModeAsync, GrabModeAsync, None, None);

  if (modifier == AnyModifier)
    return;

  if (numlock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);


  if (capslock_mask)
    XGrabButton (dpy, button, modifier | capslock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (scrolllock_mask)
    XGrabButton (dpy, button, modifier | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && capslock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask | capslock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && scrolllock_mask)
    XGrabButton (dpy, button, modifier | numlock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (capslock_mask && scrolllock_mask)
    XGrabButton (dpy, button, modifier | capslock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)),
		 False, ButtonPressMask | ButtonReleaseMask,
		 GrabModeAsync, GrabModeAsync, None, None);

  if (numlock_mask && capslock_mask && scrolllock_mask)
    XGrabButton (dpy, button,
		 modifier | numlock_mask | capslock_mask | scrolllock_mask,
		 (win ? win : DefaultRootWindow (dpy)), False,
		 ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
		 GrabModeAsync, None, None);
}



void
ungrab_all_keys (Display * dpy)
{
  int screen;

  for (screen = 0; screen < ScreenCount (dpy); screen++)
    {
      XUngrabKey (dpy, AnyKey, AnyModifier, RootWindow (dpy, screen));
      XUngrabButton (dpy, AnyButton, AnyModifier, RootWindow (dpy, screen));
    }
}


void
grab_keys (Display * dpy)
{
  int i;
  int min, max;
  int screen;

  XDisplayKeycodes (dpy, &min, &max);

  if (verbose)
    {
      printf ("\n");
      printf ("min_keycode=%d     max_keycode=%d (ie: know keycodes)\n", min,
	      max);
    }

  ungrab_all_keys (dpy);


  for (i = 0; i < nb_keys; i++)
    {
      print_key (dpy, &keys[i]);

      if (keys[i].type == SYM)
	{
	  for (screen = 0; screen < ScreenCount (dpy); screen++)
	    {
	      KeyCode code = XKeysymToKeycode (dpy, keys[i].key.sym);
	      if (code != 0)
	        {
	          my_grab_key (dpy, code,
			       keys[i].modifier, RootWindow (dpy, screen));
	        }
	      else
	        {
	          fprintf (stderr, "--- xbindkeys error ---\n");
	          if (!verbose)
		    {
		      verbose = 1;
		      print_key (dpy, &keys[i]);
		      verbose = 0;
		    }
		  fprintf (stderr,
                           "  The key symbol '%s' cannot be used, as it's not mapped\n"
                           "  on your keyboard.\n"
                           "  xbindkeys will keep running, but will ignore this symbol.\n",
                           XKeysymToString (keys[i].key.sym));
	        }

	    }
	}
      else if (keys[i].type == BUTTON)
	{
	  for (screen = 0; screen < ScreenCount (dpy); screen++)
	    {
	      my_grab_button (dpy, keys[i].key.button, keys[i].modifier,
			      RootWindow (dpy, screen));
	    }
	}
      else
	{
	  if (keys[i].key.code >= min && keys[i].key.code <= max)
	    {
	      for (screen = 0; screen < ScreenCount (dpy); screen++)
		{
		  my_grab_key (dpy, keys[i].key.code, keys[i].modifier,
			       RootWindow (dpy, screen));
		}
	    }
	  else
	    {
	      fprintf (stderr, "--- xbindkeys error ---\n");

	      if (!verbose)
		{
		  verbose = 1;
		  print_key (dpy, &keys[i]);
		}

	      fprintf (stderr,
		       "  The keycode %d cannot be used, as it's not between the\n"
		       "  min(%d) and max(%d) keycode of your keyboard.\n"
		       "  Please increase the 'maximum' value in\n"
		       "    /usr/X11R6/lib/X11/xkb/keycodes/xfree86,\n"
		       "  then restart X.\n", keys[i].key.code, min, max);
	      end_it_all (dpy);
	      exit (-1);
	    }
	}
    }
}
