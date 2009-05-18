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


#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include "keys.h"
#include "options.h"



#define STOP_KEY "q"


#define INNER_WINDOW_WIDTH 50
#define INNER_WINDOW_HEIGHT 50
#define INNER_WINDOW_BORDER 4
#define INNER_WINDOW_X 10
#define INNER_WINDOW_Y 10
#define OUTER_WINDOW_MIN_WIDTH (INNER_WINDOW_WIDTH + \
				2 * (INNER_WINDOW_BORDER + INNER_WINDOW_X))
#define OUTER_WINDOW_MIN_HEIGHT (INNER_WINDOW_HEIGHT + \
				2 * (INNER_WINDOW_BORDER + INNER_WINDOW_Y))
#define OUTER_WINDOW_DEF_WIDTH (OUTER_WINDOW_MIN_WIDTH + 200)
#define OUTER_WINDOW_DEF_HEIGHT (OUTER_WINDOW_MIN_HEIGHT + 4)
#define OUTER_WINDOW_DEF_X 200
#define OUTER_WINDOW_DEF_Y 4



void
set_sizehints (Display * dpy, XSizeHints * hintp, int min_width,
	       int min_height, int defwidth, int defheight, int defx,
	       int defy)
{
  int geom_result;

  /* set the size hints, algorithm from xlib xbiff */

  hintp->width = hintp->min_width = min_width;
  hintp->height = hintp->min_height = min_height;
  hintp->flags = PMinSize;
  hintp->x = hintp->y = 0;
  geom_result = NoValue;
  if (geom != NULL)
    {
      geom_result = XParseGeometry (geom, &hintp->x, &hintp->y,
				    (unsigned int *) &hintp->width,
				    (unsigned int *) &hintp->height);
      if ((geom_result & WidthValue) && (geom_result & HeightValue))
	{
#define max(a,b) ((a) > (b) ? (a) : (b))
	  hintp->width = max (hintp->width, hintp->min_width);
	  hintp->height = max (hintp->height, hintp->min_height);
	  hintp->flags |= USSize;
	}
      if ((geom_result & XValue) && (geom_result & YValue))
	{
	  hintp->flags += USPosition;
	}
    }
  if (!(hintp->flags & USSize))
    {
      hintp->width = defwidth;
      hintp->height = defheight;
      hintp->flags |= PSize;
    }

  if (geom_result & XNegative)
    {
      hintp->x = DisplayWidth (dpy, DefaultScreen (dpy)) + hintp->x -
	hintp->width;
    }
  if (geom_result & YNegative)
    {
      hintp->y = DisplayHeight (dpy, DefaultScreen (dpy)) + hintp->y -
	hintp->height;
    }
}



void
get_key_binding (Display * dpy, char **argv, int argc)
{
  XSizeHints hints;
  int borderwidth = 2;
  Window w;
  XSetWindowAttributes attr;
  unsigned long mask = 0L;
  int done;
  char *name = "XBindKey: Hit a key";
  int screen;
  Keys_t key;
  int min_keycode, max_keycode;

  printf ("Press combination of keys or/and click under the window.\n");
  printf ("You can use one of the two lines after \"NoCommand\"\n");
  printf ("in $HOME/.xbindkeysrc to bind a key.\n");
  if (have_to_get_binding == 2)
    printf ("\n--- Press \"%s\" to stop. ---\n", STOP_KEY);

  screen = DefaultScreen (dpy);

  attr.event_mask = KeyReleaseMask | ButtonReleaseMask;

  set_sizehints (dpy, &hints, OUTER_WINDOW_MIN_WIDTH, OUTER_WINDOW_MIN_HEIGHT,
		 OUTER_WINDOW_DEF_WIDTH, OUTER_WINDOW_DEF_HEIGHT,
		 OUTER_WINDOW_DEF_X, OUTER_WINDOW_DEF_Y);

  attr.background_pixel = WhitePixel (dpy, screen);;
  attr.border_pixel = BlackPixel (dpy, screen);;
  mask |= (CWBackPixel | CWBorderPixel | CWEventMask);

  w = XCreateWindow (dpy, RootWindow (dpy, screen), hints.x, hints.y,
		     hints.width, hints.height, borderwidth, 0,
		     InputOutput, (Visual *) CopyFromParent, mask, &attr);

  XSetStandardProperties (dpy, w, name, NULL, (Pixmap) 0, argv, argc, &hints);

  XMapWindow (dpy, w);

  XDisplayKeycodes (dpy, &min_keycode, &max_keycode);

  sleep (1);

  verbose = 1;

  for (done = 0; !done;)
    {
      XEvent event;

      XNextEvent (dpy, &event);

      switch (event.type)
	{
	case KeyRelease:
	  key.type = CODE;
	  key.event_type = PRESS;
	  key.key.code = event.xkey.keycode;
	  key.modifier = event.xkey.state;
	  key.command = NULL;

	  if (have_to_get_binding == 2)
	    {
	      print_key (dpy, &key);

	      if (event.xkey.keycode ==
		  XKeysymToKeycode (dpy, XStringToKeysym (STOP_KEY)))
		{
		  done = 1;
		}
	    }
	  else
	    {
	      print_key (dpy, &key);

	      done = 1;
	    }

	  if (event.xkey.keycode < min_keycode
	      || event.xkey.keycode > max_keycode)
	    {
	      fprintf (stderr,
		       "Note:\n"
		       "  The keycode %d cannot be used, as it's not between the\n"
		       "  min(%d) and max(%d) keycode of your keyboard.\n"
		       "  Please increase the 'maximum' value in\n"
		       "    /usr/X11R6/lib/X11/xkb/keycodes/xfree86,\n"
		       "  then restart X.\n",
		       event.xkey.keycode, min_keycode, max_keycode);
	    }

	  break;

	case ButtonRelease :
	  key.type = BUTTON;
	  key.event_type = PRESS;
	  key.key.button = event.xbutton.button;
	  key.modifier = event.xbutton.state;
	  key.command = NULL;

	  print_key (dpy, &key);

	  if (have_to_get_binding == 1)
	    done = 1;
	  break;


	default:
	  break;
	}
    }
}
