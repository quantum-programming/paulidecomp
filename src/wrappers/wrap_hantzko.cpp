#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python/numpy.hpp>

#include "common.hpp"
#include "hantzko.hpp"

namespace paulidecomp {

namespace {

namespace py = boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(_hantzko) {
    Py_Initialize();
    np::initialize();
    py::def(
        "_calc_pauli_vector_recursive",
        wrap_calc_pauli_vector<hantzko::calc_pauli_vector_recursive_internal>);
    py::def(
        "_calc_pauli_vector_iterative",
        wrap_calc_pauli_vector<hantzko::calc_pauli_vector_iterative_internal>);
}

}  // namespace

}  // namespace paulidecomp
