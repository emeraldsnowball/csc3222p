#pragma once
#include "../../Common/Vector2.h"
namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class CollisionVolume {
		public:
			CollisionVolume();
			~CollisionVolume();
			Vector2 GetPosition() const {
				return position;
			};

			void SetPosition(const Vector2& newPosition) {
				position = newPosition;
			};

			enum class objectType {
				PLAYER,
				SPELL,
				PIXIE,
				PIXIEDUST,
				GUARD,
				FROGGO,
				FRUIT,
				LADDER,
				WALL,
				GROUND
			};


			objectType GetType() const {
				return type;
			};

			void SetType(const objectType newtype) {
				type = newtype;
			};

			enum class behaviour {
				STATIC,
				DYNAMIC
			};

			behaviour GetBehaviour() {
				return behaviour;
			};

			void SetBehaviour(const behaviour newbehaviour) {
				behaviour = newbehaviour;
			};


			virtual bool overlaps(const CollisionVolume&) const = 0;

			char shape; // just to help draw debug shapes
		protected:
			behaviour behaviour;
			objectType type;
			Vector2 position;
		};
	}
}

