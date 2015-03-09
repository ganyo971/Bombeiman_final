/* monster.c
 *
 *  Created on: 24 févr. 2015
 *      Author: fumont
 */
#include <SDL/SDL_image.h>
#include <assert.h>
#include <time.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <player.h>



struct monster {
	int x, y;
	enum direction current_direction;
	struct monster* next;
};

struct monster*
list_insert_tail(struct monster* l)  {

  struct monster* e = malloc(sizeof(struct monster));
  if (!e)
  error("Memory error");
  e->current_direction=SOUTH;
  e->next=NULL;

  if(l==NULL){
    return e;
  }
  else
    {
      struct monster* p =l;
      while(p->next!=NULL){
    	  	  p=p->next;
      }
      p->next=e;

      return l;
    }
}

struct monster* monster_init(struct map* map) {
	struct monster* monster = malloc(sizeof(*monster));
	monster=NULL;

	int i, j;
		for (i = 0; i < map_get_width(map); i++)        {
			for (j = 0; j < map_get_height(map); j++)         {
				if (map_get_cell_type(map, i, j) == CELL_MONSTER)         {

						monster=list_insert_tail(monster);

				}
			}
		}
	return monster;
}


void monster_free(struct monster* monster) {
	while (monster !=NULL)      {
		assert(monster);
		free(monster);
		monster=monster->next;
	}
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->current_direction = way;
}

void monster_from_map(struct monster* monster, struct map* map) {
	assert(monster);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
				monster->x = i;
				monster->y = j;	 // a ce niveau il faudrait fr un return 0 car il n'y a qu'un unique cell_player ds une grille
				monster=monster->next;
			}
		}
	}
}


static int monster_move_aux(struct monster* monster, struct map* map, int x, int y, struct player* player) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;    // 0 dc mtnt le monstre ne transperce pas les roches et arbres
		break;

	case CELL_CASE:
		return 0;    // idem pour caisses
		break;

	case CELL_PLAYER:
	{
			perte_de_vie(player, 2);

			return 0;
						}
		break;

	case CELL_MONSTER:
			return 0;
			break;

	default:
		break;
	}

	// Monster has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map, struct player* player) {  //rajout de struct player* player pour pouvoir faire décroître la vie du joueur
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->current_direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1, player)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1, player)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y,player)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y,player)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}
	return move;
}

void monster_display(struct monster* monster) {

	while (monster !=NULL)      {

		assert(monster);
		window_display_image(sprite_get_monster(monster->current_direction),
				monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);

		monster=monster->next;
			}
}

// donne une direction aléatoire aux monstres

void random_direction_monster (struct map* map, struct monster* monster, struct player* player)  {

	int rand_a_b(int a, int b, int c)  {             // fonction qui génère des nombres entre a et b
			srand(time(NULL)+c);
			int x=rand()%(b+1-a)+a;
			return x;

		}


		while (monster !=NULL)     {

			int c=4;
			c=c+17;

			int direction_aleatoire;

			direction_aleatoire=rand_a_b(0,3,c);


			switch(direction_aleatoire)  {
			case NORTH:
				monster_set_current_way(monster, NORTH);
				monster_move(monster, map, player);
				break;
			case SOUTH:
					monster_set_current_way(monster, SOUTH);
					monster_move(monster, map, player);
					break;
			case EAST:
					monster_set_current_way(monster, EAST);
					monster_move(monster, map, player);
					break;
			case WEST:
					monster_set_current_way(monster, WEST);
					monster_move(monster, map, player);
					break;
			}

			monster = monster->next;
		}
}


