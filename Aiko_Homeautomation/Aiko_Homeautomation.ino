/*
  Serial Event example
 
 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and 
 clears it.
 
 A good test for this is to try it with a GPS receiver 
 that sends out NMEA 0183 sentences. 
 
 Created 9 May 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialEvent
 
 */

#include <EEPROM.h>


    int tempPin = A0;   // the output pin of LM35
    int fan = 11;       // the pin where fan is
    int light=9;
    int plug=10;
  
    int temp;
    int tempMin = 25;   // the temperature to start the fan
    int tempMax = 40;   // the maximum temperature when fan is at 100%
    int fanSpeed;

    int TempModeFlag=0;

    
String inputString = "";   
String RXString = "";   
// a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const int BUFSIZE = 20;
char buf[BUFSIZE];



// Absolute min and max eeprom addresses. Actual values are hardware-dependent.
// These values can be changed e.g. to protect eeprom cells outside this range.
const int EEPROM_MIN_ADDR = 0;
const int EEPROM_MAX_ADDR = 511;


// Returns true if the address is between the
// minimum and maximum allowed values, false otherwise.
//
// This function is used by the other, higher-level functions
// to prevent bugs and runtime errors due to invalid addresses.
boolean eeprom_is_addr_ok(int addr) {
return ((addr >= EEPROM_MIN_ADDR) && (addr <= EEPROM_MAX_ADDR));
}


// Writes a sequence of bytes to eeprom starting at the specified address.
// Returns true if the whole array is successfully written.
// Returns false if the start or end addresses aren't between
// the minimum and maximum allowed values.
// When returning false, nothing gets written to eeprom.
boolean eeprom_write_bytes(int startAddr, const byte* array, int numBytes) {
// counter
int i;

// both first byte and last byte addresses must fall within
// the allowed range
if (!eeprom_is_addr_ok(startAddr) || !eeprom_is_addr_ok(startAddr + numBytes)) {
return false;
}

for (i = 0; i < numBytes; i++) {
EEPROM.write(startAddr + i, array[i]);
}

return true;
}


// Writes a string starting at the specified address.
// Returns true if the whole string is successfully written.
// Returns false if the address of one or more bytes fall outside the allowed range.
// If false is returned, nothing gets written to the eeprom.
boolean eeprom_write_string(int addr, const char* string) {

int numBytes; // actual number of bytes to be written

//write the string contents plus the string terminator byte (0x00)
numBytes = strlen(string) + 1;

return eeprom_write_bytes(addr, (const byte*)string, numBytes);
}


// Reads a string starting from the specified address.
// Returns true if at least one byte (even only the string terminator one) is read.
// Returns false if the start address falls outside the allowed range or declare buffer size is zero.
//
// The reading might stop for several reasons:
// - no more space in the provided buffer
// - last eeprom address reached
// - string terminator byte (0x00) encountered.
boolean eeprom_read_string(int addr, char* buffer, int bufSize) {
byte ch; // byte read from eeprom
int bytesRead; // number of bytes read so far

if (!eeprom_is_addr_ok(addr)) { // check start address
return false;
}

if (bufSize == 0) { // how can we store bytes in an empty buffer ?
return false;
}

// is there is room for the string terminator only, no reason to go further
if (bufSize == 1) {
buffer[0] = 0;
return true;
}

bytesRead = 0; // initialize byte counter
ch = EEPROM.read(addr + bytesRead); // read next byte from eeprom
buffer[bytesRead] = ch; // store it into the user buffer
bytesRead++; // increment byte counter

// stop conditions:
// - the character just read is the string terminator one (0x00)
// - we have filled the user buffer
// - we have reached the last eeprom address
while ( (ch != 0x00) && (bytesRead < bufSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR) ) {
// if no stop condition is met, read the next byte from eeprom
ch = EEPROM.read(addr + bytesRead);
buffer[bytesRead] = ch; // store it into the user buffer
bytesRead++; // increment byte counter
}

// make sure the user buffer has a string terminator, (0x00) as its last byte
if ((ch != 0x00) && (bytesRead >= 1)) {
buffer[bytesRead - 1] = 0;
}

return true;
}



void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
   pinMode(light, OUTPUT);
   pinMode(plug, OUTPUT);

   eeprom_read_string(0, buf, BUFSIZE);
//Serial.println(buf);

String RXString =  String(buf);

inputString= RXString ;

if(find_string(inputString,"L0") ) 
         {
          digitalWrite(light, LOW);
         Serial.println("light off");
         }
         
          else if(find_string(inputString,"L1") ) 
         {
          digitalWrite(light, HIGH);
          Serial.println("light on");
         }

         
        if(find_string(inputString,"P0") ) 
         {
          digitalWrite(plug, LOW);
         Serial.println("plug off");
         }
         
          else if(find_string(inputString,"P1") ) 
         {
          digitalWrite(plug, HIGH);
          Serial.println("pplug on");
         }

       
    
       
       
         if(find_string(inputString,"M5") ) 
       {

         
       TempModeFlag=1;
         Serial.println("fan automatic mode");

         
       }
          else if(find_string(inputString,"M0") ) 
       {
        TempModeFlag=0;
        analogWrite(fan, 0);
         Serial.println("fan off");
       }
         else if(find_string(inputString,"M1") ) 
       {

        TempModeFlag=0;
        analogWrite(fan, 50);
         Serial.println("fan speed 1");
       }
       
        else if(find_string(inputString,"M2") ) 
       {
        analogWrite(fan, 150);
        TempModeFlag=0;
         Serial.println("fan speed 2");
       }
       
        else if(find_string(inputString,"M3") ) 
       {TempModeFlag=0;
       analogWrite(fan, 255);
         Serial.println("fan speed 3");
       }



//Serial.println(RXString);
}

void loop() {
  temp = readTemp();     // get the temperature
  // print the string when a newline arrives:
  if (stringComplete) {








   
    if( RXString!=inputString)
    {
       Serial.println("new string"); 


         if(find_string(inputString,"L0") ) 
         {
          digitalWrite(light, LOW);
         Serial.println("light off");
         savestring();

        


         }
         
          else if(find_string(inputString,"L1") ) 
         {
          digitalWrite(light, HIGH);
          Serial.println("light on");
          savestring();

          
         }

         
        if(find_string(inputString,"P0") ) 
         {
          digitalWrite(plug, LOW);
         Serial.println("plug off");
         savestring();

       

         }
         
          else if(find_string(inputString,"P1") ) 
         {
          digitalWrite(plug, HIGH);
          Serial.println("pplug on");
          savestring();
          
         }

       
    
       
       
         if(find_string(inputString,"M5") ) 
       {

         
       TempModeFlag=1;
         Serial.println("fan automatic mode");
         savestring();
        
         
       }
          else if(find_string(inputString,"M0") ) 
       {
        TempModeFlag=0;
        analogWrite(fan, 0);
         Serial.println("fan off");
         savestring();
       }
         else if(find_string(inputString,"M1") ) 
       {

        TempModeFlag=0;
        analogWrite(fan, 50);
         Serial.println("fan speed 1");
         savestring();
       }
       
        else if(find_string(inputString,"M2") ) 
       {
        analogWrite(fan, 150);
        TempModeFlag=0;
         Serial.println("fan speed 2");
         savestring();
       }
       
        else if(find_string(inputString,"M3") ) 
       {TempModeFlag=0;
       analogWrite(fan, 255);
         Serial.println("fan speed 3");
         savestring();
       }

      
    }
    
       RXString=inputString;
   
    // clear the string:
    inputString = "";
    stringComplete = false;
  
  }


  if(TempModeFlag==1)
  {
    
     if(temp < tempMin) {   // if temp is lower than minimum temp
           fanSpeed = 0;      // fan is not spinning
           digitalWrite(fan, LOW);       
       } 
       if((temp >= tempMin) && (temp <= tempMax)) {  // if temperature is higher than minimum temp
           fanSpeed = map(temp, tempMin, tempMax, 50, 255); // the actual speed of fan
           
           analogWrite(fan, fanSpeed);  // spin the fan at the fanSpeed speed
          Serial.print(temp);
            Serial.print(" degree - ");
             Serial.println(fanSpeed);
       } 
  }
  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '*') {
      stringComplete = true;
    } 
  }
}


boolean find_string(String base, String search)
{
  int len = search.length(); // find the length of the base string

  for(int m = 0; m<(base.length()-len);m++)// Iterate from the beginning of the base string till the end minus length of the substring
  {    
    if(base.substring(m,(m+len))==search) // Check if the extracted Substring Matches the Search String
    {
      return true;        // if it matches exit the function with a true value
    }
  }
  return false; // if the above loop did not find any matches, control would come here and return a false value
}


  int readTemp() {  // get the temperature and convert it to celsius
      temp = analogRead(tempPin);
      return temp * 0.48828125;
    }


    void savestring()
    {
       inputString.toCharArray(buf, BUFSIZE); 
eeprom_write_string(0, buf);
eeprom_read_string(0, buf, BUFSIZE);
    
    }

