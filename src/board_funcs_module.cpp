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
    // Parse arguments
    if (!PyArg_ParseTuple(args, "s", &input)) {
        return NULL;
    }

    char * result;
    PyObject * ret;

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

static PyObject * get_board_wrapper(PyObject * self, PyObject * args) {
    // There are no arguments
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    auto list = PyList_New(32);
    auto b = get_board();
    for (size_t i = 0; i < 32; i++) {
      size_t len = 1;
      if (b.tiles[i] == 0x0) {
        len = 0;
      }
      if (PyList_SetItem(list, i, PyUnicode_FromStringAndSize(&b.tiles[i], len)) == -1) {
        // Handle error here?
      }
    }
    return list;
}


/****************************************************************/
/*****                    Module Exports                    *****/
/****************************************************************/


static PyMethodDef BoardFuncMethods[] = {
    { "test", test_wrapper, METH_VARARGS, "Testing"},
    { "setup_board", setup_board_wrapper, METH_VARARGS, "Setup the initial board state"},
    { "get_board", get_board_wrapper, METH_VARARGS, "Get the current board state"},
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
