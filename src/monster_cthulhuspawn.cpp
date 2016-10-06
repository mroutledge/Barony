/*-------------------------------------------------------------------------------

BARONY
File: monster_rat.cpp
Desc: implements all of the cthulhu spawn monster's code

Copyright 2013-2016 (c) Turning Wheel LLC, all rights reserved.
See LICENSE for details.

-------------------------------------------------------------------------------*/

#include "main.hpp"
#include "game.hpp"
#include "stat.hpp"
#include "entity.hpp"
#include "monster.hpp"
#include "sound.hpp"
#include "items.hpp"
#include "net.hpp"
#include "collision.hpp"

void initSpawn(Entity *my, stat_t *myStats) {
	my->sprite = 415; // spawn model

	my->flags[UPDATENEEDED] = TRUE;
	my->flags[INVISIBLE] = FALSE;

	if (multiplayer != CLIENT) {
		MONSTER_SPOTSND = 60;
		MONSTER_SPOTVAR = 1;
		MONSTER_IDLESND = 29;
		MONSTER_IDLEVAR = 1;
	}

	if (multiplayer != CLIENT && !MONSTER_INIT) {
		myStats->sex = static_cast<sex_t>(rand() % 2);
		myStats->appearance = rand();
		myStats->lookupKey = "spawn";
		myStats->hitType = WRESTLE;
		strcpy(myStats->name, "");
		myStats->inventory.first = NULL;
		myStats->inventory.last = NULL;
		myStats->HP = 100; myStats->MAXHP = 100;
		myStats->MP = 10; myStats->MAXMP = 10;
		myStats->OLDHP = myStats->HP;
		myStats->STR = 2;
		myStats->DEX = 2;
		myStats->CON = 1;
		myStats->INT = -2;
		myStats->PER = 0;
		myStats->CHR = -1;
		myStats->EXP = 0;
		myStats->LVL = 4;
		myStats->GOLD = 0;
		myStats->HUNGER = 900;
		if (!myStats->leader_uid)
			myStats->leader_uid = 0;

		myStats->FOLLOWERS.first = NULL; myStats->FOLLOWERS.last = NULL;

		for (int c = 0; c < std::max(NUMPROFICIENCIES, NUMEFFECTS); c++) {
			if (c < NUMPROFICIENCIES)
				myStats->PROFICIENCIES[c] = 0;
			if (c < NUMEFFECTS)
				myStats->EFFECTS[c] = FALSE;
			if (c < NUMEFFECTS)
				myStats->EFFECTS_TIMERS[c] = 0;
		}
		myStats->helmet = NULL;
		myStats->breastplate = NULL;
		myStats->gloves = NULL;
		myStats->shoes = NULL;
		myStats->shield = NULL;
		myStats->weapon = NULL;
		myStats->cloak = NULL;
		myStats->amulet = NULL;
		myStats->ring = NULL;
		myStats->mask = NULL;

		newItem(CLOAK_PROTECTION, EXCELLENT, 0, 1, rand(), FALSE, &myStats->inventory);
	}
}

void spawnAnimate(Entity *my, double dist) {
	if ((ticks % 10 == 0 && dist > 0.1) || (MONSTER_ATTACKTIME != MONSTER_ATTACK)) {
		MONSTER_ATTACKTIME = MONSTER_ATTACK;
		if (my->sprite == 415) {
			my->sprite = 416;
		}
		else {
			my->sprite = 415;
		}
	}
}

void spawnDie(Entity *my) {
	for (int c = 0; c < 5; c++) {
		Entity *gib = spawnGib(my);
		serverSpawnGibForClient(gib);
	}
	if (spawn_blood) {
		int x, y;
		x = std::min<unsigned int>(std::max(0, (int)(my->x / 16)), map.width - 1);
		y = std::min<unsigned int>(std::max(0, (int)(my->y / 16)), map.height - 1);
		if (map.tiles[y*MAPLAYERS + x*MAPLAYERS*map.height]) {
			if (!checkObstacle(my->x, my->y, my, NULL)) {
				Entity *entity = newEntity(160, 1, map.entities);
				entity->x = my->x;
				entity->y = my->y;
				entity->z = 7.4 + (rand() % 20) / 100.f;
				entity->parent = my->uid;
				entity->sizex = 2;
				entity->sizey = 2;
				entity->yaw = (rand() % 360)*PI / 180.0;
				entity->flags[UPDATENEEDED] = TRUE;
				entity->flags[PASSABLE] = TRUE;
			}
		}
	}
	playSoundEntity(my, 30, 64);
	list_RemoveNode(my->mynode);
	return;
}
