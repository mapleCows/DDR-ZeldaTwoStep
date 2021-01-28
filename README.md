# DDR-ZeldaTwoStep

## Introduction
Dance Dance Revolution is a two-dimensional dance game in which the user matches the icons
on screens in time with inputs. The player will use a SNES controller to match the icons as they
go up the LED matrix. The game will continue until the song is completed. There will be 3
different speed difficulties. The aim is to match as many icons as possible in order to gain a
higher score.


![Board](https://user-images.githubusercontent.com/38027847/106088980-d7d38500-60db-11eb-9c07-8084551899ab.jpg)

## User Guide
Rules:
1. Press the corresponding direction pad arrow when the block reaches the middle
of the two red lines.
Controls:
1. Directional pad arrows
2. Select = first song
3. Start = second song
4. L & R = Reset high score

## Technologies
In this project, I learned how serial data encoding works. I learned how a clock and latch
works to control the data being sent from one component to the other. I also learned how
EEPROM memory works and how to write and read from it. I created custom characters by
setting bits high, drawing, on the LCD display and how to write to the LCD memory. I learned
how Arduino is used and how LED matrices are made and used. Lastly, I learned how
concurrent state machine task management is programmed.

## Parts List
The hardware that was used in this design is listed below. The equipment that was not taught in
this course has been bolded.
● ATMega1284 microcontroller

● Arduino UNO

● Adafruit led 8x8 matrix

● Buzzer

● 16x2 LCD display

● Custom Characters

● SNES Controller

● EEPROM


## Components (Pin-out)
● Inputs
  ○ SNES Controller
  ○ Reset and start buttons
● Outputs
  ○ 8x8 LED matrix. This will be used to display the game
  ○ LCD screen to display the score and start menu
  ○ Sound buzzer for music
  
  ![Atmega Pin layout](https://user-images.githubusercontent.com/38027847/106089094-1d904d80-60dc-11eb-928c-825261d07c90.PNG)

## Software
I used three concurrent state machines: ZeldaTunes(500ms) Score(50ms) Menu(500ms)

ZeldaTunes handles the music
![ZeldaTunes](https://user-images.githubusercontent.com/38027847/106089132-300a8700-60dc-11eb-9847-c1fd320c3d41.jpg)

Score handles the key press matching and adding score if correct
![Scores](https://user-images.githubusercontent.com/38027847/106089229-60eabc00-60dc-11eb-9f51-8ffe6c44cc83.jpg)

Menu hanldes the menu interface at start-up and end of song
![Menu](https://user-images.githubusercontent.com/38027847/106089267-7364f580-60dc-11eb-8509-4b716d2a923f.jpg)

## Youtube Link
[Youtube Demo Video] (https://youtu.be/WVLpBQQFC5A)


## Known Bugs and Shortcomings
● If the player double presses one of the inputs when the corresponding input is in the red
barrier two points will be added instead of one. A fix to this would be to add to not allow
any input checking after one input until the pin is set to low.

● The wires into the control sometimes become loose and require them to be pushed back
in a little. A fix to this would be to cut the wire on the controller and solder the input wires
to the wires on the controller directly.

## Future work
I would like to add two player functionality for people to compete head to head on the same
song at the same time. I would also want to add more songs and a harder difficulty that requires
the use of more button inputs.



 
