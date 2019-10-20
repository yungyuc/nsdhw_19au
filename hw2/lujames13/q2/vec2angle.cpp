#include <pybind11/pybind11.h>

namespace py = pybind11;

// simple function to test if python can import package or not
int add(int i, int j){
	return i + j;
}


PYBIND11_MODULE(vec2angle, m){
	m.doc() = "pybin11 example plugin";
	m.def("add", &add, "A function which adds two numbers", py::arg("i")=1, py::arg("j")=2);

	m.attr("the_answer") = 42;
	py::object world = py::cast("World");
	m.attr("what") = world;
}