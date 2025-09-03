#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "CLCD_interface.h"
#include "CLCD_config.h"
#include "CLCD_private.h"
#include <util/delay.h>




static void CLCD_voidSendCommand(u8 Copy_u8Command)
{
	//set rs ,rw -> 0
	DIO_voidSetPinVal(CTRL_PORT,RS,PIN_LOW);
	DIO_voidSetPinVal(CTRL_PORT,RW,PIN_LOW);
	//set command to data port
	DIO_voidSetPortVal(DATA_PORT,Copy_u8Command);
	//Set Enable pulse
	DIO_voidSetPinVal(CTRL_PORT,EN,PIN_HIGH);
	_delay_ms(1);
	DIO_voidSetPinVal(CTRL_PORT,EN,PIN_LOW);
}
static void CLCD_voidSendData(u8 Copy_u8Data)
{
	//set rs=1 ,rw -> 0
	DIO_voidSetPinVal(CTRL_PORT,RS,PIN_HIGH);
	DIO_voidSetPinVal(CTRL_PORT,RW,PIN_LOW);
	//set command to data port
	DIO_voidSetPortVal(DATA_PORT,Copy_u8Data);
	//Set Enable pulse
	DIO_voidSetPinVal(CTRL_PORT,EN,PIN_HIGH);
	_delay_ms(1);
	DIO_voidSetPinVal(CTRL_PORT,EN,PIN_LOW);
}

void CLCD_voidInit(void)
{
	_delay_ms(40);
	CLCD_voidSendCommand(0b00111000);
	_delay_ms(1);
	CLCD_voidSendCommand(0b00001100);
	_delay_ms(1);
	CLCD_voidSendCommand(1);
	_delay_ms(2);
}
void CLCD_voidSendString(const char *pcString)
{
	u8 Local_u8Counter=0;
	while(pcString[Local_u8Counter] != '\0')
	{
		CLCD_voidSendData(pcString[Local_u8Counter]);
		Local_u8Counter++;
	}
}

void CLCD_voidSetPosition(u8 Copy_u8x,u8 Copy_u8y)
{
	u8 Local_u8Position;
	switch(Copy_u8x)
	{
	case LINE0:Local_u8Position=Copy_u8y;break;
	case LINE1:Local_u8Position=Copy_u8y+LINE1_BASE;break;
	}
	CLCD_voidSendCommand(Local_u8Position+MSB);
}

void CLCD_voidClrDisplay(void)
{
	CLCD_voidSendCommand(1);
}
void CLCD_voidSendSpecialCharecter(u8 *pu_u8arr,
		u8 Copy_u8BlockNum,
		u8 Copy_u8x,
		u8 Copy_u8y)
{
	u8 Local_u8CgramAddress=Copy_u8BlockNum*8;
	CLCD_voidSendCommand(Local_u8CgramAddress+64);
	for(u8 Local_u8Counter=0;Local_u8Counter<8;Local_u8Counter++)
	{
		CLCD_voidSendData(pu_u8arr[Local_u8Counter]);
	}
	CLCD_voidSetPosition(Copy_u8x,Copy_u8y);
	CLCD_voidSendData(Copy_u8BlockNum);
}

void CLCD_voidSendNum(u16 Copy_u16Num)
{
	u8 arr[10],i=0,j;

	if(Copy_u16Num==0)
	{
		CLCD_voidSendData('0');
	}
	else
	{
		while(Copy_u16Num)
		{
			arr[i]=	Copy_u16Num%10 +'0';
			Copy_u16Num/=10;
			i++;
		}
	}
	for(j=i;j>0;j--)
	{
		CLCD_voidSendData(arr[j-1]);
	}
}
