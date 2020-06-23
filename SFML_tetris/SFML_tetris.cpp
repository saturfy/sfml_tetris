#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <functional>
#include <iostream>

//play area size
const int M = 20;
const int N = 10;

//play area state hold in the field variable
// field values:
//8 : empty
//0-7 : colored tile
std::array < std::array<int, N>, M> field = 
{ {
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,1,1,8,8,1,1,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
	{8,8,8,8,8,8,8,8,8,8},
} };

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
std::array<Point, 4> a, ta; 




// collision detection
// we take the array which holds the propsed tetromino coordiantes and check if the positions overlap any boundaries:
//return 0-no collison, 1: collision in the x direction 2: collision in the y direction
int coldet(std::array<Point, 4> a, std::array < std::array<int, N>, M> field)
{
	for (int i = 0; i < 4; i++) 
	{
		//check the boundaries 
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
		{
			return 1;
		}
		
		// check the field
		else if (field[a[i].y][a[i].x] != 8) // it is an overlap when the proposed postion is not empty (number 8)
		{
			return 1;
		}

	}
	
	return 0;
}


int main()
{	
	//random number generator 
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0, 7);
	auto randgen = std::bind(distribution, generator);
	

	// spawn tetromino
	int n = 2; //choose a tetromino from the list of 7 in figures
	int colornum = 2; // the number corresponding to the color of the tetromino
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
	

	//variables for moving the tetromino
	int dx = 0;

	float timer = 0;
	float delay = 0.1;

	sf::Clock clock;
	int counter = 0;
	// flags for game logic
	bool fall=true; // true if the tetromino has to move down 1 step in this cycle
	bool rotate = false; // true if the user pressed the rotate button and tetromina has to be rotated in this cycle
	bool lock = false; // true if the tetromni touched something below it. It stops moving and "locked" 

	int D = 10; // sets the falling speed (1 step fall per D update cycle)

	

	while (window.isOpen())
	{
		// timing the game loop
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;


		//checking the user input
		sf::Event e;
		while (window.pollEvent(e))
		{
			//Window closes
			if (e.type == sf::Event::Closed)
				window.close();

			//User input
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Up) rotate = true;
				else if (e.key.code == sf::Keyboard::Left) dx = -1;
				else if (e.key.code == sf::Keyboard::Right) dx = 1;
			}
		}


		//Movement and game logic is updated after the delay time 
		if (timer > delay)
		{

			// difficulty: this sets how many times the delay time passes before the tetromino falls 1 step
			counter++;
			if (counter == D)
			{
				fall = true;
				counter = 0;
			}


			//Movement always happens in order
			// First we take user input (move OR rotate) and check if its possible
			// After user input we make the tetromino fall if fall flag is true

			//sideways movement 
			if (dx != 0)
			{
				for (int i = 0; i < 4; i++)
				{
					//temporarily store the old values
					ta[i].x = a[i].x;
					ta[i].y = a[i].y;
					//calculate the new values
					a[i].x = a[i].x + dx;
				}
				//Reset flags
				dx = 0;

				//collision detection 		
				if (coldet(a,field) != 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// if collide set it back to the original values
						a[i].x = ta[i].x;
						a[i].y = ta[i].y;
					}

				}

			}

			//Rotate
			else if (rotate == true)
			{
				Point p = a[1]; //center of rotation
				for (int i = 0; i < 4; i++)
				{
					//first store old values
					ta[i].x = a[i].x;
					ta[i].y = a[i].y;
					//calculate the new ones
					a[i].x = p.x + p.y - ta[i].y;
					a[i].y = p.y - p.x + ta[i].x;

				}
				//Reste flags
				rotate = false;

				//collision detection 		
				if (coldet(a,field) != 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// if collide set it back to the original values
						a[i].x = ta[i].x;
						a[i].y = ta[i].y;
					}

				}


			}

			// Fall
			if (fall == true)
			{
				for (int i = 0; i < 4; i++)
				{
					//temporarily store the old values
					ta[i].x = a[i].x;
					ta[i].y = a[i].y;
					//fall
					a[i].y += 1;
				}
				//Reset flags
				fall = false;

				//collision detection 		
				if (coldet(a,field) != 0)
				{
					for (int i = 0; i < 4; i++)
					{
						// if collide set it back to the original values
						a[i].x = ta[i].x;
						a[i].y = ta[i].y;
					}
					// if we are there than the piece is collided while it fall this means it has to be locked
					// lock tetromino into field 

					for (int i = 0; i < 4; i++)
					{
						field[a[i].y][a[i].x] = colornum;
					}
					
					
					// spawn new tetromino
					// one random number generates the form color combination (they are locked)
					n = randgen();
					colornum = n;
					//creating starting coordinates
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[n][i] % 2 + 5;
						a[i].y = figures[n][i] / 2;
					}
				}
			}




			//clear the window
			window.clear(sf::Color::White);

			//draw game elements

			// draw field
			for (int i = 0; i < M; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (field[i][j] == 8)
					{
						continue;
					}
					
					// set the texture rect for the correct color
					s.setTextureRect(sf::IntRect(field[i][j] * 18, 0, 18, 18));
					s.setPosition(j * 18, i * 18);
					window.draw(s);
				}
			}

			//draw a tetromino
			// set the texture rect for the correct color
			s.setTextureRect(sf::IntRect( colornum * 18,0, 18, 18));

			// draw tetromino pieces
			for (int i = 0; i < 4; i++)
			{
				// set the texture position
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
