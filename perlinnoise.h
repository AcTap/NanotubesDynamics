#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include <vector>
class PerlinNoise {
        std::vector<int> p;
public:
        PerlinNoise();
        PerlinNoise(unsigned int seed);
        double noise(double x, double y, double z);
        double Generate(double x, double y, double z);
        double step=1.5;
        double octaves=5;
        double gain=0.65;
private:
        double fade(double t);
        double lerp(double t, double a, double b);
        double grad(int hash, double x, double y, double z);
};
#endif // PERLINNOISE_H
