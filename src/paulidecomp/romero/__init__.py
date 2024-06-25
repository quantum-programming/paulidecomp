import numpy as np

from paulidecomp._common import _validate_density_matrix
from paulidecomp._romero import _calc_inner_prods, _calc_pauli_vector


def calc_pauli_vector(matrix):
    _validate_density_matrix(matrix)
    return _calc_pauli_vector(matrix.astype(np.complex_))


def calc_inner_prods(matrix, inds):
    _validate_density_matrix(matrix)
    assert 0 <= inds.min() and inds.max() < matrix.shape[0] ** 2
    return _calc_inner_prods(matrix.astype(np.complex_), inds.astype(np.int32))
