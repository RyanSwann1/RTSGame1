#include "Frontier.h"

Frontier::Frontier()
	: m_frontier(),
	m_ID(0)
{}

void Frontier::add(const sf::Vector2f & position)
{
	int cameFromID;
	(m_ID > 0 ? cameFromID = m_ID : cameFromID = 0);
	
	m_frontier.emplace_back(position, m_ID, cameFromID);
	++m_ID;
}
