#pragma once
#include "SimObject.h"

namespace NCL::CSC3222 {
	class Guard : public SimObject	{
	public:
		Guard(RigidBody* player);
		~Guard();
		bool UpdateObject(float dt) override;
		
		float StunTimer = 0.0f;
		void Stunned(float dt) {
			StunTimer = dt;
			currentAnimState = GuardState::Stunned;
			isStunned = true;
		}
		bool isStunned = false;

		bool walkFlip = false;

		bool isAttack = false;

		RigidBody* PlayerPointer;
		void setPlayer(RigidBody* player) {
			PlayerPointer = player;
		}
		
	protected:
		enum class GuardState {
			Stunned,
			Attack,
			Walk

		};
		GuardState		currentAnimState;

	};
}