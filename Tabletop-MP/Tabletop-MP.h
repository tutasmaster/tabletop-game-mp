
#pragma once

#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"

class Tabletop {
public:
	class Entity {
	public:
		unsigned int id = 0;
		float x = 0, y = 0;
		unsigned int asset_id = 0;
	};

	class Card : public Tabletop::Entity{
	public:
		bool is_flipped = false;
		unsigned int front_id = 0;
		unsigned int back_id = 0;
	};

	class PlayArea {
		unsigned int width = 0, height = 0;
		std::vector<Tabletop::Entity> entity_list;
	};
};