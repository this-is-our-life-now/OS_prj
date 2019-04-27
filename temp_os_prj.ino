#include <LiquidCrystal.h>
#include <IRremote.h>
#include <String.h>

void setup()
{
  // serial for testing purposes
  Serial.begin(9600);
  
  lcd_display( "thisisourlifenow" );
  ir_input();
}

void loop(){}

/*---------------------------LCD Section---------------------------*/

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_display( String msg )
{
    // LCD's number of (columns, rows)
    lcd.begin(16, 2);
    lcd.print(msg);

    int i = 0; // temporarily have it print once until process handling implemented
    while( i < 1 )
    {
        Serial.println( msg );
        // Turn off the display:
        lcd.display();
        delay( 500 );
        // Turn on the display:
        lcd.noDisplay();
        delay( 500 );
        
        i++;
    }
}

/*---------------------------Input Section---------------------------*/

// initialize infrared receiver
const int ir_pin = 8;
IRrecv ir_receiver( ir_pin );
decode_results results;

/*--------------------------------------------------------
TODO: if we use IR for input: it gives inconsistent
values perbutton press so the corresponding values
and logic will have to be tested
--------------------------------------------------------*/

void ir_input()
{
    ir_receiver.enableIRIn();

    while( true )
    {
        if ( ir_receiver.decode( &results ) )
        {
            lcd_display( String( results.value, HEX ) );
            ir_receiver.resume();
        }
    }
}
