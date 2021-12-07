# ifndef TEXTURE_H
# define TEXTURE_H

# include <vector>
# include "ray.h"

class texture {
    public:
        virtual std::vector<r3_vec> reflection(
            const r3_vec& norm,
            const r3_vec& pos,
            const r3_vec& in
        ) const = 0;
        virtual double brightness(
            double source,
            double reflection
        ) const = 0;

};

# endif
