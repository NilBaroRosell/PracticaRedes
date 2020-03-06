#pragma once
enum class Comands { READY, CHOOSE_NUM_PLAYERS, NUM_PLAYERS, WAIT, START, TURN, CLUE_REQUEST, CLUE_CARD, MOVE, GO_TO, ROOM_FOUND, DEDUCTION, DENY, DENY_RESPONSE, SHOW_DENYRESPONE, ACCUSATION, ACCUSATION_RESULT };

enum class CardType {CHARACTER, WEAPON, ROOM};

struct card {
	std::string name;
	CardType type;
};

struct Vector2
{
	int x;
	int y;

	Vector2() { x = -1; y = -1; }
	Vector2(int _x, int _y) { x = _x; y = _y; }
};

const int ROWS = 40;
const int COLUMNS = 30;

