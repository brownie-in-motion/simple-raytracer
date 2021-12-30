# ifndef COLORMAP_H
# define COLORMAP_H

# include "r3_vec.h"

class colormap {
    public:
        virtual r3_vec map(const r3_vec& pos) const = 0;
};

class flat : public colormap {
    public:
        flat(const r3_vec& color) : color(color) {}
        r3_vec map(const r3_vec& pos) const { return color; }
    public:
        r3_vec color;
};

class checkers : public colormap {
    public:
        checkers(const r3_vec& color1, const r3_vec& color2, size_t size) :
            color1(color1),
            color2(color2),
            size(size)
        {}

        r3_vec map(const r3_vec& pos) const {
            int i = round(pos.x() / size);
            int j = round(pos.z() / size);
            return (i + j) % 2 ? color1 : color2;
        }
    public:
        r3_vec color1, color2;
        size_t size;
};

# endif
