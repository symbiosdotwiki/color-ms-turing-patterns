#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>

const double PI = 3.1415926535;

template <typename T> int round(T num){
	if (num - (int)num > 0.5)
		return (int)num + 1;
	else		return (int)num;
}

void RGBtoHSV(sf::Uint8 pixel[3], float hsvpixel[3])
{
	float min, max, delta;
	float r = (float)pixel[0] / 255.0;
	float g = (float)pixel[1] / 255.0;
	float b = (float)pixel[2] / 255.0;
	min = min(r, min(g, b));
	max = max(r, max(g, b));
	hsvpixel[2] = max;				// v
	delta = max - min;
	if (max != 0)
		hsvpixel[1] = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		hsvpixel[1] = 0;
		hsvpixel[0] = -1;
		return;
	}
	if (r == max)
		hsvpixel[0] = (g - b) / delta;		// between yellow & magenta
	else if (g == max)
		hsvpixel[0] = 2 + (b - r) / delta;	// between cyan & yellow
	else
		hsvpixel[0] = 4 + (r - g) / delta;	// between magenta & cyan
	hsvpixel[0] *= 60;				// degrees
	if (hsvpixel[0] < 0)
		hsvpixel[0] += 360;
}
void HSVtoRGB(sf::Uint8 pixel[3], float hsvpixel[3])
{
	int i;
	float h = hsvpixel[0], s = hsvpixel[1], v = hsvpixel[2];
	float r, g, b, f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		r = g = b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:		// case 5:
		r = v;
		g = p;
		b = q;
		break;
	}
	pixel[0] = r * 255;
	pixel[1] = g * 255;
	pixel[2] = b * 255;
}

void Saturate(sf::Uint8 pixel[3], float sat){
	float hsvpixel[3];
	RGBtoHSV(pixel, hsvpixel);
	hsvpixel[1] *= sat;
	if (hsvpixel[1] > 1)
		hsvpixel[1] = 1.0;
	HSVtoRGB(pixel, hsvpixel);
}

void Hue(sf::Uint8 pixel[3], float hue){
	float hsvpixel[3];
	RGBtoHSV(pixel, hsvpixel);
	hsvpixel[0] += hue;
	while (hsvpixel[0] > 360)
		hsvpixel[0] -= 360.0;
	HSVtoRGB(pixel, hsvpixel);
}

void Brightness(sf::Uint8 pixel[3], float bright){
	float hsvpixel[3];
	RGBtoHSV(pixel, hsvpixel);
	hsvpixel[2] *= bright;
	if (hsvpixel[2] > 1)
		hsvpixel[2] = 1.0;
	HSVtoRGB(pixel, hsvpixel);
}

void Contrast(sf::Uint8 pixel[3], float contrast){
	float dummy[3];
	for (int i = 0; i < 3; i++){
		dummy[i] = (float)pixel[i] / 255.0;
		dummy[i] -= 0.5;
		if (dummy[i] > -.25 && dummy[i] < .25)
			dummy[i] *= contrast;
		else if (dummy[i] < -.25)
			dummy[i] -= (dummy[i] + .5) * (1 - contrast);
		else
			dummy[i] += (.5 - dummy[i]) * (1 - contrast);
		dummy[i] += .5;
		pixel[i] = 255 * dummy[i];
		if (pixel[i] > 255)
			pixel[i] = 255;
		if (pixel[i] < 0)
			pixel[i] = 0;
	}
}
