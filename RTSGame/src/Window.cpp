#include "Window.h"
#include <InputEvent.h>
#include <Managers\EventManager.h>
#include <Locators\EntityManagerLocator.h>
#include <Managers\EntityManager.h>
#include <iostream>

//Input Handler
Window::InputHandler::InputHandler(EventManager<InputEvent>& inputEventManager)
	: m_inputEventManager(inputEventManager)
{}

EventManager<InputEvent>& Window::InputHandler::getInputEventManager()
{
	return m_inputEventManager;
}

void Window::InputHandler::update(const sf::Event & sfmlEvent, sf::RenderWindow& window)
{
	switch (sfmlEvent.type)
	{
	case sf::Event::Closed:
	{
		m_inputEventManager.notify(InputEvent::CloseWindow);
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_inputEventManager.notify(InputEvent::LeftClick);
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			m_inputEventManager.notify(InputEvent::RightClick);
		}
		break;
	}
	case sf::Event::MouseButtonReleased:
	{
		if (sfmlEvent.mouseButton.button == sf::Mouse::Left)
		{
			m_inputEventManager.notify(InputEvent::LeftClickReleased);
		}
		else if (sfmlEvent.mouseButton.button == sf::Mouse::Right)
		{
			m_inputEventManager.notify(InputEvent::RightClickReleased);
		}
		break;
	}
	case sf::Event::KeyPressed:
	{
		if (sfmlEvent.key.code == sf::Keyboard::P)
		{
			const auto mousePosition = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
			auto& entityManager = EntityManagerLocator::getEntityManager();
			const auto spawnLocation = sf::Vector2f(std::floor(mousePosition.x / 16) * 16, std::floor(mousePosition.y / 16) * 16);
			entityManager.addEntity("Marine", spawnLocation);
			std::cout << "SpawnLocation.x: " << spawnLocation.x << ". SpawnLocation.y: " << spawnLocation.y << "\n";
		}
		break;
	}
	}
}

//Window
Window::Window(const sf::Vector2i & size, const std::string & name, EventManager<InputEvent>& inputEventManager)
	: m_inputHandler(inputEventManager),
	m_window(sf::VideoMode(size.x, size.y), name, sf::Style::Default)
{
	inputEventManager.subscribe(InputEvent::CloseWindow, std::bind(&Window::closeWindow, this), "CloseWindow");
}

Window::~Window()
{
	m_inputHandler.getInputEventManager().unsubscribe(InputEvent::CloseWindow, "CloseWindow");
}

sf::RenderWindow & Window::getWindow()
{
	return m_window;
}

bool Window::isOpen() const
{
	return m_window.isOpen();
}

void Window::clearWindow()
{
	m_window.clear(sf::Color::Black);
}

void Window::draw(sf::Drawable & drawable)
{
	m_window.draw(drawable);
}

void Window::display()
{
	m_window.display();
}

void Window::update()
{
	sf::Event sfmlEvent;
	while (m_window.pollEvent(sfmlEvent))
	{
		m_inputHandler.update(sfmlEvent, m_window);
	}
}

void Window::closeWindow()
{
	m_window.close();
}
