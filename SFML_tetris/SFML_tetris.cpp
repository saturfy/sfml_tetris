#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

//play area size
const int M = 20;
const int N = 10;

//play area state hold in the field variable
std::array < std::array<int, N>, M> field;

// define the tetrominos
std::array< std::array<int, 4>, 7> figures =
{ {
	{1,3,5,7}, //I
	{2,4,5,7}, //Z
	{3,5,4,6}, //S
	{3,5,4,7}, //T
	{2,3,5,7}, //L
	{3,5,7,6}, //J
	{2,3,4,5}, //O

} };

//coordinates for drawing the tetrominos
struct Point 
{
	int x;
	int y;
};

// point a will contain the tetromino parts
std::array<Point, 4> a;
//choose a tetromino from the list of 7 in figures


int main()
{	
	
	int n = 3;
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

	

	//opena a render window, only close, no resize
	sf::RenderWindow window(sf::VideoMode(320, 480),"Tetris" ,sf::Style::Titlebar | sf::Style::Close);

	// loading texture for tetrominos the file is in the main directory
	sf::Texture t;
	//t.loadFromFile("P:\prog\cpp_tutorials\PROJECTS\SFML projects\SFML_tetris");
	t.loadFromFile("../tiles.png");

	// create drawable sprite from the texture
	sf::Sprite s(t);
	s.setTextureRect(sf::IntRect(0, 0, 18, 18));

	//variables for moving the tetromino
	int dx = 0;
	bool rotate = 0;
	int colorNum = 1;

	float timer = 0;
	float delay = 0.3;

	sf::Clock clock;

	while (window.isOpen())
	{
		// timing the game loop
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		//checking to close the window
		sf::Event e;
		while (window.pollEvent(e))
		{
			//Window closes
			if (e.type == sf::Event::Closed)
				window.close();

			//User input
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Space) rotate = true;
				else if (e.key.code == sf::Keyboard::Left) dx = -1;
				else if (e.key.code == sf::Keyboard::Right) dx = 1;
			}
		}
		

		//Tick
		if (timer > delay)
		{
			//fall
			for (int i = 0; i < 4; i++)
			{
				a[i].y += 1;
				
			}

			//MOVE
			for (int i = 0; i < 4; i++)
			{
				a[i].x += dx;
				
			}
			dx = 0;

			

			//Rotate
			if (rotate)
			{
				Point p = a[1]; //center of rotation
				for (int i = 0; i < 4; i++)
				{
					//first store old values
					int x = a[i].x;
					int y = a[i].y;
					a[i].x = p.x + p.y - y;
					a[i].y = p.y - p.x + x;
					
				}
				rotate = false;
			}

			//clear the window
			window.clear(sf::Color::White);

			//draw game elements

			//draw a tetromino
			for (int i = 0; i < 4; i++)
			{
				s.setPosition(a[i].x * 18, a[i].y * 18);
				window.draw(s);
			}


			// render everything drawn so far 
			window.display();

			timer = 0;

		}

		
	}

	return 0;
}
