#ifndef LAYER
#define LAYER

#include "Piksel.h"

struct layer
{
	Piksel*** cover;
	int opacity=100;
	bool selected = false;
	layer* next;
	layer* prev;
};

#endif