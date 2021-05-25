#pragma once
#include <vector>
#include "GameSimsPhysics.h"

namespace NCL::CSC3222 {
	class GameMap;
	class SimObject;
	class GameSimsRenderer;
	class GameSimsPhysics;
	class TextureBase;
	class TextureManager;
	class PlayerCharacter;

	class FruitWizardGame {
	public:
		FruitWizardGame();
		~FruitWizardGame();

		void Update(float dt);

		void AddNewObject(SimObject* object);
		int magicCount;

		bool checkCollisionObject(CollisionPair*);

	protected:
		void InitialiseGame();

		TextureManager*		texManager;
		GameSimsRenderer*	renderer;
		GameSimsPhysics*	physics;
		GameMap* currentMap;

		PlayerCharacter* player;


		float gameTime;

		int currentScore;

		int dustCount;
		int lives;

		std::vector<SimObject*> gameObjects;
		std::vector<SimObject*> newObjects;
	};
}