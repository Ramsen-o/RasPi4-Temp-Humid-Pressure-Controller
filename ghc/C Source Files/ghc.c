/** @brief Defines the entry point for the console application
 *  @file ghc.c
 *  @author Ramsen Oraha
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "ghcontrol.h"
#include <curses.h>

/** Begins the greenhouse display and enters a loop to continuously monitor and control diff>
 * @param tempc set temperature control, humidc sets humidity control, creading displays sensor readings
 * @return EXIT_FAILURE
 */
int main(void)
{
    time_t now;
    int logged;

    GhControllerInit();

    reading_s creadings = {0};
    control_s ctrl = {0};
    setpoint_s sets = {0};
    alarmlimit_s alimits= {0};
    alarm_s warn[NALARMS];
    struct fb_t *fb;
    fb = ShInit(fb);
    sets = GhSetTargets();

    while(1)
    {
	   now = time(NULL);
       ctrl = GhSetControls(sets,creadings);
	   creadings = GhGetReadings();
	   GhDisplayAll(creadings, sets, fb);
	   GhDisplayReadings(creadings);
	   //GhDisplayTargets(sets);
	   alarm_s * arecord;
       arecord = (alarm_s *) calloc(1,sizeof(alarm_s));
       if(arecord == NULL)

           {
               fprintf(stderr,"\nCannot allocate memory\n");
               return EXIT_FAILURE;
           }
	   GhSetAlarms(warn, alimits, creadings);
	   alimits = GhSetAlarmLimits();
	   //GhDisplayControls(ctrl);
	   //GhDisplayAlarms(warn);
	   GhDelay(GHUPDATE);
    }

    return EXIT_FAILURE;
}
