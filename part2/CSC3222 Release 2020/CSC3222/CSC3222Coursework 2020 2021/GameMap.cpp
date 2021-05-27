#include "GameMap.h"
#include "GameSimsRenderer.h"
#include "TextureManager.h"
#include "../../Common/Maths.h"
#include "../../Common/Assets.h"
#include "../../Common/TextureLoader.h"
#include <fstream>
#include <iostream>
#include "CollisionVolume.h"
#include "SimObject.h"

using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

GameMap::GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager, GameSimsPhysics* physics)	{
	tileTexture = texManager.GetTexture("FruitWizard//arcade_platformerV2.png");

	std::ifstream mapFile(Assets::DATADIR + filename);

	if (!mapFile) {
		std::cout << "GameMap can't be loaded in!" << std::endl;
		return;
	}

	mapFile >> mapWidth;
	mapFile >> mapHeight;

	mapData		= new char[mapWidth * mapHeight];
	mapCosts	= new int[mapWidth * mapHeight];

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = ((mapHeight - 1 - y) * mapWidth) + x;
			mapFile >> mapData[tileIndex];
		}
	}

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = ((mapHeight - 1 - y) * mapWidth) + x;
			char c;
			mapFile >> c;
			mapCosts[tileIndex] = c - '0';
		}
	}

	AddMapColliders(physics);
	BuildMapMesh();
}

GameMap::~GameMap()	{
	delete[] mapData;
	delete[] mapCosts;
	delete[] mapTexCoords;

	delete mapMesh;
}
// manually added the game map colliders
void GameMap::AddMapColliders(GameSimsPhysics* physics) {
	// walls 
	CollisionVolume* leftWall = new RectangleCollider(CollisionVolume::objectType::WALL, nullptr, 16, 304);
	leftWall->SetBehaviour(CollisionVolume::behaviour::STATIC);
	leftWall->SetPosition(Vector2(8, 160));
	physics->AddCollider(leftWall);

	CollisionVolume* rightWall = new RectangleCollider(CollisionVolume::objectType::WALL, nullptr, 16, 304);
	rightWall->SetBehaviour(CollisionVolume::behaviour::STATIC);
	rightWall->SetPosition(Vector2(472, 160));
	physics->AddCollider(rightWall);

	
	// ladders 

	CollisionVolume* ladder1a = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder1a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder1a->SetPosition(Vector2(112, 48 + 16));
	physics->AddCollider(ladder1a);

	CollisionVolume* ladder1b = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder1b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder1b->SetPosition(Vector2(256, 48 + 16));
	physics->AddCollider(ladder1b);

	CollisionVolume* ladder2a = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 2 * 32);
	ladder2a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder2a->SetPosition(Vector2(32, 48 + 96 + 16));
	physics->AddCollider(ladder2a);

	CollisionVolume* ladder2b = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder2b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder2b->SetPosition(Vector2(368, 48 + 64 + 16));
	physics->AddCollider(ladder2b);

	CollisionVolume* ladder3a = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder3a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder3a->SetPosition(Vector2(192, 48 + 64 + 64 + 16));
	physics->AddCollider(ladder3a);

	CollisionVolume* ladder3b = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder3b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder3b->SetPosition(Vector2(304, 48 + 64 + 64 + 16));
	physics->AddCollider(ladder3b);

	CollisionVolume* ladder4a = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 2 * 32);
	ladder4a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder4a->SetPosition(Vector2(432, 48 + 64 + 96 + 16));
	physics->AddCollider(ladder4a);

	CollisionVolume* ladder4b = new RectangleCollider(CollisionVolume::objectType::LADDER, nullptr, 2 * 16, 2 * 32);
	ladder4b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ladder4b->SetPosition(Vector2(96, 48 + 64 + 64 + 64 + 16));
	physics->AddCollider(ladder4b);
	
	//ground / platforms

	CollisionVolume* ground = new RectangleCollider(CollisionVolume::objectType::BASE, nullptr, 240*2, 8*2);
	ground->SetBehaviour(CollisionVolume::behaviour::STATIC);
	ground->SetPosition(Vector2(240, 8));
	physics->AddCollider(ground);

	CollisionVolume* platform1a = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 64 * 2, 8 * 2);
	platform1a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform1a->SetPosition(Vector2(80, 72));
	physics->AddCollider(platform1a);

	CollisionVolume* platform1b = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 144 * 2, 8 * 2);
	platform1b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform1b->SetPosition(Vector2(320, 72));
	physics->AddCollider(platform1b);

	CollisionVolume* platform2 = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 144 * 2, 8 * 2);
	platform2->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform2->SetPosition(Vector2(320, 72 + 64));
	physics->AddCollider(platform2);

	CollisionVolume* platform3a = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 112 * 2, 8 * 2);
	platform3a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform3a->SetPosition(Vector2(112, 72 + 64 + 64));
	physics->AddCollider(platform3a);

	CollisionVolume* platform3b = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 48 * 2, 8 * 2);
	platform3b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform3b->SetPosition(Vector2(320, 72 + 64 + 64));
	physics->AddCollider(platform3b);

	CollisionVolume* platform4a = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 128 * 2, 8 * 2);
	platform4a->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform4a->SetPosition(Vector2(320 + 16, 72 + 64 + 64 + 64));
	physics->AddCollider(platform4a);

	CollisionVolume* platform4b = new RectangleCollider(CollisionVolume::objectType::GROUND, nullptr, 56 * 2, 8 * 2);
	platform4b->SetBehaviour(CollisionVolume::behaviour::STATIC);
	platform4b->SetPosition(Vector2(72, 72 + 64 + 64 + 64));
	physics->AddCollider(platform4b);

	

}


void GameMap::DrawMap(GameSimsRenderer & r) {
	r.DrawMesh((OGLMesh*)mapMesh, (OGLTexture*)tileTexture);
}

void GameMap::BuildMapMesh() {
	vector<Vector2> texCoords;
	vector<Vector3> positions;

	Vector2 primaryTile;

	AddNewTilePos(0, 0, positions, Vector2(mapWidth *16.0f, mapHeight * 16.0f));
	AddNewTileTex(0, 176, texCoords, Vector2(160, 128));

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileType = mapData[(y * mapWidth) + x];
	
			switch (tileType) {
			case '0': {
				continue;
			}break; //Top of ladder
				//the normal tile types first
				case 'T':primaryTile = Vector2(320, 0); break; //Top of ladder
				case 'L':primaryTile = Vector2(320, 16); break; //Ladder
				case 'B':primaryTile = Vector2(320, 32); break; //Bottom of ladder

				case 't':primaryTile = Vector2(336, 0); break; //Top of ladder
				case 'l':primaryTile = Vector2(336, 16); break; //Ladder
				case 'b':primaryTile = Vector2(336, 32); break; //Bottom of ladder

				case 'G': {
					primaryTile = Vector2(224, 48);
				}break;//Random grass tiles
				case 'F': { //Left End
					primaryTile = Vector2(208, 48);
				}break;//Random grass tiles
				case 'H': { //Right End
					primaryTile = Vector2(240, 48);
				}break;//Random grass tiles

				case 'W': {//wall
					primaryTile = Vector2(256, 112);
				}break;//Random grass tiles
				case 'w': {//wall top
					primaryTile = Vector2(256, 96);
				}break;//Random grass tiles
			}			
			AddNewTilePos(x, y, positions);
			AddNewTileTex((int)primaryTile.x, (int)primaryTile.y, texCoords);
		}
	}
	mapMesh = new OGLMesh();
	mapMesh->SetVertexPositions(positions);
	mapMesh->SetVertexTextureCoords(texCoords);
	mapMesh->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
	mapMesh->UploadToGPU();
}

void GameMap::AddNewTilePos(int x, int y, std::vector<Vector3>& pos, Vector2 tileSize) {
	Vector3 topLeft		= Vector3((x + 0) * tileSize.x, (y + 0) * tileSize.y, 0);
	Vector3 topRight	= Vector3((x + 1) * tileSize.x, (y + 0) * tileSize.y, 0);
	Vector3 bottomLeft	= Vector3((x + 0) * tileSize.x, (y + 1) * tileSize.y, 0);
	Vector3 bottomRight = Vector3((x + 1) * tileSize.x, (y + 1) * tileSize.y, 0);

	pos.emplace_back(topLeft);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(topRight);

	pos.emplace_back(topRight);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(bottomRight);
}

void GameMap::AddNewTileTex(int x, int y, std::vector<Vector2>& tex, Vector2 texSize) {
	Vector2 topLeft		= Vector2((x + 0.0f) , (y + texSize.y));
	Vector2 topRight	= Vector2((x + texSize.x), (y + texSize.y));
	Vector2 bottomLeft	= Vector2((x + 0.0f) , (y + 0.0f));
	Vector2 bottomRight = Vector2((x + texSize.x), (y + 0.0f));

	tex.emplace_back(topLeft);
	tex.emplace_back(bottomLeft); 
	tex.emplace_back(topRight);

	tex.emplace_back(topRight);
	tex.emplace_back(bottomLeft);
	tex.emplace_back(bottomRight);
}