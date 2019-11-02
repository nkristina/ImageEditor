#ifndef LAYER
#define LAYER

#include "Piksel.h"

struct layer
{
	Piksel*** cover;
	int opacity=100;
	layer* next;
	layer* prev;
};

#endif