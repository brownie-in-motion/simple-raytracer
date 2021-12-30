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
            sphere_texture(texture),
            invert(false)
        {}

        sphere(r3_vec center, double radius, texture* texture, bool invert):
            center(center),
            radius(radius),
            sphere_texture(texture),
            invert(true)
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
            return r3_vec::normalized(point - center) * (invert ? -1 : 1);
        }

        virtual std::vector<r3_vec> reflection(
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            return sphere_texture->reflection(normal(pos), pos, in);
        }

        virtual std::vector<r3_vec> refraction(
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            return sphere_texture->refraction(normal(pos), pos, in);
        }

        virtual bool is_transparent() const {
            return sphere_texture->is_transparent();
        }

        virtual r3_vec color_mix(
            r3_vec position,
            r3_vec source,
            r3_vec reflection,
            r3_vec refraction
        ) const {
            return sphere_texture->color_mix(
                position,
                source,
                reflection,
                refraction
            );
        }

    public:
        r3_vec center;
        double radius;
        texture* sphere_texture;
        bool invert;
};
