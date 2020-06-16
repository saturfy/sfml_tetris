#include <SFML/Graphics.hpp>

int main()
{
	//opena a render window, only close, no resize
	sf::RenderWindow window(sf::VideoMode(320, 480),"Tetris" ,sf::Style::Titlebar | sf::Style::Close);

	// loading texture for tetrominos the file is in the main directory
	sf::Texture t;
	//t.loadFromFile("P:\prog\cpp_tutorials\PROJECTS\SFML projects\SFML_tetris");
	t.loadFromFile("../tiles.png");

	// create drawable sprite from the texture
	sf::Sprite s(t);
	s.setTextureRect(sf::IntRect(0, 0, 144, 18));

	while (window.isOpen())
	{
		//checking to close the window
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		//clear the window
		window.clear(sf::Color::White);
		
		//draw game elements
		window.draw(s);

		// render everything drawn so far 
		window.display();
	}

	return 0;
}
