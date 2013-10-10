/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#ifndef ATTROPL_H
#define ATTROPL_H

#include <Python.h>
#include <pbs_ifl.h>

/* Typedefs for PBS C attrl and attropl */
typedef struct attrl attrl;
typedef struct attropl attropl;

struct attropl_py {
    PyObject_HEAD
    PyObject *name;
    PyObject *resource;
    PyObject *value;
    enum batch_op op;
};

/* Typedef both attrl and attropl to the same struct */
typedef struct attrpol_py attrl_py;
typedef struct attropl_py attropl_py;

#endif  /* !defined(ATTROPL_H) */
