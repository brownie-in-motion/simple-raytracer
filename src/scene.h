# include <vector>

# include "ray.h"
# include "object.h"

class scene {
    public:
        scene() {}

        void add_object(object* o) {
            objects.push_back(o);
        }

        void add_light(r3_vec* v) {
            lights.push_back(v);
        }

        double trace(ray& r, int depth) const {
            if (depth <= 0) return 0;

            double min_t = 1e9;
            object* min_obj = NULL;
            for (object* o : objects) {
                double t = o->intersect(r);
                if (t > 0 && t < min_t) {
                    min_t = t;
                    min_obj = o;
                }
            }

            if (min_obj == NULL) return 0;

            r3_vec position = r.at(min_t);

            double brightness = 0;

            for (r3_vec* l : lights) {
                r3_vec direction = *l - position;
                bool in_shadow = false;
                for (object* o : objects) {
                    r3_vec norm = o->normal(position);
                    ray shadow_ray(position + 1e-6 * norm, direction);

                    double t = o->intersect(shadow_ray);
                    if (t >= 0 && t < direction.norm()) {
                        in_shadow = true;
                        break;
                    }
                }

                if (!in_shadow) {
                    // TODO: handle diffusion in object
                    brightness += fabs(
                        min_obj->normal(position) *
                        r3_vec::normalized(direction)
                    );
                }
            }

            double reflect_brightness = 0;
            std::vector<r3_vec> reflected = min_obj->reflection(
                position,
                r.direction
            );
            r3_vec norm = min_obj->normal(position);
            for (r3_vec v : reflected) {
                ray reflect_ray(position + 1e-6 * norm, v);
                reflect_brightness += trace(reflect_ray, depth - 1);
            }

            return min_obj->brightness(
                brightness,
                reflect_brightness / reflected.size()
            );
        }

        std::vector<std::vector<r3_vec>> render(
            r3_vec camera_position, int width, int height, int samples
        ) const {
            std::vector<std::vector<r3_vec>> image = {};
            for (int i = 0; i < height; i += 1) {
                printf("\r%d / %d rows done", i, height);
                fflush(stdout);
                std::vector<r3_vec> row = {};
                for (int j = 0; j < width; j += 1) {
                    double brightness = 0;
                    ray c = ray(
                        camera_position,
                        r3_vec(j - width / 2, i - height / 2, 0) - camera_position
                    );

                    for (int k = 0; k < samples; k++) {
                        brightness += trace(c, 5);
                    }

                    double mean = brightness / samples;
                    int color = 255 * mean;
                    row.push_back(
                        r3_vec(color, color, color)
                    );
                }
                image.push_back(row);
            };
            return image;
        }

    public:
        std::vector<object*> objects;
        std::vector<r3_vec*> lights;
};
