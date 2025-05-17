# context-switch-in-embedded-systems


What is a `context`?

- Register set: CPU registers - general purpose, stack pointer and program counter
- Processor status: status flags, interrupt mask (NIR, IR), processor mode bits
- Memory management: MMU settings, memory protection configuration
- FPU state: floating point registers
- Cache state: cache configuration and state


## ARM Cortex-M Context Switch Implementation

```c
/* Define task control block */
typedef struct {
    uint32_t *psp;      /* Process Stack Pointer */
    uint32_t priority;
    uint8_t state;
    void (*task_func)(void); /* Task entry point */
    /* Additional task metadata */
} TCB_t;

/* Current and next task pointers */
TCB_t *current_task;
TCB_t *next_task;

/* PendSV handler for context switching */
__attribute__((naked)) void PendSV_Handler(void) {
    /* Disable interrupts during context switch */
    __asm volatile ("cpsid i");
    
    /* Save current context */
    __asm volatile (
        "mrs r0, psp\n"          /* Get current process stack pointer */
        "stmdb r0!, {r4-r11}\n"  /* Store r4-r11 on process stack */
        "ldr r1, =current_task\n" /* Get pointer to current_task */
        "ldr r1, [r1]\n"         /* r1 = current_task */
        "str r0, [r1]\n"         /* Store PSP in TCB */
    );
    
    /* Perform task switch */
    __asm volatile (
        "ldr r0, =current_task\n"
        "ldr r1, =next_task\n"
        "ldr r1, [r1]\n"         /* r1 = next_task */
        "str r1, [r0]\n"         /* current_task = next_task */
    );
    
    /* Restore new context */
    __asm volatile (
        "ldr r0, [r1]\n"         /* r0 = next_task->psp */
        "ldmia r0!, {r4-r11}\n"  /* Restore r4-r11 from stack */
        "msr psp, r0\n"          /* Set PSP to new stack */
        "isb\n"                  /* Instruction barrier */
        "cpsie i\n"              /* Enable interrupts */
        "bx lr\n"                /* Return to thread mode */
    );
}

/* Request a context switch */
void trigger_context_switch(void) {
    /* Set PendSV to pending */
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    /* Instruction barrier */
    __asm volatile ("isb");
}
```

This implementation uses the PendSV exception which is commonly used for context switching in ARM Cortex-M based systems. It has the lowest exception priority, ensuring that all other interrupts are serviced before the context switch occurs.

ref: https://developer.arm.com/documentation/107706/0100/System-exceptions/Pended-SVC---PendSV
