#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <string>

enum class InputEvent;
template <class InputEvent>
class EventManager;
class Window
{
	class InputHandler
	{
	public:
		InputHandler(EventManager<InputEvent>& inputEventManager);

		EventManager<InputEvent>& getInputEventManager();
		void update(const sf::Event& sfmlEvent, sf::RenderWindow& window);

	private:
		EventManager<InputEvent>& m_inputEventManager;
	};

public:
	Window(const sf::Vector2i& size, const std::string& name, EventManager<InputEvent>& inputEventManager);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window&& operator=(Window&&) = delete;

	sf::RenderWindow& getWindow();
	bool isOpen() const;
	void clearWindow();
	void draw(sf::Drawable& drawable);
	void display();

	void update();

private:
	InputHandler m_inputHandler;
	sf::RenderWindow m_window;

	void closeWindow();
};