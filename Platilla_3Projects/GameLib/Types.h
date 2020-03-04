#pragma once
enum class Comands { READY, WAIT, START, TURN, CLUE_REQUEST, CLUE_CARD, MOVE, GO_TO, ROOM_FOUND, DEDUCTION, DENY, DENY_RESPONSE };

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

std::vector<card> full_Deck;

const int ROWS = 40;
const int COLUMNS = 30;

void initializeCards()
{
	//Characters
	full_Deck.push_back({ "Prado", CardType::CHARACTER });
	full_Deck.push_back({ "Rubio", CardType::CHARACTER });
	full_Deck.push_back({ "Orquidea", CardType::CHARACTER });
	full_Deck.push_back({ "Celeste", CardType::CHARACTER });
	full_Deck.push_back({ "Mora", CardType::CHARACTER });
	full_Deck.push_back({ "Amapola", CardType::CHARACTER });

	//Weapons
	full_Deck.push_back({ "Candelabro", CardType::WEAPON });
	full_Deck.push_back({ "Punal", CardType::WEAPON });
	full_Deck.push_back({ "Tuberia de plomo", CardType::WEAPON });
	full_Deck.push_back({ "Pistola", CardType::WEAPON });
	full_Deck.push_back({ "Cuerda", CardType::WEAPON });
	full_Deck.push_back({ "Herramienta", CardType::WEAPON });

	//Rooms
	full_Deck.push_back({ "Sala de baile", CardType::ROOM });
	full_Deck.push_back({ "Sala de billar", CardType::ROOM });
	full_Deck.push_back({ "Invernadero", CardType::ROOM });
	full_Deck.push_back({ "Comedor", CardType::ROOM });
	full_Deck.push_back({ "Vestibulo", CardType::ROOM });
	full_Deck.push_back({ "Cocina", CardType::ROOM });
	full_Deck.push_back({ "Biblioteca", CardType::ROOM });
	full_Deck.push_back({ "Salon", CardType::ROOM });
	full_Deck.push_back({ "Estudio", CardType::ROOM });
}