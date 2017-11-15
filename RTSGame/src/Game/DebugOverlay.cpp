#include <vector>
#include "DebugOverlay.h"

std::vector<sf::RectangleShape> m_shapes;

void DebugOverlay::draw(sf::RenderWindow & window)
{
	for (const auto& shape : m_shapes)
	{
		window.draw(shape);
	}
}

void DebugOverlay::addShape(const sf::Vector2f & position)
{
	sf::RectangleShape rect(sf::Vector2f(16, 16));
	rect.setPosition(position);
	rect.setFillColor(sf::Color::Red);

	m_shapes.push_back(rect);
}

void DebugOverlay::clearShapes()
{
	m_shapes.clear();
}

void DebugOverlay::clearShape(const sf::Vector2f & position)
{
	auto iter = std::find_if(m_shapes.begin(), m_shapes.end(), [&position](const auto& shape) { return position == shape.getPosition(); });
	if (iter != m_shapes.end())
	{
		m_shapes.erase(iter);
	}
}