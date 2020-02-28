#pragma once
enum class Comands { READY, WAIT, START, TURN, CLUE_REQUEST, CLUE_CARD, MOVE, GO_TO, ROOM_FOUND, DEDUCTION, DENY, DENY_RESPONSE };

enum class CardType {CHARACTER, WEAPON, ROOM};

struct card {
	std::string name;
	CardType type;
};

std::vector<card> full_Deck;

const int ROWS = 40;
const int COLUMNS = 30;