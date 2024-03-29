// EPOS RealView PBX-A9 MCU Metainfo and Configuration

#ifndef __machine_traits_h
#define __machine_traits_h

#include <system/config.h>

__BEGIN_SYS

class Machine_Common;
template<> struct Traits<Machine_Common>: public Traits<void>
{
    static const bool debugged = Traits<void>::debugged;
};

template<> struct Traits<Machine>: public Traits<Machine_Common>
{
    static const unsigned int CPUS = Traits<Build>::CPUS;
    static const unsigned int NOT_USED    = 0xffffffff;

    // Physical Memory
    static const unsigned int MEM_BASE    = 0x00000000;
    static const unsigned int MEM_TOP     = 0x07ffffff; // 8 KB (MAX for 32-bit is 0x70000000 / 1792 MB)
    static const unsigned int BOOT_STACK  = 0x07effffc; // MEM_TOP - sizeof(int)
    static const unsigned int BOOT        = 0x00000000;
    static const unsigned int VECTOR_TBL  = 0x00010000;
    static const unsigned int TTB_ADDRS   = 0x07f00000;

    // Logical Memory Map
    static const unsigned int APP_LOW     = 0x00000000;
    static const unsigned int APP_CODE    = 0x00010000;
    static const unsigned int APP_DATA    = 0x00010000;
    static const unsigned int APP_HIGH    = 0x07ffffff;

    static const unsigned int PHY_MEM     = 0x80000000;
    static const unsigned int IO_BASE     = 0xf0000000;
    static const unsigned int IO_TOP      = 0xff400000;

    static const unsigned int SYS         = 0xff400000;
    static const unsigned int SYS_CODE    = 0xff700000; // Library mode only => APP + SYS
    static const unsigned int SYS_DATA    = 0xff740000; // Library mode only => APP + SYS

    // Default Sizes and Quantities
    static const unsigned int STACK_SIZE  = 16348;
    static const unsigned int HEAP_SIZE   = 16777216;
    static const unsigned int MAX_THREADS = 16;

    static const unsigned int SETUP             = NOT_USED;
    static const unsigned int INIT              = NOT_USED;
    static const unsigned int BOOT_LENGTH_MIN   = NOT_USED;
    static const unsigned int BOOT_LENGTH_MAX   = NOT_USED;
};

template<> struct Traits<IC>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<Timer>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;

    // Meaningful values for the timer frequency range from 100 to
    // 10000 Hz. The choice must respect the scheduler time-slice, i. e.,
    // it must be higher than the scheduler invocation frequency.
    static const int FREQUENCY = 1000; // Hz
};

template<> struct Traits<UART>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 4;

    static const unsigned int CLOCK = Traits<CPU>::CLOCK;

    static const unsigned int DEF_UNIT = 1;
    static const unsigned int DEF_BAUD_RATE = 115200;
    static const unsigned int DEF_DATA_BITS = 8;
    static const unsigned int DEF_PARITY = 0; // none
    static const unsigned int DEF_STOP_BITS = 1;
};

template<> struct Traits<USB>: public Traits<Machine_Common>
{
    static const bool enabled = false;
    static const unsigned int UNITS = 0;
    static const bool blocking = true;
};

template<> struct Traits<Scratchpad>: public Traits<Machine_Common>
{
    static const bool enabled = false;
};

__END_SYS

#endif
