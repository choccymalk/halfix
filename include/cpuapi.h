#ifndef CPUAPI_H
#define CPUAPI_H

// Common CPU API
#include "util.h"
#include <stdint.h>

enum
{
    FEATURE_EAX_1 = 0,
    FEATURE_EAX_80000000,
    FEATURE_SIZE_MAX
};
struct cpuid_level_info
{
    int eax;
    int ecx;
    int edx;
    int ebx;

    int level;
};

struct cpu_config
{
    char *vendor_name;
    int level;

    struct cpuid_level_info features[FEATURE_SIZE_MAX];
};

int cpu_init(void);
void cpu_reset(void);
int cpu_init_mem(int size);
int cpu_add_rom(int addr, int size, void *data);
int cpu_set_cpuid(struct cpu_config *cfg);

// Clear a page of code translations
void cpu_invalidate_page(uint32_t addr);
void cpu_write_memory(uint32_t addr, void* data, uint32_t length);

// Necessary for proper timing
int cpu_in_hlt(void);
void cpu_add_cycles(itick_t);
uint64_t cpu_get_cycles(void);

// Read the number of instructions executed. Useful for printing IPS values.
uint64_t cpu_get_real_cycles(void);

// Note: will do nothing on emulated CPU because it needs to return to the main thread anyways
void cpu_set_break(void);

// Don't return from CPU loop
#define EXIT_STATUS_NORMAL 0
// Exit the loop to raise an IRQ
#define EXIT_STATUS_IRQ 1 // Note: used internally. cpu_get_exit_reason will NOT return this value
// Exit out of the loop due to some kind of async operation
#define EXIT_STATUS_ASYNC 2
// Exit out of the loop because of a HLT instruction
#define EXIT_STATUS_HLT 3

// Requests that the CPU halt execution and return to the main loop
void cpu_request_fast_return(int);
// Raise the INTR line to the CPU
void cpu_raise_intr_line(void);
void cpu_lower_intr_line(void);
// Checks if the INTR line is high and IF is set
int cpu_interrupt_pending(void);

// Run "cycles" instructions. Returns number of cycles actually executed
int cpu_run(int cycles);
// Get reason why CPU broke out of main loop. Reason is reset every loop.
int cpu_get_exit_reason(void);
void cpu_halt(void);

void cpu_raise_interrupt(int i);
void cpu_set_a20(int a20);

// Get a pointer to RAM
void *cpu_get_ram_ptr(void);

// Is there an APIC connected to the CPU in some way??
int cpu_apic_connected(void);

// Debug API
void cpu_debug(void);

#define MEM_RDONLY 1

#endif
