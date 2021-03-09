#include "Fruit.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Fruit::Fruit() : SimObject() {
	texture = texManager->GetTexture("FruitWizard\\Fruit.png");

	int fruitID = rand() % 16;

}

Fruit::~Fruit() {
}

bool Fruit::UpdateObject(float dt) {
	return true;
}