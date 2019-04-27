/*-----------------------------------------------
Everything we output and handling displays will
go through here
-----------------------------------------------*/

#include "lcd_display.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_init()
{
    //  LCD (columns, rows):
    lcd.begin(16, 2);
    // output to LCD
    lcd.print("this_is_our_life_now");
}

void lcd()
{
    lcd_init();

    while( true )
    {
        lcd.noDisplay();
        delay(500);

        lcd.display();
        delay(500);
    }
}
