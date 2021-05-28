#pragma once
#include <vector>
#include "GameSimsPhysics.h"
#include "Fruit.h"
#include "Guard.h"
#include "PixieDust.h"
#include "Pixie.h"

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

		bool checkCollisionObject(CollisionPair*, float dt);

		void spawner(int fruitCount, int GuardCount, RigidBody* player) {
			spawnFruit(fruitCount);
			spawnGuard(GuardCount, player);
		}

		void spawnFruit(int count) {
			srand(time(0));
			float arrY[4] = { 8, 72, 136, 200 };
			for (int i = 0; i < count; i++) {
				float arrX[4] = { (float)(rand() % 320) + 32 , (float)(rand() % 280) + 160, (float)(rand() % 280) + 160 , (float)(rand() % 224) + 32 };
				Fruit* temp = new Fruit();
				int tempi = (rand() % 4);
				float x = arrX[tempi];
				float y = arrY[tempi] + 16;
				//float y = 22;
				temp->SetPosition(Vector2(x, y));
				AddNewObject(temp);
				fruits.emplace_back(temp);
			}
		}

		void spawnGuard(int GuardCount, RigidBody* player) {
			//srand(time(0));
			float arrY[4] = { 8, 72, 136, 200 };
			for (int i = 0; i < GuardCount; i++) {
				float arrX[4] = { 240,(rand() % 2 == 0) ? 320 : 80 , 320, 112 };
				Guard* temp = new Guard(player);
				float x = arrX[i];
				float y = arrY[i] + 24;
				temp->SetPosition(Vector2(x, y));
				AddNewObject(temp);
				guards.emplace_back(temp);
			}
		}

		void spawnDust(int count) {
			//srand(time(0));
			float arrY[4] = { 8, 72, 136, 200 };
			for (int i = 0; i < count; i++) {
				float arrX[4] = { (float)(rand() % 320) + 32 , (float)(rand() % 280) + 160, (float)(rand() % 280) + 160 , (float)(rand() % 224) + 32 };
				PixieDust* temp = new PixieDust();
				int tempi = (rand() % 4);
				float x = arrX[tempi];
				float y = arrY[tempi] + 24;
				//float y = 22;
				temp->SetPosition(Vector2(x, y));
				AddNewObject(temp);
			}
		}

		void spawnPixie(int count, RigidBody* player) {
			//srand(time(0));
			float arrY[4] = { 8, 72, 136, 200 };
			for (int i = 0; i < count; i++) {
				float arrX[4] = { (float)(rand() % 320) + 32 , (float)(rand() % 280) + 160, (float)(rand() % 280) + 160 , (float)(rand() % 224) + 32 };
				Pixie* temp = new Pixie();
				int tempi = (rand() % 4);
				float x = arrX[tempi];
				float y = arrY[tempi] + 24;
				//float y = 22;
				temp->SetPosition(Vector2(x, y));
				temp->SetSpringTarget(*player);
				AddNewObject(temp);
				pixies.emplace_back(temp);
			}
		}

		GameMap* GetCurrentMap() {
			return currentMap;
		}
		int lives;

		std::vector<Pixie*> getPixies() {
			return pixies;
		}

		std::vector<Guard*> getGuards() {
			return guards;
		}

		int pixieCollect = 0;
	protected:
		void InitialiseGame();

		TextureManager*		texManager;
		GameSimsRenderer*	renderer;
		GameSimsPhysics*	physics;
		GameMap* currentMap;

		PlayerCharacter* player;


		float gameTime;

		int currentScore;

		int dustCount = 0;
		int fruitCount = 16;
		int guardCount = 4;

		std::vector<SimObject*> gameObjects;
		std::vector<SimObject*> newObjects;

		
		std::vector<Fruit*> fruits;
		std::vector<Guard*> guards;
		std::vector<Pixie*> pixies;
	};
}