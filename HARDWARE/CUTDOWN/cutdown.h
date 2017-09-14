#ifndef __CUTDOWN_H
#define __CUTDOWN_H  
#include "sys.h"

void cutdown_running(void);
void cutdown(uint8_t key,uint8_t remote_key);
void cutdown_set_display(uint8_t key,uint8_t remote_key);
#endif
