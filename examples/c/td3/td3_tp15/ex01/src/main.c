/*
1_Cree un programa usando FreeRTOS que cumpla con las siguientes cosignas:
 - Posea dos tareas, Tarea1 y Tarea2 con los niveles de prioridad correspondientes y un mecanismo de sincronizacion.
 - La Tarea1, debe ejecutarse de manera continua un bucle waiting de 1s aprox, indicando el estado por el puerto serie.
 - La Tarea2, debe estar serializada, y esperar por cada loop de Tarea1, cambiando el estado del led azul, cada vez que Tarea1
complete el loop.
*/

#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"

#define mainDELAY_LOOP_COUNT (0xffffff)

const char *pcTextoTarea1 = "Tarea1 corriendo\r\n";
const char *pcTextoTarea2 = "Tarea2 corriendo\r\n";

SemaphoreHandle_t sem_serie; //semaforo.

static void vTarea1(void){

	char *pcTaskName;
	const char *pcBucle = "Ejecutando Busy Waiting\r\n";
	volatile uint32_t i;

	pcTaskName = (char *)pcParameters;

	while(1){
		vPrintString(pcTaskName);

		for(i = 0; i < mainDELAY_LOOP_COUNT; i ++){
			vPrintString(pcBlucle);
		}

		xSemaphoreGive(sem_serie);//Libero semaforo.
	}
}

static void vTarea2(void){

	char *pcTaskName;
	const TickType_t xDelay = pdMS_TO_TICKS(1000UL);

	pcTaskName = (char *)pcParameters;

	for(;;){
		vPrintString(pcTaskName);

		if(xSemaphoreTake(sem_serie,xDelay) == pdTRUE){
			Board_LED_Toggle(2); //Parpadeo led azul.
		}
	}
}

int main(void){

	sem_serie = SemaphoreCreateBinary(); //Inicio semaforo en 0.

	//Creo tareas.
	xTaskCreate(vTarea1,"Tarea1",configMINIMAL_STACK_SIZE,(void *)pcTextoTarea1,tskIDLE_PRIORITY + 1,NULL);
	xTaskCreate(vTarea2,"Tarea2",configMINIMAL_STACK_SIZE,(void *)pcTextoTarea2,tskIDLE_PRIORITY + 2,NULL);

	//Arrancar planificador.
	xTaskStartScheduler();

	for(;;);
	return(0);
}
