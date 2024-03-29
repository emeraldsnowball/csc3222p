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
	physics		= new GameSimsPhysics(this);
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
			physics->RemoveRigidBody(*i);
			delete (*i);
			 i = gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}
	
	if (fruitCount == 0) {
		currentScore += 5000;
		fruitCount = 16;
		guardCount += 1;
		spawnFruit(16);
		spawnGuard(1, player);
	}
	
	// pixie dust spawn 30sec random
	//
	if ((int)gameTime > 0 && (int)gameTime % 30 == 0) {
		if (dustCount < 4) {
			if (rand() % 300 == 0) {
				spawnDust(1);
				dustCount++;
			}
		}
	}


	// print all colliders
	/*
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
	*/

	renderer->DrawString("Score:" + std::to_string(currentScore), 
		Vector2(32, 12), Vector4(1,1,1,1), 100.0f);
	renderer->DrawString("Lives:" + std::to_string(lives), 
		Vector2(144, 12), Vector4(1, 0,0, 1), 100.0f);
	renderer->DrawString("Magic:" + std::to_string(magicCount), 
		Vector2(256, 12), Vector4(1, 0, 0.5f, 1), 100.0f);
	renderer->DrawString("Dust:"  + std::to_string(dustCount), 
		Vector2(366, 12), Vector4(0.5f, 0.3f, 0.8f, 1), 100.0f);

	/*
	// draw boxes to approximate the values for game map collider
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

	//spawnPixie(8, player);

	//PixieDust* testDust = new PixieDust();
	//testDust->SetPosition(Vector2(285, 220));
	//AddNewObject(testDust);

	//Pixie* pixie = new Pixie();
	//pixie->SetPosition(Vector2(50, 96 - 64));
	//pixie->SetPosition(Vector2(370, 285));
	//pixie->SetSpringTarget(*player);
	//AddNewObject(pixie);

	Froggo* testFroggo = new Froggo();
	testFroggo->SetPosition(Vector2(370, 285));
	//testFroggo->SetPosition(Vector2(370, 32));
	AddNewObject(testFroggo);

	spawner(fruitCount, guardCount, player);

	gameTime		= 0;
	currentScore	= 0;
	magicCount		= 3;
	dustCount		= 0;
	lives			= 3;
	fruitCount = 16;
}

void FruitWizardGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}

bool FruitWizardGame::checkCollisionObject(CollisionPair* collisionData, float dt) {


	if (collisionData->c1->GetType() == CollisionVolume::objectType::BASE && !(collisionData->c2->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}

	if (collisionData->c2->GetType() == CollisionVolume::objectType::BASE && !(collisionData->c1->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}


	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::LADDER)) {
		static_cast<PlayerCharacter*>(collisionData->o1)->SetClimb();
		
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::LADDER)) {
		static_cast<PlayerCharacter*>(collisionData->o2)->SetClimb();
		
		return false;
	}
	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::GROUND) && static_cast<PlayerCharacter*>(collisionData->o1)->canClimb && (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP) || Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))) {
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::GROUND) && static_cast<PlayerCharacter*>(collisionData->o2)->canClimb && (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP) || Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN))) {
		return false;
	}

	if (collisionData->c1->GetType() == CollisionVolume::objectType::SPELL && ((collisionData->c2->GetType() == CollisionVolume::objectType::BASE) || (collisionData->c2->GetType() == CollisionVolume::objectType::GROUND) || (collisionData->c2->GetType() == CollisionVolume::objectType::WALL))) {
		static_cast<Spell*>(collisionData->o1)->bounce--;
		return true;
	}

	if (collisionData->c2->GetType() == CollisionVolume::objectType::SPELL && (collisionData->c1->GetType() == CollisionVolume::objectType::BASE) || (collisionData->c1->GetType() == CollisionVolume::objectType::GROUND) || (collisionData->c1->GetType() == CollisionVolume::objectType::WALL)) {
		static_cast<Spell*>(collisionData->o2)->bounce--;
		return true;
	}

	
	if (collisionData->c1->GetType() == CollisionVolume::objectType::WALL && !(collisionData->c2->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}

	if (collisionData->c2->GetType() == CollisionVolume::objectType::WALL && !(collisionData->c1->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}

	if (collisionData->c1->GetType() == CollisionVolume::objectType::GROUND && !(collisionData->c2->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}

	if (collisionData->c2->GetType() == CollisionVolume::objectType::GROUND && !(collisionData->c1->GetType() == CollisionVolume::objectType::PIXIE)) {
		return true;
	}
	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::PIXIEDUST)) {
		collisionData->o2->SetToDeleteObject();
		pixieCollect++;
		currentScore += 500;
		return false;
	}

	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::PIXIEDUST)) {
		collisionData->o1->SetToDeleteObject();
		pixieCollect++;
		currentScore += 500;
		return false;
	}

	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::FRUIT)) {
		collisionData->o2->SetToDeleteObject();
		currentScore += 1000;
		fruitCount--;
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::FRUIT)) {
		collisionData->o1->SetToDeleteObject();
		currentScore += 1000;
		fruitCount--;
		return false;
	}

	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::PIXIE)) {
		collisionData->o2->SetToDeleteObject();
		currentScore += 1000;
		magicCount++;
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::PIXIE)) {
		collisionData->o1->SetToDeleteObject();
		currentScore += 1000;
		magicCount++;
		return false;
	}

	

	if (collisionData->c1->GetType() == CollisionVolume::objectType::SPELL && (collisionData->c2->GetType() == CollisionVolume::objectType::GUARD)) {
		collisionData->o1->SetToDeleteObject();
		currentScore += 200;
		static_cast<Guard*>(collisionData->o2)->Stunned(dt);
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::SPELL && (collisionData->c1->GetType() == CollisionVolume::objectType::GUARD)) {
		collisionData->o2->SetToDeleteObject();
		currentScore += 200;
		static_cast<Guard*>(collisionData->o1)->Stunned(dt);
		return false;
	}

	if (collisionData->c1->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c2->GetType() == CollisionVolume::objectType::GUARD)) {
		if (static_cast<Guard*>(collisionData->o2)->isStunned) {
			return false;
		}
		
		if (static_cast<Guard*>(collisionData->o2)->isAttack) {
			for (Fruit* f : fruits) {
				f->SetToDeleteObject();
			}
			for (Guard* g : guards) {
				g->SetToDeleteObject();
			}
			player->SetPosition(Vector2(100, 32));
			lives--;
			spawner(fruitCount, guardCount, player);
		}
		return false;
	}
	if (collisionData->c2->GetType() == CollisionVolume::objectType::PLAYER && (collisionData->c1->GetType() == CollisionVolume::objectType::GUARD)) {
		if (static_cast<Guard*>(collisionData->o2)->isStunned) {
			return false;
		}
		
		if (static_cast<Guard*>(collisionData->o1)->isAttack) {
			for (Fruit* f : fruits) {
				f->SetToDeleteObject();
			}
			for (Guard* g : guards) {
				g->SetToDeleteObject();
			}
			player->SetPosition(Vector2(100, 32));
			lives--;
			spawner(fruitCount, guardCount, player);
		}
		return false;
	}
	
	return false;
}
