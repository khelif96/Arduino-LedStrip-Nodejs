/*
RadioShack Tri Color LED strip light show.
This is the modified version of the  code supplied from RS - with some annotations to help explain
what is happening as well as support for commands issued over the serial Port
*/

#include <avr/pgmspace.h>

// ******** DEBUG ==== should auto config to adapt different mother board *********
//#define DATA_1 (PORTF |=  0X01)    // DATA 1    // for ATMEGA
//#define DATA_0 (PORTF &=  0XFE)    // DATA 0    // for ATMEGA
//#define STRIP_PINOUT DDRF=0xFF  // for ATMEGA

#define DATA_1 (PORTC |=  0X01)    // DEFINE a value for DATA as 1  // for UNO
#define DATA_0 (PORTC &=  0XFE)    // DEFINE a value for DATA 0   // for UNO
#define STRIP_PINOUT (DDRC=0x3F)  // DEFINE PORTC as OUTPUT // for UNO (change pins 0-5; leave PORTC 6 & 7 alone)

/*
Create an array of values to control each LED NODE in the standard RadioShack strip.

PROGMEM will assign values to store in the AVR chip's RAM memory as a two dimensional array.

We will then read out these values to create a serial pulse train OUTPUT to the LED Strip.

Each ROW is sent as OUTPUT and will SET the COLOR for each of the TEN (10) nodes .

Each ROW is a NEW FRAME, changing the colors again and again, creating a pattern.

The order of the doublets (byte) controlling the COLORS is: Green - Blue - Red.
ff is full brightness - so 0xff0000 is GREEN, 0xx00ff00 is BLUE, 0x0000ff is RED
*/

// Green is the last doublet
PROGMEM const unsigned long pattern_test_green[10][10]={
  {0x0000f0,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x0000f0,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x0000f0,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x0000f0,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x0000f0,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x0000f0,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x0000f0,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x0000f0,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x0000f0,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x0000f0},
};

PROGMEM const unsigned long solid_green[][10]={
  {0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0,0x0000f0}
};


// WHITE is ALL doublets
PROGMEM const unsigned long pattern_test_white[10][10]={
  {0xffffff,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0xffffff,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0xffffff,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0xffffff,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0xffffff,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0xffffff,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xffffff,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xffffff,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xffffff,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xffffff},
};

// BLUE is the second doublet
PROGMEM const unsigned long pattern_test_blue[10][10]={
  {0x00f000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x00f000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x00f000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x00f000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x00f000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x00f000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x00f000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x00f000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x00f000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x00f000},
};

PROGMEM const unsigned long solid_blue[][10]={
  {0x00f000,0x00f000,0x00f000,0x00f000,0x00f000,0x00f000,0x00f000,0x00f000,0x00f000,0x00f000}
};
// GREEN is the first doublet
PROGMEM const unsigned long pattern_test_red[10][10]={
  {0xf00000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0xf00000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0xf00000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0xf00000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0xf00000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0xf00000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xf00000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xf00000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xf00000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xf00000},
};

PROGMEM const unsigned long solid_red[][10]={
  {0xf00000,0xf00000,0xf00000,0xf00000,0xf00000,0xf00000,0xf00000,0xf00000,0xf00000,0xf00000}
};

PROGMEM const unsigned long pattern_test_yellow[10][10]={
  {0xFF00FF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0xFF00FF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0xFF00FF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0xFF00FF,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0xFF00FF,0x000000,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0xFF00FF,0x000000,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF00FF,0x000000,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF00FF,0x000000,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF00FF,0x000000},
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0xFF00FF},
};

PROGMEM const unsigned long solid_yellow[][10]={
  {0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF,0xFF00FF}
};

//  Each row changes the color and intensity of the LEDs
PROGMEM const unsigned long pattern_test_rainbow[10][10]={
  {0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000},
  {0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000},
  {0x000000,0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000},
  {0x000000,0x000000,0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff},
  {0x8f00ff,0x000000,0x000000,0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff},
  {0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00,0x0000ff},
  {0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000,0xff0000,0xff7f00,0xffff00,0x00ff00},
  {0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000,0xff0000,0xff7f00,0xffff00},
  {0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000,0xff0000,0xff7f00},
  {0xff7f00,0xffff00,0x00ff00,0x0000ff,0x6f00ff,0x8f00ff,0x000000,0x000000,0x000000,0xff0000},
};








PROGMEM const unsigned long lights_off[][10]={
  {0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000}
};


// ***********************************************************************************************************
// *
// *                            INITIALIZE the system on Power Up or after a RESET
// *
// *
// ***********************************************************************************************************
void setup() {

  STRIP_PINOUT;	// sets the output pin to control the LED Strip
  reset_strip();	// resets each of the LED nodes to OFF
  Serial.begin(9600); // Desired Serial baudrate

}

// ***********************************************************************************************************
// *
// *                            Main Program Loop
// *
// *
// ***********************************************************************************************************

// Input from the serial port
String incomingByte;
// Variable for color
String color = "";
bool pulse = false;

String type = "";
String value = "";
int delayRate = 50;

void loop()
{


  delay(50);
  // Retrieve input from serial port
  if(Serial.available() > 0){
    incomingByte = Serial.readString();
    Serial.print("I recieved: ");
    Serial.println(incomingByte);
    type = getValue(incomingByte, ':', 0);
    value = getValue(incomingByte, ':', 1);
  }


// Determine the mode of input and modify variables accordingly
if(type == "color"){
  color = value;
}else if(type == "mode"){
  if(value == "pulse"){
    pulse = true;
  }else if(value == "solid"){
    pulse = false;
  }
}else if(type == "speed"){
  delayRate = value.toInt();
}

  if(color=="blue"){
    color = "blue";
    if(pulse == true){
    send_1M_pattern(pattern_test_blue, 10, delayRate);
  }else if(pulse == false){
    send_1M_pattern(solid_blue,1,100);
  }
}else if(color=="red"){
    color = "red";
    if(pulse == true){
    send_1M_pattern(pattern_test_red, 10, delayRate);
  }else if(pulse == false){
    send_1M_pattern(solid_red,1,100);
  }
}else if(color == "green"){
    color="green";
    if(pulse == true){
    send_1M_pattern(pattern_test_green, 10, delayRate);
  }else if(pulse == false){
    send_1M_pattern(solid_green,1,100);
  }
}else if(color == "rainbow"){
    color = "rainbow";
    send_1M_pattern(pattern_test_rainbow,10,delayRate);
  }else if(color == "yellow"){
    color = "yellow";
    if(pulse == true){
    send_1M_pattern(pattern_test_yellow, 10, delayRate);
  }else if(pulse == false){
    send_1M_pattern(solid_yellow,1,100);
  }
}else if(color == "off"){
    send_1M_pattern(lights_off,1,100);
  }

  }


// Splits string up and returns values
String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

/*******************************************************************************
* Function Name:	send_1M_pattern
* Description:		Transmit pattern to whole 1 meter strip*
* Input	: 		Pointer to RAM pattern; pattern length; frame rate*
* Output:		Sends out a serial pulse train using the send_strip function
* Return:		None
*******************************************************************************/
void send_1M_pattern(const unsigned long data[][10], int pattern_no, int frame_rate)
{
  int i=0;
  int j=0;
  uint32_t temp_data;

// Each pattern sends out [x] packets - one for each NODE (RGB LED cluster)
// pattern_no is the [y] dimension of the array - the number of ROWS in each pattern array set
  for (i=0;i<pattern_no;i++)
  {
    noInterrupts(); // Turn OFF Interupts for more precise Pulse Timing
    for (j=0;j<10;j++)
    {
      temp_data=pgm_read_dword_near(&data[i][j]);
      send_strip(temp_data);
    }
    interrupts(); // Turn ON Interrupts after data is sent

    delay(frame_rate); // Delay between each pulse train - sets the duration of each Frame, before the next signal is sent;
	/* CONTROLS THE VISUAL SPEED OF THE DISPLAY CHANGES */

  }


}


/*******************************************************************************
* Function Name:	send_strip
* Description:		Creates and Transmits a serial train of 24 pulses for the LED strip
* Input:		24-bit data sets intensity of each color, which will persist until
*			the next cycle makes a change
* Output:		Sends a train of 24 pulses (bits) representing values of 0 or 1 to the output pin ()
* Return:		Nothing returned from function
*******************************************************************************/
void send_strip(uint32_t data)
{
  int i;
  unsigned long j=0x800000;


  for (i=0;i<24;i++)
  {
    if (data & j)
    {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");

/*----------------------------*/
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
/*----------------------------*/
      DATA_0;
    }
    else
    {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      DATA_0;
/*----------------------------*/
       __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
/*----------------------------*/
    }

    j>>=1;
  }


}

/*******************************************************************************
* Function Name:	reset_strip
* Description:		Send a 0 pulse to reset all color of the strip*
* Input:		None*
* Output: 		Sends out a looong value of 0 on the output pin
* Return:		None
*******************************************************************************/
void reset_strip()
{
  DATA_0;
  delayMicroseconds(20);
}
