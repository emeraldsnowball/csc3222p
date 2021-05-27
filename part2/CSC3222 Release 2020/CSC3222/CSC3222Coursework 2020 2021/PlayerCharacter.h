#pragma once
#include "SimObject.h"
#include "FruitWizardGame.h"

namespace NCL::CSC3222 {
	class PlayerCharacter : public SimObject	{
	public:
		PlayerCharacter();
		~PlayerCharacter();
		float climbTimer;
		bool canClimb = false;
		void SetClimb() {
			canClimb = true;
			climbTimer = 0.0f;
		}
		bool direction = true;
		bool UpdateObject(float dt) override;
	protected:
		enum class PlayerState {
			Left,
			Right,
			Attack,
			Fall,
			Die,
			Idle,
			Climb

		};
		PlayerState		currentAnimState;
	};
}