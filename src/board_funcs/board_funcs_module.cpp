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

    auto list = PyList_New(BOARD_ELEMENTS);
    auto b = get_board();
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
      size_t len = 1;
      if (b[i] == BLANK) {
        len = 0;
      }
      char tmp = b[i];
      switch (tmp) {
        case BLANK: tmp = ' '; break;
        case BLACK_CHECKER: tmp = 'b'; break;
        case RED_CHECKER: tmp = 'r'; break;
        case BLACK_KING: tmp = 'B'; break;
        case RED_KING: tmp = 'R'; break;
        default: tmp = '?';
      }
      if (PyList_SetItem(list, i, PyUnicode_FromStringAndSize(&tmp, len)) == -1) {
        // Handle error here?
      }
    }
    return list;
}

static int Board_Converter(PyObject * object, BoardState &board) {

    return 0;
}

static PyObject * get_possible_moves_wrapper(PyObject * self, PyObject * args) {
    int player;
    BoardState board;
    if (!PyArg_ParseTuple(args, "O&i", Board_Converter, &board, &player)) {
        return NULL;
    }

    auto b = get_possible_moves(board, player);
    auto list = PyList_New(b.size());
    for (size_t i = 0; i < b.size(); i++) {
        auto tuple = PyTuple_New(2);
        PyTuple_SET_ITEM(tuple, 0, PyLong_FromSize_t(b[i]._from));
        PyTuple_SET_ITEM(tuple, 1, PyLong_FromSize_t(b[i]._to));
        if (PyList_SetItem(list, i, tuple) == -1) {
            // Handle error here?
        }
    }
    return list;
}

static PyObject * time_boards_wrapper(PyObject * self, PyObject * args) {
    // There are no arguments
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }
    time_boards();
    Py_RETURN_NONE;
}


/****************************************************************/
/*****                    Module Exports                    *****/
/****************************************************************/


static PyMethodDef BoardFuncMethods[] = {
    { "test", test_wrapper, METH_VARARGS,
        "Testing"},
    { "setup_board", setup_board_wrapper, METH_VARARGS,
        "Setup the initial board state"},
    { "get_board", get_board_wrapper, METH_VARARGS,
        "Get the current board state"},
    { "time_boards", time_boards_wrapper, METH_VARARGS,
        "Print out the amount of time it takes for operations on each type of board" },
    { NULL, NULL, 0, NULL }
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
