#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python/numpy.hpp>

#include "common.hpp"
#include "hamaguchi.hpp"

namespace paulidecomp {

namespace {

namespace py = boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(_hamaguchi) {
    Py_Initialize();
    np::initialize();
    py::def("_calc_pauli_vector",
            wrap_calc_pauli_vector<hamaguchi::calc_pauli_vector_internal>);
}

}  // namespace

}  // namespace paulidecomp
