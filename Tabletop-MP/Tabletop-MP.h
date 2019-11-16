
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
		Player* owner = nullptr;
		unsigned int id = 0;
		float x = 0, y = 0;
		bool is_movable = true;
		unsigned int asset_id = 0;
		virtual void Flip() {}
	};

	class Card : public Tabletop::Entity{
	public:
		bool is_flipped = false;
		unsigned int front_id = 0;
		unsigned int back_id = 0;
		void Flip() override { 
			is_flipped = !is_flipped;
			asset_id = is_flipped ? front_id : back_id; 
		}
	};

	class PlayArea {
	public:
		PlayArea(std::string name = "EMPTY", float width = 500, float height = 500) : name(name), width(width), height(height){}
		unsigned char id = 0;
		std::string name = "EMPTY";
		float width = 0, height = 0;
		std::vector<std::unique_ptr<Tabletop::Entity>> entity_list;
	};

	Tabletop();

	std::vector<PlayArea> area_list;
};

class AssetManager {
public:
	struct Asset {
	public:
		virtual void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) = 0;
		virtual bool CheckCollisionAtPoint(sf::Vector2f pos, Tabletop::Entity& e) { return false; };
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
		bool CheckCollisionAtPoint(sf::Vector2f pos, Tabletop::Entity& e) override { return (pos.x > e.x && pos.x < e.x + drawable.getSize().x && pos.y > e.y && pos.y < e.y + drawable.getSize().y); }
	};

	class Sprite : public Asset {
	public:
		sf::Texture texture;
		sf::Sprite drawable;
		Sprite(const std::string path = "Assets/Empty.png", const sf::Color& color = sf::Color::White) {
			texture.loadFromFile(path);
			drawable.setTexture(texture);
			drawable.setColor(color);
		}
		void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) override {
			drawable.setPosition(sf::Vector2f(e.x, e.y));
			render_target.draw(drawable);
		}
		bool CheckCollisionAtPoint(sf::Vector2f pos, Tabletop::Entity& e) override { return (pos.x > e.x&& pos.x < e.x + texture.getSize().x && pos.y > e.y&& pos.y < e.y + texture.getSize().y); }
	};

	AssetManager() { 
		//AddAsset(Sprite("2_of_clubs.png")); 
		//AddAsset(Sprite("Assets/back.png"));
	}
	template<typename T>
	void AddRectangle(Rectangle& asset) { 
		asset_list.push_back(std::make_unique<T>(asset)); 
	}
	void AddSprite(const std::string path = "Assets/Empty.png", const sf::Color& color = sf::Color::White) {
		asset_list.push_back(std::make_unique<Sprite>(path,color));
	}
	unsigned int current_asset = 0;
	std::vector<std::unique_ptr<Asset>> asset_list;
};