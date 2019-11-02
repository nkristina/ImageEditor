#ifndef Image_Editor
#define Image_Editor
#include<stdio.h>
#include<string>
#include "Piksel.h"
#include "Layer.h"

using namespace std;
class ImageEditor
{
public:
	ImageEditor();
	bool loadImage(unsigned char* image);
	unsigned char* saveImage();
	void addLayer();
	void deletLayer();
	void selectLayer(int i);
	void setLayerOpacity(int i);
	void invertColors();
	void toGrayScale();
	void blur(int size);
	void flipHorizontal();
	void flipVertical();
	void crop(int x, int y, int w, int h);
	void setActiveColor(string hex);
	void fillRect(int x, int y, int w, int h);
	void eraseRect(int x, int y, int w, int h);
	~ImageEditor();

private:
	//Piksel*** Image;
	long int hight,width;
	char* name;
	layer* first, * last,*active,*layers;
	int colorr,colorb,colorg;

};
#endif // !Image_Editor

