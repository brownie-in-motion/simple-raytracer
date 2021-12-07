# ifndef SPHERE_TEXTURE_H
# define SPHERE_TEXTURE_H

# include "r3_vec.h"
# include "texture.h"

class matte_texture : public texture {
    public:
        matte_texture(const r3_vec& c) : color(c) {}

        virtual std::vector<r3_vec> reflection(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            r3_vec reflect = in - 2 * (in* norm) * norm;

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
            return 0.95 * source + 0.05 * reflection;
        }
    public:
        r3_vec color;
};

class mirror_texture : public texture {
    public:
        mirror_texture(double reflectivity) : reflectivity(reflectivity) {}

        virtual std::vector<r3_vec> reflection(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            r3_vec reflect = in - 2 * (in* norm) * norm;
            return {reflect};
        }

        virtual double brightness(double source, double reflection) const {
            return 0.05 * source + reflectivity * reflection;
        }
    public:
        double reflectivity;
};

# endif
