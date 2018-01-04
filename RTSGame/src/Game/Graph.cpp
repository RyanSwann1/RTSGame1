#include <Game\Graph.h>
#include <assert.h>
#include <Game\DebugOverlay.h>

Graph::Graph()
	: m_graph(),
	m_pointID(0)
{}

const std::vector<std::unique_ptr<Point>>& Graph::getGraph() const
{
	return m_graph;
}

bool Graph::isOnGraph(const sf::Vector2f & position) const
{
	return std::find_if(m_graph.cbegin(), m_graph.cend(), [&position](const auto& positionOnGraph)
		{ return position == positionOnGraph.get()->m_position; }) != m_graph.cend();
}

bool Graph::isEmpty() const
{
	return m_graph.empty();
}

void Graph::addToGraph(const sf::Vector2f & position)
{
	assert(!isOnGraph(position));
	int previousPointID = 0;
	(m_graph.empty() ? previousPointID = 0 : previousPointID = m_graph.back()->m_ID);
	m_graph.emplace_back(std::make_unique<Point>(position, m_pointID, previousPointID));
	++m_pointID;
}

void Graph::clearGraph()
{
	m_pointID = 0;
	m_graph.clear();
}

void Graph::eraseGraphUntilPosition(const sf::Vector2f & position)
{
	for (auto iter = m_graph.begin(); iter != m_graph.end(); ++iter)
	{
		const auto graphPosition = sf::Vector2f(iter->get()->m_position.x * 16, iter->get()->m_position.y * 16);

		DebugOverlay::clearShape(graphPosition);
		iter = m_graph.erase(iter);
		
		if (graphPosition == position)
		{
			break;
		}

		if (m_graph.empty())
		{
			return;
		}
	}
}