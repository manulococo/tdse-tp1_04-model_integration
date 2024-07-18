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
 * @file   : task_a.c
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
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "task_actuator_attribute.h"
#include "task_actuator_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SYSTEM_CNT_INI	0u

#define DELAY_INI	0u
#define DELAY_MED	25u
#define DELAY		15u
#define DELAY_MAX	50u

#define SYS_XX_DEL_MIN	DELAY_INI
#define SYS_XX_DEL_MED	DELAY_MED
#define SYS_XX_DEL_MAX	DELAY_MAX

/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta =

	{SYS_XX_DEL_MIN, ST_SYSTEM_STATE_0, EV_SYSTEM_AUTO_POSICION_OFF, false};

#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Modeling)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";
/********************** external data declaration *****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick;

/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_system_init), p_task_system);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_system), p_task_system_);

	g_task_system_cnt = G_TASK_SYSTEM_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_system_cnt), (int)g_task_system_cnt);

	init_queue_event_task_system();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_system_dta = &task_system_dta;

	/* Print out: Task execution FSM */
	state = p_task_system_dta->state;
	LOGGER_LOG("   %s = %d", GET_NAME(state), (int)state);

	event = p_task_system_dta->event;
	LOGGER_LOG("   %s = %d", GET_NAME(event), (int)event);

	b_event = p_task_system_dta->flag;
	LOGGER_LOG("   %s = %d\r\n", GET_NAME(b_event), (int)b_event);

	g_task_system_tick = DELAY;
}

void task_system_update(void *parameters)
{
	task_system_dta_t *p_task_system_dta;
	bool b_time_update_required = false;

	/* Update Task System Counter */
	g_task_system_cnt++;

	/* Protect shared resource (g_task_system_tick) */
	__asm("CPSID i");	/* disable interrupts*/
	if (DELAY == g_task_system_tick)
	{
		b_time_update_required = true;
	}
	if (!g_task_system_tick) {
		g_task_system_tick = DELAY;
	}

	__asm("CPSIE i");	/* enable interrupts*/


	while (b_time_update_required)
	{
		/* Protect shared resource (g_task_system_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (DELAY_INI < g_task_system_tick)
		{
			g_task_system_tick--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

		/* Update Task System Data Pointer */
		p_task_system_dta = &task_system_dta;

		if (true == any_event_task_system())
		{
			p_task_system_dta->flag = true;
			p_task_system_dta->event = get_event_task_system();
			//LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->event), (int)p_task_system_dta->event);
		}

		switch (p_task_system_dta->state)
		{
			case ST_SYSTEM_STATE_0:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_AUTO_POSICION_ON == p_task_system_dta->event))
				{
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_1;
				}

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_AUTO_POSICION_OFF == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_0;
				}

				break;

			case ST_SYSTEM_STATE_1:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_TICKET_IMPRESO_ON == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					/* Imprimir ticket */
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_D1);
					p_task_system_dta->state = ST_SYSTEM_STATE_2;
				}

				break;

			case ST_SYSTEM_STATE_2:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_TICKET_RETIRADO_ON == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					/* Subir barrera */
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_D2);
					p_task_system_dta->state = ST_SYSTEM_STATE_3;
				}

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_TICKET_RETIRADO_OFF	== p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_0;
				}

				break;

			case ST_SYSTEM_STATE_3:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_BARRERA_VERTICAL_ON	== p_task_system_dta->event))
				{
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_4;
				}

				break;


			case ST_SYSTEM_STATE_4:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_AUTO_ENTRANTE_ON == p_task_system_dta->event))
				{
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_5;
				}

				break;

			case ST_SYSTEM_STATE_5:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_AUTO_ENTRANTE_OFF == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					/* Bajar barrera */
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_D3);
					p_task_system_dta->state = ST_SYSTEM_STATE_6;
				}

				break;

			case ST_SYSTEM_STATE_6:

				if ((true == p_task_system_dta->flag) && (EV_SYSTEM_BARRERA_HORIZONTAL_ON == p_task_system_dta->event))
				{
					LOGGER_LOG("%s = %d\r\n", GET_NAME(p_task_system_dta->state), (int)p_task_system_dta->state);
					p_task_system_dta->flag = false;
					p_task_system_dta->state = ST_SYSTEM_STATE_0;
				}

				break;

			default:

				break;
		}
	}
}

/********************** end of file ******************************************/
