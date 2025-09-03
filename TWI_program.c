

#include "STD_types.h"
#include "BIT_math.h"


#include "TWI_config.h"
#include "TWI_interface.h"
#include "TWI_private.h"
#include "TWI_regisger.h"

void TWI_voidMasterInt(u8 copy_u8SlaveAddress)
{
	/* SET CLOCK fre to 400kb/s*/
	TWBR=32;
	TWSR=0;
	//CLR_BIT(TWSR,TWSR_TWPS0);
	//CLR_BIT(TWSR,TWSR_TWPS1);
	/*set address for master if it was recive*/

	TWAR=2;

	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);


}
void TWI_voidSlaveInt(u8 copy_u8SlaveAddress)
{
	/*INti address */
	TWAR= (copy_u8SlaveAddress<<1);

	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);

}

TWI_error TWI_SendStartcon(void)
{
	TWI_error local_error=NO_error;
	/* send start condition on the bus*/
	SET_BIT(TWCR,TWCR_TWSTA);

	/* clear interrupt flag of to start operation*/
	SET_BIT(TWCR,TWCR_TWINT);
	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);
	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=Start_AC)
	{
		local_error=Start_Error;
	}

	return local_error;

}
TWI_error TWI_SendRestartcon(void)
{
	TWI_error local_error=NO_error;
	/* send start condition on the bus*/
	SET_BIT(TWCR,TWCR_TWSTA);
	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);
	/* clear interrupt flag of to start operation*/
	SET_BIT(TWCR,TWCR_TWINT);
	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=RE_Start_AC)
	{
		local_error=Re_StartError;
	}

	return local_error;

}

TWI_error TWI_SendSlaveAddress_Read(u8 copy_u8SlaveAddress)
{
	TWI_error local_error=NO_error;
	/*clear Start condition*/
	//CLR_BIT(TWCR,TWCR_TWSTA);

	/*set slave Address on the 7 most bit*/
	TWDR=((copy_u8SlaveAddress<<1)|1);
	/*SET for Read request*/

	/* clear interrupt flag of to start operation*/
	TWCR=(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	/*enable TWI*/

	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=Salve_Add_RD_AC)
	{
		local_error=Slave_ADD_RD_Err;
	}

	return local_error;

}
TWI_error TWI_SendSlaveAddress_Write(u8 copy_u8SlaveAddress )
{
	TWI_error local_error=NO_error;

	TWDR=((copy_u8SlaveAddress<<1) & 0xFE);
	/*enable TWI*/
	TWCR=(1<<TWCR_TWINT)|(1<<TWCR_TWEN);
	//SET_BIT(TWCR,TWCR_TWEN);

	/*set slave Address on the 7 most bit*/


	/* clear interrupt flag of to start operation*/
	//SET_BIT(TWCR,TWCR_TWINT);
	/* wait until the falg is rised*/
	while(!(GET_BIT(TWCR,TWCR_TWINT)));
	if(( TWSR & 0xf8 ) != Salve_Add_WR_AC)
	{
		local_error=Slave_ADD_WR_Err;
	}

	return local_error;

}

TWI_error TWI_MasterWriteData(u8 copy_u8Data)
{
	TWI_error local_error=NO_error;
	/* write the data in */
	TWDR=copy_u8Data;

	/* clear interrupt flag of to start operation*/
	TWCR = (1<<TWCR_TWINT)|(1<<TWCR_TWEN);

	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=Master_WR_AC)
	{
		local_error=Master_WR_Err;
	}
	return local_error;

}
TWI_error TWI_MasterReadData(u8 *copy_u8Data)
{
	TWI_error local_error=NO_error;
	/*enable master to generate AC*/
	SET_BIT(TWCR,TWCR_TWEA);
	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);
	/* clear interrupt flag of to start operation*/
	SET_BIT(TWCR,TWCR_TWINT);
	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=Master_RD_AC)
	{
		local_error=Master_RD_Err;
	}
	else{
		/* Read the Recived data*/
		*copy_u8Data=TWDR;
	}


	return local_error;

}

void TWI_voidSendStopCondition(void)
{

	/*generate  stop condition on the path */
	//SET_BIT(TWCR,TWCR_TWSTO);
	/* clear interrupt flag of to start operation*/
	//SET_BIT(TWCR,TWCR_TWINT);

	TWCR=(1<<TWCR_TWINT)|(1<<TWCR_TWSTO)|(1<<TWCR_TWEN);


}

TWI_error TWI_MasterReadData_N(u8 *copy_u8Data)
{
	TWI_error local_error=NO_error;
	/*Disable master to generate AC*/
	CLR_BIT(TWCR,TWCR_TWEA);
	/*enable TWI*/
	SET_BIT(TWCR,TWCR_TWEN);
	/* clear interrupt flag of to start operation*/
	SET_BIT(TWCR,TWCR_TWINT);
	/* wait until the falg is rised*/
	while((GET_BIT(TWCR,TWCR_TWINT))==0);
	if((TWSR&0xf8)!=Master_RD_NAC)
	{
		local_error=Master_RD_Err;
	}
	else{
		/* Read the Recived data*/
		*copy_u8Data=TWDR;
	}


	return local_error;

}
