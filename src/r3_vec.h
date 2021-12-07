# ifndef R3_VEC_H
# define R3_VEC_H

# include <cmath>
# include <iostream>

class r3_vec;
double operator*(const r3_vec&, const r3_vec&);
r3_vec operator*(const r3_vec&, double);
r3_vec operator*(double, const r3_vec&);
r3_vec operator/(const r3_vec&, double);

class r3_vec {
    public:
        r3_vec(): v{0, 0, 0} {}
        r3_vec(double x, double y, double z): v{x, y, z} {}

        void operator+=(const r3_vec& other) {
            v[0] += other.v[0];
            v[1] += other.v[1];
            v[2] += other.v[2];
        }

        void operator-=(const r3_vec& other) {
            v[0] -= other.v[0];
            v[1] -= other.v[1];
            v[2] -= other.v[2];
        }

        void operator*=(double scalar) {
            v[0] *= scalar;
            v[1] *= scalar;
            v[2] *= scalar;
        }

        void operator/=(double scalar) {
            v[0] /= scalar;
            v[1] /= scalar;
            v[2] /= scalar;
        }

        double squared_norm() const {
            return (*this) * (*this);
        }

        double norm() const {
            return sqrt(squared_norm());
        }

        double x () const { return v[0]; }
        double y () const { return v[1]; }
        double z () const { return v[2]; }

        static r3_vec normalized(const r3_vec& v) {
            return v / v.norm();
        }

        static r3_vec random() {
            return r3_vec(
                (double) rand() / RAND_MAX - 0.5,
                (double) rand() / RAND_MAX - 0.5,
                (double) rand() / RAND_MAX - 0.5
            ) * 2;
        }

    public:
        double v[3];
};

r3_vec operator+(const r3_vec &a, const r3_vec &b) {
    return r3_vec(
        a.v[0] + b.v[0],
        a.v[1] + b.v[1],
        a.v[2] + b.v[2]
    );
}

r3_vec operator-(const r3_vec &a, const r3_vec &b) {
    return r3_vec(
        a.v[0] - b.v[0],
        a.v[1] - b.v[1],
        a.v[2] - b.v[2]
    );
}

r3_vec operator-(const r3_vec &a) {
    return r3_vec(
        -a.v[0],
        -a.v[1],
        -a.v[2]
    );
}

r3_vec operator*(const r3_vec &a, double scalar) {
    return r3_vec(
        a.v[0] * scalar,
        a.v[1] * scalar,
        a.v[2] * scalar
    );
}

r3_vec operator*(double scalar, const r3_vec &a) {
    return a * scalar;
}

double operator*(const r3_vec &a, const r3_vec &b) {
    return (
        a.v[0] * b.v[0] +
        a.v[1] * b.v[1] +
        a.v[2] * b.v[2]
    );
}

r3_vec operator/(const r3_vec &a, double scalar) {
    return r3_vec(
        a.v[0] / scalar,
        a.v[1] / scalar,
        a.v[2] / scalar
    );
}

# endif
