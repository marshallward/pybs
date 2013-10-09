/*  Python interface to the PBS C API
 *  Author: Marshall Ward <marshall.ward@gmail.com>
 * ----------------------------------------------------------------------------
 *  Distributed as part of PyBS, Copyright 2012 Marshall Ward
 */

#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>

#include "attropl.h"
#include "pbsdoc.h"

static PyObject *
pbsmod_connect(PyObject *self, PyObject *args)
{
    // TODO: Server name should also be stored in ``pbs.server``
    char *server = NULL;
    int id;

    if (!PyArg_ParseTuple(args, "|s", &server))
        return NULL;

    id = pbs_connect(server);
    return Py_BuildValue("i", id);
}


static PyObject *
pbsmod_default(PyObject *self, PyObject *args)
{
    char *server;

    if (!PyArg_ParseTuple(args, ""))
        return NULL;

    server = pbs_default();
    return Py_BuildValue("s", server);
}


static PyObject *
pbsmod_deljob(PyObject *self, PyObject *args)
{
    int connect;
    char *job_id;
    char *extend = NULL;
    int rc;

    if(!PyArg_ParseTuple(args, "is|s", &connect, &job_id, &extend))
        return NULL;

    rc = pbs_deljob(connect, job_id, extend);
    return Py_BuildValue("i", rc);
}


static PyObject *
pbsmod_disconnect(PyObject *self, PyObject *args)
{
    // TODO: rc should also be saved in ``pbs.errno``
    int connect;
    int rc;

    if (!PyArg_ParseTuple(args, "i", &connect))
        return NULL;

    rc = pbs_disconnect(connect);
    return Py_BuildValue("i", rc);
}


static PyObject *
pbsmod_selectjob(PyObject *self, PyObject *args)
{
    // Python input fields
    int server_id;
    PyObject *attribs_py;

    // Internal C fields
    int i, len, rc;
    char **jobs;

    PyObject *elem, *attr_py, *jobs_list, *jobs_str;
    attropl_c **attribs_c;
    attropl_c *attr_c;

    if (!PyArg_ParseTuple(args, "iO", &server_id, &attribs_py))
        return NULL;

    // TODO: Safely exit
    if (!PyList_Check(attribs_py))
        printf("oops not a list\n");

    // Testing list contents
    // TODO: Build an attropl chain

    len = (int)PyList_Size(attribs_py);
    attribs_c = malloc(len * sizeof(attropl_c *)); 

    // Pack list elements into a struct linked list
    for(i = 0; i < len; i++)
    {
        elem = PyList_GetItem(attribs_py, i);

        attr_c = malloc(sizeof(attropl_c));
        attribs_c[i] = attr_c;

        attr_py = PyObject_GetAttrString(elem, "name");
        if (attr_py != NULL) {
            Py_INCREF(attr_py);
            attr_c->name = PyString_AsString(attr_py);
            Py_DECREF(attr_py);
        }

        attr_py = PyObject_GetAttrString(elem, "resource");
        if (attr_py != NULL) {
            Py_INCREF(attr_py);
            attr_c->resource = PyString_AsString(attr_py);
            Py_DECREF(attr_py);
        }

        attr_py = PyObject_GetAttrString(elem, "value");
        if (attr_py != NULL) {
            Py_INCREF(attr_py);
            attr_c->value = PyString_AsString(attr_py);
            Py_DECREF(attr_py);
        }

        attr_py = PyObject_GetAttrString(elem, "op");
        if (attr_py != NULL) {
            Py_INCREF(attr_py);
            attr_c->op = (enum batch_op)PyLong_AsLong(attr_py);
            Py_DECREF(attr_py);
        }

        if (i > 0) attribs_c[i-1]->next = attr_c;
    }
    attribs_c[len-1]->next = NULL;

    jobs = pbs_selectjob(server_id, attribs_c[0], NULL);
    
    // TODO: Parse output into a python list
    jobs_list = PyList_New(len);
    Py_INCREF(jobs_list);

    for (i = 0; i < len; i++)
    {
        jobs_str = PyString_FromString(jobs[i]);
        Py_INCREF(jobs_str);
        PyList_SetItem(jobs_list, i, jobs_str);
    }

    // Clean up
    for (i = 0; i < len; i++)
        free(attribs_c[i]);
    free(attribs_c);

    return jobs_list;
}


static PyObject *
pbsmod_submit(PyObject *self, PyObject *args)
{
    // TODO: I don't know how to handle ``attrib``
    int connect;
    struct attropl *attrib = NULL;
    char *script = NULL;
    char *destination = NULL;
    char *extend = NULL;
    char *job_id;

    if(!PyArg_ParseTuple(args, "i|Osss", &connect, &attrib, &script,
                            &destination, &extend))
        return NULL;

    // Testing
    attrib = NULL;
    script = "script.sh";
    destination = NULL;
    extend = NULL;

    job_id = pbs_submit(connect, attrib, script, destination, extend);

    return Py_BuildValue("s", job_id);
}


static PyMethodDef
pbsmod_methods[] = {
    {"connect", pbsmod_connect, METH_VARARGS, pbsmod_connect_doc},
    {"default", pbsmod_default, METH_VARARGS, pbsmod_default_doc},
    {"deljob", pbsmod_deljob, METH_VARARGS, pbsmod_deljob_doc},
    {"disconnect", pbsmod_disconnect, METH_VARARGS, pbsmod_disconnect_doc},
    {"selectjob", pbsmod_selectjob, METH_VARARGS, pbsmod_selectjob_doc},
    {"submit", pbsmod_submit, METH_VARARGS, pbsmod_submit_doc},
    {NULL, NULL}
};


PyMODINIT_FUNC
initpbs()
{
    PyObject *m;
    m = Py_InitModule3("pbs", pbsmod_methods, pbsmod_doc);

    initbatch_op(m);
    initattropl(m);
}


int
main(int argc, char **argv)
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();

    initpbs();
}
