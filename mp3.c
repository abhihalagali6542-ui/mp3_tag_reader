#include <stdio.h>
#include <string.h>
#include "mp.h"
#include "type.h"

//-------------------------------------------------------------------------------//

static const char* tag[] = {"TIT2","TPE1","TALB","TYER","TCON","TCOM"};
int num=1;

//-------------------------------------------------------------------------------//

// check operation type (argv[1][1])
OperationType check_operationtype(char opt)
{
    if(opt == 'v')
    {
        return e_view;
    }
    else if(opt == 'h')
    {
        return e_help;
    }
    return e_unsupported;
}

//-------------------------------------------------------------------------------//

Status read_and_validate_args(char *argv[],ID3 *info)
{
    // check file extention (.mp3)
    char *dot = strrchr(argv[2],'.');
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERROR : Source file extention must be \".mp3\"\n");
        return e_failure;
    }
    info->mp3_fname = argv[2];

    // open file
    if(open_encode_files(info) == e_failure)
    {
        printf("ERROR : Unable to access the file\n");
        return e_failure;
    }

    // check signature first 3 byts as (ID3)
    char signature[3];
    fread(signature,3,1,info->fptr_mp3);
    signature[3]=0;
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERROR : Signature of MP3 file doesnt match\n");
         return e_failure;
    }

    // set offset at 10th position
    fseek(info->fptr_mp3,10,SEEK_SET);
    return e_success;
}

//-------------------------------------------------------------------------------//

Status open_encode_files(ID3 *info)
{
    info->fptr_mp3=fopen(info->mp3_fname,"rb");
    // check for NULL
    if(info->fptr_mp3==NULL)
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
    
    //printf("size = %u\n",size);
    return size;
}

//-------------------------------------------------------------------------------//

void view_operation(ID3 *info)
{
    char tag_buffer[5];
    unsigned char size_buffer[5];
    uint size;

    printf("-----------------------------------------------------------\n");
    printf("Sl.No |  Tag Name\t| Information\n");
    printf("-----------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
        // read 4 bytes for file for tags
        fread(tag_buffer,4,1,info->fptr_mp3);
        
        // read 4 bytes for size for song info
        fread(size_buffer,4,1,info->fptr_mp3);
        //printf("%s\n",tag_buffer);
        
        size = get_size(size_buffer);
        //printf("%d\n",size);
        
        // skip 3 bytes (2 bytes for flag and 1 bytes for null char)
        fseek(info->fptr_mp3,3,SEEK_CUR);
        
        char buffer[size];
        // read size-1 bytes of song info
        fread(buffer,size-1,1,info->fptr_mp3);
        //printf("%s\n",buffer);

        // compare tag_buffer with tags
        for(int j=0;j<6;j++)
        {
            if(!strcmp(tag_buffer,tag[j]))
            {
                printf("  %d   | %s \t\t|%s\n",num++,tag_buffer,buffer);
                break;
            }
        }
    }
    printf("-----------------------------------------------------------\n");
}    

//-------------------------------------------------------------------------------//
