#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include"Colour.h"
class Crystal
{
private:

	Colour type; //Stores the value of individual colour.
	sf::Texture tile;
	sf::Sprite sprite;
	sf::Vector2f position;
	int removeTiming = 10;
	int swapTiming = 8;
	float scaleInc=0;
public:
	int animTime;
	sf::Vector2f animDir;//[1]shrink on the x, [1] shrink on the y;
	sf::Vector2f animScale=sf::Vector2f(0.5,0.5);
	void setRemoveAnim();
	void setSwapAnim(sf::Vector2i dir);
	bool alreadySwapped;
	sf::Vector2i toSwapWith = sf::Vector2i(-1, -1);
	Crystal();
	Crystal(Colour colour);
	Crystal(sf::Vector2f pos);
	void update();
	void draw(sf::RenderWindow &win);
	sf::Vector2f &getPos();
	Colour &getCol() { return type; };
	void setCol(Colour c) { type = c; }
	void updateTextures();
	bool checked=false;
	bool toRemove=false;
	bool toSwap=false;
	bool removeAnim = false;

	~Crystal();
};
