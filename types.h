/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

/* Types and macros used in spair */

typedef struct Player  Player;
typedef struct Pairing Pairing;
typedef struct Round   Round;

struct Player {
	char   *name;
	Player *next;
};

struct Pairing {
	Player  *white;
	Player  *black;
	Pairing *next;
};

struct Round {
	Pairing *pairings;
	Round   *next;
};

void free_players(Player *player);
void free_pairings(Pairing *pairing);
void free_rounds(Round *round);
