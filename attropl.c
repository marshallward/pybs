/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>
#include <structmember.h>

#include "attropl.h"

static void
attropl_py_dealloc(attropl_py *self)
{
    Py_XDECREF(self->name);
    Py_XDECREF(self->resource);
    Py_XDECREF(self->value);
    self->ob_type->tp_free((PyObject *)self);
}


static PyObject *
attropl_py_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    attropl_py *self;

    self = (attropl_py *)type ->tp_alloc(type, 0);
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

        /* TODO: Reference batch_op.DFLT = 10 */
        self->op = 10;
    }

    return (PyObject *)self;
}


static int
attropl_init(attropl_py *self, PyObject *args, PyObject *kwds)
{
    PyObject *name=NULL, *resource=NULL, *value=NULL, *tmp;

    static char *kwlist[] = {"name", "resource", "value", "op", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOOi", kwlist,
                &name, &resource, &value, &self->op))
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
        Py_XDECREF(tmp);
    }

    if (value) {
        tmp = self->value;
        Py_INCREF(value);
        self->value = value;
        Py_XDECREF(tmp);
    }

    return 0;
}


attropl *
attropl_gen(PyObject *attropl_list)
{
    int i, len;
    PyObject *attr_py, *member_py;
    attropl *attr_head, *attr, *attr_next;

    // TODO: Iterate over attr_members (My C-fu is currently too weak)
    static char *attr_members[] = {"name", "resource", "value", NULL};
    char *mname;

    if (!attropl_list)
        return NULL;

    len = PyList_Size(attropl_list);
    if (!len)
        return NULL;

    attr_head = malloc(sizeof(attropl));
    attr_next = attr_head;

    for (i = 0; i < len; i++) {
        attr_py = PyList_GetItem(attropl_list, i);
        attr = attr_next;

        member_py = PyObject_GetAttrString(attr_py, "name");
        if (member_py != NULL) {
            Py_INCREF(member_py);
            attr->name = PyString_AsString(member_py);
            Py_DECREF(member_py);
        }

        member_py = PyObject_GetAttrString(attr_py, "resource");
        if (member_py != NULL) {
            Py_INCREF(member_py);
            attr->resource = PyString_AsString(member_py);
            Py_DECREF(member_py);
        }

        member_py = PyObject_GetAttrString(attr_py, "value");
        if (member_py != NULL) {
            Py_INCREF(member_py);
            attr->value = PyString_AsString(member_py);
            Py_DECREF(member_py);
        }

        member_py = PyObject_GetAttrString(attr_py, "op");
        if (member_py != NULL) {
            Py_INCREF(member_py);
            attr->op = (enum batch_op)PyLong_AsLong(member_py);
            Py_DECREF(member_py);
        }

        if (i < (len - 1)) {
            attr_next = malloc(sizeof(attropl));
            attr->next = attr_next;
        } else {
            attr->next = NULL;
        }
    }

    return attr_head;
}


void
attropl_free(attropl *attr)
{
    attropl *attr_next;

    while (attr) {
        attr_next = attr->next;
        free(attr);
        attr = attr_next;
    }
}


/* attrl wrapper functions */
attrl *
attrl_gen(PyObject *attrl_list)
{
    return (attrl *)attropl_gen(attrl_list);
}


void
attrl_free(attrl *attr)
{
    attropl_free((attropl *)attr);
}


static PyMethodDef attropl_methods[] = {
    {NULL}
};


static PyMemberDef attropl_members[] = {
    {"name", T_OBJECT_EX, offsetof(attropl_py, name), 0,
        "attribute name"},
    {"resource", T_OBJECT_EX, offsetof(attropl_py, resource), 0,
        "resource name"},
    {"value", T_OBJECT_EX, offsetof(attropl_py, value), 0,
        "attribute value"},
    {"op", T_INT, offsetof(attropl_py, op), 0,
        "attribute batch operator"},
    {NULL}
};


static PyTypeObject attropl_type = {
    PyObject_HEAD_INIT(NULL)
    0,                                          /* ob_size */
    "pbs.attropl",                              /* tp_name */
    sizeof(attropl_py),                         /* tp_basicsize */
    0,                                          /* tp_itemsize */
    (destructor)attropl_py_dealloc,             /* tp_dealloc */
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
    attropl_py_new                              /* tp_new */
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
