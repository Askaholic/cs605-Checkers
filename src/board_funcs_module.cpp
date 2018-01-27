// board_funcs_module.cpp
// Rohan Weeden
// Created: Jan. 27, 2018

// Python module exports for C++ functions

#include <Python.h>
#include <string>
#include <string.h>
#include "board_funcs.h"


/****************************************************************/
/*****                   Wraper Functions                   *****/
/****************************************************************/


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

static PyObject * setup_board_wrapper(PyObject * self, PyObject * args) {
    // There are no arguments
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }
    setup_board();
    Py_RETURN_NONE;
}


/****************************************************************/
/*****                    Module Exports                    *****/
/****************************************************************/


static PyMethodDef BoardFuncMethods[] = {
    { "test", test_wrapper, METH_VARARGS, "Testing"},
    { "setup_board", setup_board_wrapper, METH_VARARGS, "Setup the initial board state"},
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
