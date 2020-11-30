/*
5_ Cree un programa usando FreeRTOS que cumpla con las siguientes consignas:
 - Posea 3 tareas, Tarea1, Tarea2, Tarea3.
 - Las tareas Tarea1 y Tarea2  deben enviar por cola de mensajes a la Tarea3 el tiempo en milisegundos que debe estar encendido
el led amarillo e indicarlo por puerto serie.
 - LA Tarea3 debe leer esos tiempos de la cola de mensajes a medida que llegan y controlar el led en cuestion. Siempre, el tiempo de
apagado es de 500ms. Tambien debe indicar por puerto serie sus acciones.
 - Indique que pasa en caso de que el timeout de la escritura en la cola de mensajes sea de 300ms.
*/

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "chip.h"
#include "string.h"
#include "task.h"
#include "queue.h"

#define PRIO_TAREA1 1
#define PRIO_TAREA2 1
#define PRIO_TAREA3 1

#define TAM_COLA 8 //8 mensajes.
#define TAM_MSG 20 // 20 mensajes.

static void vTarea1(void *pcParameters){

	char cad_env[8];
	char *pcTaskName = "Tareas1 corriendo\r\n";
	char *pcEnvio = "Envio 1000ms a la Tarea3\r\n";
	const TickType_t xDelay = pdMS_TO_TICKS(1000);

	while(1){
		vPrintfString(pcTaskName);
		strcpy(cad_env, (void *)cad_env,xDelay); //Copio xDelay a cad_env.

		if(xQueueSend(cola,(void *)cad_env,xDelay) == pdPASS){
			vPrintString(pcEnvio);
		}else{
			Board_LED_Toggle(0); //RGB.
		}
	}
}

static void vTarea2(void *pcParameters){

	char cad_env[8];
	char *pcTaskName = "Tarea2 corriendo\r\n";
	char *pcEnvio = "Envio 2000ms a la Tarea3\r\n"
	const TickType_t xDelay = pdMS_TO_TICKS(2000);

	while(1){
		vPrintString(pcTaskName);
		strcpy(cadn_env, (void *)cad_env,xDelay); // Copio xDelay a cad_env.

		if(xQueueSend(cola,(void *)cad_env,xDelay) == pdPASS){
			vPrintString(pcEnvio);
		}else{
			Board_LED_Toggle(0); //RGB.
		}
	}
}

static void vTarea3(void *pcParameters){

	char cad_rec[8];
	const char *pcTaskName = "Tarea3 corriendo\r\n";

	const TickType_t xDelay = pdMS_TO_TICKS(500);
	const TicksType_t xEncendido;

	while(1){
		vPrintString(pcTaskName);

		if(xQueueReceive(cola, (void *)cad_rec,cad_rec) == pdTRUE){

			xEncendido = pdMS_TO_TICKS(cad_rec); //Recibo los ms.

			Board_LED_Set(3,TRUE);
			vTaskDelay(xEncendido); //Led Encendido  por un tiempo definido por la tareas.
			Board_LED_Set(3,FALSE);
			vTaskDelay(xDelay); // Led Apagado por 500ms.
		}
	}
}

int main(void){
	//Crear tareas.
	xTaskCreate(vTarea1,"Tarea1",configMINIMAL_STACK_SIZE,NULL,PRIO_TAREA1,NULL);
	xTaskCreate(vTarea2,"Tarea2",configMINIMAL_STACK_SIZE,NULL,PRIO_TAREA2,NULL);
	xTaskCreate(vTarea3,"Tarea3",configMINIMAL_STACK_SIZE,NULL,PRIO_TAREA3,NULL);

	//Crear cola.
	cola = xQueueCreate(TAM_COLA,sizeof(TickType_t));

	//Arrancar el planificador.
	vTaskStartScheduler();

	for(;;);
	return(0);
}

