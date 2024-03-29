// EPOS RealView PBX A9 (ARM Cortex A9) MCU Initialization

#include <machine/main.h>

#ifdef __mmod_realview_pbx__

void _startup_cache();

__BEGIN_SYS

void REALVIEWPBXA9::pre_init() {

    ASM("MCR p15, 0, %0, c12, c0, 0"::"p"(Traits<Machine>::VECTOR_TBL));

    if(Traits<System>::multicore) {

        if (Machine::cpu_id() == 0) {

            ASM("\t\n\
                // === Enable SCU ===                                                                                     \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                LDR     r1, [r0, #0x0]          // Read the SCU Control Register                                          \t\n\
                ORR     r1, r1, #0x1            // Set bit 0 (The Enable bit)                                             \t\n\
                STR     r1, [r0, #0x0]          // Write back modifed value                                               \t\n\
                                                                                                                          \t\n\
                // === Pre Join SMP ===                                                                                   \t\n\
                MOV     r0, #0x0                // Move CPU ID into r0                                                    \t\n\
                MOV     r1, #0xF                // Move 0xF (represents all four ways) into r1                            \t\n\
                                                                                                                          \t\n\
                // === Secure SCU Invalidate ===                                                                          \t\n\
                AND     r0, r0, #0x03           // Mask off unused bits of CPU ID                                         \t\n\
                MOV     r0, r0, LSL #2          // Convert into bit offset (four bits per core)                           \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of ways                                           \t\n\
                MOV     r1, r1, LSL r0          // Shift ways into the correct CPU field                                  \t\n\
                MRC     p15, 4, r2, c15, c0, 0  // Read periph base address                                               \t\n\
                STR     r1, [r2, #0x0C]         // Write to SCU Invalidate All in Secure State                            \t\n\
                                                                                                                          \t\n\
                // === Join SMP ===                                                                                       \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read ACTLR                                                             \t\n\
                ORR     r0, r0, #0x040          // Set bit 6                                                              \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write ACTLR                                                            \t\n\
                                                                                                                          \t\n\
                // === Enable Maintenance broadcast ===                                                                   \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read Aux Ctrl register                                                 \t\n\
                ORR     r0, r0, #0x01           // Set the FW bit (bit 0)                                                 \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write Aux Ctrl register                                                \t\n\
                                                                                                                          \t\n\
                // === Enable GIC ===                                                                                     \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                ADD     r0, r0, #0x1000         // Add the GIC offset                                                     \t\n\
                LDR     r1, [r0]                // Read the GIC's Enable Register  (ICDDCR)                               \t\n\
                ORR     r1, r1, #0x01           // Set bit 0, the enable bit                                              \t\n\
                STR     r1, [r0]                // Write the GIC's Enable Register  (ICDDCR)                              \t\n\
                                                                                                                          \t\n\
                // === Enable GIC processor interface ===                                                                 \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                LDR     r1, [r0, #0x100]        // Read the Processor Interface Control register   (ICCICR/ICPICR)        \t\n\
                ORR     r1, r1, #0x03           // Bit 0: Enables secure interrupts, Bit 1: Enables Non-Secure interrupts \t\n\
                STR     r1, [r0, #0x100]        // Write the Processor Interface Control register  (ICCICR/ICPICR)        \t\n\
                                                                                                                          \t\n\
                // Load on registers following the GIC seminar                                                            \t\n\
                LDR r0, =0x00010000 // All APs execute from 0x10000                                                       \t\n\
                LDR r1, =0x10000030 // SYS flagset register                                                               \t\n\
                STR r0, [r1]                                                                                              \t\n\
                                                                                                                          \t\n\
                // === Clear the BSS ===                                                                                  \t\n\
                eor     r0, r0                                                                                            \t\n\
                ldr     r1, =__bss_start__                                                                                \t\n\
                ldr     r2, =__bss_end__                                                                                  \t\n\
        .L1:    str     r0, [r1]                                                                                          \t\n\
                add     r1, #4                                                                                            \t\n\
                cmp     r1, r2                                                                                            \t\n\
                blt     .L1                                                                                               \t\n\
                                                                                                                          \t\n\
                // Load on registers following the GIC seminar                                                            \t\n\
                LDR r0, =0x0   // intID = 0                                                                               \t\n\
                LDR r1, =0x0F  // CPUs = 0xF                                                                              \t\n\
                LDR r2, =0x01  // filter = 0x01                                                                           \t\n\
                                                                                                                          \t\n\
                // === SGI to secondary CPUs ===                                                                          \t\n\
                AND     r3, r0, #0x0F           // Mask off unused bits of ID, and move to r3                             \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of target_filter                                  \t\n\
                AND     r2, r2, #0x0F           // Mask off unused bits of filter_list                                    \t\n\
                                                                                                                          \t\n\
                ORR     r3, r3, r1, LSL #16     // Combine ID and target_filter                                           \t\n\
                ORR     r3, r3, r2, LSL #24     // and now the filter list                                                \t\n\
                                                                                                                          \t\n\
                // Get the address of the GIC                                                                             \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                ADD     r0, r0, #0x1F00         // Add offset of the sgi_trigger reg                                      \t\n\
                                                                                                                          \t\n\
                STR     r3, [r0]                // Write to the Software Generated Interrupt Register  (ICDSGIR)          \t\n\
                                                                                                                          \t\n\
            ");

        } else {

            ASM("\t\n\
                // === Enable GIC processor interface ===                                                                 \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                LDR     r1, [r0, #0x100]        // Read the Processor Interface Control register   (ICCICR/ICPICR)        \t\n\
                ORR     r1, r1, #0x03           // Bit 0: Enables secure interrupts, Bit 1: Enables Non-Secure interrupts \t\n\
                STR     r1, [r0, #0x100]        // Write the Processor Interface Control register  (ICCICR/ICPICR)        \t\n\
                                                                                                                          \t\n\
                // === Setting priority Mask ===                                                                          \t\n\
                MOV     r0, #0x1F                                                                                         \t\n\
                MOV     r1, r0                  // Back up passed in ID value                                             \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                STR     r1, [r0, #0x0104]       // Write the Priority Mask register (ICCPMR/ICCIPMR)                      \t\n\
                                                                                                                          \t\n\
                // === Enable IRQ ID ===                                                                                  \t\n\
                MOV     r0, #0x0                // ID                                                                     \t\n\
                // Get base address of private perpherial space                                                           \t\n\
                MOV     r1, r0                  // Back up passed in ID value                                             \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                // Each interrupt source has an enable bit in the GIC.  These                                             \t\n\
                // are grouped into registers, with 32 sources per register                                               \t\n\
                // First, we need to identify which 32 bit block the interrupt lives in                                   \t\n\
                MOV     r2, r1                  // Make working copy of ID in r2                                          \t\n\
                MOV     r2, r2, LSR #5          // LSR by 5 places, affective divide by 32                                \t\n\
                                                // r2 now contains the 32 bit block this ID lives in                      \t\n\
                MOV     r2, r2, LSL #2          // Now multiply by 4, to covert offset into an address offset             \t\n\
                                                // (four bytes per reg)                                                   \t\n\
                // Now work out which bit within the 32 bit block the ID is                                               \t\n\
                AND     r1, r1, #0x1F           // Mask off to give offset within 32bit block                             \t\n\
                MOV     r3, #1                  // Move enable value into r3                                              \t\n\
                MOV     r3, r3, LSL r1          // Shift it left to position of ID                                        \t\n\
                ADD     r2, r2, #0x1100         // Add the base offset of the Enable Set registers to the offset for      \t\n\
                                                // the ID                                                                 \t\n\
                STR     r3, [r0, r2]            // Store out  (ICDISER)                                                   \t\n\
                                                                                                                          \t\n\
                // === Set IRQ Priority ===                                                                               \t\n\
                MOV     r0, #0x0                  // ID                                                                   \t\n\
                MOV     r1, #0x0                  // Priority                                                             \t\n\
                // Get base address of private perpherial space                                                           \t\n\
                MOV     r2, r0                  // Back up passed in ID value                                             \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                               \t\n\
                // Make sure that priority value is only 5 bits, and convert to expected format                           \t\n\
                AND     r1, r1, #0x1F                                                                                     \t\n\
                MOV     r1, r1, LSL #3                                                                                    \t\n\
                // Find which priority register this ID lives in                                                          \t\n\
                BIC     r3, r2, #0x03           // Make a copy of the ID, clearing off the bottom two bits                \t\n\
                // There are four IDs per reg, by clearing the bottom two bits we get an address offset                   \t\n\
                ADD     r3, r3, #0x1400         // Now add the offset of the Priority Level registers from the base of    \t\n\
                                                // the private peripheral space                                           \t\n\
                ADD     r0, r0, r3              // Now add in the base address of the private peripheral space, giving us \t\n\
                                                // the absolute address                                                   \t\n\
                // Now work out which ID in the register it is                                                            \t\n\
                AND     r2, r2, #0x03           // Clear all but the bottom four bits, leaves which ID in the reg it      \t\n\
                                                // is (which byte)                                                        \t\n\
                MOV     r2, r2, LSL #3          // Multiply by 8, this gives a bit offset                                 \t\n\
                // Read -> Modify -> Write                                                                                \t\n\
                MOV     r12, #0xFF              // Mask (8 bits)                                                          \t\n\
                MOV     r12, r12, LSL r2        // Move mask into correct bit position                                    \t\n\
                MOV     r1, r1, LSL r2          // Also, move passed in priority value into correct bit position          \t\n\
                LDR     r3, [r0]                // Read current value of the Priority Level register (ICDIPR)             \t\n\
                BIC     r3, r3, r12             // Clear appropiate field                                                 \t\n\
                ORR     r3, r3, r1              // Now OR in the priority value                                           \t\n\
                STR     r3, [r0]                // And store it back again  (ICDIPR)                                      \t\n\
                                                                                                                          \t\n\
                // === Pre Join SMP ===                                                                                   \t\n\
                MRC     p15, 0, r0, c0, c0, 5   // Read CPU ID register                                                   \t\n\
                ANDS    r0, r0, #0x03           // Mask off, leaving the CPU ID field                                     \t\n\
                MOV     r1, #0xf                // Move 0xF (represents all four ways) into r1                            \t\n\
                                                                                                                          \t\n\
                // === Secure SCU Invalidate ===                                                                          \t\n\
                AND     r0, r0, #0x03           // Mask off unused bits of CPU ID                                         \t\n\
                MOV     r0, r0, LSL #2          // Convert into bit offset (four bits per core)                           \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of ways                                           \t\n\
                MOV     r1, r1, LSL r0          // Shift ways into the correct CPU field                                  \t\n\
                MRC     p15, 4, r2, c15, c0, 0  // Read periph base address                                               \t\n\
                STR     r1, [r2, #0x0C]         // Write to SCU Invalidate All in Secure State                            \t\n\
                                                                                                                          \t\n\
                // === Join SMP ===                                                                                       \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read ACTLR                                                             \t\n\
                ORR     r0, r0, #0x040          // Set bit 6                                                              \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write ACTLR                                                            \t\n\
                                                                                                                          \t\n\
                // === Enable Maintenance broadcast ===                                                                   \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read Aux Ctrl register                                                 \t\n\
                ORR     r0, r0, #0x01           // Set the FW bit (bit 0)                                                 \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write Aux Ctrl register                                                \t\n\
                                                                                                                          \t\n\
            ");

        }

    }
}

void REALVIEWPBXA9::init() {
    
}

__END_SYS

#endif