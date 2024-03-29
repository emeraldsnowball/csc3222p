#pragma once
#include <vector>
#include "../../Common/Vector2.h"

namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

			std::vector<CollisionVolume*> GetAllColliders() {
				return allColliders;
			};

		protected:
			void Integration(float dt);
			void CollisionDetection(float dt);
			void IntegrateVelocity(float dt);

			float timeRemaining = 0;

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
		};
	}
}

