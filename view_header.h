#ifndef vinfo_HEADER_H
#define vinfo_HEADER_H

#include "type.h"

typedef struct
{

    char *mp3_fname;
    FILE *fptr_mp3;

}V_MP3INFO;

OperationType check_operationtype(char opt);
Status read_and_validate_args(char *argv[],V_MP3INFO *vinfo);
Status open_files(V_MP3INFO *vinfo);
void view_operation(V_MP3INFO *vinfo);
uint get_size(unsigned char *size_buffer);

#endif