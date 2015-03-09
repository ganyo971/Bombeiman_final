#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>


struct player {
	int x, y;
	enum direction current_direction;
	int nb_bomb;
	int nb_life;
};

struct player* player_init(int bomb_number, int life_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bomb = bomb_number;
	player->nb_life = 8;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb -= 1;
}

short player_get_nb_life(struct player* player) {
	assert(player);
	return player->nb_life;
}

void player_inc_nb_life(struct player* player) {
	assert(player);
	player->nb_life += 1;
}

void player_dec_nb_life(struct player* player) {
	assert(player);
	player->nb_life -= 1;
}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
			}
		}
	}
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


void perte_de_vie(struct player* player, int cas_de_perte_de_vie)  {

	int temps_actuel=SDL_GetTicks();

	static int temps_precedent=0; //grace à static cette variable sera persistante dans la fonction!
	// Elle ne sera pas réinitialisée à 0 à chaque appel de la fonction mais gardera la valeur calculée dans la fonction
	// au préalable (def de persistante)! (contrairement à si on avait fait int tps_prcdt=0)

	int timer=temps_actuel-temps_precedent;

	int temps_actuel2=SDL_GetTicks();

	static int temps_precedent2=0;

	int timer2=temps_actuel2-temps_precedent2;

	if ((player_get_nb_life(player)>0) && ((timer>TEMPS_INVULNERABILITE_JOUEUR)||(timer==temps_actuel))
		 && ((timer2>TEMPS_INVULNERABILITE_JOUEUR)||(timer2==temps_actuel2)))  {

				player_dec_nb_life(player);

				if (cas_de_perte_de_vie==1){
					temps_precedent=temps_actuel;
						}

				if (cas_de_perte_de_vie==2){
					temps_precedent2=temps_actuel2;
				}
	}
// if (player_get_nb_life(player)=0)       doit afficher game over


}


static int player_move_aux(struct player* player, struct map* map, int x, int y) {



	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {

	case CELL_SCENERY:
		return 0;    // 0 dc mtnt le jouer ne transperce pas les roches et arbres
		break;

	case CELL_CASE:
		return 0;    // idem pour caisses
		break;

	case CELL_BONUS:
		break;

	case CELL_GOAL:
		break;

	case CELL_MONSTER:

	{
		perte_de_vie(player, 1);

		return 0;
					}
		break;

	case CELL_PLAYER:
		break;

	/*case CELL_INVULNERABLE:
		return 0;
		break;*/

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);   //ptit pb dès que le joueur quitte une case il la transforme en empty mm si elle était monster avt
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);   // EN fait pas de pb car le joueur ne peut aller que sur des cases empty (ou caisse) en tout cas dès que le joeur quitte une case elle est empty!!!
	}
	return move;
}



