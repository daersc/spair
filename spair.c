/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USING_GETTEXT
	#include <langinfo.h>
#endif
#include "gettext.h"

static void init_locale(void);

static void
init_locale(void)
{
	setlocale(LC_ALL, "");
	#ifdef USING_GETTEXT
		bindtextdomain("spair", LOCALEDIR);
		bind_textdomain_codeset("spair", nl_langinfo(CODESET));
		textdomain("spair");
	#endif
}


int
main()
{
	init_locale();
	printf(_("Hello world!\n"));
	return EXIT_SUCCESS;
}
