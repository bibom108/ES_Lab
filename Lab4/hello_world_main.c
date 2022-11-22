/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#include "string.h"

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
TaskHandle_t myTaskHandle3 = NULL;
QueueHandle_t queue;

struct msg
{
    /* data */
    int id;
    char mess[50];
};



void Demo_Task(void *arg)
{
    struct msg data;
    queue = xQueueCreate(5, sizeof(data)); 
    if (queue == 0)
    {
     printf("Failed to create queue= %p\n", queue);
    }

    data.id = 2;
    sprintf(data.mess, "Hello from Demo_Task3.0");
    xQueueSend(queue, &data, (TickType_t)0);  

    data.id = 1;
    sprintf(data.mess, "Hello from Demo_Task2.0");
    xQueueSend(queue, &data, (TickType_t)0);

    data.id = 1;
    sprintf(data.mess, "Hello from Demo_Task2.1");
    xQueueSend(queue, &data, (TickType_t)0); 


    while(1){
        vTaskDelay(1000/ portTICK_RATE_MS);
    }
}

void Demo_Task2(void *arg)
{
    struct msg rxBuffer;
    while(1){
     if( xQueueReceive(queue, &(rxBuffer), (TickType_t)5))
     {
         if (rxBuffer.id == 1) {
            printf("Received data from queue == %s\n", rxBuffer.mess);
            vTaskDelay(1000/ portTICK_RATE_MS);
         }
         else {
             xQueueSendToFront(queue, &rxBuffer, (TickType_t)0);
             printf("Task2 miss read the data\n");
             vTaskDelay(1000/ portTICK_RATE_MS);
         }
     }
     else {
      printf("Empty queue===============!\n");
     }
    }
}

void Demo_Task3(void *arg)
{
    struct msg rxBuffer;
    while(1){
     if( xQueueReceive(queue, &(rxBuffer), (TickType_t)5))
     {
        if (rxBuffer.id == 2) {
            printf("Received data from queue @@ %s\n", rxBuffer.mess);
            vTaskDelay(1000/ portTICK_RATE_MS); 
         }
         else {
             xQueueSendToFront(queue, &rxBuffer, (TickType_t)0);
             printf("Task3 miss read the data\n");
             vTaskDelay(1000/ portTICK_RATE_MS); 
         }
     }
     else {
      printf("Empty queue@@@@@@@@@@@@@@@!\n");
      
     }
    }
}

void app_main(void)
{
   xTaskCreate(Demo_Task, "Demo_Task", 4096, NULL, 10, &myTaskHandle);
   xTaskCreatePinnedToCore(Demo_Task2, "Demo_Task2", 4096, NULL,10, &myTaskHandle2, 1);
   xTaskCreatePinnedToCore(Demo_Task3, "Demo_Task3", 4096, NULL,10, &myTaskHandle3, 1);
 }