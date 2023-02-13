#include "amb_light_service.h"
#include "controller.h"
#include "serial_io.h"

#include <adc.h>
#include <sci.h>


#include <FreeRTOS.h>
#include <os_task.h>
#include <os_timer.h>
#include <sys_common.h>
#include <os_queue.h>


#include <gio.h>
#include <sci.h>

#include <string.h>


/* USER CODE BEGIN */
// Include any additional headers and global variables here
static TaskHandle_t lightServiceTaskHandle = NULL;
enum light_event_t MEASURE_LIGHT;
enum light_event_t *event;
QueueHandle_t Queue;
char string[30];
event = &MEASURE_LIGHT;
/* USER CODE END */

/**
 * @brief Task that waits for MEASURE_LIGHT event and then prints the ambient light value to the serial port.
 * @param pvParameters Task parameters
 */
static void lightServiceTask(void * pvParameters); 

uint8_t initLightService(void) {
    /* USER CODE BEGIN */
    // Create the task and queue here.

    Queue = xQueueCreate(
                    QUEUE_LENGTH, QUEUE_SIZE);
   
   
    BaseType_t xReturned = pdFAIL;
    if (lightServiceTaskHandle == NULL) {
        // Create controller task
        xReturned = xTaskCreate(lightServiceTask,             /* Function that implements the task. */
                                LIGHT_SERVICE_NAME,            /* Text name for the task. */
                                LIGHT_SERVICE_STACK_SIZE,      /* Stack size in words, not bytes. */
                                NULL,                       /* Parameter passed into the task. */
                                LIGHT_SERVICE_PRIORITY,        /* Priority at which the task is created. */
                                &lightServiceTaskHandle);     /* Used to pass out the created task's handle. */
    }
    /* USER CODE END */
    return 1;
}

static void lightServiceTask(void * pvParameters) {
    /* USER CODE BEGIN */
    // Wait for MEASURE_LIGHT event in the queue and then print the ambient light value to the serial port.
    while(1){
        if(Queue != 0){
            xQueueReceive(Queue, *event, (TickType_t) 5 );
            sciBASE_t *sci = sciREG;

            
            sciPrintText(sci, *event, 100);
        }
        /* USER CODE END */
    }
}

uint8_t sendToLightServiceQueue(light_event_t *event) {
    /* USER CODE BEGIN */
    // Send the event to the queue.


    xQueueSend(Queue, *event, (TickType_t) 0 );
   
    /* USER CODE END */
    return 0;
}
