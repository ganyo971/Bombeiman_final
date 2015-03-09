/*
 * monster.h
 *
 *  Created on: 20 févr. 2015
 *      Author: fumont
 */

#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>
#include <player.h>

struct monster;
struct monster* list_insert_tail(struct monster* l);

// Creates a new monster
struct monster* monster_init(struct map* map);
void   monster_free(struct monster* monster);

// Returns the current position of the player
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the player
void monster_set_current_way(struct monster * monster, enum direction direction);

// Load the player position from the map
void monster_from_map(struct monster* monster, struct map* map);
int number_monster_from_map(struct map* map);

// Move the player according to the current direction
int monster_move(struct monster* monster, struct map* map, struct player* player);

// Display the monster on the screen
void monster_display(struct monster* monster);

void random_direction_monster(struct map* map, struct monster* monster, struct player* player);






#endif /* MONSTER_H_ */
