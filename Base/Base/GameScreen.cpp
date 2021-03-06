#include "GameScreen.h"
///////////////////////////////
// resets all the tiles to not be checked
///////////////////////////////
void GameScreen::resetChecked()
{
	for (size_t i = 0; i < gridCols; i++)
	{
		for (size_t u = 0; u < gridRows; u++)
		{
			grid.at(i).at(u).checked = false;
		}
	}
}
////////////////////////////
//marks the checked tiles to play an animation to remove
////////////////////////////
void GameScreen::MarkChecked()
{
	for (size_t i = 0; i < gridCols; i++)
	{
		for (size_t u = 0; u < gridRows; u++)
		{
			if (grid.at(i).at(u).checked == true)
			{
				//grid.at(i).at(u).toRemove = true;
				grid.at(i).at(u).setRemoveAnim();
			}
		}
	}
}


//////////////////////////////////
//default constructor
//////////////////////////////////
GameScreen::GameScreen()
{
	gridCols = 10;
	gridRows = 10;
	std::vector<Crystal> temp;
	sf::Vector2f tempPos(40, 40);
	cursorPosition = sf::Vector2i(1, 1);
	cursor.setScale(0.5, 0.5);
	if (!cursorTex.loadFromFile("Assets/Sprites/selector.png"))
	{

	}
	cursor.setTexture(cursorTex);
	if (!crystalSheet.loadFromFile("Assets/Sprites/spriteSheet.png"))
	{

	}
	for (int i = 0; i < gridCols; i++)
	{
		temp.clear();
		for (int j = 0; j < gridRows; j++)
		{
			temp.push_back(Crystal(tempPos));
			tempPos += sf::Vector2f(0, 50);
			//grid[i][j] = new Crystal();
		}
		grid.push_back(temp);
		prevGrid.push_back(temp);
		tempPos += sf::Vector2f(50, -gridRows * 50);
	}

	p1 = Player();
	//hud = HudManager();
}
/////////////////////////////////////////////
// standard constructor
/////////////////////////////////////////////
GameScreen::GameScreen(int gridHeight=10, int gridWidth=10) : gridCols(gridHeight), gridRows(gridWidth)
{
	std::vector<Crystal> temp;
	sf::Vector2f tempPos(40,40);
	cursorPosition = sf::Vector2i(1, 1);
	cursor.setScale(0.5, 0.5);
	cursor.setOrigin(50, 50);
	if (!cursorTex.loadFromFile("Assets/Sprites/selector.png"))
	{

	}
	cursor.setTexture(cursorTex);
	if (!crystalSheet.loadFromFile("Assets/Sprites/spriteSheet.png"))
	{

	}
	crystals.setTexture(crystalSheet);
	crystals.setScale(0.5, 0.5);
	for (int i = 0; i < gridCols; i++)
	{
		temp.clear();
		for (int j = 0; j < gridRows; j++)
		{
			temp.push_back(Crystal(tempPos));
			tempPos += sf::Vector2f(0, 50);
			//grid[i][j] = new Crystal();
		}
		grid.push_back(temp);
		prevGrid.push_back(temp);
		tempPos += sf::Vector2f(50, -gridRows*50);
	}
	crystalSheet.setSmooth(true);
	
}
///////////////////////////////////////////
//updates the aspects of this class, returns a message if something happens
///////////////////////////////////////////
std::string GameScreen::update(sf::RenderWindow & window)
{
	std::string message=p1.update(clock.restart());
	
	resetNull();
	if (message == "gameOver")
	{
		return message;
	}
	if (keydown == false) {
		//Movement keys change function depending on mode.
		//In Move Mode, arrow keys move the cursor around the board.
		//In Swap Move, arrow keys selects a tile to switch with.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{//Move/Swap Left
			if (cursorPosition.x > 0) {
				if (swapMode == true)
				{
					updatePrevious();
					SwapTile(sf::Vector2i(-1,0));
				}
				else
				cursorPosition += sf::Vector2i(-1, 0);
			}
			keydown = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{//Move/Swap Right
			if (cursorPosition.x < gridRows - 1) {
				if (swapMode == true)
				{
					updatePrevious();
					SwapTile(sf::Vector2i(1,0));
				}
				else
				cursorPosition += sf::Vector2i(1, 0);
			}
			keydown = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{//Move/Swap Down
			if (cursorPosition.y < gridCols - 1)
			{
				if (swapMode == true)
				{
					updatePrevious();
					SwapTile(sf::Vector2i(0, 1));
				}
				else
				cursorPosition += sf::Vector2i(0, 1);
			}
			keydown = true;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{//Move/Swap Up			
			if (cursorPosition.y > 0)
			{
				if (swapMode == true)
				{
					updatePrevious();
					SwapTile(sf::Vector2i(0, -1));
				}
				else
				cursorPosition += sf::Vector2i(0, -1);
			}
			keydown = true;

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{//Move/Swap Up			
			resetChecked();
			keydown = true;

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{//When Space is pressed, changes modes to alter directional key function, and marks the currently selected tile as ready to be swapped.
			swapMode = true;
			tileToSwap = cursorPosition;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			return "mainMenu";
		}
		//Added temporarily for the purpose of testing Ability Points.
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) { 
				p1.fever(true); 
			}
			else { 
				p1.fever(false); 
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) { 
				p1.changeAP("Green", -300); 
			}
			else { 
				p1.changeAP("Green", -100); 
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			bool confirmReverse = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
				confirmReverse = p1.reverse(true);
			}
			else {
				confirmReverse = p1.reverse(false);
			}
			if (confirmReverse) {
				reverse();
			}
		}

	}
	else
	{
		keydown = false;
	}
	cursor.setPosition(grid[cursorPosition.x][cursorPosition.y].getPos());
	for (int i = 0; i < gridCols; i++)
	{
		for (int o = 0; o < gridRows; o++)
		{
			if (grid[i][o].animTime <= 0)
			{
				if (grid.at(i).at(o).getCol() == Colour::null&&o>0)
				{
					tileToSwap = sf::Vector2i(i, o);
					SwapTile(sf::Vector2i(0, -1));
					grid.at(i).at(o).updateTextures();
				}
				if (grid[i][o].toSwap == true)
				{
					//swapFinished(tileToSwap,tileToSwap2);
					swapFinished(sf::Vector2i(i, o), grid.at(i).at(o).toSwapWith);
				}

				if (grid[i][o].toRemove == true)
				{
					removeFinished(sf::Vector2i(i,o));
				}
			}
		}
	}
	return "";
}
//////////////////////////////
// the draw method of this class
//////////////////////////////
void GameScreen::draw(sf::RenderWindow & window)
{
	crystals.setTexture(crystalSheet);
	//Iterates through each tile...
	for (int i = 0; i < gridCols; i++)
	{
		for (int j = 0; j < gridRows; j++)
		{//...and calls its Draw method.

			grid[i][j].update();
			
			crystals.setTextureRect(sf::Rect<int>(100 * static_cast<int>(grid[i][j].getCol()), 0,100,100));
			crystals.setPosition(grid[i][j].getPos());
			crystals.setScale(grid[i][j].animScale);
			crystals.setOrigin(50, 50);
			window.draw(crystals);
			//grid[i][j].draw(window);
		}
	}
	//
	cursor.setTexture(cursorTex);
	window.draw(cursor);

	hud.Draw(window, p1);
}
///////////////////////////////
//returns the score from the player container
///////////////////////////////
int GameScreen::getScoreFromPlayer()
{
	return p1.getScore();
}
//////////////////////////////////////
//reverts the current grid to a previous grid
//////////////////////////////////////
void GameScreen::reverse() {
	//For each tile in grid, revert to previous state.
	for (int i = 0; i < gridCols; i++)
	{
		for (int j = 0; j < gridRows; j++)
		{
			grid.at(i).at(j).setCol(prevGrid.at(i).at(j).getCol());
			grid.at(i).at(j).updateTextures();
		}
	}
}

////////////////////////////
//a non recursive method to mark all the matching tiles around the tile to be checked
////////////////////////////
std::pair<Colour, int> GameScreen::CheckMatch(sf::Vector2i check)
{
	
	int count = 0;
	int totalCount=0;
	//will increment when a match can't be found
	int dirChecked = 1;//1=left, 2=right, 3=up, 4=down, 5=fin
	//will check other directions until either a full match is found or all directions are checked
	sf::Vector2i currentTile=check; //will be iterated upon until no more matches and then set to check again

	//int xCount=0,yCount=0, dirChecked=0;
	//sf::Vector2i startTile=tileToSwap, currentTile=startTile, prevTile=startTile;
	//
	if (grid[check.x][check.y].getCol() != Colour::null)
	{
		while (dirChecked < 5)
		{
			assert(currentTile.x >= 0 && currentTile.x < grid.size());
			while (dirChecked == 1)//left
			{
				//checks if the next tile to be iterated to is out of bounds
				if (currentTile.x - 1 < 0)
				{
					std::cout << "out of bounds" << std::endl;
					dirChecked++;
				}
				else if ((grid.at(currentTile.x).at(currentTile.y).getCol() == grid.at(currentTile.x - 1).at(currentTile.y).getCol()))
				{//checks to the left of the current tile, and if true, moves the current tile in that direction
					currentTile.x -= 1;
					grid.at(currentTile.x).at(currentTile.y).checked = true;
					count++;
					std::cout << "Left" << std::endl;
				}
				else if ((grid.at(currentTile.x).at(currentTile.y).getCol() != grid.at(currentTile.x - 1).at(currentTile.y).getCol()))
				{//if the two tiles don't match, set the currently selected tile to the origin and iterate direction
					dirChecked++;
					currentTile = check;
				}
				else
				{//if all else fails
					std::cout << "something went wrong" << std::endl;
					dirChecked++;
				}
			}
			while (dirChecked == 2)//right
			{
				//checks if the next tile to be iterated to is out of bounds
				if (currentTile.x + 1 >= gridCols)
				{
					std::cout << "out of bounds" << std::endl;
					dirChecked++;
				}
				else if ((grid.at(currentTile.x).at(currentTile.y).getCol() == grid.at(currentTile.x + 1).at(currentTile.y).getCol()))
				{
					currentTile.x += 1;
					grid.at(currentTile.x).at(currentTile.y).checked = true;
					count++;
					std::cout << "Right" << std::endl;
				}
				else
				{
					dirChecked++;
					currentTile = check;
				}
			}
			if (count >= 3)//ensures you don't have a multi shaped match
			{
				//dirChecked = 5;//if you have 4 or more in a row, you stop checking
				totalCount += count;
				count = 0;//to not mess with the next set of checks
				grid.at(check.x).at(check.y).checked = true;

				currentTile = check;
				MarkChecked();
			}
			else
			{
				std::cout << "no X match" << std::endl;
				count = 0;//otherwise, you reset your count and try up/down
				currentTile = check;
				resetChecked();
			}
			while (dirChecked == 3)
			{
				//checks if the next tile to be iterated to is out of bounds
				if (currentTile.y - 1 < 0)
				{
					std::cout << "out of bounds" << std::endl;
					dirChecked++;
				}
				else if ((grid.at(currentTile.x).at(currentTile.y).getCol() == grid.at(currentTile.x).at(currentTile.y - 1).getCol()))
				{
					currentTile.y -= 1;
					grid.at(currentTile.x).at(currentTile.y).checked = true;
					count++;
					std::cout << "Up" << std::endl;
				}
				else
				{
					dirChecked++;
					currentTile = check;
				}
			}
			while (dirChecked == 4)
			{
				//checks if the next tile to be iterated to is out of bounds
				if (currentTile.y + 1 >= gridRows)
				{
					std::cout << "out of bounds" << std::endl;
					dirChecked++;
				}
				else if ((grid.at(currentTile.x).at(currentTile.y).getCol() == grid.at(currentTile.x).at(currentTile.y + 1).getCol()))
				{
					currentTile.y += 1;
					grid.at(currentTile.x).at(currentTile.y).checked = true;
					count++;
					std::cout << "Down" << std::endl;
				}
				else
				{
					dirChecked++;
					currentTile = check;
				}
			}
		}
	}
	if (count < 3)
	{
		std::cout << "no Y match" << std::endl;
	}
	else if (count >=3)
	{
		totalCount += count;
		grid.at(check.x).at(check.y).checked = true;
		MarkChecked();
		
	}
	std::pair<Colour, int> toReturn = std::pair<Colour, int>(grid.at(check.x).at(check.y).getCol(), totalCount);
	resetChecked();
	return toReturn;
}
///////////////////////////
//removes tiles once their animation is finished
///////////////////////////
void GameScreen::removeFinished(sf::Vector2i pos)
{
	grid.at(pos.x).at(pos.y).setCol(Colour::null);
	grid.at(pos.x).at(pos.y).toRemove = false;
}
//complete
//////////////////////////
//swaps tiles
/////////////////////////
void GameScreen::SwapTile(sf::Vector2i dir)
{
	//Simple swap action, creates a temp variable and uses it to swap colour values.

	tileToSwap2 = tileToSwap + dir;
	//auto col = grid.at(tileToSwap.x + dir.x).at(tileToSwap.y + dir.y).getCol(); //Setting up the temp...	
	//
	//grid.at(tileToSwap.x+dir.x).at(tileToSwap.y+dir.y).setCol(grid.at(tileToSwap.x).at(tileToSwap.y).getCol()); //Copies one tile's colour over the other's.
	sf::Vector2i animDir;
	if (dir.x != 0)
	{
		animDir.x = 1;
	}
	else if (dir.y != 0)
	{
		animDir.y = 1;
	}
	//grid.at(tileToSwap.x).at(tileToSwap.y).setCol(col); //Copies temp's colour over the first tile.
	grid.at(tileToSwap.x).at(tileToSwap.y).setSwapAnim(animDir);
	grid.at(tileToSwap.x).at(tileToSwap.y).toSwapWith=tileToSwap2;
	grid.at(tileToSwap.x).at(tileToSwap.y).alreadySwapped = false;
	grid.at(tileToSwap2.x).at(tileToSwap2.y).setSwapAnim(animDir);
	grid.at(tileToSwap2.x).at(tileToSwap2.y).toSwapWith = tileToSwap;
	grid.at(tileToSwap2.x).at(tileToSwap2.y).alreadySwapped = false;
	//Now, to redraw the textures of both.

	//grid.at(tileToSwap.x + dir.x).at(tileToSwap.y + dir.y).updateTextures();

	//grid.at(tileToSwap.x).at(tileToSwap.y).updateTextures();

	//Resets control mode back to Move Mode
	swapMode = false;

}
//////////////////////////////
//swaps tiles when their animation is finished
/////////////////////////////
void GameScreen::swapFinished(sf::Vector2i pos1, sf::Vector2i pos2)
{
	if (grid.at(pos1.x).at(pos1.y).alreadySwapped == false)
	{
		auto col = grid.at(pos2.x).at(pos2.y).getCol(); //Setting up the temp...	

		grid.at(pos2.x).at(pos2.y).setCol(grid.at(pos1.x).at(pos1.y).getCol()); //Copies one tile's colour over the other's.

		grid.at(pos1.x).at(pos1.y).setCol(col); //Copies temp's colour over the first tile.
		std::pair<Colour, int> temp = CheckMatch(tileToSwap);
		p1.changeScore(temp.second, temp.first);
		temp = CheckMatch(tileToSwap2);
		p1.changeScore(temp.second, temp.first);
		grid.at(pos2.x).at(pos2.y).alreadySwapped = true;
		grid.at(pos1.x).at(pos1.y).alreadySwapped = true;
		grid.at(pos2.x).at(pos2.y).updateTextures();
		grid.at(pos1.x).at(pos1.y).updateTextures();
	}

}


/////////////////////////////////
//depreciated method that swaps tiles without checking for a match
////////////////////////////////
void GameScreen::SwapTileWithoutCheck(sf::Vector2i dir, sf::Vector2i pos)
{
	//Simple swap action, creates a temp variable and uses it to swap colour values.
	auto col = grid.at(pos.x + dir.x).at(pos.y + dir.y).getCol(); //Setting up the temp...

	grid.at(pos.x + dir.x).at(pos.y + dir.y).setCol(grid.at(pos.x).at(pos.y).getCol()); //Copies one tile's colour over the other's.

	grid.at(pos.x).at(pos.y).setCol(col); //Copies temp's colour over the first tile.

														//Now, to redraw the textures of both.

	grid.at(pos.x + dir.x).at(pos.y + dir.y).updateTextures();

	grid.at(pos.x).at(pos.y).updateTextures();

}
//////////////////////
//updates the previous grid from the current grid
//////////////////////
void GameScreen::updatePrevious() {
	p1.updatePrevious();

	for (int i = 0; i < gridCols; i++)
	{
		for (int j = 0; j < gridRows; j++)
		{
			prevGrid.at(i).at(j).setCol(grid.at(i).at(j).getCol());
		}
	}
}
////////////////////////
//reset the null tiles to a random tile
////////////////////////
void GameScreen::resetNull() 
{//Resets the top row of tiles to random colours.
	//Issue: Top row doesn't draw correctly for some reason.
	for (int i = 0; i < gridCols; i++)
	{
		if (grid.at(i).at(0).getCol() == Colour::null)
		{
			Colour col = static_cast<Colour>(rand() % level); //Will change this to include later colours once added.
			grid.at(i).at(0).setCol(col);
			grid.at(i).at(0).updateTextures();
		}
	}
}
/////////////////////////
//compares two tiles to see if their colours match
/////////////////////////
bool GameScreen::compareTiles(Crystal & a, Crystal & b)
{//Checks if "type" value of both Crystals are identical.
	if (a.getCol() == b.getCol())
	{
		return true;
	}
	return false;
}
/////////////////////////
//destructor
////////////////////////
GameScreen::~GameScreen()
{
	grid.clear();
	prevGrid.clear();
}
