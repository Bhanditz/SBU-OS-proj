#include <sys/pic.h>
#include <sys/isr.h>
#include <sys/gdt.h>
#include <sys/sbunix.h>
#include <sys/proc.h>
#include <sys/paging.h>

volatile uint32_t usec;
volatile uint32_t offset;
volatile uint32_t colon;

//isr_32
void t_write();
void t_print_Ch(char); 
void time_write(int);
void clear_timer();

void timer_handler() {
	uint64_t 	tmp = usec;
	uint32_t 	tmpSec = 0;
	uint32_t 	tmpMin = 0;
	uint32_t 	sec = 0;
	uint32_t 	min = 0;
	uint32_t 	hour = 0;

	if(tmp >= 100) {
		sec = tmp / 100;
		tmpSec = sec % 60;
	} 
	if(sec >= 60) {
		min = sec / 60;
		tmpMin = min % 60;
	}
	if(min >= 60) {
		hour = min / 60;
	}
	tmp %= 100;
	tmp += (tmpSec*100);
	tmp += (tmpMin*10000);
	tmp += (hour*1000000);

    time_write(tmp);
    usec++;
    pic_sendEOI(32);
    offset = 0;
    colon = 0;

    //---------------------- scheduler --------------------------
    uint64_t			cur_rsp;
    task_struct			*next_proc;
    task_struct			*prev_proc = cur_proc;

    //when scheduler is on
    if(schedule_flag) {
    	sleep_cnt_update();

    	if(!cur_proc) {
			next_proc = get_next_proc();
			//printf("next_proc: %p\n", next_proc);
			//printf("next_proc->proc_name: %s\n", next_proc->proc_name);
			//printf("next_proc->rsp: %p\n", next_proc->rsp);
	 		//context_switch(next_proc);
	 		load_cr3(next_proc->mm_struct->pgd_t);
			//in_rsp(next_proc->rsp);
			if (next_proc->mode == USER) {
		    	//tss.rsp0 = (uint64_t)&next_proc->k_stack[511]; //the top addr of the stack
		    	//switch_to_ring3();
		    }

    	} else {
    		//store the rsp position before switch
	    	cur_rsp = out_rsp();
	 		prev_proc->rsp = cur_rsp;
	 		add_proc(prev_proc);
	 		next_proc = get_next_proc();
	 		//context_switch(next_proc);
	 		load_cr3(next_proc->mm_struct->pgd_t);
			//in_rsp(next_proc->rsp);
			if (next_proc->mode == USER) {
		    	//tss.rsp0 = (uint64_t)&next_proc->k_stack[511]; //the top addr of the stack
		    	//switch_to_ring3();
		    }
	    }
    } 
}

void time_write(int argument) {
	uint32_t tmp = 0;
	while(argument > 0) {
		tmp = argument % 10;
		argument -= tmp;
		argument /= 10;
		t_print_Ch(tmp + '0');
	}
}

void t_print_Ch(char argument) {
	t_write(&argument, 1, 0x08);
}

void t_write(const char *fmt, int len, int color) {
	register char* 		pos;

	while(len && *fmt) {
		offset += 2;
		pos = (char*)0xFFFFFFFF800B8000 + 160*25 - offset - colon;
		*pos = *fmt;
		*(pos+1) = color;
		if((offset-2)%4 == 0 && offset != 2) {
			*(pos+2) = ':';
			*(pos+3) = color;
		}
		fmt++;
		len--;
		if(offset%4 == 0) {
			colon += 2;
		}
	}
}

void clear_timer() {
	//....
}


