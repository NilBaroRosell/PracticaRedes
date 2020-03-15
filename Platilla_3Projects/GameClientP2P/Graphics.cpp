#include "Graphics.h"
#include <iostream>


Graphics::Graphics()
{
	Sala invernadero("Invernadero", 0, 0, 8, 10, sf::Color::Yellow);
	Sala salaBillar("Sala de billar", 12, 0, 6, 10, sf::Color::Red);
	Sala biblioteca("Biblioteca", 22, 0, 6, 10, sf::Color::Cyan);
	Sala estudio("Estudio", 32, 0, 8, 10, sf::Color::Magenta);
	Sala salaBaile("Sala de baile", 0, 12, 8, 6, sf::Color::Blue);
	Sala vestibulo("vestibulo", 30, 12, 10, 8, sf::Color::White);
	Sala cocina("cocina", 0, 20, 10, 10, sf::Color::Green);
	Sala comedor("comedor", 13, 20, 13, 10, sf::Color(150,0,150));
	Sala salon("salon", 30, 22, 10, 8, sf::Color(0,150,150));
	salas[0] = invernadero;
	salas[1] = salaBillar;
	salas[2] = biblioteca;
	salas[3] = estudio;
	salas[4] = salaBaile;
	salas[5] = vestibulo;
	salas[6] = cocina;
	salas[7] = comedor;
	salas[8] = salon;

	centroMensajes.color = sf::Color(150,150,150);
	centroMensajes.origen.x = 12;
	centroMensajes.origen.y = 12;
	centroMensajes.longitud.x = 14;
	centroMensajes.longitud.y = 6;
}

void Graphics::InitDungeon()
{
	_window = new sf::RenderWindow(sf::VideoMode(800, 600), "Ventanita");
	shape = new sf::RectangleShape(sf::Vector2f(SIZE, SIZE));
	shape->setOutlineColor(sf::Color::Black);
	shape->setOutlineThickness(2.f);
}

void Graphics::DrawDungeon(std::vector<sf::Vector2f> _positions, std::vector<sf::Color> _colors, bool _draw, int _direction)
{
	if (_window->isOpen())
	{
		sf::Event event;
		bool playerMoved = false;
		while (_window->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				_window->close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					_window->close();
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					_direction = 1;
				}
				else if (event.key.code == sf::Keyboard::Up)
				{
					_direction = 2;
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					_direction = 3;
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					_direction = 4;
				}
				else _direction = 0;
				break;
			}
		}
		_window->clear();

		for (int i = 0; i < W_WINDOW_TITLE; i++)
		{
			for (int j = 0; j < H_WINDOW_TITLE; j++)
			{

				shape->setFillColor(sf::Color(90, 90, 90, 255));


				shape->setPosition(sf::Vector2f(i*SIZE, j*SIZE));
				_window->draw(*shape);
			}
		}

		for (size_t i = 0; i < salas.size(); i++)
		{
			salas[i].Draw(*_window);
			
		}

		if (_draw)
		{
			for (int i = 0; i < _positions.size(); i++)
			{
				Sala a("", _positions[i].x, _positions[i].y, 1, 1, _colors[i]);
				a.Draw(*_window);
			}
		}

		centroMensajes.Draw(*_window);

		/*sf::Vector2f position;
		position.x = 0; position.y = 0;
		shape.setFillColor(sf::Color::Blue);
		shape.setFillColor(sf::Color(0, 0, 255, 255));
		shape.setPosition(sf::Vector2f(position.x*SIZE, position.y*SIZE));
		_window.draw(shape);

		position.x = W_WINDOW_TITLE - 1; position.y = H_WINDOW_TITLE - 1;
		shape.setFillColor(sf::Color::Green);
		shape.setFillColor(sf::Color(255, 255, 0, 255));
		shape.setPosition(sf::Vector2f(position.x*SIZE, position.y*SIZE));
		_window.draw(shape);*/

		_window->display();
	}
}

void Graphics::ClearDungeon()
{
	delete(_window);
	delete(shape);
	_window = nullptr;
	shape = nullptr;
}


Graphics::~Graphics()
{
}