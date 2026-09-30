#include<stdio.h>
#include<string.h>
#include"type.h"
#include"edit_header.h"

//-------------------------------------------------------------------------------//

Status read_and_validate_edit_args(char *argv[],E_MP3INFO *einfo)
{
    if(get_tag_to_edit(argv[2][1],einfo) == e_failure)
    {
        return e_failure;
    }

    char *dot = strrchr(argv[4],'.');
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERROR : Source file extention must be \".mp3\"\n");
        return e_failure;
    }
    einfo->mp3_fname = argv[4];
    einfo->temp_mp3_fname= "temp.mp3";

    if(open_edit_files(einfo) == e_failure)
    {
        printf("ERROR : Unable to access the file\n");
        return e_failure;
    }

    // check signature first 3 byts as (V_MP3INFO)
    char signature[3];
    fread(signature,3,1,einfo->fptr_mp3);
    signature[3]=0;
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERROR : Signature of MP3 file doesnt match\n");
         return e_failure;
    }

    // set offset at 10th position
    rewind(einfo->fptr_mp3);
    return e_success;
}

//-------------------------------------------------------------------------------//

Status open_edit_files(E_MP3INFO *einfo)
{
    einfo->fptr_mp3=fopen(einfo->mp3_fname,"rb");
    // check for NULL
    if(einfo->fptr_mp3==NULL)
    {
        return e_failure;
    }
    einfo->fptr_temp_mp3= fopen(einfo->temp_mp3_fname,"wb");
    if(einfo->fptr_temp_mp3==NULL)
    {
        return e_failure;
    }
    return e_success;
}

//-------------------------------------------------------------------------------//

uint get_size(unsigned char *size_buffer)
{
    // convert big endiness to little
    for(int i=0;i<2;i++)
    {
       unsigned char temp = size_buffer[i];
       size_buffer[i] = size_buffer[3-i]; 
       size_buffer[3-i] = temp;
    }
    
    // get size
    uint size;
    unsigned char *ptr = (unsigned char *)&size;
    for(int i=0;i<4;i++)
    {
        ptr[i]=size_buffer[i];
    }
    return size;
}

//-------------------------------------------------------------------------------//

void convet_little_to_big(int size, unsigned char *new_size)
{
    unsigned char *ptr = (unsigned char *)&size;
    for(int i=0;i<2;i++)
    {
        unsigned char temp = ptr[i];
        ptr[i] = prt[3-i];
        prt[3-i] = temp;
    }
    new_size = ptr;
}

//-------------------------------------------------------------------------------//

void do_edit(E_MP3INFO *einfo)
{
    char tag_buffer[5];
    char info_buffer[size];
    unsigned char size_buffer[4];
    unsigned char new_size[4];
    uint size;

    // copy 10 bytes of header
    char *header_buffer[10];
    fread(header_buffer,10,1,einfo->fptr_mp3);
    fwrite(header_buffer,10,1,einfo->fptr_temp_mp3);

    for(int i=0;i<6;i++)
    {
        // read nd write 4 bytes for file for tags
        fread(tag_buffer,4,1,einfo->fptr_mp3);
        fwrite(tag_buffer,4,1,einfo->fptr_temp_mp3);

        // read 4 bytes for size for song einfo
        fread(size_buffer,4,1,einfo->fptr_mp3);
        size = get_size(size_buffer);

        if(strcmp(tag_buffer,tag_to_edit)==0)
        {
            // add new info and new size
            convet_little_to_big((strlen(argv[3])+1),new_size);
            fwrite(size_buffer,4,1,einfo->fptr_temp_mp3);
            break;
        }

        
        fwrite(size_buffer,4,1,einfo->fptr_temp_mp3);
        
        // read 3 bytes (2 bytes for flag and 1 bytes for null char)
        fread(einfo->fptr_mp3,3,1,einfo->fptr_temp_mp3);
    
        // read size-1 bytes of song vinfo
        fread(info_buffer,size-1,1,einfo->fptr_mp3);
        fwrite(info_buffer,size-1,1,einfo->fptr_temp_mp3);
    }


    return;
}

//-------------------------------------------------------------------------------//

Status get_tag_to_edit(char e_tag, E_MP3INFO *einfo)
{
    switch(e_tag)
    {
        case 't':
           einfo->tag_to_edit = "TIT2";
            break;

        case 'a':
           einfo->tag_to_edit = "TPE1";
            break;

        case 'A':
           einfo->tag_to_edit = "TALB";
            break;

        case 'm':
           einfo->tag_to_edit = "TYER";
            break;

        case 'y':
           einfo->tag_to_edit = "TCON";
            break;

        case 'c':
           einfo->tag_to_edit = "COMM";
            break;
        default :
            return e_failure;
    }
    e_success;
}