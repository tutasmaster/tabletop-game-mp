
#include "Tabletop-MP.h"

Tabletop::Tabletop()
{
	area_list.push_back(Tabletop::PlayArea("EMPTY",1280,720));
	Tabletop::Entity board;
	board.asset_id = 2;
	board.is_movable = false;
	Tabletop::Card card;
	card.front_id = 1;
	card.x = 100;
	card.y = 100;
	card.rotation = 0;
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Entity>(board));
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Card>(card));
}
