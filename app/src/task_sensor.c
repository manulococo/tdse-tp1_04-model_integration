/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_c.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_sensor_attribute.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SENSOR_CNT_INIT	0u

#define DELAY_INI				0u
#define DELAY					15u
#define DELAY_MAX				50u

#define BUTTON_XX_DEL_MAX		DELAY_MAX
#define BUTTON_XX_DEL_MIN		DELAY_INI

/********************** internal data declaration ****************************/
const task_sensor_cfg_t task_sensor_cfg_list[] =

	{{ID_BUTTON_S1, BUTTON_S1_PORT, BUTTON_S1_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_AUTO_POSICION_OFF, EV_SYSTEM_AUTO_POSICION_ON},

	 {ID_BUTTON_S2, BUTTON_S2_PORT, BUTTON_S2_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_TICKET_IMPRESO_OFF, EV_SYSTEM_TICKET_IMPRESO_ON},

	 {ID_BUTTON_S3, BUTTON_S3_PORT, BUTTON_S3_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_TICKET_RETIRADO_OFF, EV_SYSTEM_TICKET_RETIRADO_ON},

	 {ID_BUTTON_S4, BUTTON_S4_PORT, BUTTON_S4_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_BARRERA_VERTICAL_OFF, EV_SYSTEM_BARRERA_VERTICAL_ON},

	 {ID_BUTTON_K1, BUTTON_K1_PORT, BUTTON_K1_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_AUTO_ENTRANTE_OFF, EV_SYSTEM_AUTO_ENTRANTE_ON},

	 {ID_BUTTON_K2, BUTTON_K2_PORT, BUTTON_K2_PIN, BUTTON_PRESSED,
	  BUTTON_XX_DEL_MAX, EV_SYSTEM_BARRERA_HORIZONTAL_OFF, EV_SYSTEM_BARRERA_HORIZONTAL_ON},

     {ID_BUTTON_A,  BUTTON_A_PORT,  BUTTON_A_PIN,  BUTTON_PRESSED,
      BUTTON_XX_DEL_MAX, SIG_BUTTON_A_UP,  SIG_BUTTON_A_DOWN}};

#define SENSOR_CFG_QTY	(sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] =

	{{BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

	 {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

	 {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

	 {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

	 {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

	 {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP},

     {BUTTON_XX_DEL_MIN, ST_BUTTON_XX_UP, EV_BUTTON_XX_UP}};

#define SENSOR_DTA_QTY	(sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_sensor 		= "Task Sensor (Sensor Modeling)";
const char *p_task_sensor_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration *****************************/
uint32_t g_task_sensor_cnt;
volatile uint32_t g_task_sensor_tick;

/********************** external functions definition ************************/
void task_sensor_init(void *parameters)
{
	uint32_t index;
	task_sensor_dta_t *p_task_sensor_dta;
	task_sensor_st_t state;
	task_sensor_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_sensor_init), p_task_sensor);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_sensor), p_task_sensor_);

	g_task_sensor_cnt = G_TASK_SENSOR_CNT_INIT;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_sensor_cnt), (int)g_task_sensor_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_dta_list[index];

		/* Print out: Index & Task execution FSM */
		LOGGER_LOG("   %s = %d", GET_NAME(index), (int)index);

		state = p_task_sensor_dta->state;
		LOGGER_LOG("   %s = %d", GET_NAME(state), (int)state);

		event = p_task_sensor_dta->event;
		LOGGER_LOG("   %s = %d\r\n", GET_NAME(event), (int)event);
	}
	g_task_sensor_tick = DELAY_INI;
}

void task_sensor_update(void *parameters)
{
	uint32_t index;
	const task_sensor_cfg_t *p_task_sensor_cfg;
	task_sensor_dta_t *p_task_sensor_dta;
	bool b_time_update_required = false;

	/* Update Task Sensor Counter */
	g_task_sensor_cnt++;

	/* Protect shared resource (g_task_sensor_tick) */
	__asm("CPSID i");	/* disable interrupts*/
	if (DELAY == g_task_sensor_tick)
	{
		b_time_update_required = true;
	}
	if (!g_task_sensor_tick)
	{
		g_task_sensor_tick = DELAY;
	}

    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_sensor_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (DELAY_INI == g_task_sensor_tick)
		{
			g_task_sensor_tick--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	for (index = 0; SENSOR_DTA_QTY > index; index++)
		{
    		/* Update Task Sensor Configuration & Data Pointer */
			p_task_sensor_cfg = &task_sensor_cfg_list[index];
			p_task_sensor_dta = &task_sensor_dta_list[index];

			if (p_task_sensor_cfg->pressed == HAL_GPIO_ReadPin(p_task_sensor_cfg->gpio_port, p_task_sensor_cfg->pin))
			{
				p_task_sensor_dta->event =	EV_BUTTON_XX_DOWN;
			}
			else
			{
				p_task_sensor_dta->event =	EV_BUTTON_XX_UP;
			}

			switch (p_task_sensor_dta->state)
			{
				case ST_BUTTON_XX_UP:

					if (EV_BUTTON_XX_DOWN == p_task_sensor_dta->event)
					{
						p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max;
						p_task_sensor_dta->state = ST_BUTTON_XX_FALLING;
					}

					break;

				case ST_BUTTON_XX_FALLING:

					p_task_sensor_dta->tick--;
					if (DELAY == g_task_sensor_tick)
					{
						if (EV_BUTTON_XX_DOWN == p_task_sensor_dta->event)
						{
							put_event_task_system(p_task_sensor_cfg->signal_down);
							p_task_sensor_dta->state = ST_BUTTON_XX_DOWN;
						}
						else
						{
							p_task_sensor_dta->state = ST_BUTTON_XX_UP;
						}
					}

					break;

				case ST_BUTTON_XX_DOWN:

					if (EV_BUTTON_XX_UP == p_task_sensor_dta->event)
					{
						p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max;
						p_task_sensor_dta->state = ST_BUTTON_XX_RISING;
					}

					break;

				case ST_BUTTON_XX_RISING:

					p_task_sensor_dta->tick--;
					if (BUTTON_XX_DEL_MIN == p_task_sensor_dta->tick)
					{
						if (EV_BUTTON_XX_UP == p_task_sensor_dta->event)
						{
							put_event_task_system(p_task_sensor_cfg->signal_up);
							p_task_sensor_dta->state = ST_BUTTON_XX_UP;
						}
						else
						{
							p_task_sensor_dta->state = ST_BUTTON_XX_DOWN;
						}
					}

					break;

				default:

					break;
			}
		}
    }
}

/********************** end of file ******************************************/
