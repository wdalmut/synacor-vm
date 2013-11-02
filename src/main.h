#ifndef __MAIN_H__
#define __MAIN_H__

#define MODULO 32768

#define SHOW_BOOTSTRAP 1
#define DEBUG 0

void run_program(void);

void    halt_state(void);
void    set_state(void);
void    push_state(void);
void    pop_state(void);
void    eq_state(void);
void    gt_state(void);
void    jmp_state(void);
void    jt_state(void);
void    jf_state(void);
void    add_state(void);
void    mult_state(void);
void    mod_state(void);
void    and_state(void);
void    or_state(void);
void    not_state(void);
void    rmem_state(void);
void    wmem_state(void);
void    call_state(void);
void    ret_state(void);
void    out_state(void);
void    in_state(void);
void    noop_state(void);

#endif
