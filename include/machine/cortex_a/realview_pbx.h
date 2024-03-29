// EPOS RealView PBX-A9 (ARM Cortex-A9) MCU Mediator Declarations

#ifndef __realviewpbxa9_h
#define __realviewpbxa9_h

#include <architecture/cpu.h>
#include <architecture/tsc.h>
#include <machine/rtc.h>
#include <system.h>

__BEGIN_SYS

class REALVIEWPBXA9
{
    friend class TSC;

protected:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    static const unsigned int IRQS = 96;
    static const unsigned int TIMERS = 4;
    static const unsigned int UARTS = 4;
    static const unsigned int GPIO_PORTS = 3;
    static const bool supports_gpio_power_up = false;

    // Base addresses for memory-mapped I/O devices
    enum {
        WDT0_BASE                   = 0x1000f000, // Watchdog Timer 0
        WDT1_BASE                   = 0x10010000, // Watchdog Timer 1

        GPIO0_BASE                  = 0x10013000, // GPIO0
        GPIO1_BASE                  = 0x10014000, // GPIO1
        GPIO2_BASE                  = 0x10015000, // GPIO2

        USART_BASE                  = 0x1000d000, // Synchronous Serial Port
        UART0_BASE                  = 0x10009000, // UART 0
        UART1_BASE                  = 0x1000a000, // UART 1
        UART2_BASE                  = 0x1000b000, // UART 2
        UART3_BASE                  = 0x1000c000, // UART 3

        TIMER0_BASE                 = 0x10011000, // Timer 0
        TIMER1_BASE                 = 0x10012000, // Timer 1
        TIMER2_BASE                 = 0x10018000, // Timer 2
        TIMER3_BASE                 = 0x10019000, // Timer 3

        SCR0_BASE                   = 0x10001000, // System Control 0
        SCR1_BASE                   = 0x1001a000, // System Control 1
        SYSTEM_CONTROLLER_BASE      = 0x10001000,

        FLASH0_BASE                 = 0x18000300, // Flash Controller

        GIC1_BASE                   = 0x10040000, // GIC 1 nIRQ
        GIC2_BASE                   = 0x1e020000, // GIC 2 nFIQ
        GIC3_BASE                   = 0x1e030000, // GIC 3 nIRQ
        GIC4_BASE                   = 0x10070000, // GIC 4 nFIQ

        PERIPHERAL_BASE             = 0x1f000000,

    };

    // PERIPHERAL_BASE Offsets
    enum {
        SCU                 = 0X0000,
        GIC                 = 0X0100,
        GLOBAL_TIMER        = 0X0200,
        PRIVATE_TIMERS      = 0x0600,
        INT_DIST            = 0x1000,
    };

    // System Control Registers offsets to SCR_BASE
    enum {
        SRC_ID              = 0x000, // System Identity
        SRC_SW              = 0x004,
        SRC_LED             = 0x008,
        SRC_OSC0            = 0x00C,
        SRC_OSC1            = 0x010,
        SRC_OSC2            = 0x014,
        SRC_OSC3            = 0x018,
        SRC_OSC4            = 0x01C,
        SRC_LOCK            = 0x020,
        SRC_HUNDRED_HZ      = 0x024,
        SRC_CONFIGDATA      = 0x028,
        SRC_FLAGS           = 0x030,
        SRC_FLAGSSET        = 0x030,
        SRC_FLAGSCLR        = 0x034,
        SRC_NVFLAGS         = 0x038,
        SRC_NVFLAGSSET      = 0x038,
        SRC_NVFLAGSCCLR     = 0x03C,
        SRC_PCICTL          = 0x044,
        SRC_MCI             = 0x048,
        SRC_FLASH           = 0x04C,
        SRC_CLCD            = 0x050,
        SRC_CLCDSER         = 0x054,
        SRC_BOOTCS          = 0x058,
        SRC_TWENTY_FOUR_MHZ = 0x05C,
        SRC_MISC            = 0x060,
        SRC_DMAPSR0         = 0x064,
        SRC_DMAPSR1         = 0x068,
        SRC_DMAPSR2         = 0x06C,
        SRC_IOSEL           = 0x070,
        SRC_PLDCTL          = 0x074,
        SRC_BUSID           = 0x080,
        SRC_PROCID          = 0x084,
        SRC_OSCRESET        = 0x08C,
        SRC_VOLTAGE         = 0x0A0,
        SRC_TEST_OCS        = 0x0D4,
    };

    // GIC CPU Offsets
    enum {
        GIC_CPU_CONTROL      = 0x000,
        GIC_PRIORITY_CONTROL = 0x004,
        GIC_BINARY_POINT     = 0x008,
        GIC_INT_ACK          = 0x00C,
        GIC_EOI              = 0x010,
        GIC_RUNNING_INT      = 0x014,
        GIC_HIGHEST_INT      = 0x018,
    };

    // GIC Distributtors Offsets
    enum {
        GIC_DIST_CONTROL_REGISTER = 0x000,
        GIC_DIST_SETENABLE0       = 0x100,
        GIC_DIST_SETENABLE1       = 0x104,
        GIC_DIST_SETENABLE2       = 0x108,
        GIC_DIST_CLEARENABLE0     = 0x180,
        GIC_DIST_CLEARENABLE1     = 0x184,
        GIC_DIST_CLEARENABLE2     = 0x188,
    };

    // Global Timer Registers offsets
    enum {                                      // Description
        GTCTRL                      = 0x00,     // Low Counter
        GTCTRH                      = 0x04,     // High Counter
        GTCLR                       = 0x08,     // Control
        GTISR                       = 0x0C      // Interrupt Status
    };

    // Useful bits in GTCLR
    enum {                                         // Description                  Type    Value after reset
        GT_TIMER_ENABLE                = 1 << 0,   // Enable                       r/w     0
        GT_IRQ_EN                      = 1 << 2    // Enable interrupt             r/w     0
    };

    // Private Timer Registers offsets
    enum {                                      // Description
        PTLR                        = 0x00,     // Load
        PTCTR                       = 0x04,     // Counter
        PTCLR                       = 0x08,     // Control
        PTISR                       = 0x0C      // Interrupt Status
    };

    // Useful bits in PTCLR
    enum {                                         // Description                  Type    Value after reset
        PT_TIMER_ENABLE                = 1 << 0,   // Enable                       r/w     0
        PT_AUTO_RELOAD                 = 1 << 1,   // Auto reload                  r/w     0
        PT_IRQ_EN                      = 1 << 2    // Enable interrupt             r/w     0
    };

    // Useful bits in PTISR
    enum {                                          // Description                  Type    Value after reset
        PT_INT_CLR                     = 1 << 0     // Interrupt clear bit          r/w     0
    };

protected:
    REALVIEWPBXA9() {}

    static void reboot() {
        // Reg32 val = scs(AIRCR) & (~((-1u / VECTKEY) * VECTKEY));
        // val |= 0x05fa * VECTKEY | SYSRESREQ;
        // scs(AIRCR) = val;
    }

    static void delay(const RTC::Microsecond & time) {
        // assert(Traits<TSC>::enabled);
        // unsigned long long ts = static_cast<unsigned long long>(time) * TSC::frequency() / 1000000;
        // tsc(GPTMTAILR) = ts;
        // tsc(GPTMTAPR) = ts >> 32;
        // tsc(GPTMCTL) |= TAEN;
        // while(!(tsc(GPTMRIS) & TATO_INT));
        // tsc(GPTMCTL) &= ~TAEN;
        // tsc(GPTMICR) |= TATO_INT;
    }

    static const UUID & uuid() { return System::info()->bm.uuid; } // TODO: System_Info is not populated in this machine

    static unsigned int cpu_id() {
        int id;
        ASM("mrc p15, 0, %0, c0, c0, 5" : "=r"(id) : : );
        return id & 0x3;
    }

    // Device enabling
    static void enable_uart(unsigned int unit) {
        // assert(unit < UARTS);
        // power_uart(unit, FULL);
        // gpioa(AFSEL) |= 3 << (unit * 2);                // Pins A[1:0] are multiplexed between GPIO and UART 0. Select UART.
        // gpioa(DEN) |= 3 << (unit * 2);                  // Enable digital I/O on Pins A[1:0]
    }
    static void enable_usb(unsigned int unit) {}

    // Power Management
    static void power_uart(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < UARTS);
        // switch(mode) {
        // case ENROLL:
        //  break;
        // case DISMISS:
        //  break;
        // case SAME:
        //  break;
        // case FULL:
        //  break;
        // case LIGHT:
        //  break;
        // case SLEEP:
        //     scr(RCGC1) |= 1 << unit;                   // Activate UART "unit" clock
        //     scr(RCGC2) |= 1 << unit;                   // Activate port "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC1) &= ~(1 << unit);                // Deactivate UART "unit" clock
        //     scr(RCGC2) &= ~(1 << unit);                // Deactivate port "unit" clock
        //     break;
        // }
    }

    static void power_user_timer(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < TIMERS);
        // switch(mode) {
        // case ENROLL:
        //  break;
        // case DISMISS:
        //  break;
        // case SAME:
        //  break;
        // case FULL:
        //  break;
        // case LIGHT:
        //  break;
        // case SLEEP:
        //     scr(RCGC1) |= 1 << (unit + 16);             // Activate GPTM "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC1) &= ~(1 << (unit + 16));          // Deactivate GPTM "unit" clock
        //     break;
        // }
    }

    static void power_usb(unsigned int unit, const Power_Mode & mode) {}


    // GPIO
    static void gpio_init() {}
    static void power_gpio(unsigned int unit, const Power_Mode & mode) {
        // assert(unit < UARTS);
        // switch(mode) {
        // case ENROLL:
        //  break;
        // case DISMISS:
        //  break;
        // case SAME:
        //  break;
        // case FULL:
        //  break;
        // case LIGHT:
        //  break;
        // case SLEEP:
        //     scr(RCGC2) |= 1 << unit;                   // Activate port "unit" clock
        //     break;
        // case OFF:
        //     scr(RCGC2) &= ~(1 << unit);                // Deactivate port "unit" clock
        //     break;
        // }
    }
    // void gpio_pull_up(unsigned int port, unsigned int pin) { gpio(port, PUR) &= 1 << pin; }
    // void gpio_pull_down(unsigned int port, unsigned int pin) { gpio(port, PDR) &= 1 << pin; }
    // void gpio_floating(unsigned int port, unsigned int pin) { gpio(port, ODR) &= 1 << pin; }

    // // ADC (not implemented for this model)
    // static void adc_config(unsigned char channel);

    // // PWM (not implemented for this model)
    // static void pwm_config(unsigned int timer, char gpio_port, unsigned int gpio_pin);

    // // IEEE 802.15.4 (not present in this model)
    // static void power_ieee802_15_4(const Power_Mode & mode);

public:
    static volatile Reg32 & int_dist(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(PERIPHERAL_BASE + INT_DIST)[o / sizeof(Reg32)]; }
    static volatile Reg32 & gic(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(PERIPHERAL_BASE + GIC)[o / sizeof(Reg32)]; }
    static volatile Reg32 & global_timer(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(PERIPHERAL_BASE + GLOBAL_TIMER)[o / sizeof(Reg32)]; }
    static volatile Reg32 & priv_timer(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(PERIPHERAL_BASE + PRIVATE_TIMERS)[o / sizeof(Reg32)]; }

protected:
    static void pre_init();
    static void init();
};

typedef REALVIEWPBXA9 Machine_Model;

__END_SYS

#endif
