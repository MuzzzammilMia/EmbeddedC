

Code submitted is for Part 1 only. This project was used in congunction TI RLSK kit,
which consists of a ARM Cortex M4F microcontroller in conjuction with motors, bump
switches and LEDs. An Mp4 file has been attached to show how the 'buggy' in operation.

Assignment Brief

Develop a program that allows the robot that has two modes of operation:

o(Operation 1: Autonomous mode) upon pressing switch SW1, the robot operates
using predefined route and immediately stop when any of bump switches are touched.

o(Operation2:Free-motion mode) upon pressing switch SW2, the robot freely move 
forward but will change direction of movement according to the interrupted bump
switches.

TASKS
•Use the coloured LED to represent the interrupted bump switches.
•Devise a way that allows the interrupt latency to be measured and compare it with
the poling mode i.e. bump switches are monitored with polling.
•Ensure all of this can run at the same time simultaneously

Useful Information

The movement for coloured LED
    WHITE:   Forward
    BLUE:    Turn right
    YELLOW:  Turn left
    GREEN:   Backward
