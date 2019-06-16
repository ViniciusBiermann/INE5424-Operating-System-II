// EPOS Cortex-A SETUP

#include <system/config.h>

extern "C" { void _vector_table() __attribute__ ((used, naked, section(".init"))); }

// Interrupt Vector Table
void _vector_table()
{
    ASM("\t\n\
        _vector_table: \t\n\
            b   _reset                                                                                    \t\n\
            b   _undefined_instruction                                                                    \t\n\
            b   _software_interrupt                                                                       \t\n\
            b   _prefetch_abort                                                                           \t\n\
            b   _data_abort                                                                               \t\n\
            nop                           // Reserved                                                     \t\n\
            b   _int_entry                                                                                \t\n\
            b   _fiq                                                                                      \t\n\
        _reset:                                                                                           \t\n\
            MRC p15, 0, r1, c1, c0, 0                                                                     \t\n\
            BIC r1, r1, #0x1                                                                              \t\n\
            BIC r1, r1, #(1 << 12)      // i-cache off                                                    \t\n\
            BIC r1, r1, #(1 << 2)       // d-cache & L2-$ off                                             \t\n\
            MCR p15, 0, r1, c1, c0, 0                                                                     \t\n\
                                                                                                          \t\n\
            MOV r0, #0                                                                                    \t\n\
            MCR p15, 0, r0, c7, c5, 0   // Invalidate Instruction Cache                                   \t\n\
            MCR p15, 0, r0, c7, c5, 6   // Invalidate branch prediction array                             \t\n\
            MCR p15, 0, r0, c8, c7, 0   // Invalidate entire Unified Main TLB                             \t\n\
            ISB                         // instr sync barrier                                             \t\n\
                                                                                                          \t\n\
            MRC p15, 0, r0, c1, c0, 0   // System control register                                        \t\n\
            ORR r0, r0, #1 << 12        // Instruction cache enable                                       \t\n\
            ORR r0, r0, #1 << 11        // Program flow prediction                                        \t\n\
            MCR p15, 0, r0, c1, c0, 0   // System control register                                        \t\n\
                                                                                                          \t\n\
            MRC p15, 1, R0, c0, c0, 1   // Read CLIDR into R0                                             \t\n\
            ANDS R3, R0, #0x07000000                                                                      \t\n\
            MOV R3, R3, LSR #23         // Cache level value (naturally aligned)                          \t\n\
            BEQ Finished                                                                                  \t\n\
            MOV R10, #0                                                                                   \t\n\
        Loop1:                                                                                            \t\n\
            ADD R2, R10, R10, LSR #1    // Work out 3 x cachelevel                                        \t\n\
            MOV R1, R0, LSR R2          // bottom 3 bits are the Cache type for this level                \t\n\
            AND R1, R1, #7              // get those 3 bits alone                                         \t\n\
            CMP R1, #2                                                                                    \t\n\
            BLT Skip                    // no cache or only instruction cache at this                     \t\n\
            MCR p15, 2, R10, c0, c0, 0  // write CSSELR from R10                                          \t\n\
            ISB                         // ISB to sync the change to the CCSIDR                           \t\n\
            MRC p15, 1, R1, c0, c0, 0   // read current CCSIDR to R1                                      \t\n\
            AND R2, R1, #7              // extract the line length field                                  \t\n\
            ADD R2, R2, #4              // add 4 for the line length offset (log2 16 bytes)               \t\n\
            LDR R4, =0x3FF                                                                                \t\n\
            ANDS R4, R4, R1, LSR #3     // R4 is the max number on the way size (right aligned)           \t\n\
            CLZ R5, R4                  // R5 is the bit position of the way size increment               \t\n\
            MOV R9, R4                  // R9 working copy of the max way size (right aligned)            \t\n\
        Loop2:                                                                                            \t\n\
            LDR R7, =0x00007FFF                                                                           \t\n\
            ANDS R7, R7, R1, LSR #13    // R7 is the max number of the index size (right aligned)         \t\n\
        Loop3:                                                                                            \t\n\
            ORR R11, R10, R9, LSL R5    // factor in the way number and cache number into R11             \t\n\
            ORR R11, R11, R7, LSL R2    // factor in the index number                                     \t\n\
            MCR p15, 0, R11, c7, c10, 2 // DCCSW, clean by set/way                                        \t\n\
            SUBS R7, R7, #1             // decrement the index                                            \t\n\
            BGE Loop3                                                                                     \t\n\
            SUBS R9, R9, #1             // decrement the way number                                       \t\n\
            BGE Loop2                                                                                     \t\n\
                                                                                                          \t\n\
        Skip:                                                                                             \t\n\
            ADD R10, R10, #2            // increment the cache number                                     \t\n\
            CMP R3, R10                                                                                   \t\n\
            BGT Loop1                                                                                     \t\n\
            DSB                                                                                           \t\n\
        Finished:                                                                                         \t\n\
            ADD R4, #0                                                                                    \t\n\
            ADD R4, R4, #10                                                                               \t\n\
                                                                                                          \t\n\
                                                                                                          \t\n\
            MRC p15, 1, r0, c0, c0, 0                                                                     \t\n\
            LDR r3, =#0x1ff                                                                               \t\n\
            AND r0, r3, r0, LSR #13                                                                       \t\n\
            MOV r1, #0                                                                                    \t\n\
        way_loop:                                                                                         \t\n\
            MOV r3, #0                                                                                    \t\n\
        set_loop:                                                                                         \t\n\
            MOV r2, r1, LSL #30                                                                           \t\n\
            ORR r2, r3, LSL #5                                                                            \t\n\
            MCR p15, 0, r2, c7, c6, 2                                                                     \t\n\
            ADD r3, r3, #1                                                                                \t\n\
            CMP r0, r3                                                                                    \t\n\
            BGT set_loop                                                                                  \t\n\
            ADD r1, r1, #1                                                                                \t\n\
            CMP r1, #4                                                                                    \t\n\
            BNE way_loop                                                                                  \t\n\
                                                                                                          \t\n\
            MCR p15, 0, r1, c8, c7, 0                                                                     \t\n\
                                                                                                          \t\n\
            LDR r0, =0x10c0a                                                                                \t\n\
            LDR r1, =0x07f00000                                                                             \t\n\
            LDR r3, = 4095                                                                                \t\n\
        write_pte:                                                                                        \t\n\
            ORR r2, r0, r3, LSL #20                                                                       \t\n\
            STR r2, [r1, r3, LSL #2]                                                                      \t\n\
            SUB r3, r3, #1                                                                                \t\n\
            CMP r3, #-1                                                                                   \t\n\
            BNE write_pte                                                                                 \t\n\
                                                                                                          \t\n\
            BIC r0, r0, #0xC                                                                              \t\n\
            ORR r0, r0, #0X4                                                                              \t\n\
            BIC r0, r0, #0x7000                                                                           \t\n\
            ORR r0, r0, #0x5000                                                                           \t\n\
            ORR r0, r0, #0x10000                                                                          \t\n\
            STR r0, [r1]                                                                                  \t\n\
                                                                                                          \t\n\
            MOV r1,#0x0                                                                                   \t\n\
            MCR p15, 0, r1, c2, c0, 2                                                                     \t\n\
            LDR r1, =0x07f00000                                                                             \t\n\
            MCR p15, 0, r1, c2, c0, 0                                                                     \t\n\
                                                                                                          \t\n\
            LDR r1, =0x55555555                                                                           \t\n\
            MCR p15, 0, r1, c3, c0, 0                                                                     \t\n\
                                                                                                          \t\n\
            MRC p15, 0, r1, c1, c0, 0                                                                     \t\n\
            ORR r1, r1, #0x1                                                                              \t\n\
            MCR p15, 0, r1, c1, c0, 0                                                                     \t\n\
                                                                                                          \t\n\
            b _mcu_start                                                                                      \t\n\
        ");
}
