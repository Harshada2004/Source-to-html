#include "utility.h"

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        print_usage(argv[0]);
        return 1;
    }
    char *input_file  = argv[1];
    char output_file[20] = "coverted.html";
    source2html(input_file,output_file);
    return 0;
}