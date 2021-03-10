#include "Fruit.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Fruit::Fruit() : SimObject() {
	texture = texManager->GetTexture("FruitWizard\\Fruit.png");

	int fruitID = rand() % 16;

	animFrameData = Vector4((fruitID / 4) * 16.0f, (fruitID % 4) * 16.0f, 16.0f, 16.0f);

	
	collider = new CircleCollider(CollisionVolume::objectType::FRUIT, 6.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::STATIC);
	SetCollider(collider);
	collider->SetPosition(position);
	
}

Fruit::~Fruit() {
}

bool Fruit::UpdateObject(float dt) {
	collider->SetPosition(position);
	return true;
}