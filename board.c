#include "board.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>

//#define MAX(a,b) (a)>(b)?(a):(b)
//#define MIN(a,b) (a)<(b)?(a):(b)


static char* board_image = "./src/board.png";
static int   board_image_width = 864;
static int   board_image_height = 696; 
static int   chessman_image_width = 67; 
static int   chessman_image_height = 67;

static int   grid_width = 95;
static int   grid_height = 67;

static int   board_position_x = 40;
static int   board_position_y = 55;


static int  soldier_moveto(Chessman* man, int i, int j);
static int  cannon_moveto(Chessman* man, int i, int j);
static int  tank_moveto(Chessman* man, int i, int j);
static int  horse_moveto(Chessman* man, int i, int j);
static int  chancellor_moveto(Chessman* man, int i, int j);
static int  scholar_moveto(Chessman* man, int i, int j);
static int  general_moveto(Chessman* man, int i, int j);

Chessman*
create_chessman(Color color, Element element, int id, Board* board,
		User* user, const char* image_path, int x, int y) {
	Chessman* man = (Chessman*)malloc(sizeof(Chessman));
	memset(man, 0, sizeof(Chessman));

	man->color	= color;
	man->element	= element;
	man->id		= id;
	man->user	= user;
	man->board	= board;
	
 	int len = strlen(image_path) +1;
	man->image_path = (char*)malloc(sizeof(char)*len);
	memset(man->image_path, 0, len);
	strcpy(man->image_path, image_path);
	
	man->image_width = chessman_image_width;
	man->image_height = chessman_image_height;

	man->position_x = x;
	man->position_y = y;

	man->board->man[man->position_x][man->position_y] = man;
	if(man->color==red)
		man->user->man[id] = man;
	else
		man->user->man[id-16] = man;

	return man;
}

void
draw_chessman(Chessman* chessman, GdkPixmap* pixmap) {
	GdkPixbuf* pixbuf = NULL;
	GError*  error =NULL;
	GdkRgbDither dither;

	pixbuf = gdk_pixbuf_new_from_file(chessman->image_path, &error);
	if(!pixbuf) {
		printf("chess %d %d\n",chessman->color,chessman->element);
		printf("chess image open error!\n%s\n",chessman->image_path);
		fflush(stdout);
		exit(1);
	}
	int dest_x = chessman->board->position_x[chessman->position_x][chessman->position_y]; 
	int dest_y = chessman->board->position_y[chessman->position_x][chessman->position_y]; 
	gdk_draw_pixbuf(pixmap, NULL,
			pixbuf, 0,0,
			dest_y,dest_x,
			chessman->image_width,chessman->image_height,
			dither, 0,0);

}


int
chessman_moveto(Chessman* man, int i, int j) {
	int re = 0;
	switch(man->element) {
		case soldier:
			re = soldier_moveto(man, i, j);
			break;
		case cannon:
			re = cannon_moveto(man,i,j);
			break;
		case tank:
			re = tank_moveto(man,i, j);
			break;
		case horse:
			re = horse_moveto(man, i,j);
			break;
		case chancellor:
			re = chancellor_moveto(man, i, j);
			break;
		case scholar:
			re = scholar_moveto(man, i, j);
			break;
		case general:
			re = general_moveto(man, i, j);
			break;
		default:
			break;
	}
	return re;
}

int
chessman_eat(Chessman* man, Chessman* eaten_man) {
	int re = chessman_moveto(man, eaten_man->position_x, eaten_man->position_y);
	if(re == 1) {
		if(eaten_man->element == general)
			return 1;	//general dead
		else {
			eaten_man->board->man[eaten_man->position_x][eaten_man->position_y] = man;
			destroy_chessman(eaten_man);
			return 2;	//other chessman dead
		}
	}else
		return 0;
}

static int
soldier_moveto(Chessman* man , int i , int j) {
	int re;
	if(man->color == red) {
		if(man->position_x < 5) {
			if(i != man->position_x + 1 || j != man->position_y)
				re = 0;	
			else{
				re = 1;
			}
		}else {
			if((i == man->position_x+1 && j == man->position_y) ||
			   (i == man->position_x && j == man->position_y+1) ||
			   (i == man->position_x && j == man->position_y-1))
				re = 1;
			else
				re = 0;
		}
	}else {
		if(man->position_x > 4) {
			if(i == man->position_x-1 && j == man->position_y)
				re = 1;
			else
				re = 0;
		}else {
			if((i == man->position_x-1 && j == man->position_y) ||
			   (i == man->position_x && j == man->position_y+1) ||
			   (i == man->position_x && j == man->position_y-1))
				re = 1;
			else
				re = 0;
		}
	}
	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}

static int
cannon_moveto(Chessman* man, int i, int j) {
	int re = 0;
	if(man->position_x == i && man->position_y ==j)
		re = 0;
	if(man->position_x == i || man->position_y == j) {
		int min = 0;
		int max = 0;
		int n  = 0;
		int k;
		if(man->position_x == i){
			min = MIN(man->position_y, j);
			max = MAX(man->position_y, j);
			k = min + 1;
			for(; k < max; k ++) {
				if(man->board->man[i][k] != NULL)
					n ++;
			}
		}else {
			min = MIN(man->position_x,i);
			max = MAX(man->position_x,i);
			k = min +1;
			for(; k < max; k ++) {
				if(man->board->man[k][j] != NULL)
					n ++;
			}
		}
		
		switch(n) {
			case 0:
				if(man->board->man[i][j] == NULL)
					re = 1;
				else
					re = 0;
				break;
			case 1:
				if(man->board->man[i][j] == NULL)
					re = 0;
				else {
					if(man->user == man->board->man[i][j]->user)
						re = 0;
					else 
						re = 1;
				}
				break;
			default:
				re = 0;
				break;
		}
	}
	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}


static int
tank_moveto(Chessman* man, int i, int j) {
	int re = 0;
	int old_i = man->position_x;
	int old_j = man->position_y;

	if(old_i == i && old_j == j)
		re = 0;
	if(old_i == i || old_j == j ) {
		int min = 0;
		int max = 0;
		int k ;
		if(old_i == i){
			min = MIN(old_j,j);
			max = MAX(old_j,j);
			k = min +1;
			for(;k < max; k ++)
				if(man->board->man[i][k] != NULL)
					break;
		}else{
			min = MIN(old_i,i);
			max = MAX(old_i,i);
			k = min +1;
			for(; k < max; k ++)
				if(man->board->man[k][i] != NULL)
					break;
		}
		if(k == max)
			re = 1;
		else
			re = 0;
	}
	if(re == 1) {
		printf("tank move ok\n");
		fflush(stdout);
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}

static int
horse_moveto(Chessman* man, int i, int j) {
	int re = 0;
	int old_i = man->position_x;
	int old_j = man->position_y;
	if((i == old_i-2 && j == old_j+1 && man->board->man[old_i-1][old_j]==NULL) ||
	   (i == old_i-2 && j == old_j-1 && man->board->man[old_i-1][old_j]==NULL) ||
	   (i == old_i-1 && j == old_j-2 && man->board->man[old_i][old_j-1]==NULL) ||
	   (i == old_i+1 && j == old_j-2 && man->board->man[old_i][old_j-1]==NULL) ||
	   (i == old_i+2 && j == old_j-1 && man->board->man[old_i+1][old_j]==NULL) ||
	   (i == old_i+2 && j == old_j+1 && man->board->man[old_i+1][old_j]==NULL) ||
	   (i == old_i+1 && j == old_j+2 && man->board->man[old_i][old_j+1]==NULL) ||
	   (i == old_i-1 && j == old_j+2 && man->board->man[old_i][old_j+1]==NULL))
		re = 1;
	else 
		re = 0;
	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}

static int
chancellor_moveto(Chessman* man, int i, int j) {
	int re = 0;
	int old_i = man->position_x;
	int old_j = man->position_y;
	int min = man->color==red?0:5;
	int max = man->color==red?4:9;

	if(i > max || i < min)
		re = 0;
	else{
		if((i==old_i-2 && j==old_j-2 && man->board->man[old_i-1][old_j-1]==NULL) ||
		   (i==old_i+2 && j==old_j-2 && man->board->man[old_i+1][old_j-1]==NULL) ||
		   (i==old_i+2 && j==old_j+2 && man->board->man[old_i+1][old_j+1]==NULL) ||
		   (i==old_i-2 && j==old_j+2 && man->board->man[old_i-1][old_j+1]==NULL))
			re = 1;
		else
			re = 0;
	}

	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}


static int
scholar_moveto(Chessman* man, int i, int j) {
	int re = 0;
	int old_i = man->position_x;
	int old_j = man->position_y;

	int min_x = man->color==red?0:7;
	int max_x = man->color==red?2:9;
	int min_y = 3;
	int max_y = 5;

	if(i > max_x || i < min_x || j > max_y || j < min_y)
		re = 0;
	else{
		if((i==old_i-1 && j==old_j-1) ||
		   (i==old_i+1 && j==old_j-1) ||
		   (i==old_i+1 && j==old_j+1) ||
		   (i==old_i-1 && j==old_j+1))
			re = 1;
		else 
			re = 0;
	}
	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}

static int
general_moveto(Chessman* man, int i, int j) {
	int re = 0;
	int old_i = man->position_x;
	int old_j = man->position_y;

	int min_x = man->color==red?0:7;
	int max_x = man->color==red?2:9;
	int min_y = 3;
	int max_y = 5;

	if(i > max_x || i < min_x || j > max_y || j < min_y)
		re = 0;
	else{
		if((i==old_i && j==old_j-1) ||
		   (i==old_i && j==old_j+1) ||
		   (i==old_i+1 && j==old_j) ||
		   (i==old_i-1 && j==old_j))
			re = 1;
		else 
			re = 0;
	}
	if(re == 1) {
		man->board->man[man->position_x][man->position_y] = NULL;
		man->position_x = i;
		man->position_y = j;
		man->board->man[i][j] = man;
	}
	return re;
}
void
destroy_chessman(Chessman* man) {
	if(man == NULL) 
		return ;
	else {
		if(man->image_path != NULL)
			free(man->image_path);

		man->image_path = NULL;
		free(man);
		man = NULL;
	}
}

User*
create_user(Color color) {
	User* user = (User*)malloc(sizeof(User));
	user->color = color;
	int i = 0;
	for(i=0; i<16; i++) {
		user->man[i] = NULL;
	}
	user->isaction = 0;
	return user;
}

void
destroy_user(User* user) {
	if(user == NULL)
		return;
	free(user);
}
Board*
create_board() {
	Board* board = (Board*)malloc(sizeof(Board));
	
	int i = 0;
	int j = 0;

	for(i = 0; i<10; i++) {
		for(j=0; j<9; j++) {
			board->position_x[i][j] = board_position_x - chessman_image_height/2 + i * grid_height;
			board->position_y[i][j] = board_position_y - chessman_image_width/2 + j * grid_width;
			board->man[i][j] = NULL;
		}
	}
	
	board->image_path   = board_image;
	board->grid_width   = grid_width;
	board->grid_height  = grid_height;
	board->image_width  = board_image_width;
	board->image_height = board_image_height;
	
	return board;
}
void
draw_board(Board* board, GdkPixmap* pixmap) {
	GdkPixbuf* pixbuf = NULL;
	GError*  error =NULL;
	GdkRgbDither dither;

	pixbuf = gdk_pixbuf_new_from_file(board->image_path, &error);
	if(!pixbuf) {
		fprintf(stderr, "board image open error!!!!\n%s\n",board->image_path);
		exit(1);
	}

	gdk_draw_pixbuf(pixmap, NULL,
			pixbuf, 0,0,0,0,
			board->image_width,board->image_height,
			dither, 0,0);
}

void
destroy_board(Board* board) {
	if(board == NULL)
		return;
	else {

		board->image_path = NULL;
		free(board);
		board = NULL;
	}
}
