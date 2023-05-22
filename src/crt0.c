#include <stdint.h>

extern uint32_t _stack_top;
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;
extern void _start(void), main(void);
extern void __libc_init_array(void);
extern void uart_interrupt_handler(void);

__attribute__ ((interrupt()))
void bus_error_handler()
{}	

__attribute__ ((interrupt()))
void address_error_handler()
{}

__attribute__ ((interrupt()))
void illegal_instruction_handler()
{}	

__attribute__ ((interrupt()))
void divide_by_zero_handler()
{}

__attribute__ ((interrupt()))
void chk_instruction_handler()
{}

__attribute__ ((interrupt()))
void trapv_instruction_handler()
{}

__attribute__ ((interrupt()))
void privilidge_violation_handler()
{}

__attribute__ ((interrupt()))
void trace_handler()
{}

__attribute__ ((interrupt()))
void line_1010_handler()
{}

__attribute__ ((interrupt()))
void line_1111_handler()
{}

__attribute__ ((interrupt()))
void unassigned_exception_handler()
{}

__attribute__ ((interrupt()))
void uninitialised_vector_handler()
{}

__attribute__ ((interrupt()))
void spurious_interrupt_handler()
{}

__attribute__ ((interrupt()))
void level_1_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_2_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_3_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_4_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_5_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_6_autovector_handler()
{}

__attribute__ ((interrupt()))
void level_7_autovector_handler()
{}

__attribute__ ((interrupt()))
void trap_0_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_1_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_2_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_3_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_4_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_5_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_6_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_7_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_8_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_9_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_10_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_11_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_12_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_13_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_14_vector_handler()
{}

__attribute__ ((interrupt()))
void trap_15_vector_handler()
{}

__attribute__ ((interrupt()))
void unassigned_user_interrupt_handler()
{}

__attribute__ ((section(".vectors")))
void (*const vectors[])(void) =
{
    (void (*const)(void)) &_stack_top,
    &_start,
	bus_error_handler,	
	address_error_handler,
	illegal_instruction_handler,	
	divide_by_zero_handler,
    chk_instruction_handler,
    trapv_instruction_handler,
    privilidge_violation_handler,
	trace_handler,
	line_1010_handler,		
	line_1111_handler,	
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
    uninitialised_vector_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
/* spurious_interrupt_handler should be at 0x060 */
	spurious_interrupt_handler,
	level_1_autovector_handler,
	level_2_autovector_handler,
	level_3_autovector_handler,
	level_4_autovector_handler,
	level_5_autovector_handler,
	level_6_autovector_handler,
	level_7_autovector_handler,
/* trap_0_vector_handler should be at 0x080 */
	trap_0_vector_handler,
	trap_1_vector_handler,
	trap_2_vector_handler,
	trap_3_vector_handler,
	trap_4_vector_handler,
	trap_5_vector_handler,
	trap_6_vector_handler,
	trap_7_vector_handler,
	trap_8_vector_handler,
	trap_9_vector_handler,
	trap_10_vector_handler,
	trap_11_vector_handler,
	trap_12_vector_handler,
	trap_13_vector_handler,
	trap_14_vector_handler,
	trap_15_vector_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
	unassigned_exception_handler,
/* uart_interrupt_handler should be at 0x100 */
	uart_interrupt_handler,	
	unassigned_user_interrupt_handler /* 191 of these */
};

void _start()
{
    uint32_t *src, *dst;
    
    src = &_etext;
    dst = &_sdata;

    while (dst != &_edata) 
        *dst++ = *src++;

    dst = &_sbss;

    while (dst != &_ebss) 
        *dst++ = 0;

    //__libc_init_array();
    
    main();
}