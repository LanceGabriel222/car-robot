/*
 * File:   main.c
 * Author: hamza
 *
 * Created on June 17, 2022, 10:55 PM
 */
#include <xc.h>
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 1000000
#define Trigger RC1 //Rc1 is Trigger
#define Echo RC0//RC0 is Echo 

int time_taken;
int distance;
void back_off() //used to drive the robot backward
{
      RC3=1; RC4=0; //Motor 1 reverse
      RC5=0; RC6=1; //Motor 2 reverse  
      __delay_ms(500);   
     }

void turn_left() //used to turn the robot
{
      RC3=0; RC4=1; //Motor 1 forward
      RC5=0; RC6=0; //Motor 2 stop  
      __delay_ms(1000);   
}
void turn_right() //used to turn the robot
{
      RC3=0; RC4=0; //Motor 1 stop
      RC5=0; RC6=1; //Motor 2 forward  
      __delay_ms(500);   
}
void stop_robot() //used to stop the robot
{
      RC3=0; RC4=0; //Motor 1 stop
      RC5=0; RC6=0; //Motor 2 stop
      __delay_ms(500);   
}
void robot_forward() //used to stop the robot
{
      RC3=0; RC4=1; //Motor 1 forward
      RC5=1; RC6=0; //Motor 2 forward
      __delay_ms(500);   
}
void servoRotate0() //0 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RC2 = 1;
    __delay_us(800);
    RC2 = 0;
    __delay_us(19200);
  }
}

void servoRotate90() //90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RC2 = 1;
    __delay_us(1500);
    RC2 = 0;
    __delay_us(18500);
  }
}

void servoRotate180() //180 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
 {
    RC2 = 1;
    __delay_us(2200); // delay of 2200us
    RC2 = 0;
    __delay_us(17800); //delay of 17800us
 }
}
void calculate_distance() //function to calculate distance of US

     {
         distance=0;
         TMR1H =0; TMR1L =0; //clear the timer bits 
        Trigger = 1; 
        __delay_us(10);           
        Trigger = 0;  
        while (Echo==0){};
            TMR1ON = 1;
        while (Echo==1){};
            TMR1ON = 0;
        time_taken = (TMR1L | (TMR1H<<8)); 
        distance= (0.0272*time_taken)/2;        
     }

     

void main()

{
    
     TRISC=0x01; //only RC0 is input expect all declared as output
     T1CON=0x20;
     while(1)
     { 
         servoRotate0();    // servo at 0 position
         servoRotate90();   // servo move at 90 degree
        calculate_distance();    //calculate distance      
        if (distance>5)
        {
            robot_forward();  //robot is forward moving
        }
        if (distance<5)
        { 
            stop_robot();     // robot stop
            back_off();      // then robot back
            turn_left();     // then robot turn left
            robot_forward();   // robot forward move
         }
        servoRotate180();         // servo move 180 degree
        calculate_distance();     // calculate distance
        if (distance>5) 
        {
            robot_forward();      //robot forward move
        } 
        if (distance<5)
        { 
            stop_robot();  //robot stop
            back_off();    //robot back
            turn_right();  //robot turn right
            robot_forward(); //robot forward move
         }
     }
}

