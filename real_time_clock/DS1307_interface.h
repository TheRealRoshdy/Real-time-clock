

#ifndef DS1307_INTERFACE_H_
#define DS1307_INTERFACE_H_


typedef struct
{
	u8 second;
	u8 min;
	u8 hour;
	u8 APM;
	u8 day;
	u8 month;
	u8 year;
	u8 week_day;
}DS1307_t;



typedef enum
{
	AM,
	PM
}APM_t;

typedef enum
{
	sat,
	sun,
	mon,
	tue,
	wed,
	thu,
	fri
}Weekday_t;


void DS_SetTime(DS1307_t *copy_DSconfig);

void DS_ReadTime(DS1307_t *copy_DSconfig);


#endif /* DS1307_INTERFACE_H_ */
