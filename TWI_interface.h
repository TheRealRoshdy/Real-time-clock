

#ifndef TWI_INTERFACE_H_
#define TWI_INTERFACE_H_

typedef enum{
	NO_error,Start_Error,Re_StartError,Slave_ADD_RD_Err,Slave_ADD_WR_Err
	,Master_WR_Err,Master_RD_Err
}TWI_error;
/* pass 0 */
void TWI_voidMasterInt(u8 copy_u8SlaveAddress);
void TWI_voidSlaveInt(u8 copy_u8SlaveAddress);

TWI_error TWI_SendStartcon(void);
TWI_error TWI_SendRestartcon(void);

TWI_error TWI_SendSlaveAddress_Read(u8 copy_u8SlaveAddress);
TWI_error TWI_SendSlaveAddress_Write(u8 copy_u8SlaveAddress );

TWI_error TWI_MasterWriteData(u8 copy_u8Data);
TWI_error TWI_MasterReadData(u8 *copy_u8Data);

void TWI_voidSendStopCondition(void);

/*********************************************************/
void i2c_start(void);
void i2c__MasterTransmission(void);
TWI_error TWI_MasterReadData_N(u8 *copy_u8Data);




#endif
