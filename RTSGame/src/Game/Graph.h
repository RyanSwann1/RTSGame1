#pragma once

#include <vector>
#include <memory>
#include <SFML\Graphics.hpp>
#include <Game\Point.h>

class Graph
{
public:
	Graph();
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;
	Graph(Graph&&) = delete;
	Graph&& operator=(Graph&&) = delete;

	const std::vector<std::unique_ptr<Point>>& getGraph() const;
	bool isOnGraph(const sf::Vector2f& position) const;
	bool isEmpty() const;

	void addToGraph(const sf::Vector2f& position);
	void clearGraph();
	void eraseGraphUntilPosition(const sf::Vector2f& position);

private:
	std::vector<std::unique_ptr<Point>> m_graph;
	int m_pointID;
};