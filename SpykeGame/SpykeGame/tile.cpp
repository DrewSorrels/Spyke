#include <SFML/Graphics.hpp>

#include "stdafx.h"
#include "animation_handler.hpp"
#include "tile.hpp"

void Tile::draw(sf::RenderWindow& window, float dt)
{
	/* Change to sprite to reflect the tile variant */
	this->animHandler.changeAnim(this->tileVariant);

	/* Update the animation */
	this->animHandler.update(dt);

	/* Update the sprite */
	this->sprite.setTextureRect(this->animHandler.bounds);

	/* Draw the tile */
	window.draw(this->sprite);

	return;
}

void Tile::update()
{
	// Do nothing
}

std::string tileTypeToStr(TileType type)
{
	switch (type)
	{
	default:
	case TileType::VOID:
		return "Void";
	case TileType::GRASS:
		return "Grass";
	case TileType::DIRT:
		return "Dirt";
	case TileType::STONE:
		return "Stone";
	}
}