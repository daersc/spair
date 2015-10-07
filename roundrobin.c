/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

/* This file implements the round robin pairing algorithm. */

#include <stdlib.h>

#include "roundrobin.h"

static size_t  create_even_sized_array(Player *pl, Player ***arr_ptr);
static size_t  get_player_idx(size_t pcnt, size_t rno, size_t pno, size_t cno);
static Round   *create_round(size_t pcnt, size_t rno, Player **arr_ptr);
static Pairing *create_pairing(size_t pcnt, size_t rno, size_t pno,
                               Player **arr_ptr);

Round *
roundrobin(Player *pl)
{
	Player **parr;
	Round *res, *curr = NULL;
	size_t pcnt = create_even_sized_array(pl, &parr);
	size_t rno = 0;
	if (!pcnt || pcnt == (size_t) -1)
		return NULL;
	while (rno < pcnt-1) {
		Round *round = create_round(pcnt, rno, parr);
		if (!round) {
			if (res)
				free_rounds(res);
			free(parr);
			return NULL;
		}
		if (!curr) {
			curr = res = round;
		} else {
			curr->next = round;
			curr = round;
		}
		rno++;
	}
	free(parr);
	return res;
}

static size_t
create_even_sized_array(Player *pl, Player ***arr_ptr)
{ size_t i=0;
	Player *pp = pl;

	if (!pl) {
		*arr_ptr = NULL;
		return 0;
	}

	/* define size and allocate */
	while (pp) {
		i++;
		pp = pp->next;
	}
	*arr_ptr = malloc(sizeof(Player *) * (i%2 ? i+1 : i));
	if (!*arr_ptr)
		return (size_t) -1;

	/* copy player pointer */
	pp = pl;
	i  = 0;
	while (pp) {
		(*arr_ptr)[i++] = pp;
		pp = pp->next;
	}

	/* insert "BYE" on odd size */
	if (i % 2)
		(*arr_ptr)[i++] = NULL;
	return i;
}

static size_t
get_player_idx(size_t pcnt, size_t rno, size_t pno, size_t cno)
{
	if (rno >= pcnt || pno > pcnt/2 || cno > 1)
		return (size_t) -1;
	if (!pcnt)
		return 0;
	if (!pno) { /* first pairing */
		if ((rno+cno)%2)
			return pcnt-1;
		else
			return rno;
	} else { /* other pairings */
		if ((pno+cno)%2)
			return (rno+pno) % (pcnt-1);
		else
			return (pcnt-1+rno-pno) % (pcnt-1); /* make (rno-pno)%(pcnt-1) positive */
	}
}

static Round *
create_round(size_t pcnt, size_t rno, Player **arr_ptr)
{
	Round   *round;
	Pairing *curp;
	size_t  pno;
	round = malloc(sizeof(Round));
	if (!round)
		return NULL;
	round->next            = NULL;
	round->pairings = curp = NULL;
	for (pno = 0; pno < pcnt/2; pno++) {
		Pairing *pairing = create_pairing(pcnt, rno, pno, arr_ptr);
		if (!pairing) {
			free_rounds(round);
			return NULL;
		}
		if (!curp) {
			curp = round->pairings = pairing;
		} else {
			curp->next = pairing;
			curp = pairing;
		}
	}
	return round;
}

static Pairing *
create_pairing(size_t pcnt, size_t rno, size_t pno, Player **arr_ptr)
{
	Pairing *pairing;
	size_t widx = get_player_idx(pcnt, rno, pno, 0);
	size_t bidx = get_player_idx(pcnt, rno, pno, 1);
	if (widx == (size_t) -1 || bidx == (size_t) -1)
		return NULL;
	pairing = malloc(sizeof(Pairing));
	if (!pairing)
		return NULL;
	pairing->next = NULL;
	pairing->white = arr_ptr[widx];
	pairing->black = arr_ptr[bidx];
	return pairing;
}
