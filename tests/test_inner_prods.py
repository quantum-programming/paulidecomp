import random

import numpy as np

from DensePauliDecomposer.methods import hamaguchi as hamaguchinumba
from DensePauliDecomposer.methods import hantzko as hantzkonumba
from DensePauliDecomposer.methods import jones as jonesnumba
from DensePauliDecomposer.methods import new_romero as romeronumba
from DensePauliDecomposer.utils.randomiser import getRandomMatrix
from paulidecomp import hamaguchi as hamaguchicpp
from paulidecomp import hantzko as hantzkocpp
from paulidecomp import jones as jonescpp
from paulidecomp import romero as romerocpp

max_qubits = 5


def test_hamaguchi_simple():
    for n in range(0, max_qubits + 1):
        mat = np.eye(1 << n, dtype=np.complex_)
        for _ in range(10):
            length = random.randint(1, 4**n)
            inds = np.array(random.sample(range(4**n), length))
            print(inds)
            assert np.allclose(
                hamaguchicpp.calc_inner_prods(mat, inds),
                hamaguchinumba.calcInnerProds(mat, inds),
            )


def test_hantzko_simple():
    for n in range(0, max_qubits + 1):
        mat = np.eye(1 << n, dtype=np.complex_)
        for _ in range(10):
            length = random.randint(1, 4**n)
            inds = np.array(random.sample(range(4**n), length))
            assert np.allclose(
                hantzkocpp.calc_inner_prods(mat, inds),
                hantzkonumba.calcInnerProds(mat, inds),
            )


def test_jones_simple():
    for n in range(0, max_qubits + 1):
        mat = np.eye(1 << n, dtype=np.complex_)
        for _ in range(10):
            length = random.randint(1, 4**n)
            inds = np.array(random.sample(range(4**n), length))
            assert np.allclose(
                jonescpp.calc_inner_prods(mat, inds),
                jonesnumba.calcInnerProds(mat, inds),
            )


def test_romero_simple():
    for n in range(0, max_qubits + 1):
        mat = np.eye(1 << n, dtype=np.complex_)
        for _ in range(10):
            length = random.randint(1, 4**n)
            inds = np.array(random.sample(range(4**n), length))
            assert np.allclose(
                romerocpp.calc_inner_prods(mat, inds),
                romeronumba.calcInnerProds(mat, inds),
            )


def test_all():
    methods = [
        hamaguchicpp.calc_inner_prods,
        hamaguchinumba.calcInnerProds,
        hantzkocpp.calc_inner_prods,
        hantzkonumba.calcInnerProds,
        jonescpp.calc_inner_prods,
        jonesnumba.calcInnerProds,
        romerocpp.calc_inner_prods,
        romeronumba.calcInnerProds,
    ]
    for n in range(1, max_qubits + 1):
        mat = getRandomMatrix(n)
        length = random.randint(1, 4**n)
        inds = np.array(random.sample(range(4**n), length))
        results = [method(mat, inds) for method in methods]
        for result in results[1:]:
            assert np.allclose(results[0], result)
