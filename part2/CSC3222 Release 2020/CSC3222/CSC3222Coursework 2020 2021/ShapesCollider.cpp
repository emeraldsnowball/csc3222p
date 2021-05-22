#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "ShapesCollider.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {

        // Direct a rectangle-shape* collision
        bool CollisionManager::collides(
            const RectangleCollider& lhs,
            const std::shared_ptr<CollisionVolume>& rhs, CollisionPair& collisiondata)
        {
            // If we successfully cast rhs to rectangle, perform rectangle-rectangle comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square, collisiondata);
            // If we successfully cast rhs to circle, perform circle-rectangle comparison
            if (auto circle = std::dynamic_pointer_cast<CircleCollider>(rhs))
                return collides(*circle, lhs, collisiondata);

            // Else there is no implementation yet for the given shape
            throw std::bad_cast();
        }
        
        // Direct a circle-shape* collision
        bool CollisionManager::collides(
            const CircleCollider& lhs,
            const std::shared_ptr<CollisionVolume>& rhs, CollisionPair& collisiondata)
        {
            // If we successfully cast rhs to square, perform circle-rectangle comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square, collisiondata);
            // If we successfully cast rhs to circle, perform circle-circle comparison
            if (auto circle = std::dynamic_pointer_cast<CircleCollider>(rhs))
                return collides(lhs, *circle, collisiondata);

            // Else there is no implementation yet for the given shape
            //TODO throw an exception instead?
            return false;
        }

        // Calculate a rectangle-rectangle collision
        bool CollisionManager::collides(const RectangleCollider& lhs, const RectangleCollider& rhs, CollisionPair& collisiondata)
        {
            Vector2 posA = lhs.GetPosition();
            Vector2 posB = rhs.GetPosition();
            Vector2 delta = posB - posA;
            Vector2 totalSize = Vector2(lhs.length() / 2, lhs.width() / 2) + Vector2(rhs.length() / 2, rhs.width() / 2);

            if (fabs(delta.x) < totalSize.x && fabs(delta.y) < totalSize.y) {
                float penX = totalSize.x - delta.x;
                float penY = totalSize.y - delta.y;
                if (penX < penY) {
                    collisiondata.overlapDistance = penX;

                    if(posA.x > posB.x)
                        collisiondata.collisionNormal = Vector2(1, 0);
                    if (posA.x < posB.x)
                        collisiondata.collisionNormal = Vector2(-1, 0);
                }
                else {
                    collisiondata.overlapDistance = penY;
                    if (posA.y > posB.y)
                        collisiondata.collisionNormal = Vector2(0, 1);
                    if (posA.y < posB.y)
                        collisiondata.collisionNormal = Vector2(0, -1);
                }

                return true;
            }

            return false;
        }

        // Calculate a circle-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const CircleCollider& rhs, CollisionPair& collisiondata)
        {
            float distance = sqrt(pow(lhs.GetPosition().x - rhs.GetPosition().x, 2) + pow(lhs.GetPosition().y - rhs.GetPosition().y, 2));
            float sumradii = lhs.radius() + rhs.radius();
            // Determine if the distance between the centers is less than sum of radii
            if (distance <= sumradii) {
                collisiondata.overlapDistance = sumradii - distance;
                collisiondata.collisionNormal = (lhs.GetPosition() - rhs.GetPosition()).Normalised();
                return true;
            }

            return false;
        }

        // Calculate a rectangle-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const RectangleCollider& rhs, CollisionPair& collisiondata)
        {
            // Find closest point on square to the circle
            float closest_x = clamp(lhs.GetPosition().x, rhs.GetPosition().x - rhs.length() / 2, rhs.GetPosition().x + rhs.length()/2);
            float closest_y = clamp(lhs.GetPosition().y, rhs.GetPosition().y - rhs.width() / 2, rhs.GetPosition().y + rhs.width()/2);
            Vector2 closestPoint(closest_x, closest_y);

            float distance = fabs((closestPoint - lhs.GetPosition()).Length());

            if (distance < lhs.radius()) {
                collisiondata.overlapDistance = lhs.radius() - distance;
                collisiondata.collisionNormal = (closestPoint - lhs.GetPosition()).Normalised();
                return true;
            }
            return false;
        }

        // Clamp function to restrict value to given range
        float CollisionManager::clamp(float p, const float min, const float max)
        {
            if (p < min) p = min;
            else if (p > max) p = max;
            return p;
        }

        CircleCollider::CircleCollider(objectType type, RigidBody* rb, float radius) {
            this->SetType(type);
            this->radius_ = radius;
            this->shape = 'c';
            this->rb = rb;
        }

        CircleCollider::CircleCollider(const CircleCollider& copy) {
            this->SetType(copy.GetType());
            radius_ = copy.radius_;
            this->shape = 'c';
            this->rb = copy.GetRigidBody();
        }

        CircleCollider:: ~CircleCollider() = default;

        bool CircleCollider::overlaps(const CollisionVolume& rhs, CollisionPair& collisiondata) const{
            if (rhs.shape == 'c') {
                const CircleCollider* c = dynamic_cast<const CircleCollider*>(&rhs);
                return CollisionManager::collides(*this, *c, collisiondata);
            }

            else {
                const RectangleCollider* r = dynamic_cast<const RectangleCollider*>(&rhs);
                return CollisionManager::collides(*this, *r, collisiondata);
            }
            
        }

        float CircleCollider::radius() const{
            return radius_;
        }
        
        float CircleCollider::GetMinExtent() const {
            return this->GetPosition().x - this->radius();
        }


        float CircleCollider::GetMaxExtent() const {
            return this->GetPosition().x + this->radius();
        }
        
        RectangleCollider::RectangleCollider(objectType type, RigidBody* rb, float length, float width) {
            this->SetType(type);
            this->length_ = length;
            this->width_ = width;
            this->shape = 'r';
            this->rb = rb;
        }

        RectangleCollider::RectangleCollider(const RectangleCollider& copy) {
            this->SetType(copy.GetType());
            this->length_ = copy.length_;
            this->width_ = copy.width_;
            this->shape = 'r';
            this->rb = copy.GetRigidBody();
        }

        RectangleCollider::~RectangleCollider() = default;

        bool RectangleCollider::overlaps(const CollisionVolume& rhs, CollisionPair& collisiondata) const {
            if (rhs.shape == 'c') {
                const CircleCollider* c = dynamic_cast<const CircleCollider*>(&rhs);
                return CollisionManager::collides(*c, *this, collisiondata);
            }

            else {
                const RectangleCollider* r = dynamic_cast<const RectangleCollider*>(&rhs);
                return CollisionManager::collides(*this, *r, collisiondata);
            }
        }

        float RectangleCollider::length() const {
            return length_;
        }

        float RectangleCollider::width() const {
            return width_;
        }
        
        float RectangleCollider::GetMinExtent() const {
            return this->GetPosition().x - this->length()/2;
        }


        float RectangleCollider::GetMaxExtent() const {
            return this->GetPosition().x + this->length()/2;
        }
        
}
}
