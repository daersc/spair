/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

#include <stdlib.h>

#include "types.h"

void
free_players(Player *player)
{
	if (player) {
		free_players(player->next);
		if (player->name) free(player->name);
		free(player);
	}
}

void
free_pairings(Pairing *pairing)
{
	if (pairing) {
		free_pairings(pairing->next);
		free(pairing);
	}
}

void
free_rounds(Round *round)
{
	if (round) {
		free_rounds(round->next);
		free_pairings(round->pairings);
		free(round);
	}
}
