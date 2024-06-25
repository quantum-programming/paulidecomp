#pragma once

#include <complex>
#include <iostream>
#include <vector>

#include "common.hpp"

namespace paulidecomp {

namespace hantzko {

std::complex<double>* calc_pauli_vector_recursive_internal(
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    const int size = density_matrix.size();
    const int n_qubit = get_log2(size);

    if (size == 1) {
        auto coeff = new std::complex<double>[1];
        *coeff = density_matrix[0][0];
        return coeff;
    }

    const int half_size = size / 2;

    // make a list { coeff1, coeffX, coeffY, coeffZ } for the loop.
    // 4 x half_size x half_size
    std::vector<std::vector<std::vector<std::complex<double>>>> coeff_list(
        4, std::vector<std::vector<std::complex<double>>>(
               half_size, std::vector<std::complex<double>>(half_size)));

    for (int i = 0; i < half_size; i++) {
        for (int j = 0; j < half_size; j++) {
            coeff_list[0][i][j] =
                0.5 * (density_matrix[i][j] +
                       density_matrix[i + half_size][j + half_size]);
            coeff_list[1][i][j] = 0.5 * (density_matrix[i + half_size][j] +
                                         density_matrix[i][j + half_size]);
            coeff_list[2][i][j] = std::complex<double>(0.0, -0.5) *
                                  (density_matrix[i + half_size][j] -
                                   density_matrix[i][j + half_size]);
            coeff_list[3][i][j] =
                0.5 * (density_matrix[i][j] -
                       density_matrix[i + half_size][j + half_size]);
        }
    }

    // free matrix
    density_matrix = {{}};

    const int length_full = size * size;
    const int length_quarter = length_full / 4;
    auto coeffs = new std::complex<double>[length_full];
    auto insert_ptr = coeffs;
    for (auto& mat : coeff_list) {
        auto sub_dec = calc_pauli_vector_recursive_internal(mat);
        insert_ptr = std::copy_n(sub_dec, length_quarter, insert_ptr);
        delete[] sub_dec;
    }

    return coeffs;
}

std::complex<double>* calc_pauli_vector_iterative_internal(
    std::vector<std::vector<std::complex<double>>>& density_matrix) {
    const int size = density_matrix.size();
    const int n_qubit = get_log2(size);
    const int length = size * size;

    std::vector<std::complex<double>> omega_list;
    omega_list.reserve(length);
    for (auto&& row : density_matrix) {
        omega_list.insert(omega_list.end(), row.begin(), row.end());
    }

    // free matrix
    density_matrix = {{}};

    for (int i = n_qubit - 1; i >= 0; i--) {
        const int half_size = 1 << i;
        const int full_size = 1 << (i + 1);
        const int half_size_squared = half_size * half_size;
        const int full_size_squared = full_size * full_size;
        std::vector<std::complex<double>> new_omega_list(length);
        for (int offset = 0; offset < length; offset += full_size_squared) {
            for (int i = 0; i < half_size; i++) {
                for (int j = 0; j < half_size; j++) {
                    auto i0j0 = omega_list[offset + i * full_size + j];
                    auto i1j1 =
                        omega_list[offset + (i + half_size) * full_size + j +
                                   half_size];

                    new_omega_list[offset + i * half_size + j] = i0j0 + i1j1;
                    new_omega_list[offset + 3 * half_size_squared +
                                   i * half_size + j] = i0j0 - i1j1;

                    auto i1j0 =
                        omega_list[offset + (i + half_size) * full_size + j];
                    auto i0j1 =
                        omega_list[offset + i * full_size + j + half_size];

                    new_omega_list[offset + 1 * half_size_squared +
                                   i * half_size + j] = i1j0 + i0j1;
                    new_omega_list[offset + 2 * half_size_squared +
                                   i * half_size + j] =
                        std::complex<double>(0.0, -1.0) * (i1j0 - i0j1);
                }
            }
        }
        omega_list = std::move(new_omega_list);
    }

    auto result = new std::complex<double>[length];
    const double normalize = 1.0 / size;
    for (int i = 0; i < length; i++) {
        result[i] = omega_list[i] * normalize;
    }

    return result;
}

}  // namespace hantzko

}  // namespace paulidecomp
