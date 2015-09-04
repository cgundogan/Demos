/*
 * Copyright (C) 2015 Hauke Petersen <mail@haukepetersen.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @ingroup     lndw15_sensor
 * @{
 *
 * @file
 * @brief       Sensing module
 *
 * @author      Hauke Petersen <mail@haukepetersen.de>
 *
 * @}
 */

#include <stdio.h>

#include "thread.h"
#include "xtimer.h"
#include "srf02.h"

#include "demo_config.h"

#define BASELINE_COUNT          (30U)
#define THRESHOLD               (50U)


#define PRIO                    (THREAD_PRIORITY_MAIN - 3)
static char stack[THREAD_STACKSIZE_MAIN];

static void(*on_data)(uint16_t);

static int init_counter = 0;
static int evt_counter = 0;
static uint32_t baseline = 0;


static void process(uint16_t range)
{
    if (range < (baseline - THRESHOLD)) {
        ++evt_counter;
    } else {
        evt_counter = 0;
    }
    if (evt_counter == 1) {
        puts("EVENT");
    }
}

static void startup(uint16_t range)
{
    ++init_counter;
    baseline += range;
    if (init_counter == BASELINE_COUNT) {
        on_data = process;
        baseline /= BASELINE_COUNT;
        printf("now processing - baseline: %u\n", (unsigned)baseline);
    }
}

void *sense_thread(void *arg)
{
    (void)arg;
    srf02_t dev;
    uint32_t last_wakeup = xtimer_now();
    uint16_t range;

    /* set initial data handler */
    on_data = startup;

    /* initialize the distance sensor */
    srf02_init(&dev, CONF_SENSE_I2C, SRF02_DEFAULT_ADDR);

    /* read sensor periodically */
    while (1) {
        xtimer_usleep_until(&last_wakeup, CONF_SENSE_PERIOD);
        range = srf02_get_distance(&dev, SRF02_MODE_REAL_CM);
        on_data(range);
    }

    /* never to be reached */
    return NULL;
}

void sense_run(kernel_pid_t radio)
{
    /* start the sampling thread */
    thread_create(stack, sizeof(stack), PRIO, 0, sense_thread, NULL, "sense");
}
