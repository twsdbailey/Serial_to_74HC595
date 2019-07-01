# Serial data to 74HC595 shift register
Send bytes of data to a 74HC595 shift register from a computer via an Arduino to create patterned outputs. Can be used for POV LED applications, and other things.


This example code demonstrates how to send bytes of data to 74HC595 shift registers serially via an Arduino controller.   Each shift register represents one byte (or 8 bits) of data, and therefore provides eight controlled outputs. The 74HC595 shift registers can be daisy chained together to add additional outputs in groups of eight.  The maximum number of daisy-chained shift registers will depend on the application.

When a byte of character data is sent to the arduino from a computer,  it gets parsed and stored into a two-dimensional array representing rows and columns.  The number of columns is determined by the number of shift registers used (8 columns per).  The number of rows to render can be set to any number, depending on the amount of RAM available on the controller.  The example code as written uses two shift registers to create 16 columns, and renders 16 rows of patterned outputs.  The amount of RAM needed to make an array of this size (16 X 16) is 256 bytes.  Arduino UNOs have 1KB of RAM.  If you need more RAM, consider using an Arduino Mega (8KB of RAM), or something similar.



**Note: The Information below is specific to the included example code.**

Data sent to the arduino needs to be sent as ASCII characters.  Sending the number characters 255 will turn-on all eight outputs on the first shift register.  Sending a second set of 255 number characters will turn-on all eight outputs on the second shift register.  Conversely, sending a 0 number character will turn-off all eight outputs on the first shift registers.  Sending a second 0 number character will turn-off all eight outputs on the second shift register.

When sending each two byte set, both number characters need to be followed by a comma  x,x,  No spaces.

**Example:**

Turn off all outputs on both shift registers:
        0,0,    then send using carriage return

Turn on all outputs on both shift registers:
        255,255, then send using carriage return

Since the data array is sized 16 rows X 2 bytes (16 outputs),  16 sets of x,x, will need to be sent before the outputs will be activated.  Once all the data is in, the outputs will create the pattern sequence one time.

If an “l” character (do not include the quotes) followed by a carriage return is sent, the pattern will continuously loop through the pattern.

If an “s” character (do not include the quotes) followed be a carriage return is sent, the looping will stop when it reaches the end of the pattern.

If an “n” character (do not include the quotes) followed be a carriage return, the data will be cleared, and a new data set can be sent.


**Designing patterns**

When creating a pattern, it might be helpful to divide each row of 16 columns into two, each representing one shift register.  Then decide which outputs will be turned-on and turned-off in each of the rows.  Use 1s and 0s to represent the output states for the given rows. Then use an online binary to decimal converter to get the decimal value equivalents.

https://www.rapidtables.com/convert/number/binary-to-decimal.html

**Example:  X pattern LED flasher**

```
Rows	     Line index      Columns index (2 byte elements)   Decimal equivalent	
                                    (0)      (1)
Row one: 	(0)             10000000    00000001	    =     128,1,
Row two         (1)             01000000    00000010	    =  	  64,2,
Row three       (2)             00100000    00000100	    =  	  32,4,
Row four	(3)             00010000    00001000	    =     16,8,
Row five        (4)             00001000    00010000	    =	  8,16,
Row six		(5)             00000100    00100000	    =	  4,32,
Row seven       (6)             00000010    01000000	    =	  2,64,
Row eight	(7)             00000001    10000000	    =	  1,128,
Row nine	(8)             00000001    10000000	    =	  1,128,
Row ten		(9)             00000010    01000000	    =	  2,64,
Row eleven      (10)            00000100    00100000	    =	  4,32,
Row twelve      (11)            00001000    00010000	    =	  8,16,
Row thirteen    (12)            00010000    00001000	    =     16,8,
Row fourteen    (13)            00100000    00000100	    =  	  32,4,
Row fifteen	(14)            01000000    00000010	    =  	  64,2,
Row sixteen     (15)            10000000    00000001        =     128,1,
```
