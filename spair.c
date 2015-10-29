/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef USING_GETTEXT
	#include <langinfo.h>
#endif
#include "gettext.h"

#include "roundrobin.h"

#define CL_OPT_STRING ":hvp:"

static void init_locale(void);
static void parse_args(int argc, char **argv);
static void print_version(void);
static void print_help(void);
static void print_pairing_table(unsigned int count);
static void pair_roundrobin(void);

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
		case 'p': {
			unsigned int count;
			if (!sscanf(optarg, "%u", &count)) {
				perror("Parsing argument failed");
				exit(EXIT_FAILURE);
			}
			print_pairing_table(count);
			exit(EXIT_SUCCESS);
		}
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
	const char *fmt = " -%c  %s\n";
	printf(_("Usage: %s\n"), "spair [-v] [-h] [-p]");
	printf(_("Read list of players from STDIN and print roundobin pairings.\n"));
	printf(fmt, 'h', _("Print this help and exit."));
	printf(fmt, 'v', _("Print version information and exit."));
	printf(fmt, 'p', _("Print pairing table for a specific number of players."));
}

static void
print_pairing_table(unsigned int count)
{
	Player *list = NULL, *lptr = NULL;
	Round  *hist = NULL, *rptr = NULL;
	char   buffer[32];
	unsigned int i;
	for (i=1; i<=count; i++) {
		Player *player = malloc(sizeof(Player));
		if (!player) {
			perror(_("Could not allocate memory for player"));
			goto cleanup;
		}
		sprintf(buffer, "%u", i);
		player->next = NULL;
		player->name = malloc(sizeof(char)*(strlen(buffer)+1));
		if (!player->name) {
			perror(_("Could not allocate memory for player's name"));
			free(player);
			goto cleanup;
		}
		strcpy(player->name, buffer);
		if (lptr) lptr->next = player;
		else list = player;
		lptr = player;
	}

	hist = rptr = roundrobin(list);
	i = 1;
	while (rptr) {
		Pairing *pptr = rptr->pairings;
		printf("%s %u:", _("Round"), i);
		while (pptr) {
			printf("\t%s-%s", pptr->white ? pptr->white->name : "/",
			                 pptr->black ? pptr->black->name : "/");
			pptr = pptr->next;
		}
		printf("\n");
		rptr = rptr->next;
		i++;
	}

	cleanup:
	free_players(list);
	free_rounds(hist);
}

static void
pair_roundrobin(void)
{
	Player *list = NULL, *lptr = NULL;
	Round  *hist = NULL, *rptr = NULL;
	char   *buffer = NULL;
	size_t buflen  = 0;
	unsigned int i;
	while (getline(&buffer, &buflen, stdin) != -1) {
		Player *player = malloc(sizeof(Player));
		size_t pos;
		if (!player) {
			perror(_("Could not allocate memory for player"));
			goto cleanup;
		}
		for (pos = 0; pos < buflen; pos++) {
			if (buffer[pos] == '\n') {
				buffer[pos] = '\0';
				break;
			}
		}
		player->next = NULL;
		player->name = malloc(sizeof(char) * (strlen(buffer)+1));
		if (!player->name) {
			perror(_("Could not allocate memory for player's name"));
			free(player);
			goto cleanup;
		}
		strcpy(player->name, buffer);
		if (lptr) lptr->next = player;
		else list = player;
		lptr = player;
	}

	hist = rptr = roundrobin(list);
	i = 1;
	while (rptr) {
		Pairing *pptr = rptr->pairings;
		printf(i==1 ? "%s %u\n" : "\n%s %u\n", _("Round"), i);
		while (pptr) {
			printf(" %20s : %-20s\n", pptr->white ? pptr->white->name : _("<bye>"),
			                          pptr->black ? pptr->black->name : _("<bye>"));
			pptr = pptr->next;
		}
		rptr = rptr->next;
		i++;
	}

	cleanup:
	free(buffer);
	free_players(list);
	free_rounds(hist);
}

int
main(int argc, char **argv)
{
	init_locale();
	parse_args(argc, argv);
	pair_roundrobin();
	return EXIT_SUCCESS;
}
