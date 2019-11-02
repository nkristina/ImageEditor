#include "Piksel.h"

Piksel::Piksel()
{
	r = -1;
	g = -1;
	b = -1;
}

Piksel::Piksel(int B, int G, int R)
{
	r = R;
	b = B;
	g = G;
}

void Piksel::ChangeCollor(int B, int G, int R)
{
	b = B;
	r = R;
	g = G;
}

int Piksel::R()
{
	return r;
}

int Piksel::G()
{
	return g;
}

int Piksel::B()
{
	return b;
}
