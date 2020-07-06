#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <functional>
#include <iostream>

// tetromino block size in pixels on the texture
const int block_size = 18;

//play area size in tetromino blocks
const int M = 20;
const int N = 10;

// play area shift in pixels (moves it around in the window, so it can be matched with the backgorund)
const int xshift = 28; // horizontal shift
const int yshift = 31; // vertical shift

// the colors of the tetromni blcks 
// the index of the given color corresponds to the position of the colored block in the texture
enum color
{
	blue,
	purple,
	red,
	green,
	yellow,
	cyan,
	orange

};

//play area state is stored in the field variable
// entry values are corresponding to the tetromino colors
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
	{0,0,0,0,0,0,0,0,0,0},
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

//collision detection results
enum colres
{
	not_collide,
	collide
	
};


// collision detection
// we take the array which holds the propsed tetromino coordiantes and check if the positions overlap any boundaries:
//return colres type corresponding to whether or not we had collision
colres coldet(const std::array<Point, 4> & a, const std::array < std::array<int, N>, M> & field)
{
	for (int i = 0; i < 4; i++) 
	{
		//check the boundaries 
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
		{
			return colres::collide;
		}
		
		// check the field
		else if (field[a[i].y][a[i].x] != 8) // it is an overlap when the proposed postion is not empty (number 8)
		{
			return colres::collide;
		}

	}
	
	return colres::not_collide;
}

// line check
//returns true if the line is full of tetrmino pieces, means no colorcode in that line is 8
bool linecheck(std::array<int, N> line)
{
	for (int i = 0; i < N; i++)
	{
		if (line[i] == 8)
		{
			return false;
		}
	}

	return true;
}

//delete line
//deletes one line from the field and shifts it down
void deleteline(int n, std::array < std::array<int, N>, M>& field)
{
	//Shift the lines down into the empty space
	for (int i = n; i > 0; i--)
	{
		field[i] = field[i - 1];
	}

	// fill the new upmost line with empty space
	for (int j = 0; j < N; j++)
	{
		field[0][j] = 8;
	}
}


int main()
{	
	//random number generator 
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0, 6);
	auto randgen = std::bind(distribution, generator);
	

	// spawn the very first tetromino
	int n = randgen(); //choose a tetromino from the list of 7 in figures
	int colornum = n; // the number corresponding to the color of the tetromino
	for (int i = 0; i < 4; i++)
	{
		a[i].x = figures[n][i] % 2;
		a[i].y = figures[n][i] / 2;
	}

	// prepare the next tetromino
	int n_next = randgen();
	int colornum_next = n_next;

	

	//opena a render window, only close, no resize
	sf::RenderWindow window(sf::VideoMode(320, 480),"Tetris" ,sf::Style::Titlebar | sf::Style::Close);

	// loading texture for tetrominos the file is in the main directory
	sf::Texture t;
	t.loadFromFile("../tiles.png");

	// create drawable sprite from the texture
	sf::Sprite s(t);

	//load texture for the backgorund
	sf::Texture bg;
	bg.loadFromFile("../background.png");

	//create backgorund sprite
	sf::Sprite bgs(bg);

	//load texture for the forground frame
	sf::Texture fg;
	fg.loadFromFile("../frame.png");

	//create foreground sprite
	sf::Sprite fgs(fg);


	

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

	int D = 5; // sets the falling speed (1 step fall per D update cycle)

	

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
			std::cout << n_next;

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
				//temporarily store the old values
				ta = a;
				
				//calculate the new values
				for (int i = 0; i < 4; i++)
				{				
					
					a[i].x = a[i].x + dx;
				}
				//Reset flags
				dx = 0;

				//collision detection 		
				if (coldet(a,field) == colres::collide)
				{
					// if collide set it back to the original values
					a = ta;
					
				}

			}

			//Rotate
			else if (rotate == true && n != 6) //square is never rotated
			{
				Point p = a[1]; //center of rotation
								
				//first store old values
				ta = a;

				//calculate the new ones
				for (int i = 0; i < 4; i++)
				{
									
					a[i].x = p.x + p.y - ta[i].y;
					a[i].y = p.y - p.x + ta[i].x;

				}
				//Reste flags
				rotate = false;

				//collision detection 		
				if (coldet(a,field) == colres::collide)
				{
					// if collide set it back to the original values
					a = ta;
				}


			}

			// Fall
			if (fall == true)
			{
				//temporarily store the old values
				ta = a;
				for (int i = 0; i < 4; i++)
				{
					//fall
					a[i].y += 1;
				}
				//Reset flags
				fall = false;

				//collision detection 		
				if (coldet(a,field) == colres::collide)
				{
					// if collide set it back to the original values
					a = ta;
					
					// if we are there than the piece is collided while it fall this means it has to be locked
					// lock tetromino into field 

					for (int i = 0; i < 4; i++)
					{
						field[a[i].y][a[i].x] = colornum;
					}
					
					
					// spawn the next tetromino and generate the new next
					// one random number generates the form color combination (they are locked)
					n = n_next;
					colornum = n;
					//creating starting coordinates
					for (int i = 0; i < 4; i++)
					{
						a[i].x = figures[n][i] % 2 + 5;
						a[i].y = figures[n][i] / 2;
					}
					//genertate the neww one
					n_next = randgen();
					colornum_next = n_next;
				
				
				}	
					
			

			//check line completion bottom to top
				
				for (int i = M-1; i >= 0; i--)
				{
					if (linecheck(field[i]))
					{
						// delete the compelete line
						deleteline(i, field);
						// increase the counter to check the new line again because the numbers shift down
						i++;
					}

				}

			}




			//clear the window
			window.clear(sf::Color::White);

			//draw game elements
			//draw backgorund 
			window.draw(bgs);

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
					s.setTextureRect(sf::IntRect(field[i][j] * block_size, 0, block_size, block_size));
					s.setPosition(j * block_size + xshift, i * block_size + yshift);
					window.draw(s);
				}
			}

			//draw a tetromino
			// set the texture rect for the correct color
			s.setTextureRect(sf::IntRect( colornum * block_size,0, block_size, block_size));

			// draw tetromino pieces
			for (int i = 0; i < 4; i++)
			{
				// set the texture position
				s.setPosition(a[i].x * block_size + xshift , a[i].y * block_size + yshift );
				window.draw(s);
			}


			//draw foreground
			window.draw(fgs);

			// render everything drawn so far 
			window.display();
			

			timer = 0;

		}

		
	}

	return 0;
}
