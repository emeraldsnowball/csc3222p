#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"
#include "FruitWizardGame.h"
#include "Spell.h"


using namespace NCL;
using namespace CSC3222;

Vector4 runFrames[] = {
	Vector4(64,160, 32, 32),
	Vector4(96,160, 32, 32),
	Vector4(128,160, 32, 32),
	Vector4(160,160, 32, 32),
	Vector4(192,160, 32, 32),
	Vector4(224,160, 32, 32),
};

Vector4 attackFrames[] = {
	Vector4(128,288, 32, 30),
	Vector4(64,224, 32, 32),
	Vector4(160,288, 32, 30),
	Vector4(96,224, 32, 32),
	Vector4(192,288, 32, 29),
	Vector4(64,256, 32, 32)
};

Vector4 idleFrames[] = {
	Vector4(64,128, 32, 32),
	Vector4(96,128, 32, 32),
	Vector4(128,128, 32, 32),
	Vector4(160,128, 32, 32),
	Vector4(128,128, 32, 32),
	Vector4(224,128, 32, 32)
};

Vector4 fallFrames[] = {
	Vector4(64,320, 32, 32),
	Vector4(64,320, 32, 32),
	Vector4(64,320, 32, 32),
	Vector4(96,320, 32, 32),
	Vector4(96,320, 32, 32),
	Vector4(96,320, 32, 32)
};

Vector4 deathFrames[] = {
	Vector4(96,352, 32, 32),
	Vector4(128,352, 32, 32),
	Vector4(96,352, 32, 32),	
	Vector4(128,352, 32, 32),
	Vector4(96,352, 32, 32),
	Vector4(128,352, 32, 32),
};

Vector4 ladderFrames[] = {//Not an ideal spritesheet for ladders!
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
	Vector4(64,224, 32, 32),
};


PlayerCharacter::PlayerCharacter() : SimObject() {
	currentAnimState	= PlayerState::Left;
	texture				= texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	animFrameCount		= 6;

	//collider = new RectangleCollider(CollisionVolume::objectType::PLAYER, 10.0f, 12.0f);
	collider = new CircleCollider(CollisionVolume::objectType::PLAYER, 12.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);
	collider->SetPosition(position);
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {
	float testSpeed = 64;
	Vector4* animSource = idleFrames;
	Vector2 newVelocity;

	if (currentAnimState == PlayerState::Attack) {
		animSource = attackFrames;
		if (currentanimFrame >= 5) {
			currentAnimState = PlayerState::Idle;
		}
	}
	else {
		currentAnimState = PlayerState::Idle;
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			animSource = runFrames;
			currentAnimState = PlayerState::Left;
			//newVelocity.x = -testSpeed * dt;
			AddForce(Vector2(-1200, 0));
			flipAnimFrame = true;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			animSource = runFrames;
			currentAnimState = PlayerState::Right;
			//newVelocity.x = testSpeed * dt;
			AddForce(Vector2(1200, 0));
			flipAnimFrame = false;
		}	
		if (game->magicCount > 0 && Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			currentAnimState = PlayerState::Attack;
			currentanimFrame = 0;
			game->AddNewObject(new Spell(this->GetPosition())); // create spell on attack
			game->magicCount--;
		}
	}

	position += newVelocity;

	collider->SetPosition(position);

	animFrameData = animSource[currentanimFrame];

	return true;
}