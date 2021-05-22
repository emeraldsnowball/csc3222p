#pragma once
#include <vector>
#include "../../Common/Vector2.h"

namespace NCL {
	namespace CSC3222 {
		using namespace Maths;
		class RigidBody;
		class CollisionVolume;

		struct CollisionPair {
			RigidBody* o1;
			RigidBody* o2;
			CollisionVolume* c1;
			CollisionVolume* c2;
			Vector2 collisionNormal;
			float overlapDistance;
		};

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

			void CollisionResolution(float dt);

			float timeRemaining = 0;

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
		};
	}
}

