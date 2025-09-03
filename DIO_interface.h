/**************************************************/
/**************************************************/
/******** SWC : DIO                  **************/
/******** Date 13 August 2024        **************/
/******** Version : 1.0              **************/
/******** Author : Yousef Ahmad      **************/
/**************************************************/
/**************************************************/

#ifndef DIO_INTERFACE_H_
#define DIO_INTERFACE_H_

#define PORT_u8A       1
#define PORT_u8B       2
#define PORT_u8C       3
#define PORT_u8D       4

#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7


#define PORT_OUT     0XFF
#define PORT_IN      0X00

#define PORT_HIGH     0XFF
#define PORT_LOW      0X00

#define PIN_OUT    1
#define PIN_IN     0

#define PIN_HIGH    1
#define PIN_LOW     0



void DIO_voidSetPortDir(u8 Copy_u8PortNum,u8 Copy_u8PortDir);
void DIO_voidSetPortVal(u8 Copy_u8PortNum,u8 Copy_u8PortVal);

void DIO_voidSetPinDir(u8 Copy_u8PortNum,u8 Copy_u8PinNum, u8 Copy_u8PinDir);
void DIO_voidSetPinVal(u8 Copy_u8PortNum,u8 Copy_u8PinNum, u8 Copy_u8PinVal);

u8 DIO_u8GetPinVal(u8 Copy_u8PortNum,u8 Copy_u8PinNum);






#endif