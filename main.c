/*
 * main.c
 *
 *  Created on: Nov 8, 2024
 *      Author: HP
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "TWI_interface.h"
#include "CLCD_interface.h"
#include "DS1307_interface.h"

#include <util/delay.h>

u8 *days[7]={"Sat","Sun","Mon","Tue","Wen","Thu","Fri"};



u8 concatenate_numbers(u8 num1, u8 num2) {
	// Check if num2 is zero to ensure it is still concatenated
	if (num2 == 0) {
		num1 *= 10;  // Shift num1 by one decimal place and append zero
	} else {
		u8 temp = num2;
		while (temp > 0) {
			num1 *= 10;  // Shift num1 by one decimal place
			temp /= 10;  // Reduce temp to find the number of digits in num2
		}
	}
	return num1 + num2;  // Combine num1 and num2
}


typedef enum{
	SET_TIME=11,
	SET_ALARM,
	STOPWATCH,
	DOWNCOUNTER

}status;

typedef enum{
	FALSE,
	TRUE
}CASES;


//roshdy

volatile u8 sec_units = 0;
volatile u8 sec_tens = 0;
volatile u8 min_units = 0;
volatile u8 min_tens = 0;
volatile u8 hours_units = 0;
volatile u8 hours_tens = 0;
volatile u8 running = 0;

void Update_Display(void)
{
	CLCD_voidSetPosition(1, 0);
	CLCD_voidSendNum(hours_tens);
	CLCD_voidSetPosition(1, 1);
	CLCD_voidSendNum(hours_units);
	CLCD_voidSetPosition(1, 3);
	CLCD_voidSendNum(min_tens);
	CLCD_voidSetPosition(1, 4);
	CLCD_voidSendNum(min_units);
	CLCD_voidSetPosition(1, 6);
	CLCD_voidSendNum(sec_tens);
	CLCD_voidSetPosition(1, 7);
	CLCD_voidSendNum(sec_units);
}

void Update_Time(void)
{
	sec_units++;
	if (sec_units > 9)
	{
		sec_units = 0;
		sec_tens++;
	}
	if (sec_tens > 5)
	{
		sec_tens = 0;
		min_units++;
	}
	if (min_units > 9)
	{
		min_units = 0;
		min_tens++;
	}
	if (min_tens > 5)
	{
		min_tens = 0;
		hours_units++;
	}
	if (hours_units > 9)
	{
		hours_units = 0;
		hours_tens++;
	}
}



//ali fekry

// Function to capture a two-digit number from the keypad
u8 getTwoDigitInput()
{
	u8 firstDigit,secondDigit;
	// Wait for the first digit
	firstDigit=KPD_u8GetPressedKey();
	while (firstDigit==255)
	{
		firstDigit=KPD_u8GetPressedKey();
	}
	CLCD_voidSendNum(firstDigit);
	// Wait for the second digit
	secondDigit=KPD_u8GetPressedKey();
	while (secondDigit==255)
	{
		secondDigit=KPD_u8GetPressedKey();
	}
	CLCD_voidSendNum(secondDigit);
	// Combine the two digits to form a two-digit number
	return firstDigit*10+secondDigit;
}
// Function to display the current time on the LCD
void displayTime(u8 hours,u8 minutes,u8 seconds)
{
	CLCD_voidSetPosition(1,0);
	CLCD_voidSendNum(hours/10);    // Display tens place of hours
	CLCD_voidSendNum(hours%10);    // Display units place of hours
	CLCD_voidSetPosition(1,3);
	CLCD_voidSendNum(minutes/10);  // Display tens place of minutes
	CLCD_voidSendNum(minutes%10);  // Display units place of minutes
	CLCD_voidSetPosition(1,6);
	CLCD_voidSendNum(seconds/10);  // Display tens place of seconds
	CLCD_voidSendNum(seconds%10);  // Display units place of seconds
}

void main(void)
{
	u8 control,day1,day2,month1,month2,year1,year2,hrs1,hrs2,mins1,mins2,sec1,sec2;
	CASES TimeIsNotSet=TRUE;
	CASES AlarmIsNotSet=TRUE;
	DIO_voidSetPortDir(PORT_u8B,PORT_OUT);
	// Declare any required variables
	volatile u8 running = 0;
	u8 seconds = 0, minutes = 0, hours = 0;

	DIO_voidSetPinDir(PORT_u8C,PIN3,PIN_OUT);
	DIO_voidSetPinDir(PORT_u8C,PIN5,PIN_OUT);
	DIO_voidSetPinDir(PORT_u8C,PIN6,PIN_OUT);
	DIO_voidSetPinDir(PORT_u8C,PIN7,PIN_OUT);
	CLCD_voidInit();
	KPD_voidInit();
	TWI_voidMasterInt(0);
	DS1307_t current_time;
	DS1307_t alarm;



	while(1)
	{

		control=KPD_u8GetPressedKey();



		DS_ReadTime(&current_time);
		CLCD_voidSetPosition(LINE0,0);
		CLCD_voidSendNum(current_time.hour%12);
		CLCD_voidSendString(":");
		CLCD_voidSendNum(current_time.min);
		CLCD_voidSendString(":");
		CLCD_voidSendNum(current_time.second);

		if(current_time.second>59)
		{
			CLCD_voidSetPosition(LINE0,5);
			CLCD_voidSendString(" ");
		}

		CLCD_voidSetPosition(LINE0,8);
		if(current_time.APM)
		{
			CLCD_voidSendString("  PM");
		}
		else
		{
			CLCD_voidSendString("  AM");
		}
		CLCD_voidSetPosition(LINE1,0);
		CLCD_voidSendNum(current_time.day);
		CLCD_voidSendString("/");
		CLCD_voidSendNum(current_time.month);
		CLCD_voidSendString("/20");
		CLCD_voidSendNum(current_time.year);
		CLCD_voidSendString(" ");
		CLCD_voidSendString((char *)days[current_time.week_day-1]);
		_delay_ms(100);
		//if(alarm.min==current_time.min&&alarm.hour==current_time.hour&&alarm.APM==current_time.APM)
		if (alarm.hour == (current_time.hour % 12) && alarm.min == current_time.min && alarm.APM == current_time.APM)

		{
			DIO_voidSetPinVal(PORT_u8C,PIN3,PIN_HIGH);
			CLCD_voidClrDisplay();
			CLCD_voidSendString("ALARM IS ON!");
			_delay_ms(750);
			CLCD_voidClrDisplay();

			if(control==100)
			{
				DIO_voidSetPinVal(PORT_u8C,PIN3,PIN_LOW);
				CLCD_voidClrDisplay();
				alarm.min=255;

			}

		}
		if(control==SET_TIME)
		{


			CLCD_voidClrDisplay();
			while(TimeIsNotSet)
			{
				CLCD_voidSendString("SET DATE:");
				_delay_ms(500);

				CLCD_voidClrDisplay();
				CLCD_voidSendString("DD/MM/YY");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString(">");


				//days
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();


				}
				day1=control;
				CLCD_voidSendNum(day1);
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();


				}
				day2=control;
				CLCD_voidSendNum(day2);


				current_time.day=concatenate_numbers(day1,day2);


				CLCD_voidSendString("/");

				//month
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				month1=control;
				CLCD_voidSendNum(month1);

				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				month2=control;
				CLCD_voidSendNum(month2);



				current_time.month=concatenate_numbers(month1,month2);


				CLCD_voidSendString("/");




				//year
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				year1=control;
				CLCD_voidSendNum(year1);

				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				year2=control;
				CLCD_voidSendNum(year2);
				current_time.year=concatenate_numbers(year1,year2);
				_delay_ms(100);

				CLCD_voidClrDisplay();
				CLCD_voidSendString("ENTER THE WEEK");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString("DAY");
				_delay_ms(100);
				CLCD_voidClrDisplay();
				CLCD_voidSendString("1-S 2-S 3-M 4-T");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString("5-W 6-T 7-F");
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				current_time.week_day=control;
				if(current_time.day>30||current_time.month>12)
				{
					TimeIsNotSet=TRUE;
					CLCD_voidClrDisplay();
					CLCD_voidSendString("INVALID DATE");
					_delay_ms(500);
					CLCD_voidClrDisplay();
				}
				else
				{
					TimeIsNotSet=FALSE;
				}
			}


			CLCD_voidClrDisplay();
			TimeIsNotSet=TRUE;
			while(TimeIsNotSet)
			{
				CLCD_voidSendString("SET TIME:");
				_delay_ms(500);
				CLCD_voidClrDisplay();



				//hours
				CLCD_voidSendString("HRS:MIN:SEC");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString(">");
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				hrs1=control;
				CLCD_voidSendNum(hrs1);
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				hrs2=control;
				CLCD_voidSendNum(hrs2);
				current_time.hour=concatenate_numbers(hrs1,hrs2);

				CLCD_voidSendString(":");



				//mins
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				mins1=control;
				CLCD_voidSendNum(mins1);
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				mins2=control;
				CLCD_voidSendNum(mins2);

				current_time.min=concatenate_numbers(mins1,mins2);

				CLCD_voidSendString(":");



				//secs
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				sec1=control;
				CLCD_voidSendNum(sec1);

				control=255;

				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				sec2=control;
				CLCD_voidSendNum(sec2);
				current_time.second=concatenate_numbers(sec1,sec2);


				CLCD_voidClrDisplay();

				CLCD_voidSendString("ENTER 0-AM");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString("OR 1-PM");
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				current_time.APM=control;
				if(current_time.hour>12||current_time.min>59||current_time.second>59||current_time.APM>1)
				{
					TimeIsNotSet=TRUE;
					CLCD_voidClrDisplay();
					CLCD_voidSendString("INVALID TIME");
					_delay_ms(500);
					CLCD_voidClrDisplay();
				}

				else
				{
					TimeIsNotSet=FALSE;
					CLCD_voidClrDisplay();
				}



			}


			DS_SetTimee2(&current_time);

		}


		else if(control==SET_ALARM)
		{
			control=255;
			AlarmIsNotSet=TRUE;
			while(AlarmIsNotSet==TRUE)
			{
				CLCD_voidClrDisplay();
				CLCD_voidSendString("SET ALARM:");
				_delay_ms(500);
				CLCD_voidClrDisplay();
				CLCD_voidSendString("HRS:MIN");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString(">");
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				hrs1=control;
				CLCD_voidSendNum(hrs1);
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				hrs2=control;
				CLCD_voidSendNum(hrs2);
				alarm.hour=concatenate_numbers(hrs1,hrs2);

				CLCD_voidSendString(":");



				//mins
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				mins1=control;
				CLCD_voidSendNum(mins1);
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				mins2=control;
				CLCD_voidSendNum(mins2);
				alarm.min=concatenate_numbers(mins1,mins2);
				CLCD_voidClrDisplay();
				CLCD_voidSendString("ENTER 0-AM");
				CLCD_voidSetPosition(LINE1,0);
				CLCD_voidSendString("OR 1-PM");
				control=255;
				while(control==255)
				{
					control=KPD_u8GetPressedKey();

				}
				alarm.APM=control;


				if(alarm.hour>12||alarm.min>59||alarm.APM>1)
				{
					AlarmIsNotSet=TRUE;
					CLCD_voidClrDisplay();
					CLCD_voidSendString("INVALID ALARM");
					_delay_ms(500);
					CLCD_voidClrDisplay();
				}

				else
				{
					AlarmIsNotSet=FALSE;
					CLCD_voidClrDisplay();
				}
			}



		}

		else if(control==STOPWATCH)
		{
			running = 1; // Set the running flag to true to indicate that the stopwatch is active
			CLCD_voidClrDisplay();
			CLCD_voidSendString("STOPWATCH:");
			CLCD_voidSetPosition(LINE1,0);
			CLCD_voidSendString("00:00:00");

			// Enter a loop to keep the stopwatch running
			while (running) {
				// Update the display with current time
				Update_Display();

				// Simulate a 1-second delay
				_delay_ms(1000);

				// Increment the time
				Update_Time();

				// Check for user input to stop or reset the stopwatch
				u8 pressedKey = KPD_u8GetPressedKey();
				if (pressedKey == 100)
				{ // Replace STOP_KEY with the actual key to stop
					running = 0; // Exit the stopwatch mode
				}
				else if (pressedKey == 0) { // Replace RESET_KEY with the actual key to reset

					sec_units = 0;
					sec_tens = 0;
					min_units = 0;
					min_tens = 0;
					hours_units = 0;
					hours_tens = 0;
					Update_Display(); // Immediately show reset state
				}

			}
			CLCD_voidClrDisplay();
		}
		else if(control==DOWNCOUNTER)
		{
			u8 key;
			// Display initial prompt
			CLCD_voidClrDisplay();
			CLCD_voidSendString("hrs:mins:secs");
			CLCD_voidSetPosition(1,0);
			CLCD_voidSendString("00:00:00");
			// Get hours, minutes, and seconds input from the user
			CLCD_voidSetPosition(1,0);
			hours=getTwoDigitInput();
			CLCD_voidSetPosition(1,3);
			minutes=getTwoDigitInput();
			CLCD_voidSetPosition(1,6);
			seconds=getTwoDigitInput();
			// Display the initial time input by the user
			displayTime(hours,minutes,seconds);
			// Wait for a specific key to start the countdown (e.g., key 150)
			do
			{
				key = KPD_u8GetPressedKey();
			} while (key != 150);
			// Countdown loop
			while (hours>0 || minutes>0 || seconds>0)
			{
				// Display the current time
				displayTime(hours,minutes,seconds);
				_delay_ms(1000); // Delay for 1 second to simulate real-time countdown
				// Decrement the time
				if (seconds==0)
				{
					if (minutes==0)
					{
						if (hours>0)
						{
							hours--;
							minutes=59;
							seconds=59;
						}
					}
					else
					{
						minutes--;
						seconds=59;
					}
				}
				else
				{
					seconds--;
				}
			}

			// Display final state when countdown reaches 0
			displayTime(0,0,0);
			DIO_voidSetPinVal(PORT_u8C,PIN3,PIN_HIGH);
			_delay_ms(5000);
			DIO_voidSetPinVal(PORT_u8C,PIN3,PIN_LOW);
			CLCD_voidClrDisplay();

		}

	}

}


