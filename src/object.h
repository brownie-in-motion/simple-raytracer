# ifndef OBJECT_H
# define OBJECT_H

# include <vector>
# include "ray.h"

class object {
    public:
        virtual double intersect(const ray& r) = 0;
        virtual r3_vec normal(const r3_vec& p) const = 0;
        virtual std::vector<r3_vec> reflection(
            const r3_vec& pos,
            const r3_vec& in
        ) const = 0;
        virtual std::vector<r3_vec> refraction(
            const r3_vec& pos,
            const r3_vec& in
        ) const = 0;
        virtual bool is_transparent() const { return false; };
        virtual r3_vec color_mix(
            r3_vec position,
            r3_vec source,
            r3_vec reflection,
            r3_vec refraction
        ) const = 0;
};

# endif
