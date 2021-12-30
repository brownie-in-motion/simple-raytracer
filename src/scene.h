# include <vector>

# include "ray.h"
# include "light.h"
# include "object.h"

class scene {
    public:
        scene() {}

        void add_object(object* o) {
            objects.push_back(o);
        }

        void add_light(light* v) {
            lights.push_back(v);
        }

        r3_vec trace(ray& r, int depth) const {
            if (depth <= 0) return r3_vec(0, 0, 0);

            double min_t = 1e9;
            object* min_obj = NULL;
            for (object* o : objects) {
                double t = o->intersect(r);
                if (t > 0 && t < min_t) {
                    min_t = t;
                    min_obj = o;
                }
            }

            if (min_obj == NULL) return r3_vec(0, 0, 0);

            r3_vec position = r.at(min_t);

            r3_vec brightness = r3_vec(0, 0, 0);

            for (light* l : lights) {
                r3_vec light_position = l->position;
                r3_vec light_color = l->color;
                r3_vec direction = light_position - position;

                bool in_shadow = false;
                for (object* o : objects) {
                    if (o->is_transparent()) continue;
                    r3_vec norm = o->normal(position);

                    ray shadow_ray(position + 1e-6 * norm, direction);

                    double t = o->intersect(shadow_ray);
                    if (t >= 0 && t < direction.norm()) {
                        in_shadow = true;
                        break;
                    }
                }

                if (!in_shadow) {
                    double value = fabs(
                        min_obj->normal(position) *
                        r3_vec::normalized(direction)
                    );
                    brightness += value * light_color;
                }
            }

            r3_vec reflect_brightness = r3_vec(0, 0, 0);
            std::vector<r3_vec> reflected = min_obj->reflection(
                position,
                r.direction
            );

            r3_vec norm = min_obj->normal(position);
            for (r3_vec v : reflected) {
                ray reflect_ray(position + 1e-6 * norm, v);
                reflect_brightness += trace(reflect_ray, depth - 1);
            }

            r3_vec refract_brightness = r3_vec(0, 0, 0);
            std::vector<r3_vec> refracted = min_obj->refraction(
                position,
                r.direction
            );
            for (r3_vec v : refracted) {
                ray refract_ray(position - 1e-6 * norm, v);
                refract_brightness += trace(refract_ray, depth - 1);
            }

            return min_obj->color_mix(
                position,
                brightness,
                reflect_brightness / reflected.size(),
                refract_brightness / refracted.size()
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
                    r3_vec color = r3_vec(0, 0, 0);
                    for (int k = 0; k < samples; k++) {
                        r3_vec direction = r3_vec(
                            j - width / 2,
                            i - height / 2,
                            0
                        ) - camera_position;
                        ray c = ray(
                            camera_position,
                            r3_vec::normalized(direction) +
                            r3_vec::random() * 0.0005
                        );

                        color += trace(c, 10);
                    }

                    row.push_back(
                        color / samples
                    );
                }
                image.push_back(row);
            }
            return image;
        }

    public:
        std::vector<object*> objects;
        std::vector<light*> lights;
};
