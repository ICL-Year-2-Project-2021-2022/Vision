#include "Fir_Filter.hpp"

#include "VipMath.hpp"
#include "QuantizedCoefficients.hpp"

Fir_Filter::Fir_Filter(unsigned long base_address, bool coeff_signed, unsigned int coeff_int_bits, unsigned int coeff_fraction_bits) :
    VipCore(base_address), coeff_signed(coeff_signed), coeff_int_bits(coeff_int_bits), coeff_fraction_bits(coeff_fraction_bits)
{
}

void Fir_Filter::apply(const FirCoefficients & coeffs)
{
    QuantizedCoefficients quantized_coeffs(coeff_signed, coeff_int_bits, coeff_fraction_bits, coeffs, coeffs.get_number_of_coefficients());
    apply(quantized_coeffs, coeffs.get_number_of_coefficients());
}

void Fir_Filter::apply(const long *quantized_coeffs, unsigned int n)
{
    for (unsigned int k = 0; k < n; ++k) {
        do_write(FIR_COEFFICIENTS_BASE + k, quantized_coeffs[k]);
    }
    do_write(FIR_COEFFICIENTS_COMMIT, 1);
}
