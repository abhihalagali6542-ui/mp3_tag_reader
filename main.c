#include <stdio.h>
#include "type.h"
#include "view_header.h"
#include "edit_header.h"

//-------------------------------------------------------------------------------//

int main(int argc,char *argv[])
{
   // check argv for (v)
   if(argc > 2 && check_operationtype(argv[1][1])==e_view)
   {
      V_MP3INFO vinfo;
        if(read_and_validate_args(argv, &vinfo)==e_failure)
        {
            printf("\t--- Invalid input ---\n");
            printf("Usage : \n");
            printf("To vinfo : \t./a.out -v filename.mp3\n");
            return 0;
        }
        view_operation(&vinfo);
   }
   else if(check_operationtype(argv[1][1])==e_edit)
   {
          E_MP3INFO einfo;
          if(read_and_validate_edit_args(argv,&einfo)==e_failure)
          {
               printf("\t--- Invalid input ---\n");
               printf("Usage : \n");
               printf("To Edit : \t./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
               return 0;    
          }
          do_edit(&einfo);
   }
   // check argv for (h)
   else if(check_operationtype(argv[1][1])==e_help)
   {
        printf("Usage : \n");
        printf("To vinfo : \t./a.out -v filename.mp3\n");
        printf("To Edit : \t./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }
   else
   {
          printf("\t--- Invalid input ---\n");
          printf("Usage : \n");
          printf("To vinfo : \t./a.out -v filename.mp3\n");
          printf("To Edit : \t./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }
     return 0;
}

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
    else if(opt == 'e')
    {
        return e_edit;
    }
    return e_unsupported;
}

//-------------------------------------------------------------------------------//
