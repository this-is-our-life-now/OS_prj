/*-----------------------------------------------
Katie Pitz, Paige Huffman, Megan Cash
OS Final Project
Simple OS for Arduino
-----------------------------------------------*/

/*-----------------------------------------------
TODO:
    > process management/threading/scheduling
    > some sort of file sys on SD card
    > timer along with 7 segment display
    > LCD display for statuses
    > some sort of input sensor stuff so we
    have a reason to write to files/log stuff
-----------------------------------------------*/

#include "external.c"
#include "file_system.c"
#include "lcd_display.c"
#include "processing.c"

void setup()
{
    lcd();
}

void loop(){}