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

#ifndef __OPTIONS_H
#define __OPTIONS_H


extern void get_options (int argc, char **argv);
extern void show_options (void);

extern int rc_file_exist (void);

extern int get_rc_file (void);
#ifdef GUILE_FLAG
extern int get_rc_guile_file (void);
#endif


extern char *display_name;

extern int verbose;
extern int have_to_show_binding;
extern int have_to_get_binding;
extern int have_to_start_as_daemon;
extern int detectable_ar;

extern char *geom;



#endif /* __OPTIONS_H */
