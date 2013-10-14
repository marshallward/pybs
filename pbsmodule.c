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

static PyObject *PbsError;

static PyObject *
pbsmod_alterjob(PyObject *self, PyObject *args)
{
    // Python input variables
    int connect;
    char *job_id;
    PyObject *attrib_list;
    char *extend;

    // Internal variables
    attrl *attrib_head;

    attrib_list = NULL;
    extend = NULL;
    if (!PyArg_ParseTuple(args, "is|Os",
                          &connect, &job_id, &attrib_list, &extend))
        return NULL;

    attrib_head = attrl_gen(attrib_list);
    if (pbs_alterjob(connect, job_id, attrib_head, extend))
    {
        PyErr_SetString(PbsError, pbs_geterrmsg(connect));
        return NULL;
    }

    attrl_free(attrib_head);
    Py_RETURN_NONE;
}

static PyObject *
pbsmod_connect(PyObject *self, PyObject *args)
{
    // TODO: Server name should also be stored in ``pbs.server``
    char *server;
    int id;

    server = NULL;
    if (!PyArg_ParseTuple(args, "|s",
                          &server))
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
    char *extend;
    int rc;

    extend = NULL;
    if(!PyArg_ParseTuple(args, "is|s", &connect, &job_id, &extend))
        return NULL;

    rc = pbs_deljob(connect, job_id, extend);
    if (rc != 0) {
        PyErr_SetString(PbsError, pbs_geterrmsg(connect));
        return NULL;
    }

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
    int connect;
    PyObject *attrib_list;
    char *extend;

    // Internal C fields
    int i, len;
    attropl *attr_head;
    char **jobs;
    PyObject *jobs_list, *jobs_str;

    attrib_list = NULL;
    extend = NULL;
    if (!PyArg_ParseTuple(args, "i|Os",
                          &connect, &attrib_list, &extend))
        return NULL;

    if (attrib_list && !PyList_Check(attrib_list)) {
        PyErr_SetString(PyExc_TypeError,
                        "Second argument must be an attribute list");
        return NULL;
    }

    attr_head = attropl_gen(attrib_list);
    jobs = pbs_selectjob(connect, attr_head, extend);

    /* Pre-count the number of elements */
    for (len = 0; jobs[len]; len++);

    jobs_list = PyList_New(len);
    Py_INCREF(jobs_list);

    for (i = 0; i < len; i++) {
        jobs_str = PyString_FromString(jobs[i]);
        Py_INCREF(jobs_str);
        PyList_SetItem(jobs_list, i, jobs_str);
    }

    attropl_free(attr_head);
    return jobs_list;
}


static PyObject *
pbsmod_submit(PyObject *self, PyObject *args)
{
    // Python input fields
    int connect;
    PyObject *attrib_list;
    char *script;
    char *destination;
    char *extend;

    // Internal variables
    attropl *attrib_head;
    char *job_id;
    PyObject *job_output;

    attrib_list = NULL;
    script = NULL;
    destination = NULL;
    extend = NULL;
    if(!PyArg_ParseTuple(args, "i|Osss", &connect, &attrib_list, &script,
                         &destination, &extend))
        return NULL;

    attrib_head = attropl_gen(attrib_list);

    job_id = pbs_submit(connect, attrib_head, script, destination, extend);
    if (job_id == NULL) {
        PyErr_SetString(PbsError, pbs_geterrmsg(connect));
        free(job_id);
        return NULL;
    }

    job_output = Py_BuildValue("s", job_id);
    free(job_id);

    return job_output;
}


static PyMethodDef
pbsmod_methods[] = {
    {"alterjob", pbsmod_alterjob, METH_VARARGS, pbsmod_alterjob_doc},
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
    if (m == NULL)
        return;

    PbsError = PyErr_NewException("pbs.error", NULL, NULL);
    Py_INCREF(PbsError);
    PyModule_AddObject(m, "error", PbsError);

    initbatch_op(m);
    initattropl(m);
    initbatch_status(m);
}


int
main(int argc, char **argv)
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();

    initpbs();
}
