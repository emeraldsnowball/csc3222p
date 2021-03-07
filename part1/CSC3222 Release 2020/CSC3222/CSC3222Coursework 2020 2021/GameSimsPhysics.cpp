#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"

using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	timeRemaining += dt;
	const float subTimeDelta = 1.0f / 120;

	while (timeRemaining > subTimeDelta) {

		Integration(subTimeDelta);
		CollisionDetection(subTimeDelta);
		IntegrateVelocity(subTimeDelta);

		for (int i = 0; i < allBodies.size(); i++) {
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
	
	for (int i = 0; i < allBodies.size(); i++) {
		Vector2 accelleration = allBodies[i]->force * allBodies[i]->inverseMass;
		allBodies[i]->velocity += accelleration * dt;
		allBodies[i]->velocity *= 0.94f;
		//allBodies[i]->position += allBodies[i]->velocity * dt;
	}

}

void GameSimsPhysics::IntegrateVelocity(float dt) {
	for (int i = 0; i < allBodies.size(); i++) {
		allBodies[i]->position += allBodies[i]->velocity * dt;
	}
}

void GameSimsPhysics::CollisionDetection(float dt) {

	for (int i = 0; i < allColliders.size() - 1; ++i) {
		for (int j = i + 1; j < allColliders.size(); ++j) {

			if (allColliders[i]->GetPosition() == Vector2(0, 0)) {
				continue;
			}

			if (allColliders[i]->overlaps(*allColliders[j])) {
				std::cout << "collision detected between: "
					<< ((allColliders[i]->shape == 'c') ? "circle " : "rectangle ")
					<< i << " & " 
					<< ((allColliders[i]->shape == 'c') ? " circle " : " rectangle ")
					<< j << "\n";
			}
		}
	}

}