#pragma once
#include "SimObject.h"
#include "FruitWizardGame.h"

namespace NCL::CSC3222 {
	class PlayerCharacter : public SimObject	{
	public:
		PlayerCharacter();
		~PlayerCharacter();

		bool canClimb = false;

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