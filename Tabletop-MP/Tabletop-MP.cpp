
#include "Tabletop-MP.h"

Tabletop::Tabletop()
{
	area_list.push_back(Tabletop::PlayArea("EMPTY",800,600));
	Tabletop::Entity simple_object;
	simple_object.asset_id = 0;
	simple_object.x = 100; simple_object.y = 100;
	area_list[0].entity_list.push_back(simple_object);
}
