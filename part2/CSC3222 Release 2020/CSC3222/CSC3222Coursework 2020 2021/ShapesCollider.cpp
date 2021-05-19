#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "ShapesCollider.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {

        // Direct a rectangle-shape* collision
        bool CollisionManager::collides(
            const RectangleCollider& lhs,
            const std::shared_ptr<CollisionVolume>& rhs)
        {
            // If we successfully cast rhs to rectangle, perform rectangle-rectangle comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square);
            // If we successfully cast rhs to circle, perform circle-rectangle comparison
            if (auto circle = std::dynamic_pointer_cast<CircleCollider>(rhs))
                return collides(*circle, lhs);

            // Else there is no implementation yet for the given shape
            throw std::bad_cast();
        }
        
        // Direct a circle-shape* collision
        bool CollisionManager::collides(
            const CircleCollider& lhs,
            const std::shared_ptr<CollisionVolume>& rhs)
        {
            // If we successfully cast rhs to square, perform circle-rectangle comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square);
            // If we successfully cast rhs to circle, perform circle-circle comparison
            if (auto circle = std::dynamic_pointer_cast<CircleCollider>(rhs))
                return collides(lhs, *circle);

            // Else there is no implementation yet for the given shape
            //TODO throw an exception instead?
            return false;
        }

        // Calculate a rectangle-rectangle collision
        bool CollisionManager::collides(const RectangleCollider& lhs, const RectangleCollider& rhs)
        {
            Vector2 posA = lhs.GetPosition();
            Vector2 posB = rhs.GetPosition();
            Vector2 delta = posB - posA;
            Vector2 totalSize = Vector2(lhs.length() / 2, lhs.width() / 2) + Vector2(rhs.length() / 2, rhs.width() / 2);

            if (fabs(delta.x) < totalSize.x && fabs(delta.y) < totalSize.y) {
                return true;
            }

            return false;
        }

        // Calculate a circle-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const CircleCollider& rhs)
        {
            // Determine if the distance between the centers is less than both radii
            return (pow(lhs.GetPosition().x - rhs.GetPosition().x, 2) + pow(lhs.GetPosition().y - rhs.GetPosition().y, 2))
                <= pow(lhs.radius() + rhs.radius(), 2);
        }

        // Calculate a rectangle-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const RectangleCollider& rhs)
        {
            // Find closest point on square to the circle
            float closest_x = clamp(lhs.GetPosition().x, rhs.GetPosition().x - rhs.length() / 2, rhs.GetPosition().x + rhs.length()/2);
            float closest_y = clamp(lhs.GetPosition().y, rhs.GetPosition().y - rhs.width() / 2, rhs.GetPosition().y + rhs.width()/2);
            Vector2 closestPoint(closest_x, closest_y);

            float distance = fabs((closestPoint - lhs.GetPosition()).Length());

            return (distance < lhs.radius());
        }

        // Clamp function to restrict value to given range
        float CollisionManager::clamp(float p, const float min, const float max)
        {
            if (p < min) p = min;
            else if (p > max) p = max;
            return p;
        }

        CircleCollider::CircleCollider(objectType type, float radius) {
            this->SetType(type);
            this->radius_ = radius;
            this->shape = 'c';
        }

        CircleCollider::CircleCollider(const CircleCollider& copy) {
            this->SetType(copy.GetType());
            radius_ = copy.radius_;
            this->shape = 'c';
        }

        CircleCollider:: ~CircleCollider() = default;

        bool CircleCollider::overlaps(const CollisionVolume& rhs) const{
            if (rhs.shape == 'c') {
                const CircleCollider* c = dynamic_cast<const CircleCollider*>(&rhs);
                return CollisionManager::collides(*this, *c);
            }

            else {
                const RectangleCollider* r = dynamic_cast<const RectangleCollider*>(&rhs);
                return CollisionManager::collides(*this, *r);
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
        
        RectangleCollider::RectangleCollider(objectType type, float length, float width) {
            this->SetType(type);
            this->length_ = length;
            this->width_ = width;
            this->shape = 'r';
        }

        RectangleCollider::RectangleCollider(const RectangleCollider& copy) {
            this->SetType(copy.GetType());
            this->length_ = copy.length_;
            this->width_ = copy.width_;
            this->shape = 'r';
        }

        RectangleCollider::~RectangleCollider() = default;

        bool RectangleCollider::overlaps(const CollisionVolume& rhs) const {
            if (rhs.shape == 'c') {
                const CircleCollider* c = dynamic_cast<const CircleCollider*>(&rhs);
                return CollisionManager::collides(*c, *this);
            }

            else {
                const RectangleCollider* r = dynamic_cast<const RectangleCollider*>(&rhs);
                return CollisionManager::collides(*this, *r);
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
