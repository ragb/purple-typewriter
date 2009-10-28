plugindir = $(libdir)/purple-2
plugin_LTLIBRARIES = typewriter.la

typewriter_la_SOURCES = typewriter.c
typewriter_la_CFLAGS = $(LIBTYPEWRITER_CFLAGS) -Wall -ansi -DPURPLE_SOUNDSDIR=\"$(datadir)/sounds/purple\"
typewriter_la_LDFLAGS = -module -avoid-version
typewriter_la_LIBADD = $(LIBTYPEWRITER_LDFLAGS)
