AC_INIT(typewriter, 0.1.4, http://launchpad.net/pidgin-typewriter)
AC_CONFIG_SRCDIR(src/typewriter.c)
AM_INIT_AUTOMAKE()
AM_MAINTAINER_MODE

GNOME_COMMON_INIT
#GNOME_DOC_INIT

AM_CONFIG_HEADER(config.h)

AC_PROG_INSTALL
IT_PROG_INTLTOOL([0.35])
AC_PROG_CC
AC_ISC_POSIX
AC_HEADER_STDC

AM_DISABLE_STATIC
AM_PROG_LIBTOOL

#
# Setup GETTEXT
#


GETTEXT_PACKAGE=typewriter
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package])
ALL_LINGUAS="pt"
AM_GLIB_GNU_GETTEXT


#
# Gdk and Atk are needed for libtrayicon
#

PKG_CHECK_MODULES(LIBTYPEWRITER, 
		  purple >= 2.0.0)
AC_SUBST(LIBTYPEWRITER_CFLAGS)
AC_SUBST(LIBTYPEWRITER_LIBS)




AC_OUTPUT([
Makefile
src/Makefile
share/sounds/Makefile
po/Makefile.in
])

echo "
${PACKAGE}-${VERSION}

    Install Prefix:    ${prefix}
"

