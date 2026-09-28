#include <stdio.h>
#include <string.h>
#include "mp.h"
#include "type.h"

//-------------------------------------------------------------------------------//

static const char* tag[] = {"TIT2","TPE1","TALB","TYER","TCON","TCOM"};

//-------------------------------------------------------------------------------//

OperationType check_operationtype(char opt)
{
    if(opt == 'v')
    {
        return e_view;
    }
    return e_unsupported;
}

//-------------------------------------------------------------------------------//

Status read_and_validate_args(char *argv[],ID3 *info)
{
    char *dot = strrchr(argv[2],'.');
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERROR : Source file extention must be \".mp3\"\n");
        return e_failure;
    }
    info->mp3_fname = argv[2];

    if(open_encode_files(info) == e_failure)
    {
        printf("ERROR : Unable to access the file\n");
        return e_failure;
    }

    char signature[3];
    fread(signature,3,1,info->fptr_mp3);
    signature[3]=0;
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERROR : Signature of MP3 file doesnt match\n");
         return e_failure;
    }
    fseek(info->fptr_mp3,10,SEEK_SET);
    return e_success;
}

//-------------------------------------------------------------------------------//

Status open_encode_files(ID3 *info)
{
    info->fptr_mp3=fopen(info->mp3_fname,"rb");
    if(info->fptr_mp3==NULL)
    {
        return e_failure;
    }
    return e_success;
}

//-------------------------------------------------------------------------------//

uint get_size(unsigned char *size_buffer)
{
    for(int i=0;i<2;i++)
    {
       unsigned char temp = size_buffer[i];
       size_buffer[i] = size_buffer[3-i]; 
       size_buffer[3-i] = temp;
    }
    
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
    unsigned char size_buffer[4];
    uint size;

    printf("-----------------------------------------------------------\n");

    printf("Sl.No |  Tag Name\t| Information\n");
    printf("-----------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
        fread(tag_buffer,4,1,info->fptr_mp3);
        tag_buffer[4]='\0';
        
        fread(size_buffer,4,1,info->fptr_mp3);
        //printf("%s\n",tag_buffer);
        
        size = get_size(size_buffer);
        //printf("%d\n",size);
        fseek(info->fptr_mp3,3,SEEK_CUR);
        
        char buffer[size];
        fread(buffer,size-1,1,info->fptr_mp3);
        //buffer[size]='\0';

        //printf("%s\n",buffer);

        for(int j=0;j<6;j++)
        {
            if(!strcmp(tag_buffer,tag[j]))
            {
                printf("  %d   | %s \t\t|%s\n",i+1,tag_buffer,buffer);
                break;
            }
        }
    }
    printf("-----------------------------------------------------------\n");

}    