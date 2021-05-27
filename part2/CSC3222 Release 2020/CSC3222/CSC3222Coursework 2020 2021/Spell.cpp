#include "Spell.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

#include "../../Common/Maths.h"

#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

using namespace NCL;
using namespace CSC3222;

Vector4 activeFrames[] = {
	Vector4(512,384, 32, 32),
	Vector4(512,384, 32, 32),
	Vector4(512,384, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(384,416, 32, 32),
};

Vector4 explodeFrames[] = {
	Vector4(512,384, 32, 32),
	Vector4(384,416, 32, 32),
	Vector4(416,416, 32, 32),
	Vector4(448,416, 32, 32),
	Vector4(480,416, 32, 32),
	Vector4(512,416, 32, 32),	
};

Spell::Spell(Vector2 direction, bool direction1) : SimObject()	{
	texture		= texManager->GetTexture("FruitWizard\\mini_fantasy_sprites_oga_ver.png");
	velocity	= direction;
	animFrameCount = 6;

	//timeSpawned = 0;

	this->SetPosition(direction);

	collider = new CircleCollider(CollisionVolume::objectType::SPELL, this, 12.0f);
	collider->SetBehaviour(CollisionVolume::behaviour::DYNAMIC);
	SetCollider(collider);

	collider->SetPosition(position);
	//SetMass(10000.0f);
	SetDamping(1.0f);
	
	//AddImpulse(Vector2((rand() % 2== 0? 100 : -250), 0) + Vector2(0, (float)(rand() % 60 - 30))); // add impulse at time of creation
	
	if (!direction1) {
		AddImpulse(Vector2(500, 0)); // add impulse at time of creation
	}
	else {
		AddImpulse(Vector2(-500, 0)); // add impulse at time of creation
	}

}

Spell::~Spell()	{
}

void Spell::DrawObject(GameSimsRenderer &r) {
	Vector4	texData = explodeFrames[currentanimFrame];
	Vector2 texPos	= Vector2(texData.x, texData.y);
	Vector2 texSize = Vector2(texData.z, texData.w);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}

bool Spell::UpdateObject(float dt) {
	animFrameData = explodeFrames[currentanimFrame];

	//timeSpawned += dt;

	if (deleteflag) {
		return false;
	}
	/*
	// remove spell 1 sec after spawn
	if (timeSpawned > 1.0f) {
		return false;
	}
	*/

	if (bounce < 1) {
		return false;
	}

	collider->SetPosition(position);
	return true;
}