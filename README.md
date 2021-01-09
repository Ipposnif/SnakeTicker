![](https://www.ipposnif.com/download/snaketickervideo.gif)  

If you want to keep updated with the number of visitors to your website, the amount of the last order received, or the days elapsed since the last bug found, this ticker gives to your most meaningful number a special standout.  
In this test configuration, the ticker will show the current BTCEUR exchange rate .  

**What you need** 
* Arduino board (the ticker has been tested using Arduino UNO and Arduino Nano, but it should work with every Arduino version).
* MAX7219 8 digits led module
* PC with Windows (tested with Windows 10, but it should work with any recent Windows release)
* Arduino IDE (it is the official development environment for Arduino, it is free and you can download it from https://www.arduino.cc/en/software)
* PuTTY (https://www.putty.org/ is free software and contains the plink.exe executable needed to automate the Serial transfer from your PC to the Arduino board)


**How to set up the ticker**  
Connect the led module to Arduino like explained in this article: https://blog.circuits4you.com/2016/04/arduino-display-module-7-segment-8.html  
Connect Arduino to your PC with the USB cable.  
From the Arduino IDE, open Tools -> Library Manager and search and install the library LedControl (the one from Eberhard Fahle).  
Unzip the code of this repository to C:\Users\{usr}\Documents\Arduino\SnakeTicker  
From the Arduino IDE, open the file SnakeTicker.ino and select Upload from the menu Sketch. This will compile the program and upload it to your Arduino board.  
From the Arduino IDE, open menu Tools and read the COM port of your PC where the Arduino board is connected. In this example the port is COM4.  
Edit the file command.bat and replace COM4 with your port.  

**How to use the ticker**  
To test the ticker run command.bat. After a few instants, the ticker will show the current BTCEUR exchange rate.  
When everything is ready you can automate the execution of command.bat with Windows Task Scheduler.  

**How to customize the data**  
In this example, the data is retrieved from the web page https://www.ipposnif.com/btceur_3b570za8ob1.asp of my personal website.  
You can change the URL from command.bat.  
Only the first 8 chars returned by the web page will show on the ticker.  
The led module is capable to show only the characters: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, A, b, c, d, E, F, H, L, P, ., -, underscore, space  
The full stop is considered always a decimal separator and it is shown together with the preceding character.  
The Carriage Return and Line Feed characters are discarded.  

**Security**  
In the default configuration of this ticker, the web page returning the data must be a public page. The security is simply demanded to the complexity of the page name.   
To prevent a visitor from listing the pages of your public folder, the web folder should have a default page or in the alternative, the folder should be configured in IIS with Directory Browsing OFF. If the web site, like in this example, is protected by SSL, the page name will be encrypted together with the data.  
Anyway, don’t use this ticker to retrieve and show classified data :)

**Credits**  
The led module is controlled using the library https://github.com/wayoda/LedControl  
The web request is performed using: https://github.com/npocmaka/batch.scripts/blob/master/hybrids/jscript/winhttpjs.bat  
The BTCEUR exchange rate is updated every minute from https://www.kraken.com  

