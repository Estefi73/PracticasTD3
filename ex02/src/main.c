/*
2_ Cree un programa usando FreeRTos que cumpla con las siguientes consignas:
 -Posea una Tarea1.
 -La Tarea1 debe crear una Tarea2 cada 500ms, indicandolo por puerto serie.
 -La Tarea2 debe cambiar el estado del led amarillo, indicarlo pro puerto serie y terminar.
*/

#include "chip.h"
#include "board.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define PRIO_TAREA1 2
#define PRIO_TAREA2 1
#define TAM 150

const char *pcTextoTarea1 = "Creando tarea2\r\n";
const char *pcTextoTarea2 = "Tarea2 corriendo\r\n";

static void vTarea1(void *pcParameters){
	char *pcTaskName;

	pcTaskName = (char *)pcParameters;

	while(1){
		xTaskCreate(vTarea2,(const char *)"Tarea2",TAM,(void *)pcTextoTarea2,PRIO_TAREA2,NULL);

		printf(pcTaskName);

		vTaksDelay(50000/portTICK_RATE_MS); //portTICK_RATE_MS = 100 -> 10ms.
		// Esto seria igual a: TickType_t xDelay = pdMS_to_TICK(500UL);
	}
}

static void vTarea2(void *pcParameters){
	char *pcTaskName;

	pcTaskName = (char *)pcParameters;

	while(1){
		printf(pcTaskName);

		Board_LED_Toggle(3); //Led Amarillo.

		vTaskDelete(NULL); //Finalizar Tarea2.
	}
}

int main(void){
	//Crear Tarea1.
	xTaskCreate(vTarea1,(const char *)"Tarea1",TAM,(void *)pcTextoTarea1,PRIO_TAREA1,NULL);

	//Arrancar planificador.
	vTaskStartScheduler();

	for(;;);
	return(0);
}


