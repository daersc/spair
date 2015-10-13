/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

/* File provides the _() macro for i18n strings */

#ifdef USING_GETTEXT
	#include <libintl.h>
	#define _(str) gettext(str)
#else
	#define _(str) (str)
#endif
