#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python/numpy.hpp>

namespace paulidecomp {

constexpr std::complex<double> pow_i[4] = {1, 1j, -1, -1j};

// constexpr std::complex<double> pauli_matrices[4][2][2] = {
//     {{1, 0}, {0, 1}}, {{0, 1}, {1, 0}}, {{0, -1j}, {1j, 0}}, {{1, 0}, {0,
//     -1}}};

inline int get_power_of_2(int n) { return 1 << n; }

inline bool is_pow2(int n) { return n > 0 && ((n & -n) == n); }

inline int get_log2(int n) {
    int r = (n & 0xAAAAAAAA) != 0;
    r |= ((n & 0xFFFF0000) != 0) << 4;
    r |= ((n & 0xFF00FF00) != 0) << 3;
    r |= ((n & 0xF0F0F0F0) != 0) << 2;
    r |= ((n & 0xCCCCCCCC) != 0) << 1;
    return r;
}

inline int get_bit(int n, int t) { return (n >> t) & 1; }

inline int get_gray_code(int n) { return n ^ (n >> 1); }

inline int get_changed_bit(int i, int j) { return get_log2(i ^ j); }

inline int get_pauli_flag(int n, int t) { return (n >> (2 * t)) & 0b11; }

inline int get_mask_of_XY(int n, int n_qubit) {
    int mask = 0;
    for (int t = 0; t < n_qubit; t++) {
        int p = get_pauli_flag(n, t);
        int b = (0 < p && p < 3);
        mask |= b << t;
    }
    return mask;
}

namespace py = boost::python;
namespace np = boost::python::numpy;

std::vector<int> ndarray_to_vector_1d(np::ndarray &arr) {
    const int length = arr.shape(0);
    std::vector<int> vec(length);
    const auto ptr = arr.get_data();
    const int stride = arr.strides(0);
    for (int i = 0; i < length; i++) {
        vec[i] = *reinterpret_cast<int *>(ptr + i * stride);
    }
    return vec;
}

std::vector<std::vector<std::complex<double>>> ndarray_to_vector_2d(
    np::ndarray &arr) {
    const int height = arr.shape(0);
    const int width = arr.shape(1);
    std::vector<std::vector<std::complex<double>>> vec(
        height, std::vector<std::complex<double>>(width));
    const auto ptr = arr.get_data();
    const int i_stride = arr.strides(0);
    const int j_stride = arr.strides(1);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            vec[i][j] = *reinterpret_cast<std::complex<double> *>(
                ptr + i * i_stride + j * j_stride);
        }
    }
    return vec;
}

np::ndarray raw_array_to_ndarray_1d(std::complex<double> *ptr, int length) {
    PyObject *capsule = ::PyCapsule_New(ptr, NULL, [](PyObject *self) -> void {
        delete[] reinterpret_cast<std::complex<double> *>(
            PyCapsule_GetPointer(self, NULL));
    });
    py::handle<> h_capsule{capsule};
    py::object owner_capsule{h_capsule};
    np::ndarray result = np::from_data(
        ptr, np::dtype::get_builtin<std::complex<double>>(),
        py::make_tuple(length), py::make_tuple(sizeof(std::complex<double>)),
        owner_capsule);
    return result;
}

template <std::complex<double> *(*calc_pauli_vector_internal)(
    std::vector<std::vector<std::complex<double>>> &)>
np::ndarray wrap_calc_pauli_vector(np::ndarray density_matrix) {
    const int size = density_matrix.shape(0);

    std::vector<std::vector<std::complex<double>>> density_matrix_vec =
        ndarray_to_vector_2d(density_matrix);
    auto result_ptr = calc_pauli_vector_internal(density_matrix_vec);
    auto result_arr = raw_array_to_ndarray_1d(result_ptr, size * size);
    return result_arr;
}

template <std::complex<double> *(*calc_inner_prods_internal)(
    std::vector<std::vector<std::complex<double>>> &, std::vector<int> &)>
np::ndarray wrap_calc_inner_prods(np::ndarray density_matrix,
                                  np::ndarray indices) {
    const int size = density_matrix.shape(0);

    std::vector<std::vector<std::complex<double>>> density_matrix_vec =
        ndarray_to_vector_2d(density_matrix);
    std::vector<int> indices_vec = ndarray_to_vector_1d(indices);
    auto result_ptr =
        calc_inner_prods_internal(density_matrix_vec, indices_vec);
    auto result_arr = raw_array_to_ndarray_1d(result_ptr, indices_vec.size());
    return result_arr;
}

}  // namespace paulidecomp
