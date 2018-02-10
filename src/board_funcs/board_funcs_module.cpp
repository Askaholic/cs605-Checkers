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

static PyObject * setup_network_wrapper(PyObject * self, PyObject * args) {
    // There are no arguments
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }
    setup_network();
    Py_RETURN_NONE;
}

static void board_state_to_py_list(const BoardState & board, PyObject * list) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
      size_t len = 1;
      if (board[i] == BLANK) {
        len = 0;
      }
      char tmp = board[i];
      switch (tmp) {
        case BLANK: tmp = '1'; break;
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
}

static PyObject * get_board_wrapper(PyObject * self, PyObject * args) {
    // There are no arguments
    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    auto board = get_board();

    auto list = PyList_New(BOARD_ELEMENTS);
    board_state_to_py_list(board, list);
    return list;
}

static void string_to_board_state(char * board_string, BoardState & board) {
    for (size_t i = 0; i < BOARD_ELEMENTS; i++) {
        char tmp = board_string[i];
        switch (tmp) {
          case 'b': tmp = BLACK_CHECKER; break;
          case 'r': tmp = RED_CHECKER; break;
          case 'B': tmp = BLACK_KING; break;
          case 'R': tmp = RED_KING; break;
          default: tmp = BLANK;
        }
        board.set(i, tmp);
    }
}

static PyObject * get_possible_moves_wrapper(PyObject * self, PyObject * args) {
    int player;
    char * board_string;
    if (!PyArg_ParseTuple(args, "si", &board_string, &player)) {
        return NULL;
    }

    BoardState board;
    string_to_board_state(board_string, board);

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

static PyObject * evaluate_board_wrapper(PyObject * self, PyObject * args) {
    char * board_string;
    if (!PyArg_ParseTuple(args, "s", &board_string)) {
        return NULL;
    }

    BoardState board;
    string_to_board_state(board_string, board);

    auto f = evaluate_board(board);
    return PyFloat_FromDouble(f);
}

static PyObject * min_max_search_wrapper(PyObject * self, PyObject * args) {
    char * board_string;
    int depth;
    int player;
    if (!PyArg_ParseTuple(args, "sii", &board_string, &player, &depth)) {
        return NULL;
    }

    BoardState board;
    string_to_board_state(board_string, board);

    auto search_result = min_max_search(board, player, depth);
    auto tuple = PyTuple_New(2);
    auto list = PyList_New(BOARD_ELEMENTS);
    board_state_to_py_list(*(search_result.first), list);

    PyTuple_SET_ITEM(tuple, 0, list);
    PyTuple_SET_ITEM(tuple, 1, PyLong_FromLong(search_result.second));
    return tuple;
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
    { "setup_network", setup_network_wrapper, METH_VARARGS,
        "Setup the neural network used by the board evaluation function"},
    { "get_board", get_board_wrapper, METH_VARARGS,
        "Get the current board state"},
    { "time_boards", time_boards_wrapper, METH_VARARGS,
        "Print out the amount of time it takes for operations on each type of board" },
    { "get_possible_moves", get_possible_moves_wrapper, METH_VARARGS,
        "Finds all of the available moves given a board state, and which player's turn it is" },
    { "min_max_search", min_max_search_wrapper, METH_VARARGS,
        "Finds the best board to go to given the current board" },
    { "evaluate_board", evaluate_board_wrapper, METH_VARARGS,
        "Evaluates how good a board state is" },
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
