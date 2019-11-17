
#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include "SFML/Graphics.hpp"

class Tabletop {
public:
	class Player {
		unsigned int id = 0;
	};

	class Entity {
	public:
		enum Type {
			entity,
			card,
			deck
		}type = entity;

		bool operator== (unsigned char p) { return id == p; }

		Player* owner = nullptr;
		unsigned char id = 0;
		unsigned char area_id = 0;
		float x = 0, y = 0;
		float rotation = 0;
		float display_x = 0, display_y = 0, display_rotation = 0;
		bool is_movable = true;
		unsigned int asset_id = 0;
		virtual void Flip() {}
	};

	class Card : public Tabletop::Entity{
	public:
		bool is_flipped = false;
		unsigned int front_id = 0;
		unsigned int back_id = 0;
		Card(){ type = card; }
		void Flip() override { 
			is_flipped = !is_flipped;
			asset_id = is_flipped ? front_id : back_id; 
		}
	};
	class PlayArea;
	class Deck : public Tabletop::Entity {
	public:
		unsigned char area_id = 0;
		unsigned int empty_id = 0;
		std::vector<unsigned char> cards;
		Deck(unsigned char area_id = 0) : area_id(area_id) { type = deck; }
	};


	class PlayArea {
	public:
		PlayArea(std::string name = "EMPTY", float width = 500, float height = 500) : name(name), width(width), height(height){}
		unsigned char id = 0;
		std::string name = "EMPTY";
		float width = 0, height = 0;
		unsigned char current_id = 0;
		Tabletop::Entity * FindEntity(unsigned char id) {
			return (*std::find_if(entity_list.begin(), entity_list.end(), [&, id](const std::unique_ptr<Entity>& e) {return e->id == id; })).get();
		}

		void Flip(Entity& entity) {
			entity.Flip();
			if (entity.type == entity.deck)
				PopFromDeck((Deck&)entity);
		}

		void InsertIntoDeck(Deck&deck, unsigned char id) {
			auto& list = entity_list;
			auto it = FindEntity(id);
			if (it != nullptr) {
				deck.cards.push_back(id);
				deck.asset_id = (it)->asset_id;
				(it)->x = deck.x;
				(it)->y = deck.y;
				(it)->asset_id = 0;
			}
		}
		void PopFromDeck(Deck& deck) {
			auto& list = entity_list;
			if (deck.cards.size() > 0) {
				unsigned char id = deck.cards.back();
				auto it = FindEntity(id);
				(it)->x = deck.x;
				(it)->y = deck.y - 10;
				(it)->display_x = deck.x;
				(it)->display_y = deck.y;
				((Card*)(it))->asset_id = ((Card*)(it))->is_flipped ? ((Card*)(it))->front_id : ((Card*)(it))->back_id;
				deck.cards.pop_back();
			}
			if (deck.cards.size() == 0) {
				deck.asset_id = deck.empty_id;
			}
			else {
				Card* c = ((Card*)FindEntity(deck.cards.back()));
				deck.asset_id = c->is_flipped ? c->front_id : c->back_id;
			}
		}
		std::vector<std::unique_ptr<Tabletop::Entity>> entity_list;
	};

	Tabletop();

	std::vector<PlayArea> area_list;
};

class AssetManager {
public:
	struct Asset {
	public:
		virtual void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) {};
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
			drawable.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
		}
		void Draw(sf::RenderTarget& render_target, Tabletop::Entity& e) override {
			drawable.setPosition(sf::Vector2f(e.display_x, e.display_y));
			drawable.setRotation(e.display_rotation);
			render_target.draw(drawable);
		}
		bool CheckCollisionAtPoint(sf::Vector2f pos, Tabletop::Entity& e) override { 
			float angle = -((e.rotation / 180.0f) * 3.14159265358979323846);
			float sinus = sin(angle);
			float cosin = cos(angle);

			sf::Vector2f point = pos - sf::Vector2f(e.x, e.y);
			
			sf::Vector2f temp(
				(point.x * cosin) - (point.y * sinus), 
				(point.x * sinus) + (point.y * cosin)
			);

			std::cout << temp.x << " : " << temp.y << "\n";

			point = temp + sf::Vector2f(e.x, e.y);

			return (point.x > e.x - (texture.getSize().x / 2) && point.x < e.x + (texture.getSize().x/2) && point.y > e.y - (texture.getSize().y / 2) && point.y < e.y + (texture.getSize().y / 2));
		}
	};

	AssetManager() { 
		//AddAsset(Sprite("2_of_clubs.png")); 
		//AddAsset(Sprite("Assets/back.png"));
		asset_list.push_back(std::make_unique<Asset>());
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