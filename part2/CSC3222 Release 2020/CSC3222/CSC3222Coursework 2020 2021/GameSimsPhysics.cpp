#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"
#include <algorithm>
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

FruitWizardGame* GameSimsPhysics::game = nullptr;

GameSimsPhysics::GameSimsPhysics(FruitWizardGame* game)	{
	GameSimsPhysics::game = game;
}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	timeRemaining += dt;
	const float subTimeDelta = 1.0f / 120;

	// fixed time step update
	while (timeRemaining > subTimeDelta) {

		Integration(subTimeDelta);
		CollisionDetection(subTimeDelta);
		IntegrateVelocity(subTimeDelta);

		for ( unsigned int i = 0; i < allBodies.size(); i++) {
			allBodies[i]->force = Vector2(0, 0);
		}

		timeRemaining -= subTimeDelta;
	}
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);

	if (at != allColliders.end()) {
		//maybe delete too?
		allColliders.erase(at);
	}
}

void GameSimsPhysics::Integration(float dt) {
	
	for (unsigned int i = 0; i < allBodies.size(); i++) {
		Vector2 accelleration = allBodies[i]->force * allBodies[i]->inverseMass;
		allBodies[i]->velocity += accelleration * dt;
		allBodies[i]->velocity *= allBodies[i]->damping;
		//allBodies[i]->position += allBodies[i]->velocity * dt;
	}

}

void GameSimsPhysics::IntegrateVelocity(float dt) {
	for (unsigned int i = 0; i < allBodies.size(); i++) {
		allBodies[i]->position += allBodies[i]->velocity * dt;
	}
}

void GameSimsPhysics::CollisionDetection(float dt) {

	std::sort(allColliders.begin(), allColliders.end()); // sorting for broad phase

	for (unsigned int i = 0; i < allColliders.size() - 1; ++i) {
		for (unsigned int j = i + 1; j < allColliders.size(); ++j) {
			
			// get rid of comparision of objects filtered from broad phase for narrow phase
			if (allColliders[i]->GetMaxExtent() < allColliders[j]->GetMinExtent()) {
				continue;
			}
			
			// stop from all colliders firing at oonce at frame 1 when they are all at position (0,0)
			if (allColliders[i]->GetPosition() == Vector2(0, 0) || allColliders[j]->GetPosition() == Vector2(0, 0)) {
				continue;
			}
			// no need to compare set of two static object since they will never collide
			if (allColliders[i]->GetBehaviour() == CollisionVolume::behaviour::STATIC && allColliders[j]->GetBehaviour() == CollisionVolume::behaviour::STATIC) {
				continue;
			}
			CollisionPair collisionData = {};
			if (allColliders[i]->overlaps(*allColliders[j], collisionData)) {
				collisionData.c1 = allColliders[i];
				collisionData.c2 = allColliders[j];
				collisionData.o1 = allColliders[i]->GetRigidBody();
				collisionData.o2 = allColliders[j]->GetRigidBody();

				//print things here

				CollisionResolution(&collisionData);
				
				/*
				std::cout << "collision detected between: ";
				switch (allColliders[i]->GetType())
				{
				case CollisionVolume::objectType::PLAYER:
					std::cout << "PLAYER ";
					break;
				case CollisionVolume::objectType::SPELL:
					std::cout << "SPELL ";
					break;
				case CollisionVolume::objectType::PIXIE:
					std::cout << "PIXIE ";
					break;
				case CollisionVolume::objectType::PIXIEDUST:
					std::cout << "PIXIEDUST ";
					break;
				case CollisionVolume::objectType::GUARD:
					std::cout << "GUARD ";
					break;
				case CollisionVolume::objectType::FROGGO:
					std::cout << "FROGGO ";
					break;
				case CollisionVolume::objectType::FRUIT:
					std::cout << "FRUIT ";
					break;
				case CollisionVolume::objectType::LADDER:
					std::cout << "LADDER ";
					break;
				case CollisionVolume::objectType::WALL:
					std::cout << "WALL ";
					break;
				case CollisionVolume::objectType::GROUND:
					std::cout << "GROUND ";
					break;
				}

				std::cout << " & ";

				switch (allColliders[j]->GetType())
				{
				case CollisionVolume::objectType::PLAYER:
					std::cout << "PLAYER ";
					break;
				case CollisionVolume::objectType::SPELL:
					std::cout << "SPELL ";
					break;
				case CollisionVolume::objectType::PIXIE:
					std::cout << "PIXIE ";
					break;
				case CollisionVolume::objectType::PIXIEDUST:
					std::cout << "PIXIEDUST ";
					break;
				case CollisionVolume::objectType::GUARD:
					std::cout << "GUARD ";
					break;
				case CollisionVolume::objectType::FROGGO:
					std::cout << "FROGGO ";
					break;
				case CollisionVolume::objectType::FRUIT:
					std::cout << "FRUIT ";
					break;
				case CollisionVolume::objectType::LADDER:
					std::cout << "LADDER ";
					break;
				case CollisionVolume::objectType::WALL:
					std::cout << "WALL ";
					break;
				case CollisionVolume::objectType::GROUND:
					std::cout << "GROUND ";
					break;
				}

				std::cout << " \n";
				
				*/


			}
		}
	
	}

}

void GameSimsPhysics::CollisionResolution(CollisionPair* collisionData) {
	if (game->checkCollisionObject(collisionData)) {
		Projection(collisionData);
		Impulse(collisionData);
	}
}

void GameSimsPhysics::Projection(CollisionPair* collisionData){
	float im1 = 0;
	float im2 = 0; //inverse mass

	if (collisionData->o1) {
		im1 = collisionData->o1->inverseMass;
	}
	if (collisionData->o2) {
		im2 = collisionData->o2->inverseMass;
	}
	Vector2 pos1 = collisionData->c1->GetPosition() - (collisionData->collisionNormal * collisionData->overlapDistance) * im1 / (im1 + im2);
	Vector2 pos2 = collisionData->c2->GetPosition() - (collisionData->collisionNormal * collisionData->overlapDistance) * im2 / (im1 + im2);


	if(collisionData->o1)
		collisionData->o1->SetPosition(pos1);
	if(collisionData->o2)
		collisionData->o2->SetPosition(pos2);
}

void GameSimsPhysics::Impulse(CollisionPair* collisionData){
	float im1 = 0;
	float im2 = 0; //inverse mass
	Vector2 v1, v2 = Vector2(0, 0);

	if (collisionData->o1) {
		im1 = collisionData->o1->inverseMass;
		v1 = collisionData->o1->GetVelocity();
	}
	if (collisionData->o2) {
		im2 = collisionData->o2->inverseMass;
		v1 = collisionData->o2->GetVelocity();
	}

	Vector2 relVel = v2 - v1;
	float totalInverseMass = im1 + im2;
	float impulse;
	float e = 1.0;
	Vector2 CollisionNormal = collisionData->collisionNormal;
	

	if (totalInverseMass != 0) {

		impulse = ((-(1 + e) * (Vector2::Dot(relVel, CollisionNormal)))) / totalInverseMass;
		if (collisionData->o1) {
			collisionData->o1->velocity = collisionData->o1->velocity - CollisionNormal * im1 * impulse;
		}
		if (collisionData->o2) {
			collisionData->o2->velocity = collisionData->o2->velocity - CollisionNormal * im2 * impulse;
		}

	}

	
}
