#pragma once
#include "tictactoe32UI.h"

typedef unsigned int MAPPOS;

#define INDEX_ERROR -1
#define MEM_ALLOCATION_ERROR -2

#define PLAYER_0 0
#define PLAYER_1 1

#define MAP_A0 (MAPPOS)0x0
#define MAP_A1 (MAPPOS)0x10000
#define MAP_A2 (MAPPOS)0x20000
#define MAP_B0 (MAPPOS)0x1
#define MAP_B1 (MAPPOS)0x10001
#define MAP_B2 (MAPPOS)0x20001
#define MAP_C0 (MAPPOS)0x2
#define MAP_C1 (MAPPOS)0x10002
#define MAP_C2 (MAPPOS)0x20002

#define LOW16(X) (X & 0xFFFF)
#define HIGH16(X) ((X & 0xFFFF0000) >> 16)

#define P0_WIN 0
#define P1_WIN 1
#define ONGOING -3
#define DRAW -4

class GAME {
private:
	class PLAYER {
	public:
		PLAYER(int id);

	public:
		int won();
		int wincount();
		int clear();
		int get_id();

	private:
		int id;
		int winCount;
	};

	class MAP {
		/*	A B C
		[0] 0 1 2
		[1] 0 1 2
		[2] 0 1 2 */
	public:
		MAP();
		~MAP();
		int win_check();
		bool get_turn();
		void next_turn();
		int get_chess_count();
		int set(MAPPOS pos, PLAYER* player);
		int clear();

	private:
		int chess_count;
		bool turn;
		int totalGame;
		char pData[3][3] = { -1 };

	};

private:
	PLAYER* pPlayer[2];
	MAP* pMap;
	int game_round;

public:
	GAME();

public:
	int restart();
	int reset();
	int round();
	int turn();
	int set_map(MAPPOS pos);
};
