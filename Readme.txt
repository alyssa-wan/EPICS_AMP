The goal of this repository is to create the code to read in EMG signals from the arm and use them to activate 
motors (origionally servos and now linear actuators) in order to mimic the movement of the users arm.

Information about each file:
Name of file - Date created
State (Finished/Unfinished - Works/Doesn'tWork - InUse/NotInUse/PieceOfLarger)
Libraries used
Purpose & Funtion

first_reading 9-15-24.cpp  -   9-15-14
    Finished - Works - PieceOfLarger
    MsTimer2.h
    Read in the values from the sensors on the arm for the first time (fall 24) and even it out 
        by using windows (the average of the data over a certain amount of time)

actuator_test.c   -    
    Finished - doesn't work - not in use
    "AVR_PWM.h" (ended up being a bad library for what we want)
    This was our first attempt (Fall 24) at communicating with the linear acutators, we were
        unable to get the motors to function using this and we learned that the library we were using did not do what 
        we had hoped

9-25-24 cleanup   -   9-25-24
    InProgress - Untested - InUse
    <MsTimer2.h>, "AVR_Slow_PWM.h"
    This is very similar to AMP 9-15-24 Actu&Start Read.c but it fixes some things such as making sure
        all variables from it's predecessor files (first_reading 9-15 & AMP 9-8 actuating) were copied over 
        so that it might function, a few other things were changed for readibility and prep for combining sensing 
        and actuating. Still does not use the sensor values to make the actuators move.

AMP 9-15-24 Actu&Start Read.c  -  9-15-24
    Finished - Untested - PieceOfLarger
    <MsTimer2.h>, "AVR_Slow_PWM.h"
    This is the first try at putting both actuating and reading from the sensors together. They are not connected but 
        the code for each of the functions is there. There are minor errros as well that would prvent this from running.

AMP 9-8-24 Actuating.c F24  -  9-8-24
    Finished - Works - PieceOfLarger
    "AVR_Slow_PWM.h"
    This is the working code for making the actuators move using interupts. It does not involve reading from the sensors, 
        it merely adjusts them in a cycle.



