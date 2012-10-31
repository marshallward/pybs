#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>

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


static PyMethodDef
pbsmod_methods[] = {
    {"connect", pbsmod_connect, METH_VARARGS, pbsmod_connect_doc},
    {"default", pbsmod_default, METH_VARARGS, pbsmod_default_doc},
    {"deljob", pbsmod_deljob, METH_VARARGS, pbsmod_deljob_doc},
    {"disconnect", pbsmod_disconnect, METH_VARARGS, pbsmod_disconnect_doc},
    {NULL, NULL}
};

 
PyMODINIT_FUNC
initpbs()
{
    PyObject *m;
    m = Py_InitModule3("pbs", pbsmod_methods, pbsmod_doc);
}


int
main(int argc, char **argv)
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    
    initpbs();
}
