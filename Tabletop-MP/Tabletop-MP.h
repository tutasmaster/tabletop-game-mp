
#pragma once

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"

class Tabletop {
public:
	class Player {
		unsigned int id = 0;
	};

	class Entity {
	public:
		Player* owner;
		unsigned int id = 0;
		float x = 0, y = 0;
		unsigned int asset_id = 0;
	};

	class Card : public Tabletop::Entity{
	public:
		bool is_flipped = false;
		unsigned int front_id = 0;
		unsigned int back_id = 0;
		void Flip() { asset_id = is_flipped ? front_id : back_id; asset_id = !asset_id; }
	};

	class PlayArea {
	public:
		PlayArea(std::string name = "EMPTY", float width = 500, float height = 500) : name(name), width(width), height(height){}
		unsigned char id = 0;
		std::string name = "EMPTY";
		float width = 0, height = 0;
		std::vector<Tabletop::Entity> entity_list;
	};

	Tabletop();

	std::vector<PlayArea> area_list;
};

class AssetManager {
public:
	struct Asset {
	public:
		int id = 0;
		virtual void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) = 0;
	};

	class Rectangle : public Asset {
	public:
		sf::RectangleShape drawable;
		Rectangle(sf::Vector2f size = sf::Vector2f(100, 100), const sf::Color& color = sf::Color::Red) : drawable(size) {
			drawable.setFillColor(color);
			drawable.setSize(size);
		}
		void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) override { 
			drawable.setPosition(sf::Vector2f(e.x, e.y)); 
			render_target.draw(drawable); 
		}
	};
	AssetManager() { AddAsset(Rectangle()); }
	template<typename T>
	void AddAsset(T& asset) { 
		asset.id = current_asset++; 
		asset_list.push_back(std::make_unique<T>(asset)); 
	}
	unsigned int current_asset = 0;
	std::vector<std::unique_ptr<Asset>> asset_list;
};