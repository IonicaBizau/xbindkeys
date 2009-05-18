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

#ifndef __KEYS_H
#define __KEYS_H

#include <X11/Xlib.h>
#ifdef GUILE_FLAG
#include <libguile.h>
#endif

typedef enum
  { SYM, CODE, BUTTON }
KeyType_t;

typedef enum
  { PRESS, RELEASE}
EventType_t;

#ifndef GUILE_FLAG
typedef int SCM;
#endif

typedef struct
{
  KeyType_t type;

  EventType_t event_type;

  union
  {
    KeySym sym;
    KeyCode code;
    unsigned int button;
  }
  key;

  unsigned int modifier;
  char *command;
  SCM function;    // This is to call a scheme function instead of a shell command
                   // when command == NULL (not used when guile is not used).
}
Keys_t;


extern int init_keys (void);
extern void close_keys (void);

extern int add_key (KeyType_t type, EventType_t event_type, KeySym keysym, KeyCode keycode,
		    unsigned int button, unsigned int modifier,
		    char *command, SCM function);

extern int remove_key (KeyType_t type, EventType_t event_type, KeySym keysym, KeyCode keycode,
		       unsigned int button, unsigned int modifier);


extern void show_key_binding (Display * d);

extern void print_key (Display * d, Keys_t * key);

extern void set_keysym (Keys_t * key, EventType_t event_type, KeySym keysym,
			unsigned int modifier, char *command, SCM function);
extern void set_keycode (Keys_t * key, EventType_t event_type, KeyCode keycode,
			 unsigned int modifier, char *command, SCM function);
extern void set_button (Keys_t * key, EventType_t event_type, unsigned int button,
			unsigned int modifier, char *command, SCM function);

extern void free_key (Keys_t * key);

extern void start_command_key (Keys_t * key);

extern void modifier_to_string (unsigned int modifier, char *str);

extern void run_command (char * command);


extern int nb_keys;
extern Keys_t *keys;

#endif /* __KEYS_H */
