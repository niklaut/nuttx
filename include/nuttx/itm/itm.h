// Copyright (c) 2023, Auterion AG
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <stdint.h>

enum
{
    // Tasks
    EMDBG_TASK_START = 0,
    EMDBG_TASK_STOP = 1,
    EMDBG_TASK_RESUME = 2,
    EMDBG_TASK_RUNNABLE = 3, // custom
    // Workqueues
    EMDBG_WORKQUEUE = 4,
    // Semaphores
    EMDBG_SEMAPHORE_INIT = 5,
    EMDBG_SEMAPHORE_DECR = 6,
    EMDBG_SEMAPHORE_INCR = 7,

    // Heap
    EMDBG_HEAP_REGIONS = 8,
    EMDBG_HEAP_MALLOC_ATTEMPT = 9,
    EMDBG_HEAP_MALLOC_RESULT = 10,
    EMDBG_HEAP_FREE = 11,

    // DMA
    EMDBG_DMA_CONFIG = 12,
    EMDBG_DMA_START = 13,
    EMDBG_DMA_STOP = 14,

    // the rest are optional user channels
    EMDBG_UART4_TX = 30,
    EMDBG_UART4_RX = 31,
};

#define EMDBG_LOG_TASK_START(tcb) \
    for (const char *name = tcb->name, \
         *end = tcb->name + strnlen(tcb->name, CONFIG_TASK_NAME_SIZE); \
         name < end; name += 4) \
    { \
        uint32_t value; \
        memcpy(&value, name, 4); \
        emdbg_itm32_block(EMDBG_TASK_START, value); \
    } \
    emdbg_itm_block(EMDBG_TASK_START, tcb->pid)

#define EMDBG_LOG_TASK_STOP(tcb) \
    emdbg_itm_block(EMDBG_TASK_STOP, tcb->pid)

#define EMDBG_LOG_TASK_RESUME(tcb, prev_state) \
    emdbg_itm32_block(EMDBG_TASK_RESUME, (prev_state << 24) | (tcb->sched_priority << 16) | tcb->pid)

#define EMDBG_LOG_TASK_RUNNABLE(tcb) \
    emdbg_itm_block(EMDBG_TASK_RUNNABLE, tcb->pid)




typedef struct
{
    volatile union
    {
        volatile uint8_t  u8;
        volatile uint16_t u16;
        volatile uint32_t u32;
    }                 PORT[32u];
             uint32_t RESERVED0[864u];
    volatile uint32_t TER;
} EMDBG_ITM_Type;
#define EMDBG_ITM ((EMDBG_ITM_Type*)0xE0000000ul)


static inline void emdbg_itm8_block(uint8_t channel, uint8_t value);
static inline void emdbg_itm16_block(uint8_t channel, uint16_t value);
static inline void emdbg_itm32_block(uint8_t channel, uint32_t value);
static inline void emdbg_itm_block(uint8_t channel, uint32_t value);

void emdbg_itm8_block(uint8_t channel, uint8_t value)
{
    if (EMDBG_ITM->TER & (1ul << channel)) {
        while (!EMDBG_ITM->PORT[channel].u32) ;
        EMDBG_ITM->PORT[channel].u8 = value;
    }
}

void emdbg_itm16_block(uint8_t channel, uint16_t value)
{
    if (EMDBG_ITM->TER & (1ul << channel)) {
        while (!EMDBG_ITM->PORT[channel].u32) ;
        EMDBG_ITM->PORT[channel].u16 = value;
    }
}

void emdbg_itm32_block(uint8_t channel, uint32_t value)
{
    if (EMDBG_ITM->TER & (1ul << channel)) {
        while (!EMDBG_ITM->PORT[channel].u32) ;
        EMDBG_ITM->PORT[channel].u32 = value;
    }
}

void emdbg_itm_block(uint8_t channel, uint32_t value)
{
    if (value & 0xffff0000ul) emdbg_itm32_block(channel, value);
    else if (value & 0xff00u) emdbg_itm16_block(channel, value);
    else emdbg_itm8_block(channel, value);
}

#undef EMDBG_ITM