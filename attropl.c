/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    PyObject *name;
    PyObject *resource;
    PyObject *value;
} attropl;


static void
attropl_dealloc(attropl *self)
{
    Py_XDECREF(self->name);
    Py_XDECREF(self->resource);
    Py_XDECREF(self->value);
    self->ob_type->tp_free((PyObject *)self);
}


static PyObject *
attropl_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    attropl *self;

    self = (attropl *)type ->tp_alloc(type, 0);
    if (self != NULL) {

        self->name = PyString_FromString("");
        if (self->name == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->resource = PyString_FromString("");
        if (self->resource == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->value = PyString_FromString("");
        if (self->value == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}


static int
attropl_init(attropl *self, PyObject *args, PyObject *kwds)
{
    PyObject *name=NULL, *resource=NULL, *value=NULL, *tmp;

    static char *kwlist[] = {"name", "resource", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist,
                &name, &resource, &value))
        return -1;

    if (name) {
        tmp = self->name;
        Py_INCREF(name);
        self->name = name;
        Py_XDECREF(tmp);
    }

    if (resource) {
        tmp = self->resource;
        Py_INCREF(resource);
        self->resource = resource;
        Py_XDECREF(resource);
    }

    if (value) {
        tmp = self->value;
        Py_INCREF(value);
        self->value = value;
        Py_XDECREF(value);
    }

    return 0;
}


static PyMethodDef attropl_methods[] = {
    {NULL}
};


static PyMemberDef attropl_members[] = {
    {"name", T_OBJECT_EX, offsetof(attropl, name), 0,
        "attribute name"},
    {"resource", T_OBJECT_EX, offsetof(attropl, resource), 0,
        "resource name"},
    {"value", T_OBJECT_EX, offsetof(attropl, value), 0,
        "attribute value"},
    {NULL}
};


static PyTypeObject attropl_type = {
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
    "pbs.attropl",                              /* tp_name */
    sizeof(attropl),                            /* tp_basicsize */
    0,                                          /* tp_itemsize */
    (destructor)attropl_dealloc,                /* tp_dealloc */
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "attropl objects",                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    attropl_methods,                            /* tp_methods */
    attropl_members,                            /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)attropl_init,                     /* tp_init */
    0,                                          /* tp_alloc */
    attropl_new                                 /* tp_new */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initattropl(PyObject *m)
{
    //attropl_type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&attropl_type) < 0)
        return;

    Py_INCREF(&attropl_type);
    PyModule_AddObject(m, "attropl", (PyObject *)&attropl_type);
}
