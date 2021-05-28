#pragma once
#include "SimObject.h"
#include <vector>
#include "Guard.h"
namespace NCL::CSC3222 {
	class Pixie : public SimObject	{
	public:
		Pixie();
		~Pixie();
		bool UpdateObject(float dt) override;

		
		Vector2 Separation(std::vector<Pixie*> allBoids) {
			Vector2 dir;
			float separationThreshold = 15;
			for (Pixie* b : allBoids) {
				if (b == this) {
					continue;
				}

				float distance = (this->position - b->position).Length();

				if (distance > separationThreshold) {
					continue;
				}

				float strength = 1.0f - (distance / separationThreshold);
				dir += (this->position - b->position).Normalised() * strength*100;

			}
			
			return dir;

		}

		Vector2 Cohesion(std::vector<Pixie*> allBoids) {
			float cohesionThreshold = 30;
			Vector2 avgPos = this->position;
			float count = 1;

			for (Pixie* b : allBoids) {
				if (b == this) {
					continue;
				}

				float distance = (this->position - b->position).Length();

				if (distance > cohesionThreshold) {
					continue;
				}

				avgPos += b->position;
				count++;

			}

			avgPos /= count;

			Vector2 dir = avgPos - this->position;
			
			return dir;

		}

		Vector2 Avoidance(std::vector<Guard*> enemies) {
			Vector2 dir;
			float avoidanceThreshold = 50;

			for (SimObject* i : enemies) {
				float distance = (this->position - i->position).Length();

				if (distance > avoidanceThreshold) {
					continue;
				}

				dir += (this->position - i->position).Normalised() * 20;

			}
			return dir;
		}

		
	protected:
		
		
	};
}