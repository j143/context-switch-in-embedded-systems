/* Basic ARM Cortex-M context switch example */
    PUSH    {R4-R11}        /* Save caller-saved registers */
    STR     SP, [R0]        /* Store current stack pointer */
    LDR     SP, [R1]        /* Load new stack pointer */
    POP     {R4-R11}        /* Restore registers */
    BX      LR              /* Return to new task */