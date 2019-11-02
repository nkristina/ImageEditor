#ifndef PIKSEL
#define PIKSEL


class Piksel
{
public:
	Piksel();
	Piksel(int B, int G, int R);	
	void ChangeCollor(int B, int G, int R);
	int R();
	int G();
	int B();

private:

	int b, g, r;

};

#endif // !PIKSEL

