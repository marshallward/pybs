#include <Python.h>
#include <pbs_ifl.h>
#include <pbs_error.h>


static PyObject*
connect(PyObject *self, PyObject *args)
{
    // TODO: This also should probably create a "server" variable in pbs
    char *server = NULL;
    int id;
    
    if (!PyArg_ParseTuple(args, "|s", &server))
        return NULL;
    
    id = pbs_connect(server);
    return Py_BuildValue("i", id);
}


static char
connect_doc[] = 
    "Connect to the PBS server.";


static PyObject*
disconnect(PyObject *self, PyObject *args)
{
    return NULL;
}


static char
disconnect_doc[] = 
    "Disconnect from the PBS server.";


static PyMethodDef
pbs_methods[] = {
    {"connect", connect, METH_VARARGS, connect_doc},
    {"disconnect", disconnect, METH_VARARGS, disconnect_doc},
    {NULL, NULL}
};

 
static char
pbs_doc[] =
    "I am trying to make a python wrapper to the PBS C API.";


PyMODINIT_FUNC
initpbs()
{
    Py_InitModule3("pbs", pbs_methods, pbs_doc);
}


int
main(int argc, char **argv)
{
    Py_SetProgramName(argv[0]);
    Py_Initialize();

    initpbs();
}
