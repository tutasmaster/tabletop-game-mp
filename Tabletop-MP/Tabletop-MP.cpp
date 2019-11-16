
#include "Tabletop-MP.h"

Tabletop::Tabletop()
{
	area_list.push_back(Tabletop::PlayArea("EMPTY",800,600));
	Tabletop::Card simple_object;
	simple_object.asset_id = 0;
	simple_object.front_id = 1;
	simple_object.back_id = 0;
	simple_object.x = 100; simple_object.y = 100;
	area_list[0].entity_list.push_back(std::make_unique<Tabletop::Card>(simple_object));
}
