#pragma once

#include <complex>
#include <vector>

#include "common.hpp"

namespace paulidecomp {

namespace romero {

std::vector<int> get_ind_array(int pauli_index, int n_qubit) {
    std::vector<int> col_inds(1 << n_qubit);

    col_inds[0] = get_mask_of_XY(pauli_index, n_qubit);

    for (int i = 0; i < n_qubit; i++) {
        int j = get_power_of_2(i);
        int p = get_pauli_flag(pauli_index, i);
        int s = (p == 0 || p == 3) ? 1 : -1;
        for (int k = 0; k < j; k++) {
            col_inds[k + j] = col_inds[k] + s * j;
        }
    }

    return col_inds;
}

// It manages the phase exponent e with phase = i^e.
std::vector<int> get_coeff_array(int pauli_index, int n_qubit) {
    std::vector<int> coeffs(1 << n_qubit);
    for (int i = 0; i < n_qubit; i++) {
        int p = get_pauli_flag(pauli_index, i);
        if (p == 2) {
            coeffs[0] += 3;
        }
    }
    coeffs[0] &= 0b11;

    for (int i = 0; i < n_qubit; i++) {
        int j = get_power_of_2(i);
        int p = get_pauli_flag(pauli_index, i);
        int s = (p == 0 || p == 1) ? 0 : 2;
        for (int k = 0; k < j; k++) {
            coeffs[k + j] = coeffs[k] ^ s;
        }
    }

    return coeffs;
}

std::complex<double> calc_pauli_coeff_internal(
    int pauli_index,
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    const int size = density_matrix.size();
    const int n_qubit = get_log2(size);

    auto col_inds = get_ind_array(pauli_index, n_qubit);
    auto factors = get_coeff_array(pauli_index, n_qubit);

    std::complex<double> coeff = 0;
    for (int r = 0; r < size; r++) {
        coeff += pow_i[factors[r]] * density_matrix[col_inds[r]][r];
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

}  // namespace romero

}  // namespace paulidecomp
