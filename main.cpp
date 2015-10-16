// STD Lib Headers
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <random>
#include <ctime>
#include <string>
#include <windows.h>
#include "turing.h"
#include "imageP.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//size of pattern
const int WIDTH = 1000;
const int HEIGHT = 1000;

bool SYM = false;

int TYPE = 2;

int main()
{
	std::srand((unsigned int)time(NULL));

	/*
	//to start with image
	sf::Image img;
	if (!img.loadFromFile("unf-ob-larger.jpg"))
		return -1;
	int rw = img.getSize().x;
	int rh = img.getSize().y;
	*/

	int rw = WIDTH;
	int rh = HEIGHT;
	int iter = 0;

	Turing *test = new Turing(SYM, TYPE, WIDTH, HEIGHT);

	// create the window
	sf::RenderWindow window(sf::VideoMode(rw,rh), "My window");

	sf::Texture texture;
	sf::Uint8 *pixels = new sf::Uint8[rw*rh * 4];

	texture.create(rw, rh);
	sf::Sprite sprite(texture);
	sprite.setPosition(0, 0);
	
	//run the program as long as the window is open
	while (window.isOpen()){
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
		// "close requested" event: we close the window
		if (event.type == sf::Event::Closed)
		window.close();
		}

		//clear the window with black color
		window.clear(sf::Color::Black);

		sf::Uint8 pix[3] = { 0, 0, 0 };
		sf::Uint8 pix2[3] = { 0, 0, 0 };

		for (int i = 0; i < rh; i++){
			for (int j = 0; j < rw; j++){
				for (int k = 0; k < 3; k++){
					pix[k] = (int)(test->GetPixel(k, j, i) * 255);
					pix2[k] = (int)(test->GetPixel(k, j, i) * 255);
				}
				//Hue(pix, 20*iter*(float)(i+j)/(WIDTH+HEIGHT));
				//Saturate(pix, .8);
				//Contrast(pix, 1.6);
				//Brightness(pix, .8);
				for (int k = 0; k < 3; k++){
					pixels[4 * (i*rw + j) + k] = (pix[k] * pix2[k]) / 255;
				}
				pixels[4 * (i*rw + j) + 3] = 255;
			}
		}
		texture.update(pixels);
		window.draw(sprite);

		std::cout << iter << std::endl;

		/* 
		//For saving images
		std::string folder = "output/";
		std::string number = std::to_string(iter);
		std::string filetype = ".jpg";
		std::string file = folder + number + filetype;
		texture.copyToImage().saveToFile(file);
		*/

		iter++;
		test->iterate();

		window.display();
	}

	//Keeps terminal from closing after showing results in Windows
	//std::cout<<"\n\nPress ENTER to exit the Matrix";
	//std::cin.ignore(std::cin.rdbuf()->in_avail()+1);
	return 0;
}
