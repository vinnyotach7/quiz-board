# The Quiz Board

The aim of this project is the development of a microcontroller based
board to perfom a quiz or puzzle in public. This board is developed
with NGO's in mind, working with the audience on the streets.

The quiz is based on questions with 4 answers. Only one answer is
correct. There are up to 8 questions possible. The sketch below show
a possible design of the board. The buttons could be places within the gray area.

![sketch](https://raw.github.com/joede/quiz-board/master/docs/images/Puzzleboard-Sample--en.png)

The answer is given by pressing one of four buttons. The result is
shown immediately through a red or green LED. The puzzle is done as soon
as all questions got an answer. As an additional option, a time limit
can be enabled, so that all answers must be given within this time period.

To show the state of the operation of the board, a system wide yellow LED is planned.
The sketch above doesn't show this LED.

Since we need a larger number of IOs, we will use a
[Arduino Mega 2560](http://arduino.cc/en/Main/arduinoBoardMega2560)
board.

One important goal of the project is a simple and lean design. It should be easy
for new and unexperienced developers to get familiar with the code. For this
reason, I try to avoid heavy weight libraries in favour of simple and straight
solutions (remember the [KISS](http://en.wikipedia.org/wiki/KISS_principle)
principle).


## Features

Planned features are:

* stand-alone usage without a PC
* selectable set of *correct answers*
* power-safe mode for running on batteries
* optional observation of the time to answer all questions (timeout)

**Note:** the main audience of this project is the German NGO [BUND](http://www.bund.net),
a environmental (protection) organisation. Because of this, all further documents
are written in German. If there is interest from outside of Germany, I may
add an english translation. Feel free to send me your translations.


## License

### Software

EN: Unless otherwise stated, all software or program code and all snippets are licensed under the [GNU GPL 3.0](http://www.gnu.org/licenses/gpl).

DE: Sofern nicht anders angegeben, stehen alle Programme oder Programcode sowie alle Code-Schnipsel unter der [GNU GPL 3.0](http://www.gnu.org/licenses/gpl).

### Documentation

EN: Unless otherwise stated, all documents like manuals, specifications or instructions are licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License](href="http://creativecommons.org/licenses/by-nc-sa/3.0/).

DE: Sofern nicht anderes angegeben, stehen alle Dokumente wie Handbücher, Anleitungen oder Spezifikationen unter einer [Creative Commons Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 3.0 Unported Lizenz](http://creativecommons.org/licenses/by-nc-sa/3.0/deed.de").

![Creative Commons License](http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png)
