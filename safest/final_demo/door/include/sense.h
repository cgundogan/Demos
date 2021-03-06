/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     door
 * @{
 *
 * @file
 * @brief       Interfaces of the sensing module
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef DOOR_SENSE_H
#define DOOR_SENSE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Setup and run the sensor thread
 */
void sense_run(void);

#ifdef __cplusplus
}
#endif

#endif /* DOOR_SENSE_H*/
/** @} */
