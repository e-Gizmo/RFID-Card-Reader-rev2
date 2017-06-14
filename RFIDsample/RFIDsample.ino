/* e-Gizmo RFID kit Arduino Demo */

// defines used by the serial event
// do not modify
#define  STX 2
#define  ETX 3
#define  SERIALSTX 0
#define  SERIALETX 1
#define  SERIALRDY 2

// RFID read results
 String card_enrolled;   // = 0 if card is not enrolled = 1 if enrolled and valid
 String  card_addr;      // contains valid card index only if card is enrolled. 
 String  card_type;      // card type identifier
 String  card_number;    // card serial number

byte  serial_state;
String serialinput;
boolean  carddetected=false;



void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  Serial.println("e-Gizmo RFID reader Demo");
  delay(1000);
  Serial.print("\002v0\003");
  delay(500);
  serial_state=SERIALSTX;
  serialinput="";
  check_RFID();
  
}

void loop() {

  if(check_RFID()==true){
    
    /* Replace the following demo codes with your own
    */
    
    // Print the read information
    Serial.print("card enrolled: ");
    Serial.println(card_enrolled);
    Serial.print("card addr: ");
    Serial.println(card_addr);
    Serial.print("card type: ");
    Serial.println(card_type);
    Serial.print("card number: ");
    Serial.println(card_number);
    Serial.println("");
    
    // Sample function
    // Notify card is valid if detected as enrolled
    // and momentarily flash pin 13 LED
 
    if(card_enrolled.equals("1")){
      Serial.print("This card is valid and has index = ");
      Serial.println(card_addr);
      Serial.println("");
      // Flash LED
      digitalWrite(13,HIGH);
      delay(500);
      digitalWrite(13,LOW);
    }
  }

}


/* Do not modify anything in the following codes unless
  you absolutely know what you are doing :-) */
  
boolean check_RFID(void){

  int  strindex;
  int  strindexe;
  
      // Read if card data is available
     if(serial_state==SERIALRDY){
     
      //extract card enrolled portion
      strindex=serialinput.indexOf(",");
      card_enrolled=serialinput.substring(0,strindex);
      
      //extract card addr portion
      strindex++;
      strindexe=serialinput.indexOf(",",strindex);
      card_addr=serialinput.substring(strindex,strindexe);
      
      //extract card type portion
      strindex=strindexe+1;
      strindexe=serialinput.indexOf(",",strindex);
      card_type=serialinput.substring(strindex,strindexe);
      
      //extract card number portion
      strindex=strindexe+1;
      strindexe=serialinput.indexOf(",",strindex);
      card_number=serialinput.substring(strindex,strindexe);
      
      // clear received string and ready SERIAL for new stream
     serialinput="";
     serial_state=SERIALSTX;
     return(true);
   }
   return(false);  // return false if RFID data is not available
}


/* This is a interrupt driven serial Rx routine */

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    // Wait for the STX character
    if(serial_state==SERIALSTX){
      if(inChar==STX){
        serial_state=SERIALETX;    // STX character detected, enable next phase
        return;
        }
    }
    
    // Store rx character to serial input until ETX is detected
    if(serial_state==SERIALETX){
      if(inChar!=ETX){
        serialinput += inChar;
        return;
        }
    }
    
      // Indicate serial data is ready after ETX is detected
      serial_state=SERIALRDY;
    }
  }


