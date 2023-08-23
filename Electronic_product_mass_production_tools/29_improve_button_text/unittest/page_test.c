#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "../include/page_manager.h"

int main(int argc, char ** argv)
{
    PagesRegister();
    Page("main")->Run(NULL);
    return 0;
}
