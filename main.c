#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"

GdkPixmap* pixmap = NULL;
GdkGC*  gc = NULL;
GdkColor color;
Game* game = NULL;

GtkWidget* create_window() {
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Chinese Chess");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	return window;
}

int
configure_event_callback(GtkWidget* widget, GdkEventConfigure* event) {
	if(pixmap)
		g_object_unref(pixmap);
	pixmap = gdk_pixmap_new(widget->window,
			widget->allocation.width,
			widget->allocation.height,
			-1);
	gdk_draw_rectangle(pixmap, widget->style->white_gc,
			TRUE, 0, 0,
			widget->allocation.width,
			widget->allocation.height);
	gc = gdk_gc_new(widget->window);
	color.red = 30000;
	color.green = 0;
	color.blue = 0;
	gdk_gc_set_rgb_fg_color(gc, &color);
	return TRUE;
}
	
int
expose_event_callback(GtkWidget* widget, GdkEventExpose* event, gpointer data) {
	gdk_draw_drawable(widget->window,
			widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
			pixmap, 0, 0, 0, 0,
			widget->allocation.width,
			widget->allocation.height);
	return FALSE;
}

void destroy_callback(GtkWidget* widget, gpointer data) {
	
}

int quick_message(gchar* message) {
	GtkWidget* dialog = gtk_dialog_new_with_buttons(
							"Win",
							NULL,
							GTK_DIALOG_MODAL|GTK_DIALOG_DESTROY_WITH_PARENT,
							GTK_STOCK_OK,GTK_RESPONSE_ACCEPT,
							GTK_STOCK_CANCEL,GTK_RESPONSE_REJECT,
							NULL);
	GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget* label = gtk_label_new(message);
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);
	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_widget_show_all(dialog);
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	return result;
}

int handle_winner(int winner) {		//0:black 1:red
	gchar* message  = NULL;
	if(winner == 0)
		message = "black wins";
	else
		message = "red wins";
	gint result = quick_message(message);
	destroy_game(game);
	exit(1);
}

int mouse_press_event_callback(GtkWidget* widget, GdkEventButton* event) {
	int re = check_chessman(game, event->x, event->y);

	switch(re) {
		case CHESSMAN_BLANK:
			break;
		case CHESSMAN_SELECT:
			break;
		case CHESSMAN_MOVE:
			draw_game(game,pixmap);
			gtk_widget_draw(widget, NULL);
			break;
		case CHESSMAN_NOT_MOVE:
			break;
		case CHESSMAN_EAT:
			draw_game(game,pixmap);
			gtk_widget_draw(widget, NULL);
			break;
		case CHESSMAN_BLACK_WIN:
			draw_game(game,pixmap);
			gtk_widget_draw(widget, NULL);
			handle_winner(0);
			break;
		case CHESSMAN_RED_WIN:
			draw_game(game,pixmap);
			gtk_widget_draw(widget, NULL);
			handle_winner(1);
			break;
		default:
			break;
	}

	return re;
}


int main(int argc, char** argv) {
	gtk_init(&argc, &argv);

	GtkWidget* window = create_window();
	GtkWidget* drawing_area = gtk_drawing_area_new();
	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	gtk_widget_set_size_request(drawing_area, 864,696);
	g_signal_connect(drawing_area, "expose_event", G_CALLBACK(expose_event_callback), NULL);
	g_signal_connect(drawing_area, "configure_event", G_CALLBACK(configure_event_callback), NULL);
	g_signal_connect(drawing_area, "button_press_event", G_CALLBACK(mouse_press_event_callback), NULL);
	gtk_widget_set_events(drawing_area, GDK_BUTTON_PRESS_MASK);
	gtk_container_add(GTK_CONTAINER(window), drawing_area);
	
	gtk_widget_show_all(window);
	game = create_game(drawing_area, pixmap);
	gtk_main();
	destroy_game(game);
	return 0;
}
