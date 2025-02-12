/** @brief Constants, structures, funcion prototypes
 *  @file ghcontrol.c
 *  @author Ramsen Oraha
 */

#include "ghcontrol.h"

/** Delays the program for a specified number of milliseconds
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @param milliseconds time to delay in milliseconds
 *  @return void
 */
void GhDelay(int milliseconds)
{
    long wait;
    clock_t now, start;

    wait = milliseconds*(CLOCKS_PER_SEC/1000);
    start = clock();
    now = start;
    while((now - start)< wait)
        {
        now = clock();
        }
}

/** Generates a random number within a specified range
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @param range the upper limit of the random number range
 *  @return a random number within the specified range
 */
int GhGetRandom (int range)
{
    return rand() % range;
}

/** Initializes the greenhouse controller, including setting up random number generation
 *  @return void
 */
void GhControllerInit(void)
{
    srand((unsigned) time(NULL));
//    GhDisplayHeader("Ramsen Oraha");
    initscr();
}

/** Displays the control status of the heater and humidifier
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 
 *  @since 2024-02-16
 *  @param ctrl A control structure that had the current status of the heater and humidifier
 *  @return void
 */
void GhDisplayControls(control_s ctrl)
{
   fprintf(stdout," Controls\tHeater:%i \tHumidifier:%i \n", ctrl.heater, ctrl.humidifier);
}

/** Displays the readings from the greenhouse sensors
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @param rtime the time of the readings
 *  @param structure containing sensor readings
 *  @return void
 */
void GhDisplayReadings(reading_s rdata)
{
    mvprintw(1,1, "\nUnit:%LX %s Readings\tT: %5.1fC\tH: %5.1f%\tP: %6.1fmb\n",
            ShGetSerial(), ctime(&rdata.rtime), rdata.temperature, rdata.humidity, rdata.pressure);
    refresh();
}

/** Displays the target setpoints for temperature and humidity
 *  @return void
 */
void GhDisplayTargets(setpoint_s spts)
{
    fprintf(stdout," Setpoints\tT: %5.1fC\tH: %5.1f%\n", STEMP, SHUMID);
}

/** Simulates humidity reading
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @return 1013.25
 *  @return simulated humidity reading
 */
float GhGetHumidity(void)
{
#if SIMPRESSURE
    return GhGetRandom(USHUMID - LSHUMID) + LSHUMID;
#else
    return ShGetHumidity();
#endif
}

/** Simulates pressure reading
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @return simulated pressure reading
 *  @return 1013.25
*/
float GhGetPressure(void)
{
#if SIMPRESSURE
    return GhGetRandom(USPRESS - LSPRESS) + LSPRESS;
#else
    return ShGetPressure();
#endif
}

/** Simulates temperature reading
 *  @version CENG153, serial: 822De31
 *  @author Ramsen Oraha
 *  @since 2024-02-16
 *  @return simulated temperature reading
 *  @return 25.0
 */
float GhGetTemperature(void)
{
#if SIMTEMPERATURE
    return GhGetRandom(USTEMP - LSTEMP) + LSTEMP;
#else
    return ShGetTemperature();
#endif
}

/** Set the control status for heater and humidifier based on the given setpoints and sensor readings.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param target Pointer to a struct setpoints representing the target setpoints for temperature and humidity.
 * @param rdata Pointer to a struct readings representing the sensor readings including temperature and humidity.
 * @return cset
 */
struct controls GhSetControls(setpoint_s target, reading_s rdata)
{
    struct controls cset;

    if (rdata.temperature < target.temperature)
    {
        cset.heater = ON;
    } else
	{
        cset.heater = OFF;
	}

    if (rdata.humidity < target.humidity)
    {
        cset.humidifier = ON;
    } else
	{
        cset.humidifier = OFF;
	}

    return cset;
}

/** Sets temperature and humidity setpoints
 *  @version CENG153, serial: 822De31
 *  @since 2024-02-16
 *  @author Ramsen Oraha
 *  @param cpoints.readings to store temperaure sensor readings
 *  @param cpoints.humidity to store humidity sensor readings
 *  @return setpoints for temp and humidity
 */
struct setpoints GhSetTargets(void)
{
    struct setpoints cpoints = GhRetrieveSetpoints("setpoints.dat");

	if(cpoints.temperature == 0)
	{
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	GhSaveSetpoints("setpoints.dat", cpoints);
	}

	return cpoints;
}

/** Retrieves sensor readings
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param readings structure to store sensor readings
 * @return now
 */
struct readings GhGetReadings(void)
{
    struct readings now = {0};
    now.rtime = time(NULL);
    now.temperature = GhGetTemperature();
    now.humidity = GhGetHumidity();
    now.pressure = GhGetPressure();
    return now;
}

/** Logs sensor readings to a file
 * @version CENG153, serial: 822De31
 * @since 2024-03-08
 * @author Ramsen Oraha
 * @param fname Pointer to the filename to log data to
 * @param ghdata A struct readings containing the sensor readings to log
 * @return 1 if successful, 0 otherwise
 */
int GhLogData(char *fname, reading_s ghdata)
{
    FILE * fp;
    char ltime[CTIMESTRSZ];

    fp = fopen(fname, "a");
    if (fp == NULL)
    {
        fprintf(stderr, "\n Can't open file, data not retrieved!\n");
        return 0;
    }

    strcpy(ltime, ctime(&ghdata.rtime));
    ltime[3] = ',';
    ltime[7] = ',';
    ltime[10] =',';
    ltime[19] =',';

    fprintf(fp, "\n%.24s,%5.1lf,%5.1lf,%6.1lf", ltime, ghdata.temperature, ghdata.humidity, ghdata.pressure);

    fclose(fp);

    return 1;
}

/** Saves the setpoints for temperature and humidity to a file.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param fname The filename to save the setpoints to.
 * @param spts The setpoints for temperature and humidity to be saved.
 * @return 1 if the setpoints are good, returns 0 if otherwise.
 */
int GhSaveSetpoints(char * fname, setpoint_s spts)
{
	FILE *fp;
	fp = fopen(fname, "wb");
	if (fp == NULL)
	{
		return 0;
	}
	fwrite(&spts, sizeof(setpoint_s),1, fp);

	fclose(fp);

	return 1;
}

/** Sets a vertical bar of LEDs based on reading values
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param pxc The color to set the LEDs to.
 * @param vbar value of the bar (0 to 7).
 * @param fb Pointer to the framebuffer structure representing the SenseHAT display.
 * @return exit success if it runs, if not exit failure.
 */
struct setpoints GhRetrieveSetpoints(char * fname)
{
	struct setpoints spts = {0.0};
	FILE *fp;
	fp = fopen(fname, "rb");
	if (fp == NULL)
	{
		return spts;
	}
	fread(&spts, sizeof(struct setpoints),1, fp);
	fclose(fp);

	return spts;
}

/** Sets a vertical bar of LEDs based on values
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param pxc The colours of the LEDs
 * @param bar bar from 0 to 7
 * @param value indicates the height of the bar
 * @param fb pointer the represents the display
 * @return exit success if it runs, if not exit failure.
 */
int GhSetVerticalBar(int bar, COLOR_SENSEHAT pxc, uint8_t value, struct fb_t *fb)
{
 	int i;
	if (value > 7)
	{
		value = 7;
	}
	if (bar < 0 || bar >= 8 ||value < 0 || value >= 8)
	{
		return EXIT_FAILURE;
	}
	for (i=0; i<= value; i++)
	{
		ShLightPixel(i, bar, pxc, fb);
	}
	for (i= value + 1; i < 8; i++)
	{
		ShLightPixel(i, bar, BLACK, fb);
	}

	return EXIT_SUCCESS;
}

/**Shows the temperature, humidity, and pressure setpoints and readings on a SenseHAT
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-02-16
 * @param rd The structure of temperature, humidity, and pressure.
 * @param sd The structure of setpoints of temperature and humidity.
 * @return SENSEHat colours based on the reading values.
 */
void GhDisplayAll(reading_s rd,setpoint_s sd, struct fb_t *fb)
{
	int rv;
	int avh;
	int avl;
	int sv;

	COLOR_SENSEHAT pxc;
	ShWipeScreen(BLACK, fb);

	rv = (8.0 * (((rd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;

	GhSetVerticalBar(TBAR, GREEN, rv, fb);

	rv = (8.0 * (((rd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;

	GhSetVerticalBar(HBAR, GREEN, rv, fb);

	rv = (8.0 * (((rd.pressure-LSPRESS)/(USPRESS-LSPRESS))+0.05))-1.0;

	GhSetVerticalBar(PBAR, GREEN, rv, fb);

	sv  = (8.0 * (((sd.temperature-LSTEMP)/(USTEMP-LSTEMP))+0.05))-1.0;

	ShLightPixel(sv, TBAR, MAGENTA, fb);

	sv  = (8.0 * (((sd.humidity-LSHUMID)/(USHUMID-LSHUMID))+0.05))-1.0;

	ShLightPixel(sv,HBAR, MAGENTA, fb) ;
}

/** Sets an alarm limit for temperature, humidity, and pressure.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-6
 * @return Alarm limits based on alarmlimit_s
 */
alarmlimit_s GhSetAlarmLimits(void)
{
    alarmlimit_s calarm ={0};
    calarm.hight = UPPERATEMP;
    calarm.lowt = LOWERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.lowh = LOWERAHUMID;
    calarm.highp = UPPERAPRESS;
    calarm.lowp = LOWERAPRESS;

    return calarm;
}

const char alarmnames[NALARMS][ALARMNMSZ] = {"No Alarms","HighTemperature","LowTemperature","High Humidity",
"Low Humidity","HighPressure","Low Pressure"};

/** Shows alarm stages based on a link list.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-16
 * @param alrm a linked list to alarm_s structure that representing the alarm stage
 */
void GhDisplayAlarms(alarm_s * head)
{
    alarm_s * cur = head;
    printf("\nAlarms\n");

    while(cur != NULL)
    {
        if(cur->code==LTEMP)
            {
                printf("LOW TEMPERATURE DETECTED %s", ctime(&cur->atime));
            }
        else if(cur->code==HTEMP)
            {
                printf("HIGH TEMPERATURE DETECTED %s", ctime(&cur->atime));
            }
        else if (cur->code==LPRESS)
            {
                printf("LOW PRESSURE DETECTED %s", ctime(&cur->atime));
            }
        else if (cur->code==HPRESS)
            {
                printf("HIGH PRESSURE DETECTED %s", ctime(&cur->atime));
            }
        else if (cur->code==LHUMID)
            {
                printf("LOW HUMIDITYDETECTED %s", ctime(&cur->atime));
            }
        else if (cur->code==HHUMID)
            {
                printf("HIGH HUMIDITY DETECTED %s", ctime(&cur->atime));
            }
        cur = cur->next;
    }
}
/** Sets one alarm stage based on specified conditions.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-16
 * @param code The code representing the type of alarm (e.g., high temperature, low pressure).
 * @param atime The time when the alarm occurred.
 * @param value The value associated with the alarm condition (e.g., temperature value, pressure value).
 * @param head A pointer to the head of the linked list containing alarm stages.
 * @return 1 if the alarm is successfully set, 0 otherwise.
 */
int GhSetOneAlarm(alarm_e code, time_t atime, double value, alarm_s *head)
{
    alarm_s *last, *cur;
    cur = head;
    if (cur->code != NOALARM)
    {
        while (cur->next != NULL)
        {
            cur = cur->next;
        }
}

/**
 * memory for a new alarm stage, initializes its attributes, and adds it to the linked list.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-14
 * @param code The code representing the type of alarm
 * @param atime The time when the alarm occurred.
 * @param value The value onnected with the alarm condition
 * @param head A pointer to the head of the linked list containing alarm stages
 * @return 1 if the alarm stage is successfully added, 0 if it fails
 */
alarm_s *new_alarm = (alarm_s *)calloc(1, sizeof(alarm_s));

    if (new_alarm == NULL)
    {
        fprintf(stderr, "\nNO MORE MEMORY\n");
        return 0;
    }

    new_alarm->code = code;
    new_alarm->atime = atime;
    new_alarm->value = value;
    new_alarm->next = NULL;

    if (cur->code == NOALARM)
    {
        head = new_alarm;
    }
    else
    {

        cur->next = new_alarm;
    }

    return 1;
}

/** Clears one alarm stage based on the specified alarm code.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-14
 * @param code The code representing the type of alarm to clear
 * @param head A pointer to the head of the linked list containing alarm stages.
 * @return A pointer to the head of the linked list after clearing the alarm stage.
 */
alarm_s *GhClearOneAlarm(alarm_e code, alarm_s *head)
{
    alarm_s *last, *cur;
    last = NULL;
    cur = head;

    while (cur != NULL && cur->code != code)
    {
        last = cur;
        cur = cur->next;
    }

    if (cur != NULL)
    {
        if (last == NULL)
        {
            head = cur->next;
        }
        else
        {
            last->next = cur->next;
        }
        free(cur);
    }
    return head;
}

/** Sets alarm stages based on specified conditions.
 * @version CENG153, serial: 822De31
 * @author Ramsen Oraha
 * @since 2024-04-14
 * @param head A pointer to the head of the linked list containing alarm phases.
 * @param alarmpt An object containing the alarm limits.
 * @param rdata An object containing sensor readings.
 * @return A pointer to the head of the linked list.
 */
alarm_s *GhSetAlarms(alarm_s *head, alarmlimit_s alarmpt, reading_s rdata)
{
    if (rdata.temperature >= alarmpt.hight)
    {
        GhSetOneAlarm(HTEMP, rdata.rtime, rdata.temperature, head);
    }
    else
    {
        head = GhClearOneAlarm(HTEMP, head);
    }

    if (rdata.temperature <= alarmpt.lowt)
    {
        GhSetOneAlarm(LTEMP, rdata.rtime, rdata.temperature, head);
    }
    else
    {
        head = GhClearOneAlarm(LTEMP, head);
    }
    return head;
}


