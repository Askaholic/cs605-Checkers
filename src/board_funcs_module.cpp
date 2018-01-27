// board_funcs_module.cpp
// Rohan Weeden
// Created: Jan. 27, 2018

// Python wrapper for c++ functions

#include <Python.h>
#include <string>
#include <string.h>
#include "board_funcs.h"

static PyObject * test_wrapper(PyObject * self, PyObject * args) {
    char * input;
    char * result;
    PyObject * ret;

    // Parse arguments
    if (!PyArg_ParseTuple(args, "s", &input)) {
        return NULL;
    }

    result = test(input);

    ret = PyUnicode_FromString(result);
    free(result);

    return ret;
}

static PyMethodDef BoardFuncMethods[] = {
    { "test", test_wrapper, METH_VARARGS, "Testing"},
    { NULL, NULL, 0, NULL}
};

static struct PyModuleDef board_funcs_module = {
    PyModuleDef_HEAD_INIT,
    "board_funcs",
    NULL,
    -1,
    BoardFuncMethods
};

extern "C"
PyMODINIT_FUNC
PyInit_board_funcs(void) {
    return PyModule_Create(&board_funcs_module);
}
