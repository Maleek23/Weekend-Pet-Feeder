/*
 * File: petfeeder.c
 * Author: Abdulmalik Ajisegiri
 * License: Apache License 2.0
 *
 * -------------------------------------------------------------------------------------
 * Apache License 2.0
 * -------------------------------------------------------------------------------------
 * TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
 *
 * 1. Definitions.
 * ...
 * (Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION

1. Definitions.
"License" shall mean the terms and conditions for use, reproduction, and distribution as defined by Sections 1 through 9 of this document.
"Licensor" shall mean the copyright owner or entity authorized by the copyright owner that is granting the License.
"Legal Entity" shall mean the union of the acting entity and all other entities that control, are controlled by, or are under common control with that entity. For the purposes of this definition, "control" means (i) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (ii) ownership of fifty percent (50%) or more of the outstanding shares, or (iii) beneficial ownership of such entity.
"You" (or "Your") shall mean an individual or Legal Entity exercising permissions granted by this License.
"Source" form shall mean the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files.
"Object" form shall mean any form resulting from mechanical transformation or translation of a Source form, including but not limited to compiled object code, generated documentation, and conversions to other media types.
"Work" shall mean the work of authorship, whether in Source or Object form, made available under the License, as indicated by a copyright notice that is included in or attached to the work (an example is provided in the Appendix below).
"Derivative Works" shall mean any work, whether in Source or Object form, that is based on (or derived from) the Work and for which the editorial revisions, annotations, elaborations, or other modifications represent, as a whole, an original work of authorship. For the purposes of this License, Derivative Works shall not include works that remain separable from, or merely link (or bind by name) to the interfaces of, the Work and Derivative Works thereof.
"Contribution" shall mean any work of authorship, including the original version of the Work and any modifications or additions to that Work or Derivative Works thereof, that is intentionally submitted to Licensor for inclusion in the Work by the copyright owner or by an individual or Legal Entity authorized to submit on behalf of the copyright owner. For the purposes of this definition, "submitted" means any form of electronic, verbal, or written communication sent to the Licensor or its representatives, including but not limited to communication on electronic mailing lists, source code control systems, and issue tracking systems that are managed by, or on behalf of, the Licensor for the purpose of discussing and improving the Work, but excluding communication that is conspicuously marked or otherwise designated in writing by the copyright owner as "Not a Contribution."
"Contributor" shall mean Licensor and any individual or Legal Entity on behalf of whom a Contribution has been received by Licensor and subsequently incorporated within the Work.
2. Grant of Copyright License. Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable copyright license to reproduce, prepare Derivative Works of, publicly display, publicly perform, sublicense, and distribute the Work and such Derivative Works in Source or Object form.
3. Grant of Patent License. Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non-exclusive, no-charge, royalty-free, irrevocable (except as stated in this section) patent license to make, have made, use, offer to sell, sell, import, and otherwise transfer the Work, where such license applies only to those patent claims licensable by such Contributor that are necessarily infringed by their Contribution(s) alone or by combination of their Contribution(s) with the Work to which such Contribution(s) was submitted. If You institute patent litigation against any entity (including a cross-claim or counterclaim in a lawsuit) alleging that the Work or a Contribution incorporated within the Work constitutes direct or contributory patent infringement, then any patent licenses granted to You under this License for that Work shall terminate as of the date such litigation is filed.
4. Redistribution. You may reproduce and distribute copies of the Work or Derivative Works thereof in any medium, with or without modifications, and in Source or Object form, provided that You meet the following conditions:
(a) You must give any other recipients of the Work or Derivative Works a copy of this License; and
(b) You must cause any modified files to carry prominent notices stating that You changed the files; and
(c) You must retain, in the Source form of any Derivative Works that You distribute, all copyright, patent, trademark, and attribution notices from the Source form of the Work, excluding those notices that do not pertain to any part of the Derivative Works; and
(d) If the Work includes a "NOTICE" text file as part of its distribution, then any Derivative Works that You distribute must include a readable copy of the attribution notices contained within such NOTICE file, excluding those notices that do not pertain to any part of the Derivative Works, in at least one of the following places: within a NOTICE text file distributed as part of the Derivative Works; within the Source form or documentation, if provided along with the Derivative Works; or, within a display generated by the Derivative Works, if and wherever such third-party notices normally appear. The contents of the NOTICE file are for informational purposes only and do not modify the License. You may add Your own attribution notices within Derivative Works that You distribute, alongside or as an addendum to the NOTICE text from the Work, provided that such additional attribution notices cannot be construed as modifying the License.
You may add Your own copyright statement to Your modifications and may provide additional or different license terms and conditions for use, reproduction, or distribution of Your modifications, or for any such Derivative Works as a whole, provided Your use, reproduction
 * ...
 * -------------------------------------------------------------------------------------
 *
 * If you find this code useful or modify it, please provide attribution to the original author.
 * Let's build a collaborative and open-source community! 🚀
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "clock.h"
#include "wait.h"
#include "uart0.h"
#include "tm4c123gh6pm.h"
#include "eeprom.h"

//[lab 5]
#define PF3        (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4))) //PF3  output
#define PC7        (*((volatile uint32_t *)(0x42000000 + (0x400063FC-0x40000000)*32 + 7*4))) //PC7
//#define PC6        (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 6*4))) //PC6
#define PUMP       (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 6*4))) //PB6
#define AUGER      (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4))) //PF2
#define SPEAKER    (*((volatile uint32_t *)(0x42000000 + (0x400043FC-0x40000000)*32 + 5*4))) //PA5
#define SENSOR     (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 2*4))) //PB2
#define RED_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4))) //PF1


// PortF masks
#define PF3_MASK 8   //output
#define PC7_MASK 128   //C0-
//#define PC6_MASK 64    //C0+
#define PUMP_MASK 64 //2^6
#define AUGER_MASK 4 //2^2
#define SENSOR_MASK 4 //2^2
#define SPEAKER_MASK 32 //2^5
#define RED_LED_MASK 2 //2^3


// Part2: Max number of character accepted from user & structure for holding UI information
#define MAX_CHARS 80
#define MAX_FIELDS 6

typedef struct _USER_DATA
{
    char buffer[MAX_CHARS+1];
    uint8_t fieldCount;
    uint8_t fieldPosition[MAX_FIELDS];
    char fieldType[MAX_FIELDS];
} USER_DATA;



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//bool timeMode = false;
uint32_t time= 0;
char str[100];
float waterlevel= 0;
float b = 2615;
float slope = 0.97;
uint32_t event;
char fillMode = 'a'; // 'a' for Auto Fill mode, 'm' for Motion Fill mode
uint32_t water = 10;

uint8_t autoflag = 0;

//-----------------------------------------------------------------------------
// UART
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PART 4: Code function getsUart0()
char getsUart0(USER_DATA* data)
{
      uint32_t count = 0;
      char c;
      while (true)
      {
            c = getcUart0();
            if (c == 8 || c == 127)
            {
                  if (count > 0)
                  {
                      count--;
                  }
            }
            else if (c == 13)
            {
                  data->buffer[count] = '\0';
                  return 0;
            }
            else if (c >= 32 && c <= 126)
            {
                  if (count < MAX_CHARS)
                  {
                    data->buffer[count] = c;
                    count++;
                  }
                  else
                  {
                    data->buffer[count] = '\0';
                    return 0;
                  }
            }
      }
}

//-----------------------------------------------------------------------------
//part 5: test


//-----------------------------------------------------------------------------
//  Part6: parsefields function


void parseFields(USER_DATA* data)
{
  char prevCharType = 'd';
  int i = 0;
  data->fieldCount = 0;
  while(data->buffer[i] != '\0')
  {
    char c = data->buffer[i];
    char curCharType;
    if(isalpha(c)) {
      curCharType = 'a';
    }
    else if(isdigit(c) || c == '-' || c == '.')
    {
      curCharType = 'n' ;
    }
    else {
      curCharType = 'd';
      data->buffer[i] = '\0';
    }

    if(prevCharType == 'd' && curCharType != 'd')
    {
       data->fieldType[data->fieldCount] = curCharType;
       data->fieldPosition[data->fieldCount] = i;
       data->fieldCount++;

       if(data->fieldCount == MAX_FIELDS)
       {
         break;
       }
    }
    prevCharType = curCharType;
    i++;
  }


}

//-----------------------------------------------------------------------------
//Part7:

char* getFieldString(USER_DATA* data, uint8_t fieldNumber)
{
    if(fieldNumber < data->fieldCount && data->fieldType[fieldNumber] == 'a')
    {
        // field is in range
        return &data->buffer[data->fieldPosition[fieldNumber]];
    }
    else
    {
        // field out of range
        return NULL;
    }
}

//-----------------------------------------------------------------------------
// Part 8:

int32_t getFieldInteger(USER_DATA* data, uint8_t fieldNumber)
{
    int32_t value = 0;
    int i = 0;


    if (fieldNumber < data->fieldCount && data->fieldType[fieldNumber] == 'n')
    {
        char *str = &(data->buffer[data->fieldPosition[fieldNumber]]);

        while (str[i] >= '0' && str[i] <= '9')
        {
            value = value * 10 + (str[i] - '0');
            i++;
        }

        return value;
    }

    return 0;
}


//-----------------------------------------------------------------------------
//Part 9:

bool isCommand(USER_DATA* data, const char strCommand[], uint8_t minArguments)
{
    // Get command string
    char* command = getFieldString(data, 0);
    // Compare command string
    if(strcmp(command, strCommand) == 0)
    {
      // Check number of arguments
      if(((data->fieldCount) - 1) >= minArguments)
      {
        return true;
      }
    }
    return false;
}


// [lab 6]
//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------



void initWideTimer1()
{
    RED_LED ^= 1;

    PF3 = 1;
    waitMicrosecond(100);
    PF3 = 0;
    TIMER1_CTL_R |= TIMER_CTL_TAEN;  //turn on counter
    TIMER1_TAV_R = 0;   //reset timer

    COMP_ACINTEN_R |= COMP_ACINTEN_IN0; //enable comp interrupt
    NVIC_EN0_R = 1 << (INT_COMP0-16); //Turn on interrupt compare 0
    WTIMER1_ICR_R = TIMER_ICR_TATOCINT;  // clear interrupt flag
}






//-----------------------------------------------------------------------------
//init RTC step 3
//-----------------------------------------------------------------------------


void initRTC(int HOURS, int MINUTES)
{
    if(((HOURS < 24) && (MINUTES <= 59)) || ((HOURS == 0 && MINUTES <=59)))
    {
        while(!(HIB_CTL_R & HIB_CTL_WRC));
        HIB_RTCLD_R = ((3600 * HOURS) + (MINUTES) * 60);
    }
    else
    {
        putsUart0("Invalid time. \n");
    }
}

//-----------------------------------------------------------------------------
// PWM Module step 5
//-----------------------------------------------------------------------------


void initpwm()
{
    // Enable PWM clocks
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0; //pwm0
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1; //pwm1


    // Configure PWM

    //PB6 M0PWM0 //PUMP
    GPIO_PORTB_PCTL_R &= ~(GPIO_PCTL_PB6_M);
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB6_M0PWM0;

    // PF2 M1PWM6 //AUGER (Changed from PF0 to PF2)
    GPIO_PORTF_PCTL_R &= ~(GPIO_PCTL_PF2_M);
    GPIO_PORTF_PCTL_R |= GPIO_PCTL_PF2_M1PWM6;

    SYSCTL_SRPWM_R = SYSCTL_SRPWM_R1; // reset PWMI module
    SYSCTL_SRPWM_R = 0;    // leave reset state

    PWM0_0_CTL_R = 0; // turn-off PWM0 for PB6 generator 0 (drives outs 4 and 5)
    PWM1_3_CTL_R = 0;    // turn-off PWM1 for PF2 generator 3 (drives outs 6 and 7)

    PWM0_0_GENA_R = PWM_0_GENA_ACTCMPAD_ONE | PWM_0_GENA_ACTLOAD_ZERO; // output 5 on PWM0, gen 26, cmpb

    PWM1_3_GENA_R |= PWM_1_GENA_ACTCMPAD_ONE | PWM_1_GENA_ACTLOAD_ZERO;  // output 6 on PWM1, gen 3a, mpa

    PWM0_0_LOAD_R = 1024;  // set frequency to 40. MHz sys clock / 2 / 1024
    PWM1_3_LOAD_R = 1024;  // (internal counter counts down from load value)

    PWM0_0_CMPA_R = 0;  // red off (0-always low, 1023=always high)
    PWM1_3_CMPA_R = 0;      // RED off

    PWM0_0_CTL_R = PWM_0_CTL_ENABLE; // turn-on PWM0 generator 0
    PWM1_3_CTL_R = PWM_1_CTL_ENABLE;      // turn-on PWM1 generator 3


    PWM0_ENABLE_R = PWM_ENABLE_PWM0EN;
    PWM1_ENABLE_R = PWM_ENABLE_PWM6EN; // enable generator outputs (M0PWM0)(M1PWM6)
}


//  [lab 7]
//-----------------------------------------------------------------------------
// Initialize Eeprom
//-----------------------------------------------------------------------------

void initEeprom(void)
{
    SYSCTL_RCGCEEPROM_R = SYSCTL_RCGCEEPROM_R0;
    _delay_cycles(3);
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
}

void writeEeprom(uint16_t add, uint32_t data)
{
    EEPROM_EEBLOCK_R = add >> 4;
    EEPROM_EEOFFSET_R = add & 0xF;
    EEPROM_EERDWR_R = data;
    while (EEPROM_EEDONE_R & EEPROM_EEDONE_WORKING);
}

uint32_t readEeprom(uint16_t add)
{
    EEPROM_EEBLOCK_R = add >> 4;
    EEPROM_EEOFFSET_R = add & 0xF;
    return EEPROM_EERDWR_R;
}
//-----------------------------------------------------------------------------
//Init HW
//-----------------------------------------------------------------------------

void initHw(void)
{

    initSystemClockTo40Mhz();

    // Enable clocks
    SYSCTL_RCGCACMP_R |= SYSCTL_RCGCACMP_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0| SYSCTL_RCGCGPIO_R1 | SYSCTL_RCGCGPIO_R2| SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R4 | SYSCTL_RCGCGPIO_R5;
    SYSCTL_RCGCWTIMER_R |= SYSCTL_RCGCWTIMER_R0 | SYSCTL_RCGCWTIMER_R1 | SYSCTL_RCGCWTIMER_R3 | SYSCTL_RCGCWTIMER_R3;
    SYSCTL_RCGCTIMER_R |=  SYSCTL_RCGCTIMER_R0| SYSCTL_RCGCTIMER_R1 | SYSCTL_RCGCTIMER_R2 |SYSCTL_RCGCTIMER_R3;
    // Enable hib clocks
    SYSCTL_RCGCHIB_R |= SYSCTL_RCGCHIB_R0;
    _delay_cycles(3);


    GPIO_PORTB_DIR_R |= PUMP_MASK;
    GPIO_PORTB_DEN_R |= PUMP_MASK;

    GPIO_PORTF_DIR_R |= AUGER_MASK;
    GPIO_PORTF_DEN_R |= AUGER_MASK;

    GPIO_PORTB_DIR_R &= ~SENSOR_MASK;
    GPIO_PORTB_DEN_R |= SENSOR_MASK;

    GPIO_PORTA_DIR_R |= SPEAKER_MASK;
    GPIO_PORTA_DEN_R |= SPEAKER_MASK;

    GPIO_PORTF_DIR_R |= RED_LED_MASK;
    GPIO_PORTF_DR2R_R |= RED_LED_MASK;
    GPIO_PORTF_DEN_R |= RED_LED_MASK;


    initEeprom();
    //initHm();
    initpwm();

    //COMP_ACREFCTL_R |= COMP_ACREFCTL_EN | 0xF;
    //COMP_ACCTL0_R |= COMP_ACCTL0_ASRCP_REF | COMP_ACCTL0_CINV |COMP_ACCTL0_ISEN_RISE;

    GPIO_PORTF_DIR_R |= PF3_MASK;
    GPIO_PORTF_DEN_R |= PF3_MASK;
    GPIO_PORTC_DIR_R &= ~PC7_MASK;
    GPIO_PORTC_DEN_R &= ~PC7_MASK;

    GPIO_PORTF_AFSEL_R |= AUGER_MASK;
    GPIO_PORTB_AFSEL_R |= PUMP_MASK;

    GPIO_PORTC_AFSEL_R |= PC7_MASK;
    GPIO_PORTC_AMSEL_R |= PC7_MASK;

    //Configure Internal Voltage
   COMP_ACREFCTL_R = COMP_ACREFCTL_EN;         // Resistor Ladder Enable
   COMP_ACREFCTL_R = COMP_ACREFCTL_VREF_M;     // Resistor Ladder Voltage Ref
   COMP_ACCTL0_R = COMP_ACCTL0_ASRCP_REF;      // Internal voltage reference

   waitMicrosecond(10);

   //Analog Comparator Voltage Reference Characteristics,
   //VDDA = 3.3V, EN= 1, and RNG = 0


    //Hibernation Control
    while(!(HIB_CTL_R & HIB_CTL_WRC));
    HIB_CTL_R= HIB_CTL_CLK32EN;  //Clock enabling

    while(!(HIB_CTL_R & HIB_CTL_WRC));
    HIB_CTL_R |= HIB_CTL_RTCEN;  //Timer enabling

    // Hibernation Interrupt Mask
    while(!(HIB_CTL_R & HIB_CTL_WRC));
    HIB_IM_R |= HIB_IM_RTCALT0; // RTC Alert 0 Interrupt Mask

    while(!(HIB_CTL_R & HIB_CTL_WRC));
    NVIC_EN1_R |= 1 << (INT_HIBERNATE-16-32);
}

void enableTimerMode()
{
   TIMER1_CTL_R &= ~TIMER_CTL_TAEN;                 // turn-off timer before reconfiguring
   TIMER1_CFG_R = TIMER_CFG_32_BIT_TIMER;           // configure as 32-bit timer (A+B)
   TIMER1_TAMR_R = TIMER_TAMR_TAMR_1_SHOT| TIMER_TAMR_TACDIR;          // configure for ONE SHOT mode (count down)
   TIMER1_TAILR_R = 40E6;  // set load value to 40e6 for 1 Hz interrupt rate
   TIMER1_TAV_R = 0;                               // zero counter for first period
}

////-----------------------------------------------------------------------------
//// setAlarm  STEP 7
////-----------------------------------------------------------------------------

// Function to find the next feeding event based on the current time

void findNextEvent(void)
{
    uint32_t currentTime = 0;
    int closestEventTime= -1;
    uint32_t event;
    uint32_t closestEvent =0;

    while (!(HIB_CTL_R & HIB_CTL_WRC));
    currentTime = HIB_RTCC_R;

    for (event = 0; event < 10; event++)
    {
        uint32_t eventTime = readEeprom(16 * event + 3) * 3600 + readEeprom(16 * event + 4) * 60; // Convert hours and minutes to seconds


        // Check if the event is closer to the current time than the previously found closest event
        if (eventTime > currentTime && eventTime < closestEventTime && eventTime != 0xFFFFFFFF)
        {
            closestEventTime = eventTime;
            closestEvent = event;
        }
    }
    //return closestEventTime;

    // Function to set the next alarm based on the next feeding event

    if (closestEventTime != -1)
    {

        // Set the RTC alarm to the calculated time
        while (!(HIB_CTL_R & HIB_CTL_WRC));
        HIB_RTCM0_R = closestEventTime; // Use RTCM0 and add the time difference to the current time

        // Enable the RTC Wake-up interrupt
        while (!(HIB_CTL_R & HIB_CTL_WRC));
        HIB_RIS_R |= HIB_RIS_RTCALT0;

        // Print a message indicating that the alarm is set
        char alarmText[100];
        uint32_t alarmHour = closestEventTime / 3600;
        uint32_t alarmMinute = (closestEventTime % 3600) / 60;
        snprintf(alarmText, sizeof(alarmText), "Alarm set for feeding event %d at %02d:%02d.\n\n\n", closestEvent, alarmHour, alarmMinute);
        putsUart0(alarmText);

        // Display information about the feeding event
        char eventText[100];
        snprintf(eventText, sizeof(eventText), "Feeding event %d details:\n\n", closestEvent);
        putsUart0(eventText);

        snprintf(eventText, sizeof(eventText), "  > Duration: %d minutes\n\n", readEeprom(16 * closestEvent + 1));
        putsUart0(eventText);

        snprintf(eventText, sizeof(eventText), "  > PWM Speed: %d\n\n", readEeprom(16 * closestEvent + 2));
        putsUart0(eventText);

        char timeText[50];
        snprintf(timeText, sizeof(timeText), "  > Scheduled Time: %02d:%02d\n\n", readEeprom(16 * closestEvent + 3), readEeprom(16 * closestEvent + 4));
        putsUart0(timeText);

    }
}

//-----------------------------------------------------------------------------
// Timer 2 ISR for auger
//-----------------------------------------------------------------------------

void Timer2Isr()
{
    // Clear interrupt flag for Timer 2
    TIMER2_ICR_R = TIMER_ICR_TATOCINT;
    // Set PWM for the auger to zero
    PWM1_3_CMPA_R = 0;
    findNextEvent();
    // Disable Timer 2
    TIMER2_CTL_R &= ~TIMER_CTL_TAEN;
}

//-----------------------------------------------------------------------------
// WideTimer 2 ISR for sensor
//-----------------------------------------------------------------------------

void WideTimer2Isr()
{
    WTIMER2_ICR_R = TIMER_ICR_TATOCINT;
    RED_LED=0;
    WTIMER2_ICR_R = TIMER_CTL_TAEN;
}


//-----------------------------------------------------------------------------
// Timer 3 ISR for pump
//-----------------------------------------------------------------------------

void Timer3Isr()
{
    RED_LED = SENSOR;
    TIMER3_ICR_R = TIMER_ICR_TATOCINT;
    PWM0_0_CMPA_R = 0;
    RED_LED=0;
    TIMER3_ICR_R = TIMER_CTL_TAEN;
}

//-----------------------------------------------------------------------------
// Initialize Timer 2 for auger
//-----------------------------------------------------------------------------

void initTimer2()
{
    RED_LED = SENSOR;

    while (!(HIB_CTL_R & HIB_CTL_WRC));
    HIB_IC_R |= HIB_IC_RTCALT0; //clear


    while (!(HIB_CTL_R & HIB_CTL_WRC));
    uint32_t time = HIB_RTCC_R;
    //uint32_t closestEvent = 0;

    //turn on speaker
    //SpeakerOn();
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R2; // Enable Timer 2 clock
    _delay_cycles(3);

    // Configure Timer 2 as a one-shot timer
    TIMER2_CTL_R &=  ~TIMER_CTL_TAEN; // Turn-off Timer 2 before reconfiguring
    TIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER; // 32-bit timer mode
    TIMER2_TAMR_R |= TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR; // one-shot mode
    TIMER2_TAILR_R = 40000000 * readEeprom((16 * event) + 1);
    TIMER2_CTL_R |= TIMER_CTL_TAEN; // Enable Timer 2
    TIMER2_IMR_R = TIMER_IMR_TATOIM;
    NVIC_EN0_R = 1 << (INT_TIMER2A -16);
    PWM1_3_CMPA_R = (uint32_t)((float)(readEeprom(16 * event + 2) * 1023) / 100);

    putsUart0("RTC Configured\n");


}

//-----------------------------------------------------------------------------
// WideTimer 2 ISR for sensor
//-----------------------------------------------------------------------------

void initWideTimer2()
{
    RED_LED = SENSOR;
    // Configure Timer 2 as a one-shot timer
    WTIMER2_CTL_R &= ~TIMER_CTL_TAEN; // Turn-off Timer 2 before reconfiguring
    WTIMER2_CFG_R = TIMER_CFG_32_BIT_TIMER; // 32-bit timer mode
    WTIMER2_TAMR_R |= TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR; // one-shot mode
    WTIMER2_TAILR_R = 40000000 * readEeprom((16 * event) + 1);
    WTIMER2_CTL_R |= TIMER_CTL_TAEN; // Enable Timer 2
    WTIMER2_IMR_R = TIMER_IMR_TATOIM;
    NVIC_EN3_R = 1 << (INT_TIMER2A -16);
    putsUart0("WideTimer3 interrupt \n");

}

//-----------------------------------------------------------------------------
// Initialize Timer 3 ISR for pump
//-----------------------------------------------------------------------------


void initTimer3()
{
    RED_LED = SENSOR;
    // Configure Timer 2 as a one-shot timer
    TIMER3_CTL_R &= ~TIMER_CTL_TAEN; // Turn-off Timer 2 before reconfiguring
    TIMER3_CFG_R = TIMER_CFG_32_BIT_TIMER; // 32-bit timer mode
    TIMER3_TAMR_R |= TIMER_TAMR_TAMR_1_SHOT | TIMER_TAMR_TACDIR; // one-shot mode
    TIMER3_TAILR_R = 40000000 * 2;
    TIMER3_CTL_R |= TIMER_CTL_TAEN; // Enable Timer 2
    TIMER3_IMR_R = TIMER_IMR_TATOIM;
    NVIC_EN1_R = 1 << (INT_TIMER3A -16-32);

    putsUart0("Timer3 interrupt \n");

}

//-----------------------------------------------------------------------------
// comparator
//-----------------------------------------------------------------------------


void comparator0Isr(void)
{
    RED_LED = SENSOR;
    TIMER3_TAV_R =0;
    waitMicrosecond(10);    //delay

    TIMER3_CTL_R |= TIMER_CTL_TAEN; //turn on timer
  //BLOCKING... waiting for analog pin status to be 1
  //while(COMP_ACSTAT0_R == COMP_ACSTAT0_OVAL);
    time = (TIMER3_TAV_R/40); // Timer value, will depend on water level

    waterlevel = (time - b) / slope;

    if (waterlevel < 0)
    {
        waterlevel = 0;
    }
    else if (waterlevel >= 50 && waterlevel < 145)
        waterlevel = 100;
    else if (waterlevel >= 145 && waterlevel < 250)
        waterlevel = 200;
    else if (waterlevel >= 255 && waterlevel < 350)
        waterlevel = 300;
    else if (waterlevel >= 355 && waterlevel < 450)
        waterlevel = 400;
    else if (waterlevel >= 455)
        waterlevel = 500;

    snprintf(str, sizeof(str), "time: %7" PRIu32 " (us) \t waterlevel: %.2f (mL)\n", time, waterlevel);
    putsUart0(str);

    uint32_t waterLevelThreshold = readEeprom(16 * water);
    uint32_t mode = readEeprom(16 * water + 1);

    switch (mode)
    {
    case 0:
        if (waterlevel < waterLevelThreshold)
        {
            RED_LED = SENSOR;
            PWM0_0_CMPA_R = 1023;
            waitMicrosecond(5000000);
            PWM0_0_CMPA_R = 0;
            TIMER3_CTL_R |= TIMER_CTL_TAEN;
        }
        break;

    case 1:
        if (waterlevel < waterLevelThreshold)
        {
            PWM0_0_CMPA_R = 1023;
            TIMER3_CTL_R |= TIMER_CTL_TAEN;
        }
        else
        {
            putsUart0("Water is full\n");
        }
        break;

    // additional modes if needed

    default:
        // Handle unknown mode
        break;
    }

    if(autoflag)   //fill the water for a short period
    {
        PWM0_0_CMPA_R = 1023;
        waitMicrosecond(5000000);
        PWM0_0_CMPA_R = 0;
        snprintf(str, sizeof(str), "Fill water for 5 seconds!");
    }

    TIMER3_CTL_R |= ~TIMER_CTL_TAEN;                 // turn-off timer before reconfiguring
    TIMER3_TAV_R = 0;                               // zero counter for first period


    COMP_ACMIS_R = COMP_ACMIS_IN0; // Clearing interrupt
}



//-----------------------------------------------------------------------------
// turn on hardware
//-----------------------------------------------------------------------------



// turn on speaker
void SpeakerOn(void)
{
    int i = 0;
    for( i = 0; i < 5000; i++)
    {
        SPEAKER = 1;
        waitMicrosecond(186);
        SPEAKER = 0;
        waitMicrosecond(186);
        i++;
    }
}

//-----------------------------------------------------------------------------
// Main   [lab 4/7]
//-----------------------------------------------------------------------------



int main(void)
{
    USER_DATA data;

    //Local Variables
    char str[200];


    // Initialize hardware
    initHw();
    initUart0();
    SpeakerOn();

    // Setup UART0 baud rate
    setUart0BaudRate(115200, 40e6);

    uint32_t hours1 =0;
    uint32_t minutes1 =0;



    //Endless loop that executes the UART0 Interface
    while (true)
    {


        //valid= false;
        bool valid = false;


        //Get user data
        getsUart0(&data);

        //parse fields
        parseFields(&data);
        //echo back from the parsed field

        // Set the next alarm after displaying the time
        findNextEvent();

        //time HH:MM
        if(isCommand(&data,"time",2))
        {
            valid = true;
            hours1  =   getFieldInteger(&data, 1);
            minutes1  = getFieldInteger(&data, 2);

            initRTC(hours1, minutes1);// initialize RTC
        }


        else if (isCommand(&data, "time", 0))
        {
            valid = true;
            while(!(HIB_CTL_R & HIB_CTL_WRC));
            int RTC = HIB_RTCC_R; // initialize rtc

            int32_t HH;
            int32_t MM;
            HH = RTC / 3600;
            MM = (RTC % 3600) / 60;

            // The command should display real-time clock
            snprintf(str, sizeof(str), "\nHours: %2" PRIu32 ",\t Minutes: %2" PRIu32 " \n\n", HH, MM);
            putsUart0(str);

        }

        // feed FEEDING DURATION PWM HH:MM
        else if (isCommand(&data, "event", 5))
        {
            valid = true;

            event = getFieldInteger(&data, 1);
            uint32_t duration = getFieldInteger(&data, 2);
            uint32_t speed = getFieldInteger(&data, 3);
            uint32_t hours = getFieldInteger(&data, 4);
            uint32_t minutes = getFieldInteger(&data, 5);

            writeEeprom((16 * event), event);
            writeEeprom((16 * event) + 1, duration);
            writeEeprom((16 * event) + 2, speed);
            writeEeprom((16 * event) + 3, hours);
            writeEeprom((16 * event) + 4, minutes);

            snprintf(str, sizeof(str), "> Event %d set:\n"
                                       "> Duration: %d minutes\n"
                                       "> PWM: %d\n"
                                       "> Time: %02dh:%02dm\n",
                     readEeprom(16 * event),
                     readEeprom(16 * event + 1),
                     readEeprom(16 * event + 2),
                     readEeprom(16 * event + 3),
                     readEeprom(16 * event + 4));
            putsUart0(str);
        }


        else if (isCommand(&data, "schedule", 0))
        {
            valid = true;
