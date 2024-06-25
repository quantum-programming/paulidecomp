import json
import sys

from numba import config

from DensePauliDecomposer.methods import hamaguchi as hamaguchinumba
from DensePauliDecomposer.methods import hantzko as hantzkonumba
from DensePauliDecomposer.methods import jones as jonesnumba
from DensePauliDecomposer.methods import new_romero as romeronumba
from DensePauliDecomposer.utils.benchmarker import repeatedlyTime
from DensePauliDecomposer.utils.randomiser import getRandomMatrix
from paulidecomp import hamaguchi as hamaguchicpp
from paulidecomp import hantzko as hantzkocpp
from paulidecomp import jones as jonescpp
from paulidecomp import romero as romerocpp

# usage: python measure.py result.txt

assert len(sys.argv[1:]) == 1, "Must specify filename as command-line arg."


# Comment out methods to exclude them from the benchmark.
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


fn = sys.argv[1]

minQubits = 1
maxQubits = 15
numReps = 100


results = {
    "numReps": numReps,
    "minQubits": minQubits,
    "maxQubits": maxQubits,
    "reachedQubits": -1,
    "durations": {},
    "jit": not bool(config.DISABLE_JIT),
}


for numQubits in range(minQubits, maxQubits + 1):
    print(f"numQubits = {numQubits}")

    results["durations"][numQubits] = {}

    for name, method in method_dict.items():
        argFunc = lambda n=numQubits: (getRandomMatrix(n),)
        results["durations"][numQubits][name] = repeatedlyTime(method, argFunc, numReps)

    # save all results so far to file (overwriting, so job can be safely interrupted)
    results["reachedQubits"] = numQubits

    with open(fn, "w") as f:
        json.dump(results, f)
