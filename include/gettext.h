// $Id$

// THIS FILE MUST NOT BE INSTALLED!

#ifndef GETTEXT_H
#define GETTEXT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* NLS can be disabled through the configure --disable-nls option.  */
#if ENABLE_NLS
# include <libintl.h>
# define _(String) dgettext(PACKAGE, String)
#else
# define _(String) (String)
#endif

#endif // GETTEXT_H
