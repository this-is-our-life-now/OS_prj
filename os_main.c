/*-----------------------------------------------
Katie Pitz, Paige Huffman, Megan Cash
OS Final Project
Simple OS for Arduino
-----------------------------------------------*/

/*-----------------------------------------------
TODO:
    > process management/threading/scheduling
    > some sort of file sys on SD card
    > 7 segment display for process times
    > LCD display for statuses
    > some sort of input sensor stuff so we
    have a reason to write to files/log stuff
Further Process Ideas:
    > use remote to trigger processes and log
        results in their own files, also log
        process times in its own file maybe
    > Using DHT11 temp/humidity sensor
        button 2: log temp
        button 3: log humidity
    > using photoresisitor
        button 1: light val
    > using tilt switch
        button 4: tilt val

TODO: go back and make all the values from
    these sensors actually look sensible and
    not like random numbers
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