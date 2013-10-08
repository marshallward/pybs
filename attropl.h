/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>

typedef struct {
    PyObject_HEAD
    PyObject *name;
    PyObject *resource;
    PyObject *value;
    int op;
} attropl;
