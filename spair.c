/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef USING_GETTEXT
	#include <langinfo.h>
#endif
#include "gettext.h"

#define CL_OPT_STRING ":hv"

static void init_locale(void);
static void parse_args(int argc, char **argv);
static void print_version(void);
static void print_help(void);

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

static void
parse_args(int argc, char **argv)
{
	int c;
	while ((c = getopt(argc, argv, CL_OPT_STRING)) != -1) {
		switch (c) {
		case 'h':
			print_version();
			print_help();
			exit(EXIT_SUCCESS);
		case 'v':
			print_version();
			exit(EXIT_SUCCESS);
		case ':':
			fprintf(stderr, _("Option -%c requires an argument\n"), optopt);
			exit(EXIT_FAILURE);
		case '?':
			fprintf(stderr, _("Unrecognized option: '-%c'\n"), optopt);
			print_help();
			exit(EXIT_FAILURE);
		}
	}
}

static void
print_version(void)
{
	printf(_("spair-%s, (c) 2015 David Schury, see LICENSE for details\n"),
	       VERSION);
}

static void
print_help(void)
{
	printf(_("Usage: %s\n"), "spair [-v] [-h]");
}

int
main(int argc, char **argv)
{
	init_locale();
	parse_args(argc, argv);
	return EXIT_SUCCESS;
}
