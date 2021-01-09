//   <!--------------------------------------------------------------------------------------------->
//   <!--    Snake Ticker                                                                         -->
//   <!--    by Fabrizio Ranieri (2020)                                                           -->
//   <!--                                                                                         -->
//   <!--    This program is free software; you can redistribute it and/or                        -->
//   <!--    modify it under the terms of the GNU Lesser General Public                           -->
//   <!--    License as published by the Free Software Foundation; either                         -->
//   <!--    version 3.0 of the License, or (at your option) any later version.                   -->
//   <!--    Any re-distribution of this software, even if modified, MUST remain open-source      -->
//   <!--    and MUST show the original author full name and the GITHUB code source link          -->
//   <!--    and MUST inherit and apply the same GNU license.                                     -->
//   <!--                                                                                         -->
//   <!--    This library is distributed in the hope that it will be useful,                      -->
//   <!--    but WITHOUT ANY WARRANTY; without even the implied warranty of                       -->
//   <!--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                    -->
//   <!--    Lesser General Public License for more details.                                      -->
//   <!--------------------------------------------------------------------------------------------->

#include "LedControl.h" // library for the 8 digits led module (MAX7219/MAX7221) https://github.com/wayoda/LedControl

// USER OPTIONS
#define DIN_PIN 5       // data input pin
#define CLK_PIN 7       // clock pin
#define CS_PIN 6        // chip select pin
#define ALIGNMENT 1     // data alignment on the display 0=left 1=right
#define BRIGHTNESS 1    // the brightness of the display (0..15)
#define MSDELAY 1100    // microseconds delay between two loops, bigger is this value, slower is the ticker 

// GLOBAL VARIABLES
long ct=-1; // counter of loops used for timing the ticker
bool showInProgress=false; // flag used to avoid concurrency of shows
bool newLedControl=true; // flag used to instantiate the control only once
char data[]={"        "}; // contains the info to show
bool separator[]={0,0,0,0,0,0,0,0}; // every digit can have its decimal separator ON
byte spin[]={1,4,1,4,1,4,1,4}; // the spinning effect is done using a two segments snake every digit. The snakes start from different positions.
byte timer[]={20,20,20,20,20,20,20,20}; // used to slow down the snake before showing the digit
LedControl* lc;

void setup() {
  Serial.begin(9600); 
}

void loop() 
{
  ct++; 
  byte tmp;
  if (ct%1000==0 && showInProgress==false) // every second check for new data if there isn't a previous show in progress
  {
    if (Serial.available())
    {
      delay(200); // give time to a block of data to be fully sent
      int i=7; // assign the data bytes received to the digits by descending order
      while(Serial.available())
      {
         tmp = Serial.read(); // read one char at one time
         if (i>-1) // read a maximum of 8 valid chars
         {
           if (tmp!=13 && tmp!=10) // carriage return and line feed chars are discarded
           {
             if (tmp==46 && i<7) // the decimal separator is not saved as valid char, but as a property of the previous char
             {
                separator[i+1]=1;
             }
             else
             {
                data[i]=tmp; // assign the char to the digit
                i--;
             }
           }
        }
      } 
      if (newLedControl==true) // every time the data is sent to the ticker, Arduino is reset. The led control is instantiated and configured here, instead of the setup, to avoid a blank screen between two shows
      {
        newLedControl=false;
        lc=new LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);
        lc->shutdown(0,false);
        lc->setIntensity(0,BRIGHTNESS);  
      }
      lc->clearDisplay(0); 
      if (i>-1 && ALIGNMENT) // shift the array to the right and fill with spaces the unused digits
      {
        for(int d=0;d<8;d++) 
        {
          if (d<(7-i))
          {
            data[d]=data[d+i+1];  
            separator[d]=separator[d+i+1];  
          }
          else
          {
            data[d]=32;
            separator[d]=0;
          }
        }
      }
      ct=0;
      showInProgress=true;
    }   
  }
  
  if (showInProgress==true) // we show the data only when it is ready
  {
    for(int i=0;i<8;i++) // loop for the 8 digits
    {
      if (timer[i]>0) // when a digit is shown timer[] becomes zero and the digit stops spinning
      {
        if (ct%timer[i]==0) // the snake is not moved every loop, to eventually allow the program to do something else
        {
          lc->setLed(0,i,spin[i]-1==0 ? 6 : spin[i]-1,0); // move the snake
          lc->setLed(0,i,spin[i],1); // the snake is formed by two segments 
          spin[i]++;
          if (spin[i]==7) // when the snake has done a full loop...
          {
            spin[i]=1; // ... restart the snake
            if (ct>(2000+(i+1)*1200)) // progressive show of the digits from right to left
            {
              timer[i]=timer[i]+25; // slow down the snake before showing the digit
              if (timer[i]>125)
              {
                timer[i]=0;
                lc->setChar(0, i, data[i], separator[i]); // show the digit
                if (i==7) // on the last digit reset the flag, to allow a new show
                {
                  showInProgress=false;
                }
              }
            }
          }
          if (timer[i]>0)
          {
            lc->setLed(0,i,spin[i],1); // remove the tail of the snake
          }
        }
      }
    }
    if (showInProgress==false) // if the show has been completed, prepare the arrays for a new show
    {
      for(int d=0;d<8;d++) 
      {
        data[d]=32;
        separator[d]=0;
        timer[d]=20;
        spin[d]=1;
      }
      spin[1]=4;
      spin[3]=4;
      spin[5]=4;
      spin[7]=4;
    }
  }
  delayMicroseconds(MSDELAY);
}
// More from author: www.ipposnif.com
