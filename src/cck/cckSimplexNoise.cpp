#include "cckSimplexNoise.h"

#include <cmath>
#include <cstdlib>

double cck::SimplexNoise::Dot( const double* g, const double x, const double y, const double z ) const
{
    return g[0] * x + g[1] * y + g[2] * z;
}

double cck::SimplexNoise::Noise( const double x, const double y, const double z ) const
{
    double n0, n1, n2, n3; // Noise contributions from the four corners

    // Skew the input space to determine which simplex cell we're in
    double F3 = 1.0 / 3.0;
    double s = ( x + y + z ) * F3; // Very nice and simple skew factor for 3D
    int i = (int) floor( x + s );
    int j = (int) floor( y + s );
    int k = (int) floor( z + s );

    double G3 = 1.0 / 6.0; // Very nice and simple unskew factor, too
    double t = ( i + j + k ) * G3;
    double X0 = (double) i - t; // Unskew the cell origin back to (x,y,z) space
    double Y0 = (double) j - t;
    double Z0 = (double) k - t;
    double x0 = x - X0; // The x,y,z distances from the cell origin
    double y0 = y - Y0;
    double z0 = z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

    if ( x0 >= y0 )
    {
        if ( y0 >= z0 )
        {
            i1=1; j1=0; k1=0;
            i2=1; j2=1; k2=0;
        }
        else if ( x0 >= z0 )
        {
            i1=1; j1=0; k1=0;
            i2=1; j2=0; k2=1;
        }
        else
        {
            i1=0; j1=0; k1=1;
            i2=1; j2=0; k2=1;
        }
    }
    else
    {
        if ( y0 < z0 )
        {
            i1=0; j1=0; k1=1;
            i2=0; j2=1; k2=1;
        }
        else if ( x0 < z0 )
        {
            i1=0; j1=1; k1=0;
            i2=0; j2=1; k2=1;
        }
        else
        {
            i1=0; j1=1; k1=0;
            i2=1; j2=1; k2=0;
        }
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    double x1 = x0 - (double) i1 + G3; // Offsets for second corner in (x,y,z) coords
    double y1 = y0 - (double) j1 + G3;
    double z1 = z0 - (double) k1 + G3;
    double x2 = x0 - (double) i2 + 2.0 * G3; // Offsets for third corner in (x,y,z) coords
    double y2 = y0 - (double) j2 + 2.0 * G3;
    double z2 = z0 - (double) k2 + 2.0 * G3;
    double x3 = x0 - 1.0 + 3.0 * G3; // Offsets for last corner in (x,y,z) coords
    double y3 = y0 - 1.0 + 3.0 * G3;
    double z3 = z0 - 1.0 + 3.0 * G3;

    // Work out the hashed gradient indices of the four simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = perm[ii+perm[jj+perm[kk]]] % 12;
    int gi1 = perm[ii+i1+perm[jj+j1+perm[kk+k1]]] % 12;
    int gi2 = perm[ii+i2+perm[jj+j2+perm[kk+k2]]] % 12;
    int gi3 = perm[ii+1+perm[jj+1+perm[kk+1]]] % 12;

    // Calculate the contribution from the four corners
    double t0 = 0.6 - x0 * x0 - y0 * y0 - z0 * z0;
    if( t0 < 0.0 )
    {
        n0 = 0.0;
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * Dot( grad3[gi0], x0, y0, z0 );
    }

    double t1 = 0.6 - x1 * x1 - y1 * y1 - z1 * z1;
    if ( t1 < 0.0 )
    {
        n1 = 0.0;
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * Dot( grad3[gi1], x1, y1, z1 );
    }

    double t2 = 0.6 - x2 * x2 - y2 * y2 - z2 * z2;
    if ( t2 < 0.0 )
    {
        n2 = 0.0;
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * Dot( grad3[gi2], x2, y2, z2 );
    }

    double t3 = 0.6 - x3 * x3 - y3 * y3 - z3 * z3;
    if( t3 < 0.0 )
    {
        n3 = 0.0;
    }
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * Dot( grad3[gi3], x3, y3, z3 );
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 32.0 * ( n0 + n1 + n2 + n3 );
}

double cck::SimplexNoise::OctaveNoise( const double x, const double y, const double z, const int octaves, const double persistence, double frequency ) const
{
    double total = 0.0;
    double amplitude = 1.0;
    double maxAmplitude = 0.0;

    for( int i = 0; i < octaves; i++ )
    {
        total += Noise( x * frequency, y * frequency, z * frequency ) * amplitude;

        frequency *= 2.0;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

double cck::SimplexNoise::ScaledOctaveNoise( const double x, const double y, const double z, const int octaves, const double persistence, const double frequency, const double boundMin, const double boundMax ) const
{
    return OctaveNoise( x, y, z, octaves, persistence, frequency ) * ( boundMax - boundMin ) / 2.0 + ( boundMax + boundMin ) / 2.0;
}

cck::SimplexNoise::SimplexNoise( const unsigned int seed )
    :   grad3{  { 1.0, 1.0, 0.0 }, { -1.0, 1.0, 0.0 }, { 1.0, -1.0, 0.0},   { -1.0, -1.0, 0.0 },
                { 1.0, 0.0, 1.0 }, { -1.0, 0.0, 1.0 }, { 1.0, 0.0, -1.0 },  { -1.0, 0.0, -1.0 },
                { 0.0, 1.0, 1.0 }, { 0.0, -1.0, 1.0 }, { 0.0, 1.0, -1.0 },  { 0.0, -1.0, -1.0 } }
{
    srand( seed );

    for ( int i = 0; i < 256; i++ )
    {
        perm[i] = ( rand() % 256 ) & 255;
        perm[i+256] = perm[i];
    }
}
