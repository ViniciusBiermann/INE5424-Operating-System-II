// EPOS Cortex-A SETUP

#include <system/config.h>

extern "C" { void _vector_table() __attribute__ ((used, naked, section(".init"))); }

// Interrupt Vector Table
void _vector_table()
{
    ASM("\t\n\
        b   _reset                                                           \t\n\
        b   _undefined_instruction                                           \t\n\
        b   _software_interrupt                                              \t\n\
        b   _prefetch_abort                                                  \t\n\
        b   _data_abort                                                      \t\n\
        nop                           // Reserved                            \t\n\
        b   _irq                                                             \t\n\
        b   _fiq                                                             \t\n\

        _reset:                                                              \t\n\
            MRC p15, 0, r1, c1, c0, 0                                        \t\n\
            BIC r1, r1, #0x1                                                 \t\n\
            MCR p15, 0, r1, c1, c0, 0                                        \t\n\
                                                                             \t\n\
            MRC p15, 0, r1, c1, c0, 0                                        \t\n\
            BIC r1, r1, #(0x1 << 12)                                         \t\n\
            BIC r1, r1, #(0x1 << 2)                                          \t\n\
            MCR p15, 0, r1, c1, c0, 0                                        \t\n\
                                                                             \t\n\
            MOV r1, #0                                                       \t\n\
            MCR p15, 0, r1, c7, c5, 0                                        \t\n\
                                                                             \t\n\
            MRC p15, 1, r0, c0, c0, 0                                        \t\n\
            LDR r3, =#0x1ff                                                  \t\n\
            AND r0, r3, r0, LSR #13                                          \t\n\
            MOV r1, #0                                                       \t\n\
            way_loop:                                                        \t\n\
            MOV r3, #0                                                       \t\n\
            set_loop:                                                        \t\n\
            MOV r2, r1, LSL #30                                              \t\n\
            ORR r2, r3, LSL #5                                               \t\n\
            MCR p15, 0, r2, c7, c6, 2                                        \t\n\
            ADD r3, r3, #1                                                   \t\n\
            CMP r0, r3                                                       \t\n\
            BGT set_loop                                                     \t\n\
            ADD r1, r1, #1                                                   \t\n\
            CMP r1, #4                                                       \t\n\
            BNE way_loop                                                     \t\n\
                                                                             \t\n\
            MCR p15, 0, r1, c8, c7, 0                                        \t\n\
                                                                             \t\n\
            LDR r0, =0xDE2                                                   \t\n\
            LDR r1, =0x00004000 \t\n\
            LDR r3, = 4095                                                   \t\n\
            write_pte:                                                       \t\n\
            ORR r2, r0, r3, LSL #20                                          \t\n\
            STR r2, [r1, r3, LSL #2]                                         \t\n\
            SUB r3, r3, #1                                                   \t\n\
            CMP r3, #-1                                                      \t\n\
            BNE write_pte                                                    \t\n\
                                                                             \t\n\
            BIC r0, r0, #0xC                                                 \t\n\
            ORR r0, r0, #0X4                                                 \t\n\
            BIC r0, r0, #0x7000                                              \t\n\
            ORR r0, r0, #0x5000                                              \t\n\
            ORR r0, r0, #0x10000                                             \t\n\
            STR r0, [r1]                                                     \t\n\
                                                                             \t\n\
            MOV r1,#0x0                                                      \t\n\
            MCR p15, 0, r1, c2, c0, 2                                        \t\n\
            LDR r1, =ttb_address                                             \t\n\
            MCR p15, 0, r1, c2, c0, 0                                        \t\n\
                                                                             \t\n\
            LDR r1, =0x55555555                                              \t\n\
            MCR p15, 0, r1, c3, c0, 0                                        \t\n\
                                                                             \t\n\
            MRC p15, 0, r1, c1, c0, 0                                        \t\n\
            ORR r1, r1, #0x1                                                 \t\n\
            MCR p15, 0, r1, c1, c0, 0                                        \t\n\
        ");
}
