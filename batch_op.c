/* Python C API to PBS: batch_op type
 * Contact: Marshall Ward <pybs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>


char *ops[] = {"SET", "UNSET", "INCR", "DECR",
               "EQ", "NE", "GE", "GT", "LE", "LT",
               "DFLT", NULL};


typedef struct {
    PyObject_HEAD
} batch_op;


static PyMethodDef batch_op_methods[] = {
    {NULL}
};


static PyTypeObject batch_op_type = {
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
    "pbs.batch_op",                             /* tp_name */
    sizeof(batch_op),                           /* tp_basicsize */
    0,                                          /* tp_itemsize */
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    0,                                          /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                         /* tp_flags */
    "job batch operators"                       /* tp_doc */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initbatch_op(PyObject *m)
{
    int i;

    if (PyType_Ready(&batch_op_type) < 0)
        return;

    batch_op_type.tp_dict = PyDict_New();

    for(i = 0; ops[i]; i++)
        PyDict_SetItemString(batch_op_type.tp_dict, ops[i], PyInt_FromLong(i));

    Py_INCREF(&batch_op_type);
    PyModule_AddObject(m, "batch_op", (PyObject *)&batch_op_type);
}
