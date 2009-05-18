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

#ifndef __GRABKEY_H
#define __GRABKEY_H

extern void grab_keys (Display * dpy);
extern void ungrab_all_keys (Display * dpy);

extern void get_offending_modifiers (Display * dpy);


extern unsigned int numlock_mask;
extern unsigned int scrolllock_mask;
extern unsigned int capslock_mask;


#endif /* __GRABKEY_H */
