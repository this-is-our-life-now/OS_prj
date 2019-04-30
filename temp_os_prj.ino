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
const int ABORT     = 3;

const int RR_TIME = 4000; // round robin time quantum
const int ABORT_TIME = 20000; // give up on processes that have been running for 20s, sadly this may be necessary for my cheap DHT11

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
    float time = millis(); // to keep track of start time
    float turn_time = millis(); // to keep track of time of that round robin turn
};

// vector of processes
std::vector<prcss> buffer;

// global current process
prcss current;

void setup()
{
    // serial for testing purposes
    Serial.begin(9600);

    ir_receiver.enableIRIn();
    
    lcd_display( "thisisourlifenow", "ft KP,PH,&MC", 1500 );
}

void loop()
{
    if ( ir_receiver.decode( &results ) )
    {
        remote_input();
    }
    
    if( buffer.size() != 0 && current.status != RUNNING )
    {
        processing();
    }
}

/*------------------------Processing Section------------------------*/
void processing()
{
    float temp_time;

    current.status = RUNNING;
    current.time = buffer[0].time;
    current.type = buffer[0].type;
    current.turn_time = millis();

    if( current.type == PHOTO_PRCSS )
    {
        photo_input();
    }
    else if( current.type == TEMP_PRCSS )
    {
        dht_input( TEMP );
    }
    else if( current.type == HUM_PRCSS )
    {
        dht_input( HUMIDITY );
    }
    else // this really shouldn't happen
    {
        lcd_display( "err", "unkown process", 1500 );
    }

    if( ( current.status == WAITING ) && ( buffer.size() > 1 ) )
    {
        lcd_display( "RR triggered", "will be back", 1000 );
    }
    if( current.status == DONE )
    {
        // display process running time
        temp_time = ( millis() - current.time ) / 1000.0 ;
        lcd_display( "prcss done", String( temp_time )+"s", 1500 );
        buffer.erase( buffer.begin() );
    }
    if ( current.status == ABORT )
    {
        if( current.type == TEMP_PRCSS )
        {
            lcd_display( "temp aborted", "took too long", 1500 ) ;
        }
        else if( current.type == HUM_PRCSS )
        {
            lcd_display( "humid aborted", "took too long", 1500 ) ;
        }
        else
        {
            lcd_display( "prcss aborted", "took too long", 1500 );
        }
        buffer.erase( buffer.begin() );
    }
}

/*---------------------------LCD Section---------------------------*/
void lcd_display( String process, String msg, int time )
{
    // LCD's number of (columns, rows)
    lcd.begin( 16, 2 );
    lcd.setCursor( 0,0 );
    lcd.print( process );
    lcd.print( ":" );
    lcd.setCursor(0,1);
    lcd.print( "  " );
    lcd.print( msg );

    // Turn off the display:
    lcd.display();
    delay( time );
}

/*---------------------------Input Section---------------------------*/
void remote_input()
{
    String code;
    prcss incoming;
    incoming.status = WAITING;

    code = String( results.value, HEX ) ;

    // for some reason the remote will return inconsistent values per button
    if( code == "ff30cf" || code == "ee8f7948" )
    {
        incoming.type = PHOTO_PRCSS;
        buffer.push_back( incoming );
        lcd_display( "IR Input", "photo cell", 1500 );
    }
    else if( code == "ff18e7" || code == "3e9aec57"
        || code == "1af66ed4" || code == "6b3b8e79" )
    {
        incoming.type = TEMP_PRCSS;
        buffer.push_back( incoming );
        lcd_display( "IR Input", "temp", 1500 );
    }
    else if( code == "ff7a85" || code == "dceecd2c"
        || code == "8dcdae60" || code == "d78c801a"
        || code == "a728e649" )
    {
        incoming.type = HUM_PRCSS;
        buffer.push_back( incoming );
        lcd_display( "IR Input", "humidity", 1500 );
    }
    else
    {
        lcd_display( "trouble reading", "try again", 1500 );
    }
    
    ir_receiver.resume();
}

/*--------------------------Sensor Section--------------------------*/
/*--------------------------------------------------------
TODO: if we use dht11: warning this thing is cheap and 
will give nan and neagative values sometimes so we should
add some sort of abort on processes after waiting a
certain amount of time(for this sensor or for all tbh)
--------------------------------------------------------*/
void dht_input( int data_type )
{
    double input = 0.0;
    int valid = false;

    if( data_type == TEMP )
    {
        while( valid == false )
        {
            lcd_display( "getting temp", "patience...", 100 );
            input = dht.readTemperature();
            if( !isnan( input ) && input >= 0.0 )
            {
                valid = true;
            }
            Serial.println(buffer.size());
            if( ( millis() - current.turn_time >= RR_TIME ) )
            {
                current.status = WAITING;
                return;
            }
            else if( millis() - current.time >= ABORT_TIME )
            {
                current.status = ABORT;
                return;
            }
        }
        lcd_display( "DHT Temp", String( input )+"C", 1500 );
    }
    else if( data_type == HUMIDITY )
    {
        while( valid == false )
        {
            lcd_display( "getting humidity", "patience...", 100 );
            input = dht.readHumidity();
            if( !isnan( input ) )
            {
                valid = true;
            }
            if( ( millis() - current.turn_time >= RR_TIME ) )
            {
                current.status = WAITING;
                return;
            }
            else if( millis() - current.time >= ABORT_TIME )
            {
                current.status = ABORT;
                return;
            }
        }
        lcd_display( "DHT Humidity", String( input ), 1500 );
    }
    current.status = DONE;
}

void photo_input()
{
    lcd_display( "Photoresistor", String( analogRead( photo_pin ) ), 1500 );
    current.status = DONE;
}
