#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
#include "ShapesCollider.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {

        // Direct a square-shape* collision
        bool CollisionManager::collides(
            const RectangleCollider& lhs,
            const std::shared_ptr<CollisionVolume>& rhs)
        {
            // If we successfully cast rhs to square, perform square-square comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square);
            // If we successfully cast rhs to circle, perform circle-square comparison
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
            // If we successfully cast rhs to square, perform circle-square comparison
            if (auto square = std::dynamic_pointer_cast<RectangleCollider>(rhs))
                return collides(lhs, *square);
            // If we successfully cast rhs to circle, perform circle-circle comparison
            if (auto circle = std::dynamic_pointer_cast<CircleCollider>(rhs))
                return collides(lhs, *circle);

            // Else there is no implementation yet for the given shape
            //TODO throw an exception instead?
            return false;
        }

        // Calculate a square-square collision
        bool CollisionManager::collides(const RectangleCollider& lhs, const RectangleCollider& rhs)
        {
            // If either corner clears the other, there's no collision
            /*return !(
                lhs.GetPosition().x + lhs.size() < rhs.GetPosition().x ||
                lhs.GetPosition().x > rhs.GetPosition().x + rhs.size() ||
                lhs.GetPosition().y + lhs.size() < rhs.GetPosition().y ||
                lhs.GetPosition().y > rhs.GetPosition().y + rhs.size()
                );*/
            return(
                lhs.GetPosition().x < rhs.GetPosition().x + rhs.width() &&
                lhs.GetPosition().x + lhs.width() > rhs.GetPosition().x &&
                lhs.GetPosition().y < rhs.GetPosition().y + rhs.width() &&
                lhs.GetPosition().y + lhs.width() > rhs.GetPosition().y
                );

        }

        // Calculate a circle-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const CircleCollider& rhs)
        {
            // Determine if the distance between the centers is less than both radii
            return (pow(lhs.GetPosition().x - rhs.GetPosition().x, 2) + pow(lhs.GetPosition().y - rhs.GetPosition().y, 2))
                <= pow(lhs.radius() + rhs.radius(), 2);
        }

        // Calculate a square-circle collision
        bool CollisionManager::collides(const CircleCollider& lhs, const RectangleCollider& rhs)
        {
            // Find closest point on square to the circle
            double closest_x = clamp(lhs.GetPosition().x, rhs.GetPosition().x, rhs.GetPosition().x + rhs.length());
            double closest_y = clamp(lhs.GetPosition().y, rhs.GetPosition().y, rhs.GetPosition().y + rhs.width());

            // See if the point is closer than radius distance from the circle center
            // Use squared values to avoid slower sqrt calculations
            return pow(lhs.GetPosition().x - closest_x, 2) + pow(lhs.GetPosition().y - closest_y, 2)
                <= pow(lhs.radius(), 2);
        }

        // Clamp function to restrict value to given range
        double CollisionManager::clamp(double p, const double min, const double max)
        {
            if (p < min) p = min;
            else if (p > max) p = max;
            return p;
        }

        CircleCollider::CircleCollider(objectType type, float radius) {
            this->SetType(type);
            this->radius_ = radius;
        }

        CircleCollider::CircleCollider(const CircleCollider& copy) {
            this->SetType(copy.GetType());
            radius_ = copy.radius_;
        }

        CircleCollider:: ~CircleCollider() = default;

        bool CircleCollider::overlaps(const std::shared_ptr<CollisionVolume>& rhs) const{
            return CollisionManager::collides(*this, rhs);
        }

        float CircleCollider::radius() const{
            return radius_;
        }

        RectangleCollider::RectangleCollider(objectType type, float length, float width) {
            this->SetType(type);
            this->length_ = length;
            this->width_ = width;
        }

        RectangleCollider::RectangleCollider(const RectangleCollider& copy) {
            this->SetType(copy.GetType());
            this->length_ = copy.length_;
            this->width_ = copy.width_;
        }

        RectangleCollider::~RectangleCollider() = default;

        bool RectangleCollider::overlaps(const std::shared_ptr<CollisionVolume>& rhs) const {
            return CollisionManager::collides(*this, rhs);
        }

        float RectangleCollider::length() const {
            return length_;
        }

        float RectangleCollider::width() const {
            return width_;
        }
	}
}
