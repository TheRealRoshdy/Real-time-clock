
#ifndef TWI_register_H_
#define TWI_register_H_

#define TWDR			*((volatile u8*) 0x23)	/*TWI Data Register*/
	
#define TWAR			*((volatile u8*)0x22)	/*TWI (Slave) Address Register*/

#define TWSR			*((volatile u8*)0x21)	/*TWI Status Register*/
#define TWSR_TWPS1		1	/*TWI Prescaler Bit one*/
#define TWSR_TWPS0		0	/*TWI Prescaler Bit zero*/

#define TWCR			*((volatile u8*)0x56)/*TWI control register*/
#define TWCR_TWINT		7	/*interrupt flag*/
#define TWCR_TWEA		6	/*Acknowledge Bit*/
#define TWCR_TWSTA		5	/*START Condition Bit*/
#define TWCR_TWSTO		4	/*STOP Condition Bit*/
#define TWCR_TWWC		3	/*Write Collision Flag*/
#define TWCR_TWEN		2	/*TWI Enable Bit*/
#define TWCR_TWIE		0	/*TWI Interrupt Enable*/

#define TWBR			*((volatile u8*)0x20)	/*TWBR selects the division factor for the bit rate generator*/
#endif
