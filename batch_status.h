/* Python C API to PBS: attropl type
 * Contact: Marshall Ward <pbs@marshallward.org>
 */

#ifndef BATCH_STATUS_H
#define BATCH_STATUS_H

#include <Python.h>
#include <pbs_ifl.h>

/* Typedefs for PBS C batch_status */
typedef struct batch_status batch_status;

struct batch_status_py {
    PyObject_HEAD
    PyObject *name;
    PyObject *attribs;
    PyObject *text;
};

typedef struct batch_status_py batch_status_py;

#endif  /* !BATCH_STATUS_H */
