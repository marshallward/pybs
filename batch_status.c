/* Python C API to PBS: batch_status type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>
#include <structmember.h>

#include "attropl.h"
#include "batch_status.h"

static void
batch_status_py_free(batch_status_py *self)
{
    Py_XDECREF(self->name);
    Py_XDECREF(self->attribs);
    Py_XDECREF(self->text);
    self->ob_type->tp_free((PyObject *)self);
}


static PyObject *
batch_status_py_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    batch_status_py *self;

    self = (batch_status_py *)type ->tp_alloc(type, 0);
    if (self != NULL) {

        self->name = PyString_FromString("");
        if (!self->name) {
            Py_DECREF(self);
            return NULL;
        }

        // TODO: Implement attribs
        self->attribs = Py_None;
        Py_INCREF(Py_None);

        self->text = PyString_FromString("");
        if (self->text == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}


static int
batch_status_init(batch_status_py *self, PyObject *args, PyObject *kwds)
{
    PyObject *name=NULL, *attribs=NULL, *text=NULL, *tmp;

    static char *kwlist[] = {"name", "attribs", "text", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist,
                &name, &attribs, &text))
        return -1;

    if (name) {
        tmp = self->name;
        Py_INCREF(name);
        self->name = name;
        Py_XDECREF(tmp);
    }

    // TODO: Implement attribs

    if (text) {
        tmp = self->text;
        Py_INCREF(text);
        self->text = text;
        Py_XDECREF(tmp);
    }

    return 0;
}


static PyMethodDef batch_status_methods[] = {
    {NULL}
};


static PyMemberDef batch_status_members[] = {
    {"name", T_OBJECT_EX, offsetof(batch_status_py, name), 0,
        "dunno"},
    {"attribs", T_OBJECT_EX, offsetof(batch_status_py, attribs), 0,
        "dunno"},
    {"text", T_OBJECT_EX, offsetof(batch_status_py, text), 0,
        "dunno"},
    {NULL}
};


static PyTypeObject batch_status_type = {
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
    "pbs.batch_status",                         /* tp_name */
    sizeof(batch_status_py),                    /* tp_basicsize */
    0,                                          /* tp_itemsize */
    (destructor)batch_status_py_free,           /* tp_dealloc */
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
    "batch_status objects",                     /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    batch_status_methods,                       /* tp_methods */
    batch_status_members,                       /* tp_members */
    0,                                          /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    (initproc)batch_status_init,                     /* tp_init */
    0,                                          /* tp_alloc */
    batch_status_py_new                              /* tp_new */
};


#ifndef PyMODINIT_FUNC
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initbatch_status(PyObject *m)
{
    if (PyType_Ready(&batch_status_type) < 0)
        return;

    Py_INCREF(&batch_status_type);
    PyModule_AddObject(m, "batch_status", (PyObject *)&batch_status_type);
}
