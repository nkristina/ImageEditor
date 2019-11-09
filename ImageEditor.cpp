#include "ImageEditor.h"
#include<iostream>

using namespace std;
ImageEditor::ImageEditor()
{
	first = NULL;
	last = NULL;
	active = NULL;
	layers = NULL;
	name = NULL;
	hight = 0;
	width = 0;
	colorb = 0;
	colorg = 0;
	colorr = 0;
}

bool ImageEditor::loadImage(unsigned char* image)
{
	int i = 2;
	int k = 0;
	if (!(image[0] == 'B' && image[1] == 'M')) //proverava pocetak BM
		return false;
	if (image[2] == '=') //da li postoji naziv
	{
		i++;
		while (image[i + k] != '=') k++; //broji karaktere naziva
		name = new char[k + 1]; //alocira string za ime
		k = 0;
		while (image[i] != '=')
		{
			name[k++] = image[i++]; //upisuje ime
		}
		name[k] = '\0';
		i++;
	}
	//------------------------------------------------------------------------------------------------

	for (; i % 4 != 0; i++); //ceka mod 4
	width = image[i] + image[i + 1] * 256 + image[i + 2] * 256 * 256 + image[i + 3] * 256 * 256 * 256;
	i = i + 4; //racuna sirinu
	hight = image[i] + image[i + 1] * 256 + image[i + 2] * 256 * 256 + image[i + 3] * 256 * 256 * 256;
	i = i + 4; //racuna visinu

	//ucitavanje piksela ------------------------------------------------------------------------------
	layers = new layer; //prvi lejer
	layers->next = NULL;
	layers->prev = NULL;
	layers->cover = new Piksel * *[hight]; //alociranje niza pokazivaca na nizove pokazivaca (red)
	for (int j = hight - 1; j >= 0; j--)
	{
		layers->cover[j] = new Piksel * [width]; //alociranje svakog reda koji je niz pokazivaca na piksele
		for (int l = 0; l < width; l++)
		{
			layers->cover[j][l] = new Piksel(image[i], image[i + 1], image[i + 2]);
			i += 3;
		}
		for (; i % 4 != 0; i++);
	}
	first = layers;
	last = first;
	active = first;

	return true;
}

unsigned char* ImageEditor::saveImage()
{
	long long int size; //velicina stringa
	unsigned char* image;
	long long i = 0;
	//---RACUNANJE DUZINE-----------------------------------------
	size = 2;
	if (name != NULL)
		size = size + strlen(name) + 2;
	for (; size % 4; size++);
	size += 8;
	size = size + hight * width * 3;
	int hole = (3 * width) % 4;
	if (hole)
		hole = 4 - hole;
	size = size + hole * hight;
	//-----------------------------------------------------------
	image = new unsigned char[size];
	image[i++] = 'B';
	image[i++] = 'M';

	if (name) //dodaje ime
	{
		image[i++] = '=';
		char* name2 = name;
		for (; *name2; i++)
		{
			image[i] = *name2;
			name2++;
		}
		image[i++] = '=';
	}
	for (; i % 4; i++)
		image[i] = 0;
	//--SIRINA I VISINA---------------------------------
	long w = width, h = hight;
	for (int k = 0; k < 4; k++)
	{
		image[i++] = w % 256;
		w = w / 256;
	}
	for (int k = 0; k < 4; k++)
	{
		image[i++] = h % 256;
		h = h / 256;
	}
	//-------------------------------------------------
	for (int k = hight - 1; k >= 0; k--)
	{
		for (int j = 0; j < width; j++)
		{
			int r = 0, g = 0, b = 0;
			double precent = 0;
			layer* current = last;
			while (current)
			{
				if (current->cover[k][j]->R() != -1)
				{
					r = r + current->cover[k][j]->R() * (1 - precent) * current->opacity * 1. / 100;
					g = g + current->cover[k][j]->G() * (1 - precent) * current->opacity * 1. / 100;
					b = b + current->cover[k][j]->B() * (1 - precent) * current->opacity * 1. / 100;
					precent += (1 - precent) * current->opacity * 1. / 100;
				}
				current = current->prev;
			}
			image[i++] = b;
			image[i++] = g;
			image[i++] = r;

		}
		for (; i % 4; i++)
			image[i] = 0;
	}
	return image;
}

void ImageEditor::addLayer()
{
	layer* novi = new layer;
	novi->opacity = 100;
	novi->cover = new Piksel * *[hight];
	for (int i = 0; i < hight; i++)
	{
		novi->cover[i] = new Piksel * [width];
		for (int j = 0; j < width; j++)
		{
			novi->cover[i][j] = new Piksel();
		}
	}
	if (first == NULL || active == NULL)
	{
		return;
	}

	if (active->next)
	{
		active->next->prev = novi;
		novi->prev = active;
		novi->next = active->next;
		active->next = novi;
		active = novi;
	}
	else
	{
		active->next = novi;
		novi->next = NULL;
		novi->prev = active;
		last = novi;
		active = novi;
	}



}

void ImageEditor::deleteLayer()
{
	if (first != NULL && active == first) //ako ima samo jedan element
	{
		return;
	}
	if (first && active == last) //active na kraju
	{
		layer* curent;
		curent = last;
		last = last->prev;
		last->next = NULL;
		active = last;
		//oslobadja sloj
		for (int i = 0; i < hight; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete(curent->cover[i][j]);
			}
			delete[](curent->cover[i]);
		}
		delete[] curent->cover;
		delete curent;
		return;
	}
	if (first) //aktivan u sredini
	{
		layer* curent;
		curent = active;
		active = active->prev;
		active->next = curent->next;
		curent->next->prev = active;
		//oslobadja sloj
		for (int i = 0; i < hight; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete(curent->cover[i][j]);
			}
			delete[](curent->cover[i]);
		}
		delete[] curent->cover;
		delete curent;
	}
}

void ImageEditor::selectLayer(int i)
{
	layer* curent = first;
	if (i == 0)
	{
		active = first;
		return;
	}
	for (int j = 0; j < i; j++)
	{
		if (curent->next)
			curent = curent->next;
		else return;
	}
	active = curent;
	return;
}

void ImageEditor::setLayerOpacity(int i)
{
	layer* cur;
	cur = first->next;
	if (active && active != first)
		active->opacity = i;
	while (cur)
	{
		if (cur->selected)
			cur->opacity = i;
		cur = cur->next;
	}
	return;
}

void ImageEditor::invertColors()
{
	if (!active)
		return;
	for (int i = 0; i < hight; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int r = active->cover[i][j]->R();
			int g = active->cover[i][j]->G();
			int b = active->cover[i][j]->B();
			if (r != -1 && b != -1 && g != -1) //ako nisu prazni
				active->cover[i][j]->ChangeCollor(255 - b, 255 - g, 255 - r);
		}
	}
	cout << "promenio";
	layer* cur;
	cur = first;
	while (cur)
	{
		if (cur->selected && cur != active)
		{
			for (int i = 0; i < hight; i++)
			{
				for (int j = 0; j < width; j++)
				{
					int r = cur->cover[i][j]->R();
					int g = cur->cover[i][j]->G();
					int b = cur->cover[i][j]->B();
					if (r != -1 && b != -1 && g != -1) //ako nisu prazni
						cur->cover[i][j]->ChangeCollor(255 - b, 255 - g, 255 - r);
				}
			}
			cout << "probni";
		}
		cur = cur->next;
	}

}

void ImageEditor::toGrayScale()
{
	if (!active)
		return;
	for (int i = 0; i < hight; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int r = active->cover[i][j]->R();
			int g = active->cover[i][j]->G();
			int b = active->cover[i][j]->B();
			if (r != -1 && b != -1 && g != -1) //ako nisu prazni
			{
				int c = 0.3 * r + 0.59 * g + 0.11 * b;
				active->cover[i][j]->ChangeCollor(c, c, c);
			}
		}
	}
	layer* cur;
	cur = first;
	while (cur)
	{
		if (cur->selected && cur != active)
			for (int i = 0; i < hight; i++)
			{
				for (int j = 0; j < width; j++)
				{
					int r = cur->cover[i][j]->R();
					int g = cur->cover[i][j]->G();
					int b = cur->cover[i][j]->B();
					if (r != -1 && b != -1 && g != -1) //ako nisu prazni
					{
						int c = 0.3 * r + 0.59 * g + 0.11 * b;
						cur->cover[i][j]->ChangeCollor(c, c, c);
					}
				}
			}
		cur = cur->next;
	}

}

void ImageEditor::blur(int size)
{
	if (!active || size<0 || size>hight - 1 || size > width - 1)
		return;
	int sr = 0, sg = 0, sb = 0; //sume

	//---POMOCNA---------------------------------------------------------------------------------------------
	Piksel*** Image1;
	Image1 = new Piksel * *[hight]; //pomocna matrica
	for (int i = 0; i < hight; i++)
	{
		Image1[i] = new Piksel * [width]; //alociranje svakog reda koji je niz pokazivaca na piksele
		for (int j = 0; j < width; j++)
		{
			int r = active->cover[i][j]->R();
			int g = active->cover[i][j]->G();
			int b = active->cover[i][j]->B();
			Image1[i][j] = new Piksel(b, g, r);
		}
	}
	int k = 0; //brojac

 //-- KROZ MATRICU ---------------------------------------------------------------------------------------------

	for (int i = 0; i < hight; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//racuna za svaki piksel njegovu okolinu
			for (int x = -size; x <= size; x++)
			{
				if ((i + x) >= 0 && (i + x) <= (hight - 1)) //ako je u opsegu
					for (int y = -size; y <= size; y++)
					{
						if ((j + y) >= 0 && (j + y) <= (width - 1)) //ako je u opsegu
						{
							//proveravanje da li je prazan i ako nije dodavanje u sumu
							if (Image1[i + x][j + y]->R() != -1)
							{
								sr = sr + Image1[i + x][j + y]->R();
								sg = sg + Image1[i + x][j + y]->G();
								sb = sb + Image1[i + x][j + y]->B();
								k++;
							}
						}
					}
			}
			if (k != 0)
				active->cover[i][j]->ChangeCollor(sb / k, sg / k, sr / k); //dobija novu vrednost
			k = 0;
			sb = 0; sr = 0; sg = 0;
		}
	}
	//-- DELOCIRANJE POMOCNE ------------------------------------------------------------------------------------------
	for (int i = 0; i < hight; i++) //brise
	{
		for (int j = 0; j < width; j++)
		{
			delete(Image1[i][j]);
		}
		delete[](Image1[i]);
	}
	delete[] Image1;
	return;
}

void ImageEditor::flipVertical()
{
	if (!active)
		return;
	for (int i = 0; i < hight / 2; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Piksel* cur;
			cur = active->cover[i][j];
			active->cover[i][j] = active->cover[hight - 1 - i][j];
			active->cover[hight - 1 - i][j] = cur;
		}
	}
	return;
}

void ImageEditor::flipHorizontal()
{

	if (!active)
		return;
	for (int i = 0; i < hight; i++)
	{
		for (int j = 0; j < width / 2; j++)
		{
			Piksel* cur;
			cur = active->cover[i][j];
			active->cover[i][j] = active->cover[i][width - 1 - j];
			active->cover[i][width - 1 - j] = cur;
		}
	}
	return;
}

void ImageEditor::crop(int x, int y, int w, int h)
{
	int i = 0, j = 0, hp = 0, wp = 0;

	while ((y + i) < hight && i <= h) //odredjivanje nove granice za visinu
	{
		hp++;
		i++;
	}
	while ((x + j) < width && j <= w) //odredjivanje nove granice za sirinu
	{
		wp++;
		j++;
	}

	layer* cur = first; //kroz sve lejere
	while (cur)
	{
		//nova matrica
		Piksel*** Image1;
		Image1 = new Piksel * *[hp];
		for (i = 0; i < hp; i++)
		{
			Image1[i] = new Piksel * [wp];
			for (j = 0; j < wp; j++)
			{
				Image1[i][j] = new Piksel(cur->cover[y + i][x + j]->B(), cur->cover[y + i][x + j]->G(), cur->cover[y + i][x + j]->R());
			}
		}

		//brisanje trenutne matrice

		for (int k = 0; k < hight; k++)
		{
			for (int s = 0; s < width; s++)
			{
				delete cur->cover[k][s];
			}
			delete[] cur->cover[k];
		}
		delete[] cur->cover;
		cur->cover = Image1; //dodela nove matrice
		cur = cur->next;
	}
	hight = hp;
	width = wp;
	return;
}

void ImageEditor::setActiveColor(string hex)//#RRGGBB
{
	int i = 1;
	//RR
	if (hex[i] >= '0' && hex[i] <= '9')
		colorr = (hex[i++] - '0') * 16;
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorr = ((hex[i++] - 'A') + 10) * 16;

	if (hex[i] >= '0' && hex[i] <= '9')
		colorr += (hex[i++] - '0');
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorr += ((hex[i++] - 'A') + 10);
	//GG
	if (hex[i] >= '0' && hex[i] <= '9')
		colorg = (hex[i++] - '0') * 16;
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorg = ((hex[i++] - 'A') + 10) * 16;

	if (hex[i] >= '0' && hex[i] <= '9')
		colorg += (hex[i++] - '0');
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorg += ((hex[i++] - 'A') + 10);
	//BB
	if (hex[i] >= '0' && hex[i] <= '9')
		colorb = (hex[i++] - '0') * 16;
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorb = ((hex[i++] - 'A') + 10) * 16;
	if (hex[i] >= '0' && hex[i] <= '9')
		colorb += (hex[i++] - '0');
	else
		if (hex[i] >= 'A' && hex[i] <= 'F')
			colorb += ((hex[i++] - 'A') + 10);
}

void ImageEditor::fillRect(int x, int y, int w, int h)
{

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			active->cover[y + i][x + j]->ChangeCollor(colorb, colorg, colorr);
		}
	}
	return;
}

void ImageEditor::eraseRect(int x, int y, int w, int h)
{
	if (!active)
		return;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			active->cover[y + i][x + j]->ChangeCollor(-1, -1, -1);
		}
	}
	return;
}

void ImageEditor::rotate(bool side)
{
	int s = 0;
	layer* cur;
	cur = first;
	while (cur)
	{
		Piksel*** mat;
		mat = new Piksel * *[width];
		for (int i = 0; i < width; i++)
		{
			mat[i] = new Piksel * [hight];
			for (int j = 0; j < hight; j++)
			{
				if (side)
					mat[i][j] = new Piksel(cur->cover[hight - 1 - j][i]->B(), cur->cover[hight - 1 - j][i]->G(), cur->cover[hight - 1 - j][i]->R());
				else
				{
					mat[i][j] = new Piksel(cur->cover[j][width - 1 - i]->B(), cur->cover[j][width - 1 - i]->G(), cur->cover[j][width - 1 - i]->R());
				}
			}
		}

		for (int i = 0; i < hight; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete cur->cover[i][j];
			}
			delete[] cur->cover[i];
		}
		delete[] cur->cover;
		cur->cover = mat;
		cur = cur->next;
		s++;
	}

	long int t = hight;
	hight = width;
	width = t;
	return;
}

void ImageEditor::addLayerToSelection(int i)
{
	layer* cur;
	cur = first;
	int c = 0;
	while (cur && c < i)
	{
		cur = cur->next;
		c++;
	}
	if (cur)
		cur->selected = true;
}

void ImageEditor::deleteSelection()
{
	layer* cur;
	cur = first;
	while (cur)
	{
		cur->selected = false;
		cur = cur->next;
	}
}

/*void ImageEditor::ogledalo()
{
	layer* tekuci;
	tekuci = first;
	long int k = 2 * width;
	while (tekuci)
	{
		//----KREIRANJE POMOCNE SLIKE-----------------------------------------------------------------------
		Piksel*** pomocnaslika = new Piksel * *[hight];
		for (int i = 0; i < hight; i++)
		{
			pomocnaslika[i] = new Piksel * [k];
			for (long int j = 0; j < 2 * width; j++)
			{
				pomocnaslika[i][j] = new Piksel();
			}
		}

		for (int i = 0; i < hight; i++)
			for (int j = 0; j < width; j++)
				pomocnaslika[i][j]->ChangeCollor(tekuci->cover[i][j]->B(), tekuci->cover[i][j]->G(), tekuci->cover[i][j]->R());
		for (int i = 0; i < hight; i++)
			for (int j = 0; j < width; j++)
				pomocnaslika[i][j + width]->ChangeCollor(tekuci->cover[i][width - 1 - j]->B(), tekuci->cover[i][width - 1 - j]->G(), tekuci->cover[i][width - 1 - j]->R());

		//-----BRISANJE TEKUCE SLIKE-----------------------------------------------------------------------
		for (int i = 0; i < hight; i++)
		{
			for (int j = 0; j < width; j++)
			{
				delete tekuci->cover[i][j];
			}
			delete[] tekuci->cover[i];
		}
		delete[] tekuci->cover;
		tekuci->cover = pomocnaslika;//postavljanje pomocne slike na tekucu
		tekuci = tekuci->next;
	}
	width = 2 * width;
}*/

ImageEditor::~ImageEditor()
{
	int k = 0;
	while (first)
	{
		for (int i = 0; i < hight; i++)
		{
			for (int j = 0; j < width; j++)

			{
				delete first->cover[i][j];
			}

			delete[] first->cover[i];
		}
		delete[] first->cover;

		layer* curent;
		curent = first;
		first = first->next;
		delete curent; //brisanje sloja
		k++;
	}
	first = NULL;
	if (name != NULL) //brisanje imena ako postoji
		delete[] name;
	return;

}


