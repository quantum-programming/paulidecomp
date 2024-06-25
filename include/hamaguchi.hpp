#pragma once

#include <complex>
#include <vector>

#include "common.hpp"

namespace paulidecomp {

namespace hamaguchi {

std::complex<double>* calc_pauli_vector_internal(
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    const int size = density_matrix.size();
    const int n_qubit = get_log2(size);

    for (int k = 0; k < n_qubit; k++) {
        int shift = 1 << k;
        for (int i_offset = 0; i_offset < size; i_offset += shift * 2) {
            for (int j_offset = 0; j_offset < size; j_offset += shift * 2) {
                for (int i = i_offset; i < i_offset + shift; i++) {
                    for (int j = j_offset; j < j_offset + shift; j++) {
                        auto I = density_matrix[i][j] +
                                 density_matrix[i + shift][j + shift];
                        auto Z = density_matrix[i][j] -
                                 density_matrix[i + shift][j + shift];
                        density_matrix[i][j] = I;
                        density_matrix[i + shift][j + shift] = Z;

                        auto X = density_matrix[i][j + shift] +
                                 density_matrix[i + shift][j];
                        auto Y = (density_matrix[i][j + shift] -
                                  density_matrix[i + shift][j]) *
                                 std::complex<double>(0.0, 1.0);
                        density_matrix[i][j + shift] = X;
                        density_matrix[i + shift][j] = Y;
                    }
                }
            }
        }
    }

    // The following code is based on z order curve.
    // Reference: https://en.wikipedia.org/wiki/Z-order_curve
    // interlace_zeros[0b1011] = 0b1000101
    std::vector<int> interlace_zeros(size);
    for (int i = 0; i < size; i++) {
        for (int s = 0; s < n_qubit; s++) {
            if (i & (1 << s)) {
                interlace_zeros[i] |= 1 << (2 * s);
            }
        }
    }

    auto result = new std::complex<double>[size * size];
    const double normalize = 1.0 / size;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int index = (interlace_zeros[i] << 1) | interlace_zeros[j];
            result[index] = density_matrix[i][j] * normalize;
        }
    }

    // free matrix
    density_matrix = {{}};

    return result;
}

}  // namespace hamaguchi

}  // namespace paulidecomp
