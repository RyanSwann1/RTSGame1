#pragma once

#include <Game\Game.h>

//https://gamedev.stackexchange.com/questions/5013/how-does-pathfinding-in-rts-games-work
// A*, navmeshes, flow fields, etc
//https://www.gamasutra.com/blogs/FerdinandJosephFernandez/20130802/197471/Using_Software_Engineering_to_Streamline_a_7day_RTS.php
//http://www.redblobgames.com/pathfinding/a-star/introduction.html
// flow field pathfinding
//https://www.adityaravishankar.com/2011/11/command-and-conquer-programming-an-rts-game-in-html5-and-javascript/
//https://www.youtube.com/watch?v=Bdu5_Q5QI2Q
//https://www.reddit.com/r/gamedev/comments/19sve3/i_need_help_optimizing_my_a_pathfinding/
//Microsoft COM Model

//A*
//https://www.youtube.com/watch?v=FsParg61xGw
//https://howtorts.github.io/2014/01/04/basic-flow-fields.html

//Object Pool
//http://gameprogrammingpatterns.com/object-pool.html


//Load all textures before game starts


int main()
{
	Game game;
	while (game.isRunning())
	{
		game.update();
		game.draw();
		game.lateUpdate();
	}

	return 0;
}