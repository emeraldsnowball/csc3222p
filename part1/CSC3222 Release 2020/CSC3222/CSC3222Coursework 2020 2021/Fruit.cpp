#include "Fruit.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Fruit::Fruit() : SimObject() {
	texture = texManager->GetTexture("FruitWizard\\Fruit.png");

	int fruitID = rand() % 16;

	collider = new CircleCollider(CollisionVolume::objectType::FRUIT, 12.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);
	collider->SetPosition(position);
}

Fruit::~Fruit() {
}

bool Fruit::UpdateObject(float dt) {
	collider->SetPosition(position);
	return true;
}