#include "tictactoe32.h"

GAME::GAME() {
	pMap = new MAP;
	pPlayer[0] = new PLAYER(0);
	pPlayer[1] = new PLAYER(1);

	game_round = 1;
}
int GAME::restart() {
	pMap->clear();
	return 0;
}
int GAME::reset() {
	pMap->clear();
	return 0;
}
int GAME::round() {
	return 0;
}
int GAME::turn() {
	return pMap->get_turn();
}

GAME::PLAYER::PLAYER(int id) {
	this->id = id;
	this->winCount = 0;
}
int GAME::PLAYER::won() {
	this->winCount++;
	return 0;
}
int GAME::PLAYER::wincount() {
	return this->winCount;
}
int GAME::PLAYER::clear() {
	winCount = 0;
	return 0;
}
int GAME::PLAYER::get_id() {
	return this->id;
}
int GAME::set_map(MAPPOS pos) {
	int player_id = pMap->get_turn();
	return pMap->set(pos, pPlayer[player_id]);
}

GAME::MAP::MAP() {
	clear();

	totalGame = 0;
	chess_count = 0;
	turn = PLAYER_0;
}
GAME::MAP::~MAP() {

}
int GAME::MAP::clear() {
	for (int fecx = 0; fecx < 3; fecx++) {
		memset((char*)pData[fecx], -1, sizeof(char) * 3);
	}
	return 0;
}
int GAME::MAP::win_check() {
	char buffer[3];

	memset(buffer, -1, sizeof(char) * 3);
	for (int row = 0; row < 3; row++) {
		for (int colum = 0; colum < 3; colum++) {
			buffer[colum] = pData[row][colum];
			if (buffer[colum] == -1) {
				break;
			}
		}
		if (((buffer[0] + buffer[1] + buffer[2]) % 3) == 0 && (buffer[0] | buffer[1] | buffer[2]) != -1) {
			return buffer[0];
		}
	}

	memset(buffer, -1, sizeof(char) * 3);
	for (int colum = 0; colum < 3; colum++) {
		for (int row = 0; row < 3; row++) {
			buffer[row] = pData[row][colum];
		}
		if (((buffer[0] + buffer[1] + buffer[2]) % 3) == 0 && (buffer[0] | buffer[1] | buffer[2]) != -1) {
			return buffer[0];
		}
	}
	
	// Cross case
	memset(buffer, -1, sizeof(char) * 3);
	for (int fecx = 0; fecx < 3; fecx++) {
		buffer[fecx] = pData[fecx][fecx];
	}
	if (((buffer[0] + buffer[1] + buffer[2]) % 3) == 0 && (buffer[0] | buffer[1] | buffer[2]) != -1) {
		return buffer[0];
	}

	memset(buffer, -1, sizeof(char) * 3);
	for (int fecx = 0; fecx < 3; fecx++) {
		buffer[fecx] = pData[abs(fecx - 2)][fecx];
	}
	if (((buffer[0] + buffer[1] + buffer[2]) % 3) == 0 && (buffer[0] | buffer[1] | buffer[2]) != -1) {
		return buffer[0];
	}

	if (chess_count + 1 == 9) {
		return DRAW;
	}

	return ONGOING;
}
int GAME::MAP::set(MAPPOS pos, PLAYER* player) {
	char colum = HIGH16(pos);
	char row = LOW16(pos);
	pData[row][colum] = (char)player->get_id();

	int result = win_check();
	next_turn();

	return result;
}
bool GAME::MAP::get_turn() {
	return this->turn;
}
void GAME::MAP::next_turn() {
	chess_count++;
	this->turn = !this->turn;
	return;
}
int GAME::MAP::get_chess_count() {
	return this->chess_count;
}