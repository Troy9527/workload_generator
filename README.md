# Description
The repository contains a system workload generator and a system monitor  


# Monitor  

## implementation
Use ncurses library to display the stats. There are total four threads in the program, one thread for reading stats of CPU, one for reading stats of disks io, one for display system stats and read stats of memory, main thread is use for reading user keyboard input.

### CPU
Read /proc/stat to get the information of CPU, would block while calculate CPU utilization.

### memory
Read /proc/meminfo to get the information of memory, would not block.  
Memory_utilization = MemTotal - MemFree - Buffers - Cached - SReclaimable + Shmem

### disk io
Read /proc/diskstats to the information of disk io, would bolck while reading. 


```
main  
└─ display  
     ├─ cpu_stat
     └─ io_stat
```

## compile
use comannd `make monitor` to build the system monitor

## usage
run `./monitor.out`

# Workload  

## compile
use comannd `make load` to build the system workload generator


## usage

```
usage: workload [-c cpu_usage] [-m mem_usage] [-i io_usage io_max_speed path_to_tempfile]  
  options:  
    -c cpu_usage				stress the utilization of CPU to cpu_usage  
    -m mem_usage				stress the utilization of memory to mem_usage  
    -i io_usage io_max_speed path_to_tempfile	stress the utilization of disk io to io_usage according to io_max_speed  
```

