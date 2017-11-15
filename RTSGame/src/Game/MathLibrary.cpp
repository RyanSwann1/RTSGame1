#include <Game\MathLibrary.h>
#include <math.h>
#include <Game\Direction.h>
#include <assert.h>

Direction MathLibrary::getDirectionFromBetweenPoints(const sf::Vector2f & pointA, const sf::Vector2f & pointB)
{
	Direction direction;

	if (pointB.x > pointA.x && pointB.y == pointA.y)
	{
		direction = Direction::Right;
	}
	else if (pointB.x < pointA.x && pointB.y == pointA.y)
	{
		direction = Direction::Left;
	}
	else if (pointB.x == pointA.x && pointB.y > pointA.y)
	{
		direction = Direction::Down;
	}
	else if (pointB.x == pointA.x && pointB.y < pointA.y)
	{
		direction = Direction::Up;
	}
	else if (pointB.x < pointA.x && pointB.y < pointA.y)
	{
		direction = Direction::UpLeft;
	}
	else if (pointB.x > pointA.x && pointB.y < pointA.y)
	{
		direction = Direction::UpRight;
	}
	else if (pointB.x < pointA.x && pointB.y > pointA.y)
	{
		direction = Direction::DownLeft;
	}
	else if (pointB.x > pointA.x && pointB.y > pointA.y)
	{
		direction = Direction::DownRight;
	}
	else if (pointA == pointB)
	{
		direction = Direction::None;
	}

	return direction;
}

int MathLibrary::getDistanceBetweenPoints(const sf::Vector2f & pointA, const sf::Vector2f & pointB)
{
	return std::abs(pointA.x - pointB.x) + std::abs(pointA.y - pointB.y);
}