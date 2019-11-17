
#pragma once

#include <math.h>
#include "SFML/Graphics.hpp"

namespace Math{
	float Lerp(float l1, float l2, float v) {
		return ((l2 - l1) * v);
	}
	sf::Vector2f Lerp(sf::Vector2f l1, sf::Vector2f l2, float v) {
		return ((l2 - l1) * v);
	}

	float Clamp(float a, float min, float max) {
		return std::min(std::max(a, min),max);
	}
}