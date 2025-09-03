#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "KPD_interface.h"
#include "KPD_config.h"
#include "KPD_private.h"
#include <util/delay.h>


void KPD_voidInit()
{
	//initial state of pins -> cols output high , rows input pulled up

	DIO_voidSetPinDir(KPD_PORT,C0,PIN_OUT);
	DIO_voidSetPinDir(KPD_PORT,C1,PIN_OUT);
	DIO_voidSetPinDir(KPD_PORT,C2,PIN_OUT);
	DIO_voidSetPinDir(KPD_PORT,C3,PIN_OUT);

	DIO_voidSetPinVal(KPD_PORT,C0,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,C1,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,C2,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,C3,PIN_HIGH);

	DIO_voidSetPinDir(KPD_PORT,R0,PIN_IN);
	DIO_voidSetPinDir(KPD_PORT,R1,PIN_IN);
	DIO_voidSetPinDir(KPD_PORT,R2,PIN_IN);
	DIO_voidSetPinDir(KPD_PORT,R3,PIN_IN);

	DIO_voidSetPinVal(KPD_PORT,R0,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,R1,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,R2,PIN_HIGH);
	DIO_voidSetPinVal(KPD_PORT,R3,PIN_HIGH);

}



u8 KPD_u8GetPressedKey()
{
	u8 Local_u8Key=NO_PRESSED_KEY;
	u8 Local_u8PinState;
	u8 Local_u8ColIdx=0;
	u8 Local_u8RowIdx=0;

	//required 2d array
	static u8 KPD_ARR[MAX_ROW_NUM][MAX_COL_NUM]=KPD_VAL;
	static u8 Local_ColArr[MAX_COL_NUM]={C0,C1,C2,C3};
	static u8 Local_RowArr[MAX_ROW_NUM]={R0,R1,R2,R3};
	//for loop -> outer ->activation col

	for(Local_u8ColIdx=0;Local_u8ColIdx<MAX_COL_NUM;Local_u8ColIdx++)
	{
		//Activate col -> read 4 rows then deactivation
		DIO_voidSetPinVal(KPD_PORT,Local_ColArr[Local_u8ColIdx],PIN_LOW);
		//For  reading rows
		for(Local_u8RowIdx=0;Local_u8RowIdx<MAX_ROW_NUM;Local_u8RowIdx++)
		{
			Local_u8PinState=DIO_u8GetPinVal(KPD_PORT,Local_RowArr[Local_u8RowIdx]);
			if(Local_u8PinState==PIN_LOW)
			{
				_delay_ms(250);
				if(Local_u8PinState==PIN_LOW)
				{
					Local_u8Key=KPD_ARR[Local_u8RowIdx][Local_u8ColIdx];
				}
				//polling
				while(Local_u8PinState==PIN_LOW)
				{
					Local_u8PinState=DIO_u8GetPinVal(KPD_PORT,Local_RowArr[Local_u8RowIdx]);
				}
			}


		}
		DIO_voidSetPinVal(KPD_PORT,Local_ColArr[Local_u8ColIdx],PIN_HIGH);
	}



	return Local_u8Key;
}
