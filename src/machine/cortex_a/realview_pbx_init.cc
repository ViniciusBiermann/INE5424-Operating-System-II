// EPOS RealView PBX A9 (ARM Cortex A9) MCU Initialization

#include <machine/main.h>

#ifdef __mmod_realview_pbx__

void _startup_cache();

__BEGIN_SYS

void REALVIEWPBXA9::pre_init() {

    ASM("MCR p15, 0, %0, c12, c0, 0"::"p"(Traits<Machine>::VECTOR_TBL));

    if(Traits<System>::multicore) {

        if (Traits<System>::multicore) {

            ASM("\t\n\
                //Enable SCU                                                                                            \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                             \t\n\
                LDR     r1, [r0, #0x0]          // Read the SCU Control Register                                        \t\n\
                ORR     r1, r1, #0x1            // Set bit 0 (The Enable bit)                                           \t\n\
                STR     r1, [r0, #0x0]          // Write back modifed value                                             \t\n\
                                                                                                                        \t\n\
                //Secure SCU Invalidate                                                                                 \t\n\
                AND     r0, r0, #0x03           // Mask off unused bits of CPU ID                                       \t\n\
                MOV     r0, r0, LSL #2          // Convert into bit offset (four bits per core)                         \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of ways                                         \t\n\
                MOV     r1, r1, LSL r0          // Shift ways into the correct CPU field                                \t\n\
                MRC     p15, 4, r2, c15, c0, 0  // Read periph base address                                             \t\n\
                STR     r1, [r2, #0x0C]         // Write to SCU Invalidate All in Secure State                          \t\n\
                                                                                                                        \t\n\
                //Join SMP                                                                                              \t\n\
                MOV     r0, #0x0                // Move CPU ID into r0                                                  \t\n\
                MOV     r1, #0xF                // Move 0xF (represents all four ways) into r1                          \t\n\
                                                                                                                        \t\n\
                //Enable Maintenance broadcast                                                                          \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read Aux Ctrl register                                               \t\n\
                ORR     r0, r0, #0x01           // Set the FW bit (bit 0)                                               \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write Aux Ctrl register                                              \t\n\
                                                                                                                        \t\n\
                //Enable GIC                                                                                            \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                             \t\n\
                ADD     r0, r0, #0x1000         // Add the GIC offset                                                   \t\n\
                LDR     r1, [r0]                // Read the GIC's Enable Register  (ICDDCR)                             \t\n\
                ORR     r1, r1, #0x01           // Set bit 0, the enable bit                                            \t\n\
                STR     r1, [r0]                // Write the GIC's Enable Register  (ICDDCR)                            \t\n\
                                                                                                                        \t\n\
                // Conteúdo dos registradores?                                                                          \t\n\
                //SGI to secondary CPUs                                                                                 \t\n\
                AND     r3, r0, #0x0F           // Mask off unused bits of ID, and move to r3                           \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of target_filter                                \t\n\
                AND     r2, r2, #0x0F           // Mask off unused bits of filter_list                                  \t\n\
                                                                                                                        \t\n\
                ORR     r3, r3, r1, LSL #16     // Combine ID and target_filter                                         \t\n\
                ORR     r3, r3, r2, LSL #24     // and now the filter list                                              \t\n\
                                                                                                                        \t\n\
                // Get the address of the GIC                                                                           \t\n\
                MRC     p15, 4, r0, c15, c0, 0  // Read periph base address                                             \t\n\
                ADD     r0, r0, #0x1F00         // Add offset of the sgi_trigger reg                                    \t\n\
                                                                                                                        \t\n\
                STR     r3, [r0]                // Write to the Software Generated Interrupt Register  (ICDSGIR)        \t\n\
                                                                                                                        \t\n\
                // Conteúdo dos registradores?                                                                          \t\n\
                // Clear the BSS                                                                                        \t\n\
                eor     r0, r0                                                                                          \t\n\
                ldr     r1, =__bss_start__                                                                              \t\n\
                ldr     r2, =__bss_end__                                                                                \t\n\
        .L1:    str     r0, [r1]                                                                                        \t\n\
                add     r1, #4                                                                                          \t\n\
                cmp     r1, r2                                                                                          \t\n\
                blt     .L1                                                                                             \t\n\
                                                                                                                        \t\n\
            ");

        } else {

            ASM("\t\n\
                // Secure SCU Invalidate                                                                                \t\n\
                AND     r0, r0, #0x03           // Mask off unused bits of CPU ID                                       \t\n\
                MOV     r0, r0, LSL #2          // Convert into bit offset (four bits per core)                         \t\n\
                AND     r1, r1, #0x0F           // Mask off unused bits of ways                                         \t\n\
                MOV     r1, r1, LSL r0          // Shift ways into the correct CPU field                                \t\n\
                MRC     p15, 4, r2, c15, c0, 0  // Read periph base address                                             \t\n\
                STR     r1, [r2, #0x0C]         // Write to SCU Invalidate All in Secure State                          \t\n\
                                                                                                                        \t\n\
                //Join SMP                                                                                              \t\n\
                MOV     r0, #0x0                // Move CPU ID into r0                                                  \t\n\
                MOV     r1, #0xF                // Move 0xF (represents all four ways) into r1                          \t\n\
                                                                                                                        \t\n\
                //Enable Maintenance broadcast                                                                          \t\n\
                MRC     p15, 0, r0, c1, c0, 1   // Read Aux Ctrl register                                               \t\n\
                ORR     r0, r0, #0x01           // Set the FW bit (bit 0)                                               \t\n\
                MCR     p15, 0, r0, c1, c0, 1   // Write Aux Ctrl register                                              \t\n\
                                                                                                                        \t\n\
            ");

        }

    }

}

void REALVIEWPBXA9::init() {
    
}

__END_SYS

#endif