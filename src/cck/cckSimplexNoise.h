#ifndef CCK_SIMPLEX_NOISE_H
#define CCK_SIMPLEX_NOISE_H

//This implementation of Simplex Noise is a C++ version of Java code produced by Stefan Gustavson, provided with the following information:

/*
 * A speed-improved simplex noise algorithm for 2D, 3D and 4D in Java.
 *
 * Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method by Stefan Gustavson in 2012.
 *
 * This could be speeded up even further, but it's useful as it is.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated.
 *
 */

namespace cck
{
    class SimplexNoise
    {
    private:
        const double    grad3[12][3];
        int             perm[512];

    private:
        double          Dot( const double* g, const double x, const double y, const double z ) const;

    public:
        double          Noise( const double x, const double y, const double z ) const;
        double          OctaveNoise( const double x, const double y, const double z, const int octaves, const double persistence, double frequency ) const;
        double          ScaledOctaveNoise( const double x, const double y, const double z, const int octaves, const double persistence, const double frequency, const double boundMin, const double boundMax ) const;

    public:
        SimplexNoise( const unsigned int seed );
    };
}


#endif // CCK_SIMPLEX_NOISE_H
