#include <stdio.h>
#include "type.h"
#include "mp.h"

//-------------------------------------------------------------------------------//

int main(int argc,char *argv[])
{
   ID3 info;
   if(argc > 1 && check_operationtype(argv[1][1])==e_view)
   {
        if(read_and_validate_args(argv, &info)==e_failure)
        {
            printf("Invalid input\n");
            return 0;
        }
        view_operation(&info);
   }
   else
   {
        printf("Invalid input\n");
        return 0;
   }
}