from setuptools import Extension, setup

ext_modules_libraries = ["boost_python310", "boost_numpy310"]

setup(
    name="paulidecomp",
    version="0.1.0",
    packages=[
        "paulidecomp",
        "DensePauliDecomposer",
    ],
    package_dir={
        "": "src",
    },
    ext_package="paulidecomp",
    ext_modules=[
        Extension(
            "_hamaguchi",
            ["src/wrappers/wrap_hamaguchi.cpp"],
            include_dirs=["include"],
            libraries=ext_modules_libraries,
        ),
        Extension(
            "_hantzko",
            ["src/wrappers/wrap_hantzko.cpp"],
            include_dirs=["include"],
            libraries=ext_modules_libraries,
        ),
        Extension(
            "_jones",
            ["src/wrappers/wrap_jones.cpp"],
            include_dirs=["include"],
            libraries=ext_modules_libraries,
        ),
        Extension(
            "_romero",
            ["src/wrappers/wrap_romero.cpp"],
            include_dirs=["include"],
            libraries=ext_modules_libraries,
        ),
    ],
)
