#ifndef _RESOURCE_H_ 
#define _RESOURCE_H_ 

#include <gtk/gtk.h>



typedef enum _color {
	red = 0,
	black
}Color;

typedef enum _element {
	soldier = 0,
	cannon,
	tank,
	horse,
	chancellor,
	scholar,
	general
}Element;

#endif
