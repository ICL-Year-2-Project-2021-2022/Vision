#ifndef __FIR_FILTER_HPP__
#define __FIR_FILTER_HPP__

#include "VipCore.hpp"

#include "VipMath.hpp"
#include "FirCoefficients.hpp"

/**
 * @brief  Fir filter controller
 * @pre    Please be sure to use the Edge_Adaptive_Sharpener if the core was parameterized to sharpen edges.
 *         This controller is designed for the mode that uses run-time coefficients
 */
class Fir_Filter : public VipCore {
private:
    Fir_Filter(const Fir_Filter&);  // Disable copy constructor

public:
    // FIR specific registers
    enum FirRegisterType {
        FIR_COEFFICIENTS_COMMIT  = 6,
        FIR_COEFFICIENTS_BASE    = 7,
    };

    /**
     * @brief  constructor
     * @param  base_address                   The slave interface base address (hash-defined in system.h)
     * @param  coeff_signed                   Whether the coefficients are signed (adds one extra integer bit)
     * @param  coeff_int_bits                 Number of integer bits for the coefficients
     * @param  coeff_fraction_bits            Number of fraction bits for the coefficients
     */
    Fir_Filter(unsigned long base_address, bool coeff_signed, unsigned int coeff_int_bits, unsigned int coeff_fraction_bits);

    /**
     * @brief  Change the Fir coefficients
     * @param  FirCoefficients                a FirCoefficients set
     * @pre    coeffs was parameterized with the right number of taps and the correct symmetries
     */
    void apply(const FirCoefficients & coeffs);

    /**
     * @brief  Change the Fir coefficients
     * @param  quantized_coeffs               a set of quantized coefficients
     * @param  n                              number of coefficients coefficients
     * @pre    the number of coefficients matches the HW parameterization of the Fir Core (h_taps*v_taps or less when symmetries were declared)
     */
    void apply(const long *quantized_coeffs, unsigned int n);

    /**
     * @brief  Write new Fir coefficient
     * @param  offset  the offset of the coefficient to write in the range [FIR_COEFFICIENTS_BASE..FIR_COEFFICIENTS_BASE+n]
     * @param  value   the floating point coefficient value
     * @post   Quantize the floating-point input and program the new coefficient. The coefficient remain unused until it is committed
     * @pre    offset is valid in the range [FIR_COEFFICIENTS_BASE..FIR_COEFFICIENTS_BASE+n] where n is the number of coefficients of the FIR
     *         as parameterized in HW (at most h_taps*v_taps and less in case symmetries are applied)
     */
    inline void set_coefficient(int offset, float value) {
        set_quantized_coefficient(offset, VipMath::quantize_coefficient(coeff_signed, coeff_int_bits, coeff_fraction_bits, value));
    }

    /**
     * @brief  Write new Fir coefficient
     * @param  offset  the offset of the coefficient to write in the range [FIR_COEFFICIENTS_BASE..FIR_COEFFICIENTS_BASE+n]
     * @param  value   the quantized coefficient value
     * @post   Program the new coefficient. The coefficient remain unused until it is committed
     * @pre    offset is valid in the range [FIR_COEFFICIENTS_BASE..FIR_COEFFICIENTS_BASE+n] where n is the number of coefficients of the FIR
     *         as parameterized in HW (at most h_taps*v_taps and less in case symmetries are applied)
     */
    inline void set_quantized_coefficient(int offset, long value) {
        do_write(offset, value);
    }

    /**
     * @brief  Commit the current set of coefficients
     * @post   New coefficient values programmed into the core will be used from next frame
     */
    inline void commit_coefficients()
    {
        do_write(FIR_COEFFICIENTS_COMMIT, 1);
    }

private:
    bool coeff_signed;
    unsigned int coeff_int_bits;
    unsigned int coeff_fraction_bits;
};


#endif /* __FIR_FILTER_HPP__ */
