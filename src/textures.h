# ifndef TEXTURES_H
# define TEXTURES_H

# include "r3_vec.h"
# include "texture.h"
# include "colormap.h"

class matte_texture : public texture {
    public:
        matte_texture(colormap* c) : c(c) {}

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

        virtual std::vector<r3_vec> refraction(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            return {};
        }

        virtual r3_vec color_mix(
            r3_vec position,
            r3_vec source,
            r3_vec reflection,
            r3_vec refraction
        ) const {
            r3_vec color = c->map(position);
            r3_vec mixed = 0.9 * source + 0.1 * reflection;
            return r3_vec(
                mixed.x() * color.x(),
                mixed.y() * color.y(),
                mixed.z() * color.z()
            ) / 255;
        }
    public:
        colormap* c;
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

        virtual std::vector<r3_vec> refraction(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            return {};
        }

        virtual r3_vec color_mix(
            r3_vec position,
            r3_vec source,
            r3_vec reflection,
            r3_vec refraction
        ) const {
            return 0.05 * source + reflectivity * reflection;
        }
    public:
        double reflectivity;
};

class glass_texture : public texture {
    public:
        glass_texture(double index) : index(index) {}

        virtual std::vector<r3_vec> reflection(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            r3_vec reflect = in - 2 * (in* norm) * norm;
            return {reflect};
        }

        virtual std::vector<r3_vec> refraction(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const {
            r3_vec light = r3_vec::normalized(in);
            double r = 1 / index;
            double c = - norm * light;
            return {
                light * r +
                (r * c - sqrt(1 - r * r * (1 - c * c))) * norm
            };
        }

        virtual bool is_transparent() const {
            return true;
        }

        virtual r3_vec color_mix(
            r3_vec position,
            r3_vec source,
            r3_vec reflection,
            r3_vec refraction
        ) const {
            return refraction;
        }
    public:
        double index;
};

# endif
