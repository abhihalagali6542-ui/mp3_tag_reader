#ifndef MP_H
#define MP_H

#include "type.h"

typedef struct
{

    char *mp3_fname;
    FILE *fptr_mp3;

}ID3;

OperationType check_operationtype(char opt);
Status read_and_validate_args(char *argv[],ID3 *info);
Status open_encode_files(ID3 *info);
void view_operation(ID3 *info);
uint get_size(unsigned char *size_buffer);

#endif