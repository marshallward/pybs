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
    int rc;

    if (!PyArg_ParseTuple(args, "is|Os",
                          &connect, &job_id, &attrib_list, &extend))
        return NULL;

    // TODO: attrib support
    attrib_head = (attrl *)attropl_list_gen(attrib_list);
    rc = pbs_alterjob(connect, job_id, attrib_head, extend);

    attropl_list_free((attropl *)attrib_head);

    return Py_BuildValue("i", rc);
}

static PyObject *
pbsmod_connect(PyObject *self, PyObject *args)
{
    // TODO: Server name should also be stored in ``pbs.server``
    char *server = NULL;
    int id;

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
    char *extend = NULL;
    int rc;

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

    if (!PyArg_ParseTuple(args, "iO|s",
                          &connect, &attrib_list, &extend))
        return NULL;

    if (!PyList_Check(attrib_list)) {
        PyErr_SetString(PyExc_TypeError,
                        "Second argument must be an attribute list");
        return NULL;
    }

    attr_head = attropl_list_gen(attrib_list);
    jobs = pbs_selectjob(connect, attr_head, extend);

    // TODO: Parse output into a python list
    // First count number of elements
    len = 0;
    while (jobs[len]) len++;

    jobs_list = PyList_New(len);
    Py_INCREF(jobs_list);

    for (i = 0; i < len; i++) {
        jobs_str = PyString_FromString(jobs[i]);
        Py_INCREF(jobs_str);
        PyList_SetItem(jobs_list, i, jobs_str);
    }

    attropl_list_free(attr_head);
    return jobs_list;
}


static PyObject *
pbsmod_submit(PyObject *self, PyObject *args)
{
    int connect;
    PyObject *attrib_py = NULL;
    attropl_py *attrib;
    char *script = NULL;
    char *destination = NULL;
    char *extend = NULL;
    char *job_id;
    PyObject *job_output;

    if(!PyArg_ParseTuple(args, "i|Osss", &connect, &attrib_py, &script,
                            &destination, &extend))
        return NULL;

    // Testing
    attrib = NULL;
    script = "script.sh";
    destination = NULL;
    extend = NULL;

    // TODO: Need to construct attrib
    job_id = pbs_submit(connect, NULL, script, destination, extend);
    if (job_id == NULL) {
        PyErr_SetString(PbsError, pbs_geterrmsg(connect));
        free(job_id);
    }

    job_output = Py_BuildValue("s", job_id);
    free(job_id);

    return job_output;
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
    if (m == NULL)
        return;

    PbsError = PyErr_NewException("pbs.error", NULL, NULL);
    Py_INCREF(PbsError);
    PyModule_AddObject(m, "error", PbsError);

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
