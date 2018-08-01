#include "Python.h"

#include <stdio.h>

#include "rpc_handle_wrapper.h"

static PyObject* start_rpc_handle(PyObject *self, PyObject *args) {
  rpc_handle_t * rpc_handle = rpc_handle_create();

  rpc_handle_start(rpc_handle);

  PyObject *result = (PyObject *)rpc_handle;

  Py_XINCREF(result);

  return result;
}

static PyObject* get_query_string(PyObject *self, PyObject *args) {
  rpc_handle_t * rpc_handle = (rpc_handle_t *)args;

  char * query = rpc_handle_get_query(rpc_handle);

  if (query == NULL) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  PyObject * result = PyString_FromString(query);

  Py_XINCREF(result);

  return result;
}

static PyObject* return_selection(PyObject *self, PyObject *args) {
  PyObject * rpc_handle;
  PyObject * string_list;
  if (!PyArg_ParseTuple(args, "OO", &rpc_handle, &string_list)) {
    return NULL;
  }

  int len = PyObject_Length(string_list);
  char* ptr[len];

  for (int i = 0; index < len; i++) {
    PyObject *item;
    item = PyList_GetItem(string_list, i);
    ptr[i] = PyString_AsString(item);
  }

  rpc_handle_return_selection((rpc_handle_t *) rpc_handle, ptr);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef RPCMethods[] = {
    {"start",  start_rpc_handle, METH_VARARGS,
                    "Create a new RPC Handle and start the send and recieve threads."},
    {"get_query", get_query_string, METH_VARARGS,
                    "Get a query as a strinvg."},
    {"return_selection", return_selection, METH_VARARGS,
                    "Return the list of models as strings."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef rpcmodule = {
    PyModuleDef_HEAD_INIT,
    "rpc",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
    RPCMethods
};

PyMODINIT_FUNC PyInit_rpc(void)  {
  return PyModule_Create(&rpcmodule);
}

int main(int argc, char *argv[]) {
  /* Add a built-in module, before Py_Initialize */
  PyImport_AppendInittab("rpc", PyInit_rpc);

  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(argv[0]);

  /* Initialize the Python interpreter.  Required. */
  Py_Initialize();

  /* Optionally import the module; alternatively,
     import can be deferred until the embedded script
     imports it. */
  PyImport_ImportModule("rpc");
}