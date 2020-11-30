/*
3_Cree un programa en FreeRTOS que cumpla con las siguientes consignas:
 - Posea 3 tareas, Tarea1,Tarea2,Tarea3.
 - La Tarea1, de tener la mayor prioridad, la Tarea2 media prioridad, Tarea3 baja prioridad.
 - La Tarea1 y Tarea3 comparten el puerto serie, enviando un mensaje cada 800ms.
 - La Tarea2 es una tarea periodica que ejecuta un busy waiting de 1s aprox, un cambio de estado del led rojo y un delay de 200ms.
*/

#include "stdio.h"
#include "FreeRTOS.h"
#include "board.h"
#include "task.h"
#include "queue.h"

#define mainDELAY_LOOP_COUNT (0xffffff)
#define TAM_COLA 20
#define TAM_MSG 8

SemaphoreHandle_t sem_serie;
QueueHandle_t cola;

const char *pcTextoTarea1 = "Soy la Tarea1\r\n";
const char *pcTextoTarea3 = "Soy la Tarea3\r\n";
 
static void vTarea(void *pcParameters){

	char *pcTaskName;
	Ticktype_t xLastWakeTime;

	const TickType_t xDelay800ms = pdMS_TO_TICKS(800UL);
	const TickType_t xDelay = pdMS_TO_TICKS(10UL);

	pcTaskName = (char *)pcParameters;

	xLastWakeTime = xTaskGetTickCount();

	for(;;){
		if(xSemaphoreTake(sem_serie,portMAX_DELAY) == pdPASS){
			vPrintString(pcTaskName);
			xSemaphoreGive(sem_serie);
		}
		vTaskDelayUntil(&xLastWakeTime,xDelay800ms);
	}
}

static void vTarea2(void *pcParameters){

	char *pcTaskName = "Soy la Tarea2\r\n";
	TickType_t xLastWakeTime;
	volatile uint32_t i;

	const TickType_t xDelay200ms = pdMS_TO_TICKS(200UL);

	xLastWakeTime = xTaskGetTickCount();

	for(;;){
		for(i = 0; i < mainDELAY_LOOP_COUNT; i++){} // Busy Waiting.

		Board_LED_Set(4,TRUE);
		vTaskDelayUntil(&xLastWakeTime,xDelay200ms);
		Board_LED_Set(4,FALSE);
	}
}

int main(void){

	sem_serie = xSemaphoreCreateBinary(); //Inicio sem en 0.
	cola = xQueueCreate(TAM_COLA,TAM_MSG);

	//Creo tareas.
	xTaskCreate(vTarea,(const char *)"Tarea1",configMINIMAL_STACK_SIZE,(void *)pcTextoTarea1,tskIDLE_PRIORITY + 3,NULL);
	xTaskCreate(vTarea2,(const char *)"Tarea2",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY + 2,NULL);
	xTaskCreate(vTarea,(const char *)"Tarea3",configMINIMAL_STACK_SIZE,(void *)pcTextoTarea3,tskIDLE_PRIORITY + 1,NULL);

	//Arranco planificador.
	xTaskStartScheduler();

	for(;;);
	return(0);
}
