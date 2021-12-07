# include <vector>

# include "ray.h"
# include "r3_vec.h"
# include "object.h"

class sphere : public object {
    public:
        sphere();
        sphere(r3_vec center, double radius, bool smooth):
            center(center),
            radius(radius),
            smooth(smooth)
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
            r3_vec norm = normal(pos);
            r3_vec reflect = in - 2 * (in* norm) * norm;

            if (smooth) return {reflect};

            std::vector<r3_vec> result;
            for (int i = 0; i < 1; i++) { // only do one for now
                double theta = rand() / (double)RAND_MAX * 2 * M_PI;
                result.push_back(reflect + r3_vec(
                    cos(theta) * 0.5,
                    sin(theta) * 0.5,
                    0
                ));
            }
            return result;
        }

        virtual double brightness(double source, double reflection) const {
            if (smooth) return 0.05 * source + 0.95 * reflection;
            return 0.95 * source + 0.05 * reflection;
        }

    public:
        r3_vec center;
        double radius;
        bool smooth;
};
