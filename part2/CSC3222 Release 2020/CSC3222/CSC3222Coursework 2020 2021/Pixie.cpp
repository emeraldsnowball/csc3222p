#include "Pixie.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "FruitWizardGame.h"
#include <vector>

using namespace NCL;
using namespace CSC3222;

Vector4 pixieFrames[] = {
	Vector4(32 * 1,32 * 4, 32, 32),
	Vector4(32 * 2,32 * 4, 32, 32),
	Vector4(32 * 3,32 * 4, 32, 32),
	Vector4(32 * 4,32 * 4, 32, 32),
};

Pixie::Pixie() : SimObject() {
	texture			= texManager->GetTexture("FruitWizard\\super_random_sprites.png");
	animFrameCount	= 4;

	collider = new CircleCollider(CollisionVolume::objectType::PIXIE, this, 12.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);
	collider->SetPosition(position);

	//SetMass(5);
}

Pixie::~Pixie() {
}

bool Pixie::UpdateObject(float dt) {
	animFrameData = pixieFrames[currentanimFrame];
	if (deleteflag) {
		return false;
	}
	UpdateSprings();
	collider->SetPosition(position);

	
	Vector2 dir;

	dir += Separation(game->getPixies());
	dir += Cohesion(game->getPixies());
	dir += Avoidance(game->getGuards());

	this->position += dir * dt;

	

	return true;
}