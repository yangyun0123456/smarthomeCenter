#include<iom16v.h>
#include <macros.h>

#include "util.h"
#include "iic.h"
#include "connectToserver.h"

//pc7 scl
//pc6 sda
void mini2440_init(void)
{
    //pc6 and pc7 output mode.
    //DDRC |= 0xc0;
    //PORTC |= 0xc0;
    return;
}

