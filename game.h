#ifndef _GAME_H_
#define _GAME_H_

#include <gtk/gtk.h>
#include "board.h"
#include "resource.h"

typedef struct _game {
	Board* board;
	User*  red;
	User*  black;
	User*  current_user;
	
	Chessman* man[32];
	Chessman* current_man;
	int    level;
	int    mode;
}Game;

enum {
	CHESSMAN_BLANK = 0,
	CHESSMAN_SELECT,
	CHESSMAN_MOVE,
	CHESSMAN_NOT_MOVE,
	CHESSMAN_EAT,
	CHESSMAN_BLACK_WIN,
	CHESSMAN_RED_WIN
};

Game* create_game(GtkWidget* widget, GdkPixmap* pixmap);
void  draw_game(Game* game, GdkPixmap* pixmap);
int   check_chessman(Game* game, int x, int y);
void  destroy_game(Game* game);

#endif
