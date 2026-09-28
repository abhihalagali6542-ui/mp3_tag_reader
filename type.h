#ifndef TYPE_H
#define TYPE_H

/* User defined types */
typedef unsigned int uint;

typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported
} OperationType;

#endif