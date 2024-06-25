import numpy as np

from DensePauliDecomposer.methods import hamaguchi as hamaguchinumba
from DensePauliDecomposer.methods import hantzko as hantzkonumba
from DensePauliDecomposer.methods import jones as jonesnumba
from DensePauliDecomposer.methods import new_romero as romeronumba
from DensePauliDecomposer.utils.correcter import testMethods as helper
from paulidecomp import hamaguchi as hamaguchicpp
from paulidecomp import hantzko as hantzkocpp
from paulidecomp import jones as jonescpp
from paulidecomp import romero as romerocpp


def test_hamaguchi_simple():
    for n in range(0, 4):
        in_ = np.eye(1 << n, dtype=np.complex_)
        assert np.allclose(
            hamaguchicpp.calc_pauli_vector(in_), hamaguchinumba.calcPauliVector(in_)
        )


def test_hantzko_simple():
    for n in range(0, 4):
        in_ = np.eye(1 << n, dtype=np.complex_)
        assert np.allclose(
            hantzkocpp.calc_pauli_vector_recursive(in_),
            hantzkonumba.calcPauliVector(in_),
        )
        assert np.allclose(
            hantzkocpp.calc_pauli_vector_iterative(in_),
            hantzkonumba.calcPauliVector(in_),
        )


def test_jones_simple():
    for n in range(0, 4):
        in_ = np.eye(1 << n, dtype=np.complex_)
        assert np.allclose(
            jonescpp.calc_pauli_vector(in_), jonesnumba.calcPauliVector(in_)
        )


def test_romero_simple():
    for n in range(0, 4):
        in_ = np.eye(1 << n, dtype=np.complex_)
        assert np.allclose(
            romerocpp.calc_pauli_vector(in_), romeronumba.calcPauliVector(in_)
        )


def test_all():
    helper(
        [
            hamaguchicpp.calc_pauli_vector,
            hamaguchinumba.calcPauliVector,
            hantzkocpp.calc_pauli_vector_recursive,
            hantzkocpp.calc_pauli_vector_iterative,
            hantzkonumba.calcPauliVector,
            jonescpp.calc_pauli_vector,
            jonesnumba.calcPauliVector,
            romerocpp.calc_pauli_vector,
            romeronumba.calcPauliVector,
        ]
    )
