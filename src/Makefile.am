# define the include path to the external libraries
# external_includes = -I$(top_srcdir)/external/

# install directory for glade file
# without specific choice it is
# $(prefix)/share/trac/
# trac_dir = $(pkgdatadir)
# trac__DATA = trac.glade

bin_PROGRAMS = trac

trac_SOURCES = trac.c \
	       cmd_options.c \
	       tasks.c \
	       segments.c \
	       timestamps.c \
	       tracdir.c \
	       store_history.c \
	       load_history.c
#trac_CPPFLAGS = ${external_includes}
trac_CFLAGS  = @GTK_CFLAGS@ -g -Wall -Wextra -Wpedantic
trac_LDFLAGS = @GTK_LIBS@ -rdynamic -lm
trac_LDADD   =
