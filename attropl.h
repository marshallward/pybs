/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#ifndef ATTROPL_H
#define ATTROPL_H

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

#endif  /* !defined(ATTROPL_H) */
