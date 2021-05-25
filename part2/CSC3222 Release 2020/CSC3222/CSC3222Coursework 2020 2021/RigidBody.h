#pragma once
#include "../../Common/Vector2.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class RigidBody	{
			friend class GameSimsPhysics;
		public:
			
			RigidBody();
			~RigidBody();

			Vector2 GetPosition() const {
				return position;
			}

			void SetPosition(const Vector2& newPosition) {
				position = newPosition;
			}

			Vector2 GetVelocity() const {
				return velocity;
			}

			void SetVelocity(const Vector2& newVelocity) {
				velocity = newVelocity;
			}

			void AddForce(const Vector2& newForce) {
				force += newForce;
			}

			void AddImpulse(const Vector2& impulse) {

				velocity += impulse * inverseMass;
			}
			/*
			void SetMass(float newMass) {
				if (newMass == 0) {
					inverseMass = 0;
				}
				else
				{
					inverseMass = 1 / newMass;
				}
			}
			*/
			void SetDamping(float newdamping) {
				damping = newdamping;
			}

			void SetToDeleteObject() {
				deleteflag = true;
			}

		protected:
			Vector2 position;
			Vector2 velocity;
			Vector2 force;

			bool deleteflag = false;
			
			float inverseMass;
			float elasticity;
			float damping = 0.94f;

		};
	}
}
