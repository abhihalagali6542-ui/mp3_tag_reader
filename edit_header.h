#ifndef EDIT_HEADER_H
#define EDIT_HEADER_H

#include "type.h"

typedef struct
{

    char *mp3_fname;
    FILE *fptr_mp3;

    char *tag_to_edit;
    char *new_data;

    char *temp_mp3_fname;
    FILE *fptr_temp_mp3;

}E_MP3INFO;

OperationType check_operationtype(char opt);
Status read_and_validate_edit_args(char *argv[],E_MP3INFO *einfo);
Status get_tag_to_edit(char e_tag,E_MP3INFO *einfo);
Status open_edit_files(E_MP3INFO *einfo);
void do_edit(E_MP3INFO *einfo);
void convet_little_to_big(int size, unsigned char *new_size);
uint get_e_size(unsigned char *size_buffer);

#endif