#include <SFML/Graphics.hpp>
#include <array>
#include <random>
#include <functional>
#include <iostream>

// tetromino block size in pixels on the texture
const int BLOCKSIZE = 18;

//play area size in tetromino blocks
const int M = 20;
const int N = 10;

// play area shift in pixels (moves it around in the window, so it can be matched with the backgorund)
const int XSHIFT = 28; // horizontal shift
const int YSHIFT = 31; // vertical shift

// the colors of the tetromni blcks 
// the index of the given color corresponds to the position of the colored block in the texture
enum Color
{
	Blue,
	Purple,
	Red,
	Green,
	Yellow,
	Cyan,
	Orange

};


//coordinates for drawing the tetrominos
struct Point
{
	int x;
	int y;
};

//collision detection results
enum class ColRes
{
	NotCollide,
	Collide
};



//play area state is stored in the field variable
// entry values are corresponding to the tetromino colors
//8 : empty
//0-7 : colored tile
using field_vector = std::array < std::array<int, N>, M>;

// the coordinates of the tetromino are stored in a vector of Points
using tetr_vec = std::array<Point, 4>;


// collision detection
// we take the array which holds the propsed tetromino coordiantes and check if the positions overlap any boundaries:
//return colres type corresponding to whether or not we had collision
ColRes coldet(const std::array<Point, 4> & a, const std::array < std::array<int, N>, M> & field)
{
	for (int i = 0; i < 4; i++)
	{
		//check the boundaries 
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
		{
			return ColRes::Collide;
		}

		// check the field
		else if (field[a[i].y][a[i].x] != 8) // it is an overlap when the proposed postion is not empty (number 8)
		{
			return ColRes::Collide;
		}

	}

	return ColRes::NotCollide;
}

class Tetromino
{
public:

	//tetromino type
	int tetromino_type;

	// store the next tetrmoino type
	int tetromino_type_next;

	//tetromino color
	Color tetromino_color;
	
	//2D point a will contain the tetromino parts, ta is temporary coordinates
	tetr_vec a, ta;

	// shifts the x coordinate of the tetromino when first calculated
	int starting_shift = 5;

	//-----------Public functions----------------------------

	//fills vector "a" with the tetromino starting coordinates corresponding to the tetromino type
	void calcCoordinates() 
	{
		for (int i = 0; i < a.size(); ++i)
		{
			a[i].x = figures[tetromino_type][i] % 2 + starting_shift;// shift is added to place it into the middle of the play area
			a[i].y = figures[tetromino_type][i] / 2;

		}
	}

	//moves the tetromino sideways
	// this function takes the moving direction and the  playing field as input to calcualte collision
	void moveSideways(int & dx,field_vector & field)
	{
		//temporarily store the old values
		ta = a;

		//calculate the new values
		for (int i = 0; i < a.size(); ++i)
		{
			a[i].x = a[i].x + dx;
		}

		//collision detection 		
		if (coldet(a, field) == ColRes::Collide)
		{
			// if collide set it back to the original values
			a = ta;

		}

	}

	// rotate tetromino
	// this function takes the playing field as input to calcualte collision
	void rotate(field_vector & field)
	{
		if (tetromino_type != 6) // the square is not rotated at all
		{
			Point p = a[1]; //center of rotation

			ta = a; //first store old values

			//calculate the new ones
			for (int i = 0; i < a.size(); ++i)
			{

				a[i].x = p.x + p.y - ta[i].y;
				a[i].y = p.y - p.x + ta[i].x;

			}
			
			//collision detection 		
			if (coldet(a, field) == ColRes::Collide)
			{
				// if collide set it back to the original values
				a = ta;

			}
		}
		
	}

	// tetromino fall
	// the function takes the playing field as input to calculate the collision detection
	// this fucntion returns whether or not the falling tetromino collided, because if the piece collides while falling it has to be detected in order to lock it in place
	ColRes fall(field_vector & field)
	{
		ta = a; //temporarily store the old values
		
		// calculate the new coordiantes
		for (int i = 0; i < a.size(); ++i)
		{
			a[i].y += 1;
		}
		

		// collision detection
		if (coldet(a, field) == ColRes::Collide)
		{
			// if collide set it back to the original values
			a = ta;
			return ColRes::Collide;
		}
		
		return ColRes::NotCollide;
	}

	//---------------------------------------------------------------

private:

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

};


class PlayingField 
{
public:
	field_vector field =
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
		{8,8,8,8,8,8,8,8,8,8},
	} };


	//--------------------------Public functions-----------------------
	// this function takes the tetromino coordinate vector and tetromino color and copies them into the field variable to lock the into the playing field
	void lockTetromino(tetr_vec & a, Color color)
	{
		for (int i = 0; i < a.size(); ++i)
		{
			field[a[i].y][a[i].x] = color;
		}
	}

	//-----------------------------------------------------------------
private:
};

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

	//create tetromino object
	Tetromino ttrno;


	// spawn the very first tetromino
	ttrno.tetromino_type = randgen(); //choose a tetromino from the list of 7 in figures
	ttrno.tetromino_color = static_cast<Color>(ttrno.tetromino_type); // the number corresponding to the color of the tetromino
	ttrno.tetromino_type_next = randgen(); // choose the next tetromino
	// if we have the tetromino type we calculate the coordinates
	ttrno.calcCoordinates();

	// create playing field object
	PlayingField plf;

	

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
			
			// difficulty: this sets how many times the delay time passes before the tetromino falls 1 step
			++counter;
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
				ttrno.moveSideways(dx,plf.field);
				
				//Reset flags
				dx = 0;
								
			}

			//Rotate
			else if (rotate == true ) 
			{
				ttrno.rotate(plf.field);

				//Reste flags
				rotate = false;

			}

			// Fall
			if (fall == true)
			{
				ColRes is_collide = ttrno.fall(plf.field);
				//Reset flags
				fall = false;

				// if we are here than the piece is collided while it fall this means it has to be locked
				// lock tetromino into field 	
				if (is_collide == ColRes::Collide)
				{		
					
					plf.lockTetromino(ttrno.a, ttrno.tetromino_color);
			
					
					// spawn the next tetromino and generate the new next
					// one random number generates the form color combination (they are locked)
					ttrno.tetromino_type = ttrno.tetromino_type_next;
					ttrno.tetromino_color = static_cast<Color>(ttrno.tetromino_type);
					// generate new next tetromino
					ttrno.tetromino_type_next = randgen();
					//creating starting coordinates
					ttrno.calcCoordinates();
					
					//check line completion bottom to top

					for (int i = M - 1; i >= 0; i--)
					{
						if (linecheck(plf.field[i]))
						{
							// delete the compelete line
							deleteline(i, plf.field);
							// increase the counter to check the new line again because the numbers shift down
							i++;
						}

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
					if (plf.field[i][j] == 8)
					{
						continue;
					}
					
					// set the texture rect for the correct color
					s.setTextureRect(sf::IntRect(plf.field[i][j] * BLOCKSIZE, 0, BLOCKSIZE, BLOCKSIZE));
					s.setPosition(j * BLOCKSIZE + XSHIFT, i * BLOCKSIZE + YSHIFT);
					window.draw(s);
				}
			}

			//draw a tetromino
			// set the texture rect for the correct color
			s.setTextureRect(sf::IntRect( ttrno.tetromino_color * BLOCKSIZE,0, BLOCKSIZE, BLOCKSIZE));

			// draw tetromino pieces
			for (int i = 0; i < 4; i++)
			{
				// set the texture position
				s.setPosition(ttrno.a[i].x * BLOCKSIZE + XSHIFT , ttrno.a[i].y * BLOCKSIZE + YSHIFT );
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
