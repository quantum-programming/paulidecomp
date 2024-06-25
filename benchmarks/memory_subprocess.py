import sys

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

# usage: mprof run memory_subprocess.py method_name numQubits


method_dict = {
    "HamaguchiCpp": hamaguchicpp.calc_pauli_vector,
    "HamaguchiNumba": hamaguchinumba.calcPauliVector,
    "HantzkoIterCpp": hantzkocpp.calc_pauli_vector_iterative,
    "HantzkoRecCpp": hantzkocpp.calc_pauli_vector_recursive,
    "HantzkoRecNumba": hantzkonumba.calcPauliVector,
    "JonesCpp": jonescpp.calc_pauli_vector,
    "JonesNumba": jonesnumba.calcPauliVector,
    "RomeroCpp": romerocpp.calc_pauli_vector,
    "RomeroNumba": romeronumba.calcPauliVector,
}

assert (
    len(sys.argv[1:]) >= 1
), "Must specify method_name and numQubits as command-line arg.\ne.g. mprof run memory_subprocess.py HamaguchiNumba 7"
method_name = sys.argv[1]
numQubits = int(sys.argv[2])
assert method_name in method_dict

print(f"method: {method_name}")
print(f"{numQubits = }")

method = method_dict[method_name]
method(np.zeros((1 << numQubits, 1 << numQubits), dtype=np.complex_))
