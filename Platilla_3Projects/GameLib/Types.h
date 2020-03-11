#pragma once
enum class Comands { READY, CHOOSE_NUM_PLAYERS, NUM_PLAYERS, WAIT, START, READY_START, TURN, CLUE_REQUEST, CLUE_RESPONSE, MOVE, GO_TO, ROOM_FOUND, DEDUCTION, DENY, DENY_RESPONSE, SHOW_DENYRESPONE, ACCUSATION, ACCUSATION_RESULT, TURN_FINISHED };

enum class CardType {CHARACTER, WEAPON, ROOM};

enum class MoveOptions {MOVE, LAST, BLOCKED, STOPPED};

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

const int ROWS = 30;
const int COLUMNS = 40;

