/*-----------------------------------------------
Katie Pitz, Paige Huffman, Megan Cash
OS Final Project
Simple OS for Arduino
-----------------------------------------------*/

/*-----------------------------------------------
TODO:
    > some sort of file sys on SD card (maybe?)
        should be easy enough to log
    > go back and make all the values from
        these sensors actually look sensible
        and not like random numbers
    > implement round robin cuz the temp/humidity
        can take annoyingly long to wait on
Further Process Ideas:
    > use remote to trigger processes and log
        results in their own files, also log
        process times in its own file maybe
    > Using DHT11 temp/humidity sensor
        button 2: log temp
        button 3: log humidity
    > using photoresisitor
        button 1: light val
    > using tilt switch (maybe not necessary)
        button 4: tilt val
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