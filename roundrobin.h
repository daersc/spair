/* spair - slim chess pairing tool */
/* see LICENSE file for license and copyright information */

/* This file provides a pairing function for generating roundrobin tournaments
 * for a list of players.
 *
 * Each player will be paired against each other exactly one time. */

#include "types.h"

Round *roundrobin(Player *pl);
