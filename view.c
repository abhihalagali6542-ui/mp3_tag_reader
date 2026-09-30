#include <stdio.h>
#include <string.h>
#include "view_header.h"
#include "type.h"

//-------------------------------------------------------------------------------//

static const char* tag[] = {"TIT2","TPE1","TALB","TYER","TCON","COMM"};
int num=1;

//-------------------------------------------------------------------------------//

Status read_and_validate_args(char *argv[],V_MP3INFO *vinfo)
{
    // check file extention (.mp3)
    char *dot = strrchr(argv[2],'.');
    if(strcmp(dot,".mp3")!=0)
    {
        printf("ERROR : Source file extention must be \".mp3\"\n");
        return e_failure;
    }
    vinfo->mp3_fname = argv[2];

    // open file
    if(open_files(vinfo) == e_failure)
    {
        printf("ERROR : Unable to access the file\n");
        return e_failure;
    }

    // check signature first 3 byts as (V_MP3INFO)
    char signature[3];
    fread(signature,3,1,vinfo->fptr_mp3);
    signature[3]=0;
    if(strcmp(signature,"ID3")!=0)
    {
        printf("ERROR : Signature of MP3 file doesnt match\n");
         return e_failure;
    }

    // set offset at 10th position
    fseek(vinfo->fptr_mp3,10,SEEK_SET);
    return e_success;
}

//-------------------------------------------------------------------------------//

Status open_files(V_MP3INFO *vinfo)
{
    vinfo->fptr_mp3=fopen(vinfo->mp3_fname,"rb");
    // check for NULL
    if(vinfo->fptr_mp3==NULL)
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
    // sscanf(size_buffer,"%d",&size);
    // printf("size = %u\n",size);
    return size;
}

//-------------------------------------------------------------------------------//

void view_operation(V_MP3INFO *vinfo)
{
    char tag_buffer[5];
    unsigned char size_buffer[4];
    uint size;

    printf("-----------------------------------------------------------\n");
    printf("SL.NO |  TAG \t|  INFORMATION\n");
    printf("-----------------------------------------------------------\n");
    for(int i=0;i<6;i++)
    {
        // read 4 bytes for file for tags
        fread(tag_buffer,4,1,vinfo->fptr_mp3);
        tag_buffer[4]='\0';

        // read 4 bytes for size for song vinfo
        fread(size_buffer,4,1,vinfo->fptr_mp3);
        //printf("%s\n",tag_buffer);
        //printf("%s\n",size_buffer);

        size = get_size(size_buffer);
        //printf("%d\n",size);
        
        // skip 3 bytes (2 bytes for flag and 1 bytes for null char)
        fseek(vinfo->fptr_mp3,3,SEEK_CUR);
        
        char buffer[size];
        // read size-1 bytes of song vinfo
        fread(buffer,size-1,1,vinfo->fptr_mp3);
        buffer[size-1]='\0';
        //printf("%s\n",buffer);

        // compare tag_buffer with tags
        for(int j=0;j<6;j++)
        {
            if(!strcmp(tag_buffer,tag[j]))
            {
                printf("  %d   |  %s  \t|  %s\n",num++,tag_buffer,buffer);
                break;
            }
        }
    }
    printf("-----------------------------------------------------------\n");
    fclose(vinfo->fptr_mp3);
    return;
}    

//-------------------------------------------------------------------------------//
