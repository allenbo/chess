#include "game.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char* src_root_path = "./src";
static char* black_soldier_image = "./src/black_soldier.png";
static char* black_cannon_image = "./src/black_cannon.png";
static char* black_tank_image = "./src/black_tank.png";
static char* black_horse_image = "./src/black_horse.png";
static char* black_chancellor_image = "./src/black_chancellor.png";
static char* black_scholar_image = "./src/black_scholar.png";
static char* black_general_image = "./src/black_general.png";
static char* red_soldier_image = "./src/red_soldier.png";
static char* red_cannon_image = "./src/red_cannon.png";
static char* red_tank_image = "./src/red_tank.png";
static char* red_horse_image = "./src/red_horse.png";
static char* red_chancellor_image = "./src/red_chancellor.png";
static char* red_scholar_image = "./src/red_scholar.png";
static char* red_general_image = "./src/red_general.png";


Game* 
create_game(GtkWidget* widget, GdkPixmap* pixmap) {
	Game* game = (Game*)malloc(sizeof(Game));
	game->board = create_board();
	game->red   = create_user(red);
	game->black = create_user(black);

	//create all chessman
	int index = 0;
	int i = 0;
	//create the red soldiers
	for(i=0; i<5; i++) {
		game->man[index] = create_chessman(red, soldier, index, game->board,
				game->red, red_soldier_image,3, i*2);
		index ++;
	}
	//create the red cannons
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(red, cannon, index, game->board,
				game->red, red_cannon_image,2, 6*i + 1);
		index ++;
	}
	//create the red tanks
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(red, tank, index, game->board,
				game->red, red_tank_image,0, 8*i);
		index ++;
	}
	//create the red horses
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(red, horse, index, game->board,
				game->red, red_horse_image, 0, 6*i +1);
		index ++;
	}
	//create the red chancellors
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(red, chancellor, index, game->board,
				game->red, red_chancellor_image, 0, 4*i +2);
		index ++;
	}
	//create the red scholars
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(red, scholar, index, game->board,
				game->red, red_scholar_image, 0, 2*i +3);
		index ++;
	}
	game->man[index] = create_chessman(red, general, index, game->board,
			game->red, red_general_image, 0, 4);
	index ++;
	//create the black soldiers
	for(i=0; i<5; i++) {
		game->man[index] = create_chessman(black, soldier, index, game->board,
				game->black, black_soldier_image, 6, i*2);
		index ++;
	}
	//create the black cannons
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(black, cannon, index, game->board,
				game->black, black_cannon_image, 7, 6*i + 1);
		index ++;
	}
	//create the black tanks
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(black, tank, index, game->board,
				game->black, black_tank_image, 9, 8*i);
		index ++;
	}
	//create the black horses
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(black, horse, index, game->board,
				game->black, black_horse_image, 9, 6*i +1);
		index ++;
	}
	//create the black chancellors
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(black, chancellor, index, game->board,
				game->black, black_chancellor_image, 9, 4*i +2);
		index ++;
	}
	//create the black scholars
	for(i = 0; i < 2; i ++) {
		game->man[index] = create_chessman(black, scholar, index, game->board,
				game->black, black_scholar_image, 9, 2*i +3);
		index ++;
	}
	game->man[index] = create_chessman(black, general, index, game->board,
			game->black, black_general_image, 9, 4);
	index ++;
	game->current_user = game->red;
	game->current_man = NULL;
	game->level = 0;
	game->mode  = 0;

	draw_game(game, pixmap);
	gtk_widget_draw(widget, NULL);
	return game;
}

void
draw_game(Game* game, GdkPixmap* pixmap) {
	draw_board(game->board, pixmap);
	int i = 0;
	for(i = 0; i < 32 ; i ++) {
		if(game->man[i] != NULL)
			draw_chessman(game->man[i], pixmap);
	}
}

/* @return value
 * 0: choose nothing
 * 1: move to blank space 
 * 2: cannot move to the blank space
 * 3: choose a chess to play
 * 4: black wins
 * 5: red wins
 * 6: eat a chessman
 */
int
check_chessman(Game* game, int x ,int y) {
	int i = 0;
	int j = 0;
	int width;
	int height;
	int start_x;
	int start_y;
	Chessman* man = NULL;
	
	width = game->board->grid_width; 
	height= game->board->grid_height;
	start_x = game->board->position_x[0][0];
	start_y = game->board->position_y[0][0];

	j = (x - start_y) / width;
	i = (y - start_x) / height;

	man = game->board->man[i][j];
	if(man == NULL) {
		if(game->current_man == NULL)
			return CHESSMAN_BLANK;
		else {
			 int ismove = chessman_moveto(game->current_man, i, j);
			 if(1 == ismove) {//move sucessfully
				 game->current_man = NULL;
				 if(game->current_user == game->red)
					 game->current_user = game->black;
				 else
					 game->current_user = game->red;
				 return CHESSMAN_MOVE;
			 }else 
				 return CHESSMAN_NOT_MOVE;
			
		}
	}else{
		man->color==red?printf("red"):printf("black");
		printf("%d\n",man->id);
		if(man->user == game->current_user) {
			game->current_man = man;
			printf("select\n");
			return CHESSMAN_SELECT;
		}
		else{
			if(game->current_man == NULL) 
				return CHESSMAN_BLANK;
			else{
				int iseaten = chessman_eat(game->current_man, man);
				int re = CHESSMAN_BLANK;
				if(0 == iseaten)
					return CHESSMAN_NOT_MOVE;
				else if(2 == iseaten)
					re = CHESSMAN_EAT;
				else if(1 == iseaten && game->current_user == game->red)
					re = CHESSMAN_BLACK_WIN;	//black wins
				else if(1 == iseaten && game->current_user == game->black)
					re = CHESSMAN_RED_WIN; 
				if(re > 3) {
					game->man[man->id] = NULL;
					game->current_man = NULL;
					game->current_user = game->current_user==game->black?game->red:game->black;
					return re;
				}
			}
		}
	}

}
void
destroy_game(Game* game) {
	if(game == NULL)
		return ;
	else{
		int i = 0;
		for(; i<32; i++) {
			if(game->man[i] != NULL) {
				destroy_chessman(game->man[i]);
				game->man[i] = NULL;
			}
		}
		if(game->board != NULL)
			destroy_board(game->board);
		if(game->red != NULL)
			destroy_user(game->red);
		if(game->black != NULL)
			destroy_user(game->black);

		game->board = NULL;
		game->red   = NULL;
		game->black = NULL;
		
		free(game);
		game = NULL;
	}
}
