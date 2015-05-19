#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "stdafx.h"
#include "map.hpp"
#include "tile.hpp"

/* Load map from disk */
void Map::load(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
{
	std::ifstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);

	this->width = width;
	this->height = height;

	for (int pos = 0; pos < this->width * this->height; ++pos)
	{
		this->resources.push_back(255);

		TileType type;
		inputFile.read((char*)&type, sizeof(int));
		switch (type)
		{
		default:
		case TileType::VOID:
		case TileType::GRASS:
			this->tiles.push_back(tileAtlas.at("grass"));
			break;
		case TileType::DIRT:
			this->tiles.push_back(tileAtlas.at("dirt"));
			break;
		case TileType::STONE:
			this->tiles.push_back(tileAtlas.at("stone"));
			break;
		}

		Tile& tile = this->tiles.back();
		inputFile.read((char*)&tile.tileVariant, sizeof(int));
		inputFile.read((char*)&tile.regions, sizeof(int));
	}

	inputFile.close();

	return;
}

void Map::save(const std::string& filename)
{
	std::ofstream outputFile;
	outputFile.open(filename, std::ios::out | std::ios::binary);

	for (auto tile : this->tiles)
	{
		outputFile.write((char*)&tile.type, sizeof(int));
		outputFile.write((char*)&tile.tileVariant, sizeof(int));
		outputFile.write((char*)&tile.regions, sizeof(int));
	}

	outputFile.close();

	return;
}

void Map::draw(sf::RenderWindow& window, float dt)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			sf::Vector2f pos;
			pos.x = x * this->tileSize;
			pos.y = y * this->tileSize;
			this->tiles[y * this->width + x].sprite.setPosition(pos);

			/* Draw the tile */
			this->tiles[y * this->width + x].draw(window, dt);
		}
	}

	return;
}

void Map::updateDirection(TileType tileType)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			int pos = y*this->width + x;

			if (this->tiles[pos].type != tileType) continue;

			bool adjacentTiles[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

			/* Check for adjacent tiles of the same type */
			if (x > 0 && y > 0)
				adjacentTiles[0][0] = (this->tiles[(y - 1) * this->width + (x - 1)].type == tileType);
			if (y > 0)
				adjacentTiles[0][1] = (this->tiles[(y - 1) * this->width + (x)].type == tileType);
			if (x < this->width - 1 && y > 0)
				adjacentTiles[0][2] = (this->tiles[(y - 1) * this->width + (x + 1)].type == tileType);
			if (x > 0)
				adjacentTiles[1][0] = (this->tiles[(y) * this->width + (x - 1)].type == tileType);
			if (x < width - 1)
				adjacentTiles[1][2] = (this->tiles[(y) * this->width + (x + 1)].type == tileType);
			if (x > 0 && y < this->height - 1)
				adjacentTiles[2][0] = (this->tiles[(y + 1) * this->width + (x - 1)].type == tileType);
			if (y < this->height - 1)
				adjacentTiles[2][1] = (this->tiles[(y + 1) * this->width + (x)].type == tileType);
			if (x < this->width - 1 && y < this->height - 1)
				adjacentTiles[2][2] = (this->tiles[(y + 1) * this->width + (x + 1)].type == tileType);

			/* Change the tile variant depending on the tile position */
			if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 2;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 7;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 8;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 9;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 10;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 1;
			else if (adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 3;
			else if (adjacentTiles[0][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 4;
			else if (adjacentTiles[1][0] && adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 5;
			else if (adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 6;
			else if (adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 1;
			else if (adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 1;
		}
	}

	return;
}

