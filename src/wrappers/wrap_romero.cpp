#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python/numpy.hpp>

#include "common.hpp"
#include "romero.hpp"

namespace paulidecomp {

namespace {

namespace py = boost::python;
namespace np = boost::python::numpy;

BOOST_PYTHON_MODULE(_romero) {
    Py_Initialize();
    np::initialize();
    py::def("_calc_inner_prods",
            wrap_calc_inner_prods<romero::calc_inner_prods>);
    py::def("_calc_pauli_vector",
            wrap_calc_pauli_vector<romero::calc_pauli_vector>);
}

}  // namespace

}  // namespace paulidecomp
