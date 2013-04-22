#ifndef _BOARD_H_
#define _BOARD_H_


#include "resource.h"
#include <gtk/gtk.h>

typedef struct _board Board;
typedef struct _user  User;

typedef struct _chessman{
	Color	color;
	Element element;
	int	id;
	User*	user;
	Board*	board;
	char*   image_path;
	int	image_width;
	int	image_height;
	int	position_x;
	int	position_y;
}Chessman;


struct _user {
	Color  	  color;
	Chessman* man[16];
	int       isaction;	
};

struct _board {
	Chessman* man[10][9];
	int	  grid_width;
	int	  grid_height;
	int	  position_x[10][9];
	int	  position_y[10][9];
	
	char*	  image_path;
	int	  image_width;
	int	  image_height;
};

Chessman* create_chessman(Color color, Element element, int id, Board* board,
		User* user, const char* image_path, int x, int y);
void  draw_chessman(Chessman* man, GdkPixmap* pixmap); 
int   chessman_moveto(Chessman* man, int i, int j);
int   chessman_eat(Chessman* man, Chessman* eaten_man);
void  destroy_chessman(Chessman* man);
User* create_user(Color color);
void  destroy_user(User* user);
Board* create_board();
void   draw_board(Board* board, GdkPixmap* pixmap);
void   destroy_board(Board* board);

#endif
