#include <stdio.h>
#include "address_map.h"
#include "alarm_handler.h"
#include <unistd.h>

/*Declaration of Pointers*/

volatile int * ptr_PushKey = (int *) PUSHBUTTON_BASE; //Push Button Key
volatile int * ptr_gLED = (int *) GREEN_LED_BASE; //Green LED
volatile int * ptr_HEX3_Display = (int *) HEX3_HEX0_BASE; //HEX3 BASE
volatile int * ptr_sliderKey = (int *) SLIDER_SWITCH_BASE; //Slider Key

/*Function Structure Begins*/

int HEX3to0_display (int value, unsigned char * table);
int capacity_cal();
unsigned int alarm_startSucceed();
void set_parkingRate_type(volatile int* ptr_PushKey, volatile int* ptr_sliderKey, volatile int* ptr_HEX3_Display);
void set_vehicleParkingRate();
void set_parkingLotCapacity(volatile int* ptr_PushKey, volatile int* ptr_sliderKey, volatile int* ptr_HEX3_Display);
void paytment_mode(volatile int* ptr_PushKey, volatile int* ptr_HEX3_Display);
void alarm_stop();
void money_SEDAN();
void money_SUV();
void money_TRUCK();
alt_u32 myalarm_handler(void * context);

unsigned char table[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
static alt_alarm myalarm;

/*Function Structure Ends*/

/****************************************************/

/*Declaration and Initialization of Variables*/

int set_parkingRate = 0;
int set_parkingCapacity =0;
int parkingRate_SEDAN =0;
int parkingRate_SUV = 0;
int parkingRate_TRUCK =0;
int count_SEDAN = 0;
int count_SUV = 0;
int count_TRUCK = 0;
int count_Vehicle = 0;
int count_SEDAN_ED = 0;
int count_SUV_ED =0;
int count_TRUCK_ED = 0;
int parkingLot_capacity = 0;
int depositAmount = 0;
int allow_TRUCK =0;
int allow_SUV = 0;
int allow_SEDAN = 0;

int main(){ //Main Function
	int counter = 0;
	while(counter){
		if(*ptr_sliderKey & 1<<9){
			set_parkingRate_type(ptr_PushKey, ptr_sliderKey, ptr_HEX3_Display);
		}else if(*ptr_sliderKey & 1<<8 && set_parkingRate !=0){
			set_parkingLotCapacity(ptr_PushKey, ptr_sliderKey, ptr_HEX3_Display);
			set_parkingCapacity = 1;
		}else if(set_parkingRate !=0 && set_parkingCapacity == 1){
			paytment_mode(ptr_PushKey, ptr_HEX3_Display);
		}
		usleep(150000);
	}
	return 0;
}

alt_u32 myalarm_handler(void * context){
	depositAmount =0;
    return 30*alt_ticks_per_second();
}

unsigned int alarm_startSucceed(){
    return alt_alarm_start(&myalarm, 30*alt_ticks_per_second(), myalarm_handler, NULL) >= 0;
}

void alarm_stop(){
	return alt_alarm_stop(&myalarm);
}

int HEX3to0_display (int value , unsigned char* table){

	if(value < 10 && value >=0)
		return table[value];
	else if(value < 100 && value >=10)
		return ((table[(value/10)%10] <<  + table[value%10]);
	else if(value >=100)
		return ((table[(value/100)%100] << 16) + (table[(value/10)%10] <<  + table[value%10]);

}

void set_parkingRate_type(volatile int* ptr_PushKey, volatile int* ptr_sliderKey, volatile int* ptr_HEX3_Display){

	int counter = 1;

	while((*ptr_sliderKey & 1<<9 ) == 512){
		if(*ptr_PushKey == 1<<1){
			set_vehicleParkingRate();
			parkingRate_SEDAN = set_parkingRate;
			if(set_parkingRate = 0){
				counter =0;
			}
		}else if(*ptr_PushKey == 1<<2){
			set_vehicleParkingRate();
			parkingRate_SUV = set_parkingRate;
			if(set_parkingRate ==0){
				counter =0;
			}
		}else if(*ptr_PushKey == 1<<3){
			set_vehicleParkingRate();
			parkingRate_TRUCK = set_parkingRate;
			if(set_parkingRate == 0){
				counter =0;
			}
		}
		if((*ptr_sliderKey & 1<<9) == -512){
			set_parkingRate = 1;
			counter =0;
		}
	}//End of while
}//set_parkingRate_type

void set_vehicleParkingRate(){
	int counter =1;

	while(counter){
		int slider_MAX = *ptr_sliderKey & 0xf0;
		int slider_MIN = *ptr_sliderKey & 0x0f;
		int slider_TOTAL = (10 * slider_MAX>> 4) + slider_MIN;

		if(*ptr_PushKey == 1 << 1 && slider_TOTAL <=99 && slider_TOTAL >=0){
			set_parkingRate = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(set_parkingRate, table);
			usleep(150000);
			counter = 0;
		}else if(*ptr_PushKey == 1 << 2 && slider_TOTAL <=99 && slider_TOTAL >=0 ){
			set_parkingRate = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(set_parkingRate, table);
			usleep(150000);
			counter = 0;
		}else if(*ptr_PushKey == 1 << 3 && slider_TOTAL <=99 && slider_TOTAL >=0 ){
			set_parkingRate = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(set_parkingRate, table);
			usleep(150000);
			counter = 0;
		}
	}//End of while
}//End of set_vehicleParkingRate

void set_parkingLotCapacity(volatile int* ptr_PushKey, volatile int* ptr_sliderKey, volatile int* ptr_HEX3_Display){

	while((*ptr_sliderKey & 1<<8) == 256){

		if(*ptr_PushKey == 1 << 1 && parkingRate_SEDAN != 0 ){
			count_SEDAN = capacity_cal();
			*ptr_HEX3_Display = HEX3to0_display(count_SEDAN, table);
			usleep(150000);
		}else if((*ptr_PushKey == 1<< 2) && parkingRate_SUV != 0){
			count_SUV = capacity_cal();
			*ptr_HEX3_Display = HEX3to0_display(count_SUV, table);
			usleep(150000);
		}else if((*ptr_PushKey == 1 << 3) && parkingRate_TRUCK != 0){
			count_TRUCK = capacity_cal();
			*ptr_HEX3_Display = HEX3to0_display(count_TRUCK, table);
			usleep(150000);
		}
		
		if (count_TRUCK_ED != 0 && count_SUV_ED != 0 && count_SEDAN_ED != 0){
			allow_SUV = allow_TRUCK = allow_SEDAN = 1;
			*ptr_HEX3_Display = HEX3to0_display(321, table);
		}
		if (count_SEDAN_ED != 0 && count_TRUCK_ED != 0 && count_TRUCK != 99 && count_SEDAN != 99){
			allow_SEDAN = allow_TRUCK = 1;
			*ptr_HEX3_Display = HEX3to0_display(31, table);
		}
		if (count_TRUCK_ED != 0 && count_SUV_ED != 0 && count_TRUCK != 99 && count_SUV != 99){
			allow_SUV = allow_TRUCK = 1;
			*ptr_HEX3_Display = HEX3to0_display(32, table);
		}
		if (count_SEDAN_ED != 0 && count_SUV_ED != 0 && count_SUV != 99 && count_SEDAN != 99){
			allow_SEDAN = allow_SUV = 1;
			*ptr_HEX3_Display = HEX3to0_display(21, table);
		}
		if((*ptr_sliderKey & 1 <<  == -256){
			printf("\nParking Lot Capacity Mode Done!");
		}
	}// End of while
}// End of set_parkingLotCapacity

int capacity_cal(){
	int counter = 1;

	while(counter){
		int slider_MAX = *ptr_sliderKey & 0xf0;
		int slider_MIN = *ptr_sliderKey & 0x0f;
		int slider_TOTAL = (10 * (slider_MAX >> 4)) + slider_MIN;

		if(*ptr_PushKey == 1<<1 && slider_TOTAL <= 99 && slider_TOTAL >=0){
			parkingLot_capacity = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(parkingLot_capacity, table);
			usleep(150000);
			count_Vehicle = parkingLot_capacity;
			counter = 0;
		}else if(*ptr_PushKey == 1<<2 && slider_TOTAL <= 99 && slider_TOTAL >=0){
			parkingLot_capacity = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(parkingLot_capacity, table);
			usleep(150000);
			count_Vehicle = parkingLot_capacity;
			counter = 0;
		}else if(*ptr_PushKey == 1<<3 && slider_TOTAL <= 99 && slider_TOTAL >=0){
			parkingLot_capacity = slider_TOTAL;
			*ptr_HEX3_Display = HEX3to0_display(parkingLot_capacity, table);
			usleep(150000);
			count_Vehicle = parkingLot_capacity;
			counter = 0;
		}
	}//End of While
	
	return count_Vehicle;
}//End of capcity_cal()

void paytment_mode(volatile int* ptr_PushKey, volatile int* ptr_HEX3_Display){

	if(*ptr_PushKey == 2){
		*ptr_HEX3_Display = HEX3to0_display(count_SEDAN, table);
		usleep(150000);
		money_SEDAN();

	}else if(*ptr_PushKey == 4){
		*ptr_HEX3_Display = HEX3to0_display(count_SUV, table);
		usleep(150000);
		money_SUV();

	}else if(* ptr_PushKey == 8){
		*ptr_HEX3_Display = HEX3to0_display(count_TRUCK, table);
		usleep(150000);
		money_TRUCK();
	}
}//End of Parking Mode

void money_cal(){
	int key = 0;
	while(*ptr_PushKey){
		key = *ptr_PushKey;
		usleep(250000);
	}
	if(key == 1 << 1){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(1, table);
		usleep(150000);
		depositAmount += 1;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 1 << 2){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(2, table);
		usleep(150000);
		depositAmount += 2;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 1 << 3){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(10, table);
		usleep(150000);
		depositAmount += 10;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 6){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(5, table);
		usleep(150000);
		depositAmount += 5;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 10){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(20, table);
		usleep(150000);
		depositAmount += 20;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 12){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(50, table);
		usleep(150000);
		depositAmount += 50;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}else if(key == 14){
		alarm_stop();
		*ptr_HEX3_Display = HEX3to0_display(100, table);
		usleep(150000);
		depositAmount += 100;
		*ptr_HEX3_Display = HEX3to0_display(depositAmount, table);
		usleep(150000);
		alarm_startSucceed();
	}
}

void money_SEDAN(){
	int counter = 1;
	depositAmount = 0;

	if(count_Vehicle != 0 ){
		while(counter){
			money_cal();
			if((*ptr_sliderKey == 0x01) && depositAmount < parkingRate_SEDAN){
				alarm_stop();
				depositAmount = 0;
				*ptr_HEX3_Display = HEX3to0_display(count_SEDAN, table);
				usleep(150000);
				printf("\nMoney Returned");
				*ptr_rLED = 0;
				counter = 0;
			}else if((*ptr_sliderKey == 0x01) && depositAmount >= parkingRate_SEDAN){
				printf("\nChange Returned");
				*ptr_gLED = 128;
				count_SEDAN -= 1;
				printf("\nSedan Left");
				*ptr_HEX3_Display = HEX3to0_display(count_SEDAN, table);
				if(count_SEDAN = 0){
					*ptr_HEX3_Display = 0x4040;
					 counter =0;
				}
				usleep(150000);
				*ptr_gLED =0;
			}
			else if(count_SEDAN == 0){
				*ptr_HEX3_Display = 0x4040;
				counter =0;
			}
		}//End of while
	}//End if
	
	else{
		printf("\nParking Full");
		*ptr_HEX3_Display = 0x4040;
		counter =0;
	}
}//End of money sedan

void money_SUV(){
	int counter = 1;
	depositAmount = 0;
	if(count_Vehicle != 0){
		while(counter){
			money_cal();
			if((*ptr_sliderKey == 0x01) && depositAmount >= parkingRate_SUV){
				count_SUV -= 1;
				*ptr_HEX3_Display = HEX3to0_display(depositAmount - parkingRate_SUV, table);
				usleep(150000);
				*ptr_gLED =0;
			}else if((*ptr_sliderKey == 0x01) && depositAmount < parkingRate_SUV){
				alarm_stop();
				depositAmount = 0;
				*ptr_HEX3_Display = HEX3to0_display(count_SUV, table);
				usleep(150000);
				*ptr_rLED = 0;
				counter = 0;
			}else if(count_SUV == 0){
				*ptr_HEX3_Display = 0x4040;
				counter =0;
			}
		}//End of while
	}//End if
	
	else{
		printf("\nParking Full");
		*ptr_HEX3_Display = 0x4040;
		counter =0;
	}
}//End of money SUV

void money_TRUCK(){
	int counter = 1;
	depositAmount = 0;
	if(count_Vehicle != 0){
		while(counter){
			money_cal();
			if((*ptr_sliderKey == 0x01) && depositAmount >= parkingRate_TRUCK){
				count_TRUCK -= 1;
				*ptr_HEX3_Display = HEX3to0_display(depositAmount - parkingRate_TRUCK, table);
				usleep(150000);
				*ptr_gLED =0;
			}else if((*ptr_sliderKey == 0x01) && depositAmount < parkingRate_TRUCK){
				alarm_stop();
				depositAmount = 0;
				*ptr_HEX3_Display = HEX3to0_display(count_TRUCK, table);
				usleep(150000);
				*ptr_rLED = 0;
				counter = 0;
			}else if(count_TRUCK == 0){
				*ptr_HEX3_Display = 0x4040;
				counter =0;
			}
		}
	}
	else{
		*ptr_HEX3_Display = 0x4040;
		counter =0;
	}
}
