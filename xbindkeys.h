/***************************************************************************
        xbindkeys : a little program to bind keys to commands under X11.
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

#ifndef __XBINDKEYS_H
#define __XBINDKEYS_H

#include <X11/Xlib.h>
#include "config.h"

extern void end_it_all (Display *);
extern Display *current_display;

#endif /* __XBINDKEYS_H */
