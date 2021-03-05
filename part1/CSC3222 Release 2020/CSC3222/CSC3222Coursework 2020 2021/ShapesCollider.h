#pragma once
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class CircleCollider : public CollisionVolume {
        public:
            explicit CircleCollider(objectType type, float radius = 1);
            CircleCollider(const CircleCollider& );

            ~CircleCollider();

            // Implement pure virtual functions from base
            bool overlaps(const std::shared_ptr<CollisionVolume>&) const override;
            
            // Return the radius of the circle
            float radius() const;

        private:
            float radius_;

		};

		class RectangleCollider : public CollisionVolume {
        public:
            explicit RectangleCollider(objectType type, float length = 1, float width = 1);
            RectangleCollider(const RectangleCollider&);

            ~RectangleCollider();

            // Implement pure virtual functions from base
            bool overlaps(const std::shared_ptr<CollisionVolume>&) const override;

            // Return size of the square
            float length() const;

            float width() const;

        private:
            float length_;
            float width_;
		};

        class CollisionManager {

        public:
            // Determine the type of a pointer to base before sending for calculation
            static bool collides(const RectangleCollider&, const std::shared_ptr<CollisionVolume>&);
            static bool collides(const CircleCollider&, const std::shared_ptr<CollisionVolume>&);

            // Calculate a square-square, circle-circle, or circle-square collision
            static bool collides(const RectangleCollider&, const RectangleCollider&);
            static bool collides(const CircleCollider&, const CircleCollider&);
            static bool collides(const CircleCollider&, const RectangleCollider&);

            // Helper method to limit a value p to within min and max range
            static double clamp(double p, double min, double max);

        };

	}
}