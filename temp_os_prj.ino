#include <LiquidCrystal.h>
#include <IRremote.h>
#include <String.h>
#include <DHT.h>

// constants
const int TEMP = 0;
const int HUMIDITY = 1;
// probably move all inits into one area like up here

void setup()
{
    // serial for testing purposes
    Serial.begin(9600);
    
    lcd_display( "thisisourlifenow" );
    //ir_input();
    //dht_input( TEMP );
    dht_input( HUMIDITY );

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
        Serial.println( "LCD Out:" );
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

    while( true ) // eventually these won't go infinitely when we handle processes properly
    {
        if ( ir_receiver.decode( &results ) )
        {
            lcd_display( String( results.value, HEX ) );
            ir_receiver.resume();
        }
    }
}

// initialize DHT11 (humidity/temp sensor)

const int dht_pin = 7;
DHT dht( dht_pin, DHT11 );

/*--------------------------------------------------------
TODO: if we use dht11 warning this thing is cheap and 
will give nan values sometimes so we should remember to
add some sort of abort after waiting a certain amount of
time(for just this sensor or for all really)
--------------------------------------------------------*/

void dht_input( int data_type )
{
    double input;
    int valid = false;

    while( true ) // eventually these won't go infinitely when we handle processes properly
    {
        if( data_type == TEMP )
        {
            while( valid == false )
            {
                input = dht.readTemperature();
                if( !isnan( input ) )
                {
                    valid = true;
                }
            }
            lcd_display( String( input ) );
        }
        else if( data_type == HUMIDITY )
        {
            while( valid == false )
            {
                input = dht.readHumidity();
                if( !isnan( input ) )
                {
                    valid = true;
                }
            }
            lcd_display( String( input ) );
        }
    }
}
