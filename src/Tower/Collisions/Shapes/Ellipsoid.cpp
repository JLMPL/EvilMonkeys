#include "Ellipsoid.hpp"

void Shape::Ellipsoid::init(const vec3& r)
{
    type = Shape::BaseShape::Type::Ellipsoid;

    radii = r;

    aabb.checkPoint(radii);
    aabb.checkPoint(-radii);
}
