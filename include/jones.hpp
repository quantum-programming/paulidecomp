#pragma once

#include <complex>
#include <vector>

#include "common.hpp"

namespace paulidecomp {

namespace jones {

inline int get_pauli_elem(int n, int t, int i) {
    int p = get_pauli_flag(n, t);
    int a = get_bit(i, t);
    return (p == 2 ? 3 : 0) ^ (p >= 2 && a == 1 ? 2 : 0);
}

inline int get_factor(int n, int i, int n_qubit) {
    int factor = 0;
    for (int t = 0; t < n_qubit; t++) {
        factor += get_pauli_elem(n, t, i);
    }
    factor &= 0b11;
    return factor;
}

// It manages the phase exponent e with phase = i^e to reduce complex number
// multiplication.
std::complex<double> calc_pauli_coeff_internal(
    int pauli_index,
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    std::complex<double> coeff = 0;

    const int size = density_matrix.size();
    const int n_qubit = get_log2(size);
    const int mask = get_mask_of_XY(pauli_index, n_qubit);

    int factor = get_factor(pauli_index, 0, n_qubit);
    coeff += pow_i[factor] * density_matrix[mask][0];

    for (int k = 1; k < size; k++) {
        int i = get_gray_code(k);
        int j = get_gray_code(k - 1);
        int t = get_changed_bit(i, j);
        int c = get_pauli_elem(pauli_index, t, i);
        int d = get_pauli_elem(pauli_index, t, j);
        int f = i ^ mask;

        factor += c;
        factor -= d;
        factor &= 0b11;
        coeff += pow_i[factor] * density_matrix[f][i];
    }

    coeff /= size;
    return coeff;
}

std::complex<double>* calc_inner_prods(
    std::vector<std::vector<std::complex<double>>>& density_matrix,
    std::vector<int>& indices) {
    auto result = new std::complex<double>[indices.size()];
    for (int i = 0; i < indices.size(); i++) {
        result[i] = calc_pauli_coeff_internal(indices[i], density_matrix);
    }
    return result;
}

std::complex<double>* calc_pauli_vector(
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    const int size = density_matrix.size();
    const int length = size * size;
    auto result = new std::complex<double>[length];
    for (int index = 0; index < length; index++) {
        result[index] = calc_pauli_coeff_internal(index, density_matrix);
    }
    return result;
}

}  // namespace jones

}  // namespace paulidecomp
