/* Example of context structure in C */
typedef struct {
    uint32_t r0;        /* General purpose registers */
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t r12;
    uint32_t sp;        /* Stack pointer */
    uint32_t lr;        /* Link register */
    uint32_t pc;        /* Program counter */
    uint32_t xpsr;      /* Program status register */
} Context_t;