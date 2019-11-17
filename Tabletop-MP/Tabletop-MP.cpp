
#include "Tabletop-MP.h"

Tabletop::Tabletop()
{
	area_list.push_back(Tabletop::PlayArea("EMPTY", 1280, 720));
	Tabletop::Entity board;
	board.id = 0;
	board.asset_id = 3;
	board.is_movable = false;
	Tabletop::Card card;
	card.id = 1;
	card.asset_id = 2;
	card.front_id = 1;
	card.back_id = 2;
	card.x = 100;
	card.y = 100;
	card.rotation = 0;
	Tabletop::Card card1;
	card1.id = 2;
	card1.asset_id = 5;
	card1.front_id = 1;
	card1.back_id = 5;
	card1.x = 100;
	card1.y = 100;
	card1.rotation = 0;
	Tabletop::Card card2;
	card2.id = 3;
	card2.asset_id = 6;
	card2.front_id = 1;
	card2.back_id = 6;
	card2.x = 100;
	card2.y = 100;
	card2.rotation = 0;
	Tabletop::Deck deck(0);
	deck.id = 4;
	deck.x = 200;
	deck.y = 200;
	deck.asset_id = 4;
	deck.empty_id = 4;
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Entity>(board));
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Card>(card));
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Card>(card1));
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Card>(card2));
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Deck>(deck));
	area_list[0].InsertIntoDeck((Deck&)(*area_list[0].entity_list[4]), 1);
	area_list[0].InsertIntoDeck((Deck&)(*area_list[0].entity_list[4]), 2);
	area_list[0].InsertIntoDeck((Deck&)(*area_list[0].entity_list[4]), 3);
}
