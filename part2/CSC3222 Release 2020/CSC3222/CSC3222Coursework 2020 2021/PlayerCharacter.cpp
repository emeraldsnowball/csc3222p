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
	climbTimer = 0;

	collider = new CircleCollider(CollisionVolume::objectType::PLAYER, this, 13.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);
	collider->SetPosition(position);
	SetDamping(0.96f);
	SetMass(3);
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt) {
	float testSpeed = 64;
	Vector4* animSource = idleFrames;
	Vector2 newVelocity;
	AddForce(Vector2(0, -300));
	
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
			AddForce(Vector2(-1500, 0));
			flipAnimFrame = true;
			direction = true;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			animSource = runFrames;
			currentAnimState = PlayerState::Right;
			//newVelocity.x = testSpeed * dt;
			AddForce(Vector2(1500, 0));
			flipAnimFrame = false;
			direction = false;
		}	
		if (game->magicCount > 0 && Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			currentAnimState = PlayerState::Attack;
			currentanimFrame = 0;
			game->AddNewObject(new Spell(this->GetPosition(), direction)); // create spell on attack
			game->magicCount--;
		}
		if (canClimb && Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			animSource = ladderFrames;
			currentAnimState = PlayerState::Climb;
			AddForce(Vector2(0, 1000));
			flipAnimFrame = false;
		}
		if (canClimb && Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			animSource = ladderFrames;
			currentAnimState = PlayerState::Climb;
			AddForce(Vector2(0, -1000));
			flipAnimFrame = true;
		}
		
		if (game->pixieCollect > 3 && Window::GetKeyboard()->KeyPressed(KeyboardKeys::CONTROL)) {
			game->spawnPixie(8, this);
			game->pixieCollect = 0;
		}
		
	}
	
	position += newVelocity;

	collider->SetPosition(position);

	animFrameData = animSource[currentanimFrame];
	climbTimer += dt;
	if (climbTimer > 0.01f) {
		canClimb = false;
	}

	return true;
}