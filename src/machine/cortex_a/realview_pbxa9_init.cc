// EPOS RealView PBX A9 (ARM Cortex A9) MCU Initialization

#include <machine/main.h>

#ifdef __mmod_realview_pbx__

void _startup_cache();

__BEGIN_SYS

void REALVIEWPBXA9::pre_init()
{
    ASM("MCR p15, 0, %0, c12, c0, 0"::"p"(Traits<Machine>::VECTOR_TBL));
/*    // Initialize the clock
    CPU::Reg32 rcc = scr(RCC);

    // bypass PLL and system clock divider while initializing
    rcc |= RCC_BYPASS;
    rcc &= ~RCC_USESYSDIV;
    scr(RCC) = rcc;

    // select the crystal value and oscillator source
    rcc &= ~RCC_XTAL_8192;
    rcc |= RCC_XTAL_6000;
    rcc &= ~RCC_IOSC4;
    rcc |= RCC_MOSC;

    // activate PLL by clearing PWRDN and OEN
    rcc &= ~RCC_PWRDN;
    rcc &= ~RCC_OEN;

    // set the desired system divider and the USESYSDIV bit
    rcc &= ~RCC_SYSDIV_16;
    // Clock setup
    Reg32 sys_div;
    switch(Traits<CPU>::CLOCK) {
        default:
        case 50000000: sys_div = RCC_SYSDIV_4; break;
        case 40000000: sys_div = RCC_SYSDIV_5; break;
        case 25000000: sys_div = RCC_SYSDIV_8; break;
        case 20000000: sys_div = RCC_SYSDIV_10; break;
        case 12500000: sys_div = RCC_SYSDIV_16; break;
    }
    rcc |= sys_div;
    rcc |= RCC_USESYSDIV;
    scr(RCC) = rcc;

    // wait for the PLL to lock by polling PLLLRIS
    while(!(scr(RIS) & RIS_PLLLRIS));

    // enable use of PLL by clearing BYPASS
    rcc &= ~RCC_BYPASS;
    scr(RCC) = rcc;*/
}

void REALVIEWPBXA9::init()
{
    // db<Init, Machine>(TRC) << "REALVIEWPBXA9::init:CCR = " << scs(CCR) << endl;
    // scs(CCR) |= BASETHR; // QEMU bug: on REALVIEWPBXA9 this register is not updated, but it doesn't seem to cause any errors
    // db<Init, Machine>(TRC) << "REALVIEWPBXA9::init:CCR = " << scs(CCR) << endl;
}

__END_SYS

#endif