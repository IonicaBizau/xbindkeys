bin_PROGRAMS=xbindkeys

if GUILE
AM_CFLAGS=@CFLAGS@ @X_CFLAGS@ @FORK_FLAG@ @GUILE_FLAG@ @GUILE_CFLAGS@ -I. -Wall
xbindkeys_LDADD=@X_LIBS@ @GUILE_LDFLAGS@
else
AM_CFLAGS=@CFLAGS@ @X_CFLAGS@ @FORK_FLAG@ @GUILE_FLAG@ -I. -Wall
xbindkeys_LDADD=@X_LIBS@
endif

xbindkeys_SOURCES=xbindkeys.c xbindkeys.h \
		keys.h keys.c \
		options.c options.h \
		get_key.c get_key.h \
		grab_key.c grab_key.h



xbindkeys_DEPENDENCIES=xbindkeys.o keys.o options.o get_key.o grab_key.o

EXTRA_DIST=BUGS xbindkeys.1 \
	TODO xbindkeysrc  xbindkeys.spec \
	xbindkeysrc.scm xbindkeysrc-combo.scm \
	xbindkeys_show.1 xbindkeys_show



if TK
man_MANS_TK = xbindkeys_show.1
EXTRA_SCRIPTS=xbindkeys_show
bin_SCRIPTS=xbindkeys_show
else
man_MANS_TK = 
endif


man_MANS = xbindkeys.1 $(man_MANS_TK)
