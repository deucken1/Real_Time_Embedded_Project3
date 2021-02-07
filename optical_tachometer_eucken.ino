// EN.605.715.81.SP21 Software Development for Real-Time Embedded Systems
// Project 3 - Measurement and transmission of propeller speed 
// Douglas Eucken
// Feb 5th 2021

// Propellor/Fan speed reading via Arduino DUO and IR LED/IR Photodiode pair
// Transmits updated time and RPM data every 1 second
 
// The number of blades on the propeller/fan. Adjust accordingly.
const unsigned int BLADE_COUNT = 7;
 
// Volatile keyword is used with interrupts
// This variable is subject to change inside an ISR
volatile unsigned int break_number = 0;
 
// Used for capturing the time
unsigned long time;
 
// Used for capturing the rpm (revolutions per minute)
unsigned int rpm;
 
/**
 *  Function runs only once, after each powerup or reset of the Arduino Uno
 */
void setup() {
 
  // Open the serial port and set the data transmission rate to 9600 bits 
  // per second. 9600 is the default baud rate for Arduino Uno.
  Serial.begin(9600);
 
  // Show a welcome message as human-readable ASCII text
  Serial.println("EN.605.715.81.SP21 Software Development for Real-Time Embedded Systems");
  Serial.println("Project 3 - Measurement and transmission of propeller speed");
  Serial.println("Douglas Eucken");
  Serial.println("");
  Serial.println("TIME, RPM");
 
  // The Infrared photodiode circuit is connected to pin 2.
  // Interrupt triggers when signal goes from LOW to HIGH
  // When photodiode recieves IR light it conducts and pulls input to zero
  // When IR light is not present the photodiode does not conduct and input is pulled to VCC
  attachInterrupt(digitalPinToInterrupt(2), isr_break_count, RISING); 
 
  break_number = 0;
  rpm = 0;  
  
}
 
/**
 *  Main function
 */
void loop() {   
 
  // Update time and rpm every second
  delay(1000);
 
  // Don't process interrupts during this calculation
  noInterrupts();
 
  // Calculate the RPM taking into account the blade count 
  rpm = (60 * break_number) / BLADE_COUNT;
 
  // Display the time and rpm
  display_time_and_rpm();   

  // Reset break and rpm variables for next calcuation   
  break_number = 0;
  rpm = 0;
 
  // Restart interrupts
  interrupts();
  
}
 
/**
  * Interrupt service routine.
  * This function counts the number of interrupts
  */
void isr_break_count() {

  // Increment break by one for every trip into the ISR
  break_number++;
   
}
 
/**
  * Function displays the time and rpm
  */
void display_time_and_rpm() {
   
  // Capture the time and covert to seconds
  time = millis() / 1000;
 
  // Display the time
  Serial.print(time); 
  Serial.print(" , ");
  // Println so the next line begins on a new line
  // Display the rpm
  Serial.println(rpm); 
    
}
