#include "Guard.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameMap.h"
#include "FruitWizardGame.h"

using namespace NCL;
using namespace CSC3222;

Vector4 guardIdleFrames[] = {
	Vector4(384,32, 64, 32),
	Vector4(448,32, 64, 32),	
	Vector4(512,32, 64, 32),	
	Vector4(576,32, 64, 32),	
	Vector4(448,64, 64, 32),	
	Vector4(512,64, 64, 32),
};

Vector4 guardWalkFrames[] = {
	Vector4(384,96, 64, 32),
	Vector4(448,96, 64, 32),
	Vector4(512,96, 64, 32),
	Vector4(576,96, 64, 32),
	Vector4(640,96, 64, 32),
	Vector4(704,96, 64, 32),
};

Vector4 guardAttackFrames[] = {
	Vector4(384,128, 64, 32),
	Vector4(448,128, 64, 32),
	Vector4(512,128, 64, 32),
	Vector4(576,128, 64, 32),
	Vector4(640,128, 64, 32),
	Vector4(704,128, 64, 32),
};

Vector4 guardStunFrames[] = {
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
	Vector4(448,160, 64, 32),
};

Guard::Guard(RigidBody* player) : SimObject() {
	currentAnimState = GuardState::Walk;
	texture = texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	animFrameCount	= 6;
	setPlayer(player);
	//SetMass(1.0f);
	collider = new RectangleCollider(CollisionVolume::objectType::GUARD, this, 2*10.0f, 2*12.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);
	collider->SetPosition(position);
}

Guard::~Guard() {
}

bool Guard::UpdateObject(float dt) {


	
	isAttack = false;
	if (abs(PlayerPointer->position.y - position.y) < 32 && abs(PlayerPointer->position.x - position.x) < 17) {
		currentAnimState = GuardState::Attack;
		isAttack = true;
	}


	if (currentAnimState == GuardState::Walk) {
		animFrameData = guardWalkFrames[currentanimFrame];

		if (abs(PlayerPointer->position.y - position.y) < 32 && abs(PlayerPointer->position.x - position.x) < 64) {
			if (PlayerPointer->position.x - position.x > 0 && !(game->GetCurrentMap()->CheckTile(position - Vector2(0, 32)) == 'H')) {
				this->SetPosition(this->GetPosition() + Vector3(1, 0, 0) * 10 * dt);
				flipAnimFrame = true;
				
			}
			else if (PlayerPointer->position.x - position.x < 0 && !(game->GetCurrentMap()->CheckTile(position - Vector2(0, 32)) == 'F')) {
				this->SetPosition(this->GetPosition() + Vector3(-1, 0, 0) * 10 * dt);
				flipAnimFrame = false;
			}
		}

		else if (!walkFlip) {
			this->SetPosition(this->GetPosition() + Vector3(1, 0, 0) * 10 * dt);
			flipAnimFrame = true;
		}
		else {
			this->SetPosition(this->GetPosition() + Vector3(-1, 0, 0) * 10 * dt);
			flipAnimFrame = false;
		}


		
		
		if (game->GetCurrentMap()->CheckTile(position + Vector2(16, 0)) == 'W' || game->GetCurrentMap()->CheckTile(position - Vector2(0, 32)) == 'H') {
			walkFlip = true;
		}

		if (game->GetCurrentMap()->CheckTile(position - Vector2(16, 0)) == 'W' || game->GetCurrentMap()->CheckTile(position - Vector2(0, 32)) == 'F') {
			walkFlip = false;
		}


	}
	if (currentAnimState == GuardState::Stunned) {
		animFrameData = guardStunFrames[currentanimFrame];
		AddForce(Vector2(0, -10));

	}
	if (currentAnimState == GuardState::Attack && !isStunned) {
		animFrameData = guardAttackFrames[currentanimFrame];
		//currentAnimState = GuardState::Walk;
	}


	//this->SetPosition(this->GetPosition() + Vector3(1,0,0) * 10 * dt);
	collider->SetPosition(position);

	StunTimer += dt;
	if (StunTimer > 5.0f) {
		currentAnimState = GuardState::Walk;
		isStunned = false;
	}

	if (deleteflag) {
		return false;
	}
	//AddForce(Vector2(0, -1));
	return true;
}