#pragma once
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
namespace NCL {
	using namespace Maths;
    class RigidBody;
	namespace CSC3222 {
		class CircleCollider : public CollisionVolume {
        public:
            explicit CircleCollider(objectType type, RigidBody* rb, float radius = 1);
            CircleCollider(const CircleCollider& );

            ~CircleCollider();

            // Implement pure virtual functions from base
            bool overlaps(const CollisionVolume&, CollisionPair& collisiondata) const override;
            
            // Return the radius of the circle
            float radius() const;
            float GetMinExtent() const override;
            float GetMaxExtent() const override;

        private:
            float radius_;

		};

		class RectangleCollider : public CollisionVolume {
        public:
            explicit RectangleCollider(objectType type, RigidBody* rb, float length = 1, float width = 1);
            RectangleCollider(const RectangleCollider&);

            ~RectangleCollider();

            bool overlaps(const CollisionVolume&, CollisionPair& collisiondata) const override;

            float length() const;

            float width() const;

            float GetMinExtent() const override;
            float GetMaxExtent() const override;

        private:
            float length_;
            float width_;
		};

        class CollisionManager {

        public:
            // Determine the type of a pointer to base before sending for calculation
            static bool collides(const RectangleCollider&, const std::shared_ptr<CollisionVolume>&, CollisionPair& collisiondata);
            static bool collides(const CircleCollider&, const std::shared_ptr<CollisionVolume>&, CollisionPair& collisiondata);

            // Calculate a rectangle-rectangle, circle-circle, or circle-rectangle collision
            static bool collides(const RectangleCollider&, const RectangleCollider&, CollisionPair& collisiondata);
            static bool collides(const CircleCollider&, const CircleCollider&, CollisionPair& collisiondata);
            static bool collides(const CircleCollider&, const RectangleCollider&, CollisionPair& collisiondata);

            // Helper method to limit a value p to within min and max range
            static float clamp(float p, float min, float max);

        };

	}
}