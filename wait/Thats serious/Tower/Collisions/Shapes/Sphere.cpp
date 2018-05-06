#include "Sphere.hpp"

void Shape::Sphere::init(f32 r)
{
    type = BaseShape::Type::Sphere;

    radius = r;

    aabb.checkPoint(vec3(radius));
    aabb.checkPoint(vec3(-radius));
}
