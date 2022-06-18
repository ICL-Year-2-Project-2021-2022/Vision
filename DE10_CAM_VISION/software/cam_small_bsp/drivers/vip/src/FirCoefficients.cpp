#include "FirCoefficients.hpp"

#include <stdlib.h>

FirCoefficients::FirCoefficients(unsigned int h_taps, unsigned int v_taps, bool horizontal_symmetry, bool vertical_symmetry, bool diagonal_symmetry)
: h_taps(h_taps), v_taps(v_taps), horizontal_symmetry(horizontal_symmetry), vertical_symmetry(vertical_symmetry), diagonal_symmetry(diagonal_symmetry)
{
    if (diagonal_symmetry)
    {
        unsigned int half_taps = (h_taps + 1)/2;
        number_of_coefficients = (half_taps * (half_taps + 1)) / 2;
    }
    else
    {
        number_of_coefficients = (horizontal_symmetry ? ((h_taps + 1) / 2) : h_taps) *
                                 (vertical_symmetry ? ((v_taps + 1) / 2) : v_taps);
    }

    coeffs = (float*) malloc(number_of_coefficients * sizeof(float));
}

FirCoefficients::~FirCoefficients()
{
    free(coeffs);
}

int FirCoefficients::map_coefficient(unsigned int x, unsigned int y) const
{
    unsigned int half_h_taps = ((h_taps + 1) / 2);
    unsigned int half_v_taps = ((v_taps + 1) / 2);
    if (horizontal_symmetry && (x > half_h_taps))
    {
        x = h_taps - x - 1;
    }
    if (vertical_symmetry && (y > half_v_taps))
    {
        y = v_taps - y - 1;
    }
    unsigned int mapped_coefficient;
    if (!diagonal_symmetry)
    {
        mapped_coefficient = x + (y * (horizontal_symmetry ? half_h_taps : h_taps));
    }
    else
    {
        if (x < y)
        {
            unsigned int tmp = y;
            y = x;
            x = tmp;
        }
        mapped_coefficient = x;
        unsigned int line_length = half_h_taps;
        while (y!=0)
        {
            mapped_coefficient += (line_length - 1);
            --y;
            --line_length;
        }
    }
    return mapped_coefficient;
}
