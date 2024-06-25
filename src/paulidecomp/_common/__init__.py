import numpy as np


def _is_pow2(n: int) -> bool:
    return n > 0 and n == n & -n


def _validate_density_matrix(density_matrix):
    """It validates the given density matrix.
    If the matrix is inappropriate, it raises AssertionError.
    """
    assert isinstance(
        density_matrix, np.ndarray
    ), "The input density matrix must be a np.ndarray instance."
    assert density_matrix.ndim == 2, "The input density matrix must be two-dimensional."
    assert density_matrix.shape[0] == density_matrix.shape[1] and _is_pow2(
        density_matrix.shape[0]
    ), "The size of the input density matrix must be 2^n x 2^n."
