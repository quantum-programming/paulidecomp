import json
from collections import defaultdict

import matplotlib.pyplot as plt
import numpy as np


def from_result_to_plot(results):
    qs = list(range(results["minQubits"], results["reachedQubits"] + 1))
    time_dict = defaultdict(list)
    for q in qs:
        for method_name, time_raw_result in results["durations"][str(q)].items():
            # The first half of the result is discarded as the JIT compiler is warming up.
            time_dict[method_name].append(
                np.mean(time_raw_result[len(time_raw_result) // 2 :])
            )
    return time_dict


plt.rcParams["font.size"] = 17
plt.rcParams["text.usetex"] = True
plt.rcParams["font.family"] = "Times New Roman"

file_names = [
    "result.txt",
    "result_8n.txt",
]
figure_file_name = "paulidecomp_benchmark.pdf"

results_list = dict()
for file_name in file_names:
    with open(file_name) as f:
        results_list[file_name.partition(".")[0]] = from_result_to_plot(json.load(f))

combined_result = {
    "Hamaguchi et al.": results_list["result"]["HamaguchiCpp"],
    "Hantzko et al. (iterative)": results_list["result"]["HantzkoIterCpp"],
    "Hantzko et al. (recursive)": results_list["result"]["HantzkoRecCpp"],
    "Romero and Santos-Suárez": results_list["result_8n"]["RomeroCpp"],
    "Jones": results_list["result_8n"]["JonesCpp"],
}

fig, ax = plt.subplots(figsize=(9, 4))
ax.grid()
ax.set_yscale("log")
for method, time in combined_result.items():
    ax.plot(list(range(1, len(time) + 1)), time, label=method, marker=".")
ax.legend(bbox_to_anchor=(1.05, 1), loc="upper left", borderaxespad=0)
ax.set_xticks(np.arange(1, 13))
ax.set_yticks(10 ** np.arange(3, 11))
ax.set_xlabel("Number of qubits")
ax.set_ylabel("Runtime [ns]")

plt.tight_layout()
plt.savefig(figure_file_name)
