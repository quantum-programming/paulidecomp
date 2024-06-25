import datetime
import os
import subprocess
import sys

# Comment out methods to exclude them from the benchmark.
method_names = [
    "HamaguchiCpp",
    "HamaguchiNumba",
    "HantzkoIterCpp",
    "HantzkoRecCpp",
    "HantzkoRecNumba",
    "JonesCpp",
    "JonesNumba",
    "RomeroCpp",
    "RomeroNumba",
]


assert len(sys.argv) >= 2, "Must specify numQubits as command-line arg."
numQubits = int(sys.argv[1])

current_directory = sys.argv[0].rpartition("/")[0]
timestamp_str = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
absolute_folder_name = os.path.join(current_directory, "memory_usage", timestamp_str)
os.mkdir(absolute_folder_name)
os.mkdir(os.path.join(absolute_folder_name, "data"))
os.mkdir(os.path.join(absolute_folder_name, "imgs"))

for name in method_names:
    subprocess.run(
        [
            "mprof",
            "run",
            "-o",
            os.path.join(absolute_folder_name, "data", f"{name}.dat"),
            "-T",
            "0.02",
            "memory_subprocess.py",
            name,
            str(numQubits),
        ]
    )
    subprocess.run(
        [
            "mprof",
            "plot",
            os.path.join(absolute_folder_name, "data", f"{name}.dat"),
            "-o",
            os.path.join(absolute_folder_name, "imgs", f"{name}.pdf"),
        ]
    )
