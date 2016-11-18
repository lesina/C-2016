#include <iostream>

template<int L, int M, int t, int I, int T, int nu, int k>
class DimQ {

    double value;
//    friend std::ostream &operator<<(std::ostream &stream, const DimQ <L,M,t,I,T,nu,k> &x);

public:

    DimQ(double x) {
        value = x;
    }

    double getValue() {
        return this->value;
    }

    DimQ<L, M, t, I, T, nu, k> operator-() {
        return DimQ<L, M, t, I, T, nu, k>(-value);
    }

    DimQ<L, M, t, I, T, nu, k> operator+(DimQ<L, M, t, I, T, nu, k> x) {
        return DimQ<L, M, t, I, T, nu, k>(value + x.getValue());
    };

    DimQ<L, M, t, I, T, nu, k> operator-(DimQ<L, M, t, I, T, nu, k> x) {
        return DimQ<L, M, t, I, T, nu, k>(value - x.getValue());
    };

    template<int a, int b, int c, int d, int e, int f, int g>
    DimQ<L + a, M + b, t + c, I + d, T + e, nu + f, k + g> operator*(DimQ<a, b, c, d, e, f, g> &x) {
        return DimQ<L + a, M + b, t + c, I + d, T + e, nu + f, k + g>(this->value * x.getValue());
    };

    template<int a, int b, int c, int d, int e, int f, int g>
    DimQ<L - a, M - b, t - c, I - d, T - e, nu - f, k - g> operator/(DimQ<a, b, c, d, e, f, g> &x) {
        return DimQ<L - a, M - b, t - c, I - d, T - e, nu - f, k - g>(this->value / x.getValue());
    };

};

template <int L, int M, int t, int I, int T, int nu, int k>
std::ostream &operator<<(std::ostream &stream, DimQ <L,M,t,I,T,nu,k> &x) {
    if (L == 1 and M == 0 and t == 0 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue() << " m";
    } else if (L == 1 and M == 0 and t == -1 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue() << " m/s";
    } else if (L == 1 and M == 0 and t == -2 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue() << " m/s^2";
    } else if (L == 0 and M == 0 and t == 1 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue() << " s";
    } else if (L == 0 and M == 0 and t == 0 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue();
    } else if (L == 0 and M == 1 and t == 0 and I == 0 and T == 0 and nu == 0 and k == 0){
        return stream << x.getValue() << " kg";
    } else if (L == 0 and M == 0 and t == 0 and I == 0 and T == 0 and nu == 1 and k == 0){
        return stream << x.getValue() << " mol";
    } else {
        stream << x.getValue() << " ";
        if (L != 0){
            stream << "m^(" << L << ")";
        }
        if (M != 0){
            stream << "kg^(" << M << ")";
        }
        if (t != 0){
            stream << "s^(" << t << ")";
        }
        if (I != 0){
            stream << "A^(" << I << ")";
        }
        if (T != 0){
            stream << "K^(" << T << ")";
        }
        if (nu != 0){
            stream << "mol^(" << nu << ")";
        }
        if (k != 0){
            stream << "C^(" << k << ")";
        }
    }
    return stream;
}

typedef DimQ<1, 0, 0, 0, 0, 0, 0> Length;
typedef DimQ<1, 0, -1, 0, 0, 0, 0> Velocity;
typedef DimQ<1, 0, -2, 0, 0, 0, 0> Acceleration;
typedef DimQ<0, 0, 1, 0, 0, 0, 0> Time;
typedef DimQ<0, 0, 0, 0, 0, 0, 0> Dimensionless;
typedef DimQ<0, 1, 0, 0, 0, 0, 0> Mass;
typedef DimQ<0, 0, 0, 0, 0, 1, 0> Amount;

int main() {

    Length l = {100};
    Time t = {20};
    Velocity v = l / t;
    Acceleration a = v / t;

    auto smth = v*a*a/t;
    auto dimensionless = v/v;

    std::cout << v << std::endl;
    std::cout << a << std::endl;
    std::cout << smth << std::endl;
    std::cout << dimensionless << std::endl;

    return 0;
}