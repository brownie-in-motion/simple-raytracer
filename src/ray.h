# ifndef RAY_H
# define RAY_H

# include "r3_vec.h"

class ray {
    public:
        ray(r3_vec origin, r3_vec direction):
            origin(origin),
            direction(r3_vec::normalized(direction))
        {}

        r3_vec at(double t) const {
            return origin + t * direction;
        }
    public:
        r3_vec origin;
        r3_vec direction;
};

# endif
