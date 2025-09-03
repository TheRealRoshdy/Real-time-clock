#ifndef CLCD_INTERFACE_H_
#define CLCD_INTERFACE_H_

#define LINE0   0
#define LINE1   1

void CLCD_voidInit(void);
void CLCD_voidSendString(const char *pcString);
void CLCD_voidSetPosition(u8 Copy_u8x,u8 Copy_u8y);
void CLCD_voidClrDisplay(void);
void CLCD_voidSendNum(u16 Copy_u16Num);








#endif
