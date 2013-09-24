/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>


typedef struct {
    PyObject_HEAD
} attroplObject;


static PyTypeObject attroplType = {
    PyObject_HEAD_INIT(NULL)
    0,                              /* ob_size */
    "pbs.attropl",                  /* tp_name */
    sizeof(attroplObject),          /* tp_basicsize */
    0,                              /* tp_itemsize */
    0,                              /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_compare */
    0,                              /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    "attropl type"                  /* tp_doc */
};


static PyMethodDef attropl_methods[] = {
    {NULL}
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initattropl(PyObject *m)
{
    attroplType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&attroplType) < 0)
        return;

    Py_INCREF(&attroplType);
    PyModule_AddObject(m, "attropl", (PyObject *)&attroplType);
}
