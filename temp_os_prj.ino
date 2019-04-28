#include <ArduinoSTL.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <String.h>
#include <DHT.h>

// constants
const int TEMP      = 0;
const int HUMIDITY  = 1;

const int PHOTO_PRCSS   = 1;
const int TEMP_PRCSS    = 2;
const int HUM_PRCSS     = 3;

const int WAITING   = 0;
const int RUNNING   = 1;
const int DONE      = 2;

// initialize infrared receiver
const int ir_pin = 8;
IRrecv ir_receiver( ir_pin );
decode_results results;

// initialize LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// initialize DHT11 (humidity/temp sensor)
const int dht_pin = 7;
DHT dht( dht_pin, DHT11 );

// initialize photoresistor
const int photo_pin = A0;

// type for storing process info
struct prcss
{
    int type;
    int status;
};

// vector of processes
std::vector<prcss> buffer;

void setup()
{
    // serial for testing purposes
    Serial.begin(9600);

    ir_receiver.enableIRIn();
    
    lcd_display( "thisisourlifenow", "ft KP,PH,&MC" );
    lcd_display( "thisisourlifenow", "ft KP,PH,&MC" ); //just cuz
    //remote_input();
    //dht_input( TEMP );
    //dht_input( HUMIDITY );
    //photo_input();

}

void loop()
{
    if ( ir_receiver.decode( &results ) )
    {
        remote_input();
    }
    
    if( buffer.size() != 0 && buffer[0].status != RUNNING ) // is this safe enough? double check later
    {
        processing();
    }
}

/*------------------------Processing Section------------------------*/
void processing()
{
    buffer[0].status = RUNNING;
    if( buffer[0].type == PHOTO_PRCSS )
    {
        photo_input();
    }
    else if( buffer[0].type == TEMP_PRCSS )
    {
        dht_input( TEMP );
    }
    else if( buffer[0].type == HUM_PRCSS )
    {
        dht_input( HUMIDITY );
    }
    else // this really shouldn't happen
    {
        lcd_display( "err", "unkown process" );
    }
    buffer.erase( buffer.begin() );

}

/*---------------------------LCD Section---------------------------*/
void lcd_display( String process, String msg )
{
    // LCD's number of (columns, rows)
    lcd.begin( 16, 2 );
    lcd.setCursor( 0,0 );
    lcd.print( process );
    lcd.print( ":" );
    lcd.setCursor(0,1);
    lcd.print( "  " );
    lcd.print( msg );

    // serial for testing purposes
    Serial.print( process );
    Serial.println( ":" );
    Serial.println( msg );

    // Turn off the display:
    lcd.display();
    delay( 1500 );
    // Turn on the display:
    lcd.noDisplay();
    delay( 200 );
}

/*---------------------------Input Section---------------------------*/

void remote_input()
{
    String code;
    prcss incoming;
    incoming.status = WAITING;

    code = String( results.value, HEX ) ;
    lcd_display( "IR Input", code);

    // for some reason the remote will return inconsistent values per button
    if( code == "ff30cf" || code == "ee8f7948" )
    {
        incoming.type = PHOTO_PRCSS;
        buffer.push_back( incoming );
    }
    else if( code == "ff18e7" || code == "3e9aec57"
        || code == "1af66ed4" || code == "6b3b8e79" )
    {
        incoming.type = TEMP_PRCSS;
        buffer.push_back( incoming );
    }
    else if( code == "ff7a85" || code == "dceecd2c"
        || code == "8dcdae60" || code == "d78c801a"
        || code == "a728e649" )
    {
        incoming.type = HUM_PRCSS;
        buffer.push_back( incoming );
    }
    else
    {
        lcd_display( "trouble reading", "try again" );
    }
    
    ir_receiver.resume();
}

/*--------------------------Sensor Section--------------------------*/
/*--------------------------------------------------------
TODO: if we use dht11 warning this thing is cheap and 
will give nan values sometimes so we should remember to
add some sort of abort on processes after waiting a
certain amount of time(for this sensor or for all tbh)
--------------------------------------------------------*/
void dht_input( int data_type )
{
    double input;
    int valid = false;

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
        lcd_display( "DHT Temp", String( input ) );
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
        lcd_display( "DHT Humidity", String( input ) );
    }
}

void photo_input()
{
    lcd_display( "Photoresistor", String( analogRead( photo_pin ) ) );
}
