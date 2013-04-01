.globl   _isr_wrapper
.align   4
 
_isr_wrapper:
    pushad
    call    _interrupt_handler
    popad
    iret
