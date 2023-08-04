/* 
 *  ======== i2ctmp.c ========
 * Author:  Elliot Boesch
 * Date:    04-14-2022
 * Team:    Adaptive Irrigation System
 * Purpose: Using TI LAUNCHXL-CC1310 to read moisture and temperature data from 
 *          the Adafruit Stemma Soil Moisture Sensor over I2C.
 */

#include "Board.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/display/Display.h>

#define TASKSTACKSIZE       640

/*
 *  ======== Adafruit Registers ========
 */
#define TOUCH_REG_1         0x0F /* Seesaw Touch Base Register */
#define TOUCH_REG_2         0x10 /* Seesaw Touch Channel Offset Register */
#define TEMP_REG_1          0x00 /* Seesaw Status Base Register */
#define TEMP_REG_2          0x04 /* Seesaw Status Temp Register */

#define ADA_ADDR             0x36; /* I2C Address of the Adafruit Sensor */

static Display_Handle display;

/*
 *  ======== Main ========
 */
void *mainThread(void *arg0){

    uint16_t        sample;                     /* Variable for loop */
    uint16_t        moisture;                   /* Moisture value */
    uint16_t        temperature;                /* Temperature value */

    uint8_t         txBuffer[2];                /* Write Buffer */
    uint8_t         rxBuffer[4];                 /* Read Buffer */

    /* Setting up I2C */
    I2C_Handle      i2c;
    I2C_Params      i2cParams;
    I2C_Transaction i2cTransaction;

    /* Call driver init functions */
    Display_init();
    GPIO_init();
    I2C_init();

    /* Configure the LED */
    GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Open the HOST display for output */
    display = Display_open(Display_Type_UART, NULL);
    if (display == NULL) {
        while (1);
    }

    /* Turn on user LED */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);

    /* Create I2C for usage and display error if not initialized */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_100kHz; /* Can be I2C_400kHz or I2C_100kHz */
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);
    if (i2c == NULL) {
        Display_printf(display, 0, 0, "Error Initializing I2C\n");
        while (1);
    }

    /* I2C Transaction Setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount  = 2;

    /* Setting write buffers to correct registers */
    txBuffer[0] = TOUCH_REG_1;
    txBuffer[1] = TOUCH_REG_2;
    i2cTransaction.slaveAddress = ADA_ADDR;

    /* Take 20 samples and print them out over UART */
    for (sample = 0; sample < 20; sample++) {
        if(I2C_transfer(i2c, &i2cTransaction)){

            /* Collects the moisture value */
            moisture = ((uint16_t)rxBuffer[0] << 8) | rxBuffer[1];

            /* Prints the moisture and temperature values to the console */
            Display_printf(display, 0, 0, "Sample %u: Moisture: %d",
                sample, moisture);
        }

        else{
            Display_printf(display, 0, 0, "I2C Bus T fault.");
        }

        /* Sleep for 1 second */
        sleep(1);
    }

    /* I2C Transaction Setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 2;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount  = 4;

    /* Setting write buffers to correct registers */
    txBuffer[0] = TEMP_REG_1;
    txBuffer[1] = TEMP_REG_2;
    i2cTransaction.slaveAddress = ADA_ADDR;

    for (sample = 0; sample < 20; sample++) {
        if(I2C_transfer(i2c, &i2cTransaction)){

            /* Collects the temperature value */
            int32_t ret = ((uint32_t)rxBuffer[0] << 24) | ((uint32_t)rxBuffer[1] << 16) |
                            ((uint32_t)rxBuffer[2] << 8) | (uint32_t)rxBuffer[3];
            temperature =  (1.0 / (1UL << 16)) * ret;
            temperature = ((temperature * 1.8) + 32); /* Convert to Farenheit */

            /* Prints the moisture and temperature values to the console */
            Display_printf(display, 0, 0, "Sample %u: Temperature: %d (F)",
                sample,temperature);
        }
        
        else{
            Display_printf(display, 0, 0, "I2C Bus T fault.");
        }

        /* Sleep for 1 second */
        sleep(1);
    }

    I2C_close(i2c);
    Display_printf(display, 0, 0, "I2C closed!");

    return (NULL);
}
