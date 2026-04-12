#pragma once
#include "types.h"

struct sbiret {
    long error;
    long value;
};
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid);

struct sbiret sbi_set_timer(uint64 stime_value); 

