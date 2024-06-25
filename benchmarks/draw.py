import json
import sys
from collections import defaultdict

import matplotlib.pyplot as plt
import numpy as np

# usage: python draw.py result.txt
# It saves the figure to the same file name with its extension replaced with `.png`, such as result.png.

assert len(sys.argv[1:]) == 1, "Must specify filename as command-line arg."

plt.rcParams["font.size"] = 16

result_file_name = sys.argv[1]
figure_file_name = result_file_name.replace(".txt", ".png")

with open(result_file_name) as f:
    results = json.load(f)

qs = list(range(results["minQubits"], results["reachedQubits"] + 1))
time_dict = defaultdict(list)
for q in qs:
    for method_name, time_raw_result in results["durations"][str(q)].items():
        # The first half of the result is discarded as the JIT compiler is warming up.
        time_dict[method_name].append(
            np.mean(time_raw_result[len(time_raw_result) // 2 :])
        )

fig, ax = plt.subplots()
ax.grid()
ax.set_yscale("log")
for method, time in time_dict.items():
    ax.plot(qs, time, label=method)
ax.legend()
ax.set_xlabel("number of qubits")
ax.set_ylabel("execution time [ns]")

plt.tight_layout()
plt.savefig(figure_file_name)
