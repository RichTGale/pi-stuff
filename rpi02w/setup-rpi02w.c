#include <stdlib.h>
#include <stdbool.h>

#include "../mycutils/mycutils.h"
#include "../clocks/timer_sec/timer_sec.h"

int main()
{
    log* l;
    timer_sec* frm_timer;
    bool run;
    int run_frms;
    int frm_cnt;

    clear();

    l = log_init("log.txt");
    l->out(l->fs, "Program Started.\n");

    frm_timer = timer_sec_init(l);
    run = true;
    run_frms = 5;
    frm_cnt = 0;

    while (run)
    {
        if (timer_sec_alarm(*frm_timer, 1, l))
        {
            fsout(stdout, "A frame ran...\n");
            l->out(l->fs, "A frame ran...\n");
            frm_cnt++;
            timer_sec_reinit(frm_timer, l);
        }

        if (frm_cnt >= run_frms)
        {
            run = false;
        }
    }
    
    timer_sec_term(frm_timer);
    log_term(l); 

    exit(EXIT_SUCCESS);
}
