# include <vector>

# include "ray.h"
# include "r3_vec.h"
# include "object.h"
# include "texture.h"

class sphere : public object {
    public:
        sphere();
        sphere(r3_vec center, double radius, texture* texture):
            center(center),
            radius(radius),
            sphere_texture(texture)
        {}

        virtual double intersect(const ray& rt) {
            r3_vec difference = rt.origin - center;

            double a = rt.direction.squared_norm();
            double b = 2 * difference * rt.direction;
            double c = difference.squared_norm() - radius * radius;

            double discriminant = b * b - 4 * a * c;

            if (discriminant < 0) {
                return -1;
            }

            double time_a = (-b - sqrt(discriminant)) / (2 * a);
            double time_b = (-b + sqrt(discriminant)) / (2 * a);
            return time_a < 0 ? time_b : time_a;
        }

        virtual r3_vec normal(const r3_vec& point) const {
            return r3_vec::normalized(point - center);
        }

        virtual std::vector<r3_vec> reflection(
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            return sphere_texture->reflection(normal(pos), pos, in);
        }

        virtual double brightness(double source, double reflection) const {
            return sphere_texture->brightness(source, reflection);
        }

    public:
        r3_vec center;
        double radius;
        texture* sphere_texture;
};
