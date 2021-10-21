#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "CAN_Rx.pio.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/mutex.h"




#define CAN_BAUD_Bit_Per_Second 125000          // baudrate
#define CAN_PIO_RX_PIN 21                       // PIN for the Reception
#define CAN_PIO_TX_PIN 22                       // PIN for the Transmission
#define GPIO_CALL_BACK__Input_PIN 14            // PIN for Taster
#define GPIO_LED_PIN  15                        // PIN for LED 

#define Send_Normal_Frame 1                    // if this one pushed into the Second Core a Normal FRame will be sent
#define Send_Ack          2                    // if this one then a ACK will be sent


static uint8_t data[128];
static int achieveCRCDel = 0;
uint8_t dst[128] ;


//int sizing = 0;


//static mutex_t mtx;







struct CAN_Type_Def {               //struct that contain all the CAN Element
    
    uint16_t Message_Identifier;  // Message Identifier
    uint8_t  DLC;                 // Number of Byte
    uint8_t RxData[8];            // Received Data
    uint16_t CRC;               

};

typedef struct CAN_Type_Def CAN_Type_Def;


CAN_Type_Def txhcan;

CAN_Type_Def hcanStr;


CAN_Type_Def RX_CAN_Table[100];   //Table that contain all received CAN-structur




void Frame_Decoder(uint32_t frame); 
void IntToBinary (uint64_t  a , char *b);
int CAN_RxData_Set_Frame(uint8_t Frame , int *achieveCrcDel , CAN_Type_Def *hcan);
void CAN_Paramater_Initialise(int *ENDOfCRC  , CAN_Type_Def *hcan);
uint16_t CRCCalculator( CAN_Type_Def *hcan);
uint64_t createMask(char  a, char b);

int CAN_Send_Message(CAN_Type_Def *TxCan , PIO pio , uint sm);
void SetupThe32_bitFrame(uint32_t *frame);
void CAN_Rx_program_getc(PIO pio, uint sm , int *LastReceived );




