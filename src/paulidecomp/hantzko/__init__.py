import numpy as np

from paulidecomp._common import _validate_density_matrix
from paulidecomp._hantzko import (
    _calc_pauli_vector_iterative,
    _calc_pauli_vector_recursive,
)


def calc_pauli_vector_recursive(matrix):
    _validate_density_matrix(matrix)
    return _calc_pauli_vector_recursive(matrix.astype(np.complex_))


def calc_pauli_vector_iterative(matrix):
    _validate_density_matrix(matrix)
    return _calc_pauli_vector_iterative(matrix.astype(np.complex_))


calc_pauli_vector = calc_pauli_vector_iterative


def calc_inner_prods(matrix, inds):
    _validate_density_matrix(matrix)
    assert 0 <= inds.min() and inds.max() < matrix.shape[0] ** 2
    all_coeffs = _calc_pauli_vector_iterative(matrix)
    return [all_coeffs[i] for i in inds]
