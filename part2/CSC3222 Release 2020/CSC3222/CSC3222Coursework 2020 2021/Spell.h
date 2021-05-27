#pragma once
#include "SimObject.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Spell : public SimObject {
		public:
			Spell(Vector2 direction, bool direction1);
			~Spell();

			//float timeSpawned; // so we can remove spell after certain time 

			void DrawObject(GameSimsRenderer &r) override;
			bool UpdateObject(float dt) override;
			int bounce = 3;
		};
	}
}

