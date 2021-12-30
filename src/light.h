# ifndef LIGHT_H
# define LIGHT_H

# include "r3_vec.h"

class light {
    public:
        light(r3_vec position, r3_vec color) :
            position(position),
            color(color)
        {}
    public:
        r3_vec position;
        r3_vec color;
};

# endif
