/****************************************************************************
 * boards/arm/stm32f7/stm32f746g-disco/src/stm32_autoleds.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <debug.h>

#include <nuttx/board.h>

#include "stm32_gpio.h"
#include "stm32f7-orbmule.h"

#ifdef CONFIG_ARCH_LEDS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_autoled_initialize
 ****************************************************************************/

void board_autoled_initialize(void)
{
  /* Configure the LD1 GPIO for output. Initial state is OFF */

  stm32_configgpio(GPIO_LD1);
  stm32_configgpio(GPIO_LD2);
  stm32_configgpio(GPIO_LD3);
}

/****************************************************************************
 * Name: board_autoled_on
 ****************************************************************************/

void board_autoled_on(int led)
{
  if (led & 1) stm32_gpiowrite(GPIO_LD1, true);
  if (led & 2) stm32_gpiowrite(GPIO_LD2, true);
  if (led & 4) stm32_gpiowrite(GPIO_LD3, true);
}

/****************************************************************************
 * Name: board_autoled_off
 ****************************************************************************/

void board_autoled_off(int led)
{
  if (led & 1) stm32_gpiowrite(GPIO_LD1, false);
  if (led & 2) stm32_gpiowrite(GPIO_LD2, false);
  if (led & 4) stm32_gpiowrite(GPIO_LD3, false);
}

#endif /* CONFIG_ARCH_LEDS */
