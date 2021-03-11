#include "FruitWizardGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "Fruit.h"
#include "Guard.h"
#include "Froggo.h"
#include "Pixie.h"
#include "PixieDust.h"
#include "Spell.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

FruitWizardGame::FruitWizardGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics();
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

FruitWizardGame::~FruitWizardGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}

void FruitWizardGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;

	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		(*i)->UpdateAnimFrame(dt);
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
			physics->RemoveCollider((*i)->GetCollider());
			delete (*i);
			 i = gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			/*
			CollisionVolume* collider = (*i)->GetCollider();
			if (collider) {
				if (collider->shape == 'c') {
					const CircleCollider* c = dynamic_cast<const CircleCollider*> (collider);

					renderer->DrawCircle(collider->GetPosition(), c->radius(), Vector4(0, 1, 1, 1));
					
				}
				else if (collider->shape == 'r') {
					const RectangleCollider* r = dynamic_cast<const RectangleCollider*> (collider);
					renderer->DrawBox(collider->GetPosition(), Vector2(r->length(), r->width()), Vector4(0, 1, 1, 1));
				}
			}
			*/
			++i;
		}
	}

	for (CollisionVolume* collider : physics->GetAllColliders()) {
		if (collider->shape == 'c') {
			const CircleCollider* c = dynamic_cast<const CircleCollider*> (collider);

			renderer->DrawCircle(collider->GetPosition(), c->radius(), Vector4(0, 1, 1, 1));

		}
		else if (collider->shape == 'r') {
			const RectangleCollider* r = dynamic_cast<const RectangleCollider*> (collider);
			renderer->DrawBox(collider->GetPosition(), Vector2(r->length()/2, r->width()/2), Vector4(0, 1, 1, 1));
		}
	}


	renderer->DrawString("Score:" + std::to_string(currentScore), 
		Vector2(32, 12), Vector4(1,1,1,1), 100.0f);
	renderer->DrawString("Lives:" + std::to_string(lives), 
		Vector2(144, 12), Vector4(1, 0,0, 1), 100.0f);
	renderer->DrawString("Magic:" + std::to_string(magicCount), 
		Vector2(256, 12), Vector4(1, 0, 0.5f, 1), 100.0f);
	renderer->DrawString("Dust:"  + std::to_string(dustCount), 
		Vector2(366, 12), Vector4(0.5f, 0.3f, 0.8f, 1), 100.0f);

	/*

	// draw game map colliders 
	// wall
	renderer->DrawBox(Vector2(8, 160), Vector2(8.0f, 160), Vector4(1,0,0,1));
	renderer->DrawBox(Vector2(472, 160), Vector2(8.0f, 160), Vector4(1, 0, 0, 1));
	//ground
	renderer->DrawBox(Vector2(240, 8), Vector2(240, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(80, 72), Vector2(64, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(320, 72), Vector2(144, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(320, 72 + 64), Vector2(144, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(112, 72 + 128), Vector2(112, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(320, 72 + 128), Vector2(48, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(320 + 16, 72 + 128 + 64), Vector2(128, 8), Vector4(1, 0, 0, 1));
	renderer->DrawBox(Vector2(72, 72 + 128 + 64), Vector2(56, 8), Vector4(1, 0, 0, 1));
	//ladders
	renderer->DrawBox(Vector2(112, 48), Vector2(10, 32), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(256, 48), Vector2(10, 32), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(32, 48+96), Vector2(10, 32*2), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(368, 48+64), Vector2(10, 32), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(192, 48 + 64 + 64), Vector2(10, 32), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(304, 48 + 64 + 64), Vector2(10, 32), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(432, 48 + 64 + 96), Vector2(10, 32 * 2), Vector4(1, 0.5, 0, 1));
	renderer->DrawBox(Vector2(96, 48 + 64 + 64 + 64), Vector2(10, 32), Vector4(1, 0.5, 0, 1));


	*/
	/*
	Some examples of debug rendering! 
	These all use coordinates in the same 'space' as the game positions
	
	renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));

	renderer->DrawCircle(player->GetPosition(), 10.0f, Vector4(1, 0, 1, 1));
	renderer->DrawBox(player->GetPosition(), Vector2(8, 8), Vector4(1, 0, 0, 1));
	*/

	//collider drawing
	//renderer->DrawCircle(player->GetCollider()->GetPosition(), 12.0f, Vector4(1, 1, 1, 1));

	renderer->Render();

	// tutorial 1 

	//player->SetPosition(player->GetPosition() + Vector3(1,0,0) * 50 *dt );


}

void FruitWizardGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("FruitWizardMap.txt", gameObjects, *texManager, physics);

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	player = new PlayerCharacter();
	player->SetPosition(Vector2(100, 32));
	AddNewObject(player);

	Guard* testGuard = new Guard();
	//testGuard->SetPosition(Vector2(150, 224));
	testGuard->SetPosition(Vector2(200, 32));
	AddNewObject(testGuard);

	//Spell* testSpell = new Spell(Vector2(1,0));
	//testSpell->SetPosition(Vector2(160, 48));
	//AddNewObject(testSpell);

	Fruit* testFruit = new Fruit();
	testFruit->SetPosition(Vector2(250, 150));
	AddNewObject(testFruit);

	PixieDust* testDust = new PixieDust();
	testDust->SetPosition(Vector2(285, 220));
	AddNewObject(testDust);

	Pixie* pixie = new Pixie();
	pixie->SetPosition(Vector2(50, 96 - 64));
	pixie->SetSpringTarget(*player);
	AddNewObject(pixie);

	Froggo* testFroggo = new Froggo();
	testFroggo->SetPosition(Vector2(370, 285));
	//testFroggo->SetPosition(Vector2(370, 32));
	AddNewObject(testFroggo);

	gameTime		= 0;
	currentScore	= 0;
	magicCount		= 3;
	dustCount		= 0;
	lives			= 3;
}

void FruitWizardGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}