#ifndef __FIR_COEFFICIENTS_HPP__
#define __FIR_COEFFICIENTS_HPP__

class FirCoefficients
{
public:
    /*
     * @brief constructor
     * @param   h_taps                   Number of horizontal taps
     * @param   v_taps                   Number of vertical taps
     * @param   horizontal_symmetry      Whether horizontal symmetry is on
     * @param   vertical_symmetry        Whether vertical symmetry is on
     * @param   diagonal_symmetry        Whether diagonal symmetry is on
     * @post  Create a set of fir coefficients. Use number of taps and symmetry properties to determine how large this set is
     * @pre   diagonal_symmetry is only allowed with horizontal_symmetry and vertical_symmetry enable
     */
    FirCoefficients(unsigned int h_taps, unsigned int v_taps, bool horizontal_symmetry, bool vertical_symmetry, bool diagonal_symmetry);

    /*
     * @brief destructor
     */
    ~FirCoefficients();

    // @post set a value for the coefficient at position x,y
    inline void set_coefficient(unsigned int x, unsigned int y, float value) {
        set_coefficient(map_coefficient(x, y), value);
    }

    // @post set a value for the coefficient n
    void set_coefficient(unsigned int n, float value) {
        coeffs[n] = value;
    }

    // @return the coefficient at position x,y
    inline float get_coefficient(unsigned int x, unsigned int y) const {
        return get_coefficient(map_coefficient(x, y));
    }

    // @return the coefficient n
    inline float get_coefficient(unsigned int n) const {
        return coeffs[n];
    }

    // @return the number of unique coefficients once symmetries are taken into account
    inline int get_number_of_coefficients() const {
        return number_of_coefficients;
    }

    // @return the array of coefficients
    inline operator const float * () const {
        return coeffs;
    }

protected:
    // @return the position of coefficient x,y in the set of coefficients
    int map_coefficient(unsigned int x, unsigned int y) const;

private:
    unsigned int h_taps;
    unsigned int v_taps;
    bool horizontal_symmetry;
    bool vertical_symmetry;
    bool diagonal_symmetry;
    int number_of_coefficients;
    float *coeffs;
};

#endif  // __FIR_COEFFICIENTS_HPP__
