#include <stdio.h>
#include "type.h"
#include "mp.h"

//-------------------------------------------------------------------------------//

int main(int argc,char *argv[])
{
   ID3 info;
   // check argv for (v)
   if(argc > 1 && check_operationtype(argv[1][1])==e_view)
   {
        if(read_and_validate_args(argv, &info)==e_failure)
        {
            printf("\t--- Invalid input ---\n");
            printf("Usage : \n");
            printf("To View : \t./a.out -v filename.mp3\n");
            return 0;
        }
        view_operation(&info);
   }
   // check argv for (h)
   else if(check_operationtype(argv[1][1])==e_help)
   {
        printf("Usage : \n");
        printf("To View : \t./a.out -v filename.mp3\n");
        printf("To Edit : \t./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
   }
   else
   {
        printf("\t--- Invalid input ---\n");
        printf("Usage : \n");
        printf("To View : \t./a.out -v filename.mp3\n");
        printf("To Edit : \t./a.out -e -t/-a/-A/-m/-y/-c filename.mp3\n");
        return 0;
   }
}

//-------------------------------------------------------------------------------//
