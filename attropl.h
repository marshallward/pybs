/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#include <Python.h>
#include <pbs_ifl.h>

typedef struct {
    PyObject_HEAD
    PyObject *name;
    PyObject *resource;
    PyObject *value;
    int op;
} attropl;

typedef struct attropl attropl_c;
