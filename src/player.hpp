/*-------------------------------------------------------------------------------

	BARONY
	File: player.hpp
	Desc: contains various declarations for player code.

	Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
	See LICENSE for details.

-------------------------------------------------------------------------------*/

#include "main.hpp"

#pragma once

//Splitscreen support stuff.
extern int current_player; //This may not be necessary. Consider this: Each Player instance keeps track of whether it is a network player or a localhost player.

//TODO: Move these into each and every individual player.
extern Entity *selectedEntity;
extern Sint32 mousex, mousey;
extern Sint32 omousex, omousey;
extern Sint32 mousexrel, mouseyrel;

/*
 * TODO: Will need to make messages work for each hotseat player.
 * This will probably involve taking the current notification_messages thing and instead including that in a wrapper or something that is owned by each player instance.
 * Basically, each player will need to keep track of its own messages.
 * 
 * I believe one of the splitscreen layouts included a version where all of the messages were communal and were in the center of the screen or summat.
 */

extern bool splitscreen;

class Player
{
	//Splitscreen support. Every player gets their own screen.
	//Except in multiplayer. In that case, this is just a big old dummy class.
	SDL_Surface *screen;

	//Is this a hotseat player? If so, draw splitscreen and stuff. (Host player is automatically a hotseat player). If not, then this is a dummy container for the multiplayer client.
	bool local_host;

	int playernum;

public:
	Entity *entity;

	Player(int playernum = 0, bool local_host = true);
	~Player();
};

extern Player **players;
//In the process of switching from the old entity player array, all of the old uses of player need to be hunted down and then corrected to account for the new array.
//So, search for the comment:
//TODO: PLAYERSWAP
//and fix and verify that the information is correct.
//Then, once all have been fixed and verified, uncomment this declaration, and the accompanying definition in player.cpp; uncomment all of the TODO: PLAYERSWAP code segments, and attempt compilation and running.