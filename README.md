# Description
The repository contains a system workload generator and a system monitor  


# Monitor  

## implementation
Use ncurses library to display the stats. There are total four threads in the program, one thread for reading stats of CPU, one for reading stats of disks io, one for display system stats and read stats of memory, main thread is use for reading user keyboard input.

```
monitor  
└───── display (call mem_stat)
         ├─ cpu_stat
         └─ io_stat
```

### CPU
Read /proc/stat to get the information of CPU, would block while calculate CPU utilization.

### memory
Read /proc/meminfo to get the information of memory, would not block.  
Memory_utilization = MemTotal - MemFree - Buffers - Cached - SReclaimable + Shmem

### disk io
Read /proc/diskstats to the information of disk io, would bolck while reading. 

## compile
use comannd `make monitor` to build the system monitor

## usage
run `./monitor.out`

# Workload  

## implementation
The program use `fork()` to create child processes to generate workload.

```
main (call memloadgen)
  ├── ioloadgen
  ├── cpuloadgen
  ├──     .
  ├──     .
  ├──     .
  └── cpuloadgen

```
### CPU
Fork child processes to the number of CPU cores, and then make the time of work and sleep into provided cpu_usage.  
The parent process `main` would periodically send signal to child processes to make them sleep.

### memory
Use `malloc` to allocate memory according to provided mem_usage, and then use `mlock` to prevent the allocated memory from swapping out.

### disk io
Use `write` system call to bytes according to io_usage and io_max in specific time interval.

## compile
use comannd `make load` to build the system workload generator

## usage

```
usage: workload [-c cpu_usage] [-m mem_usage] [-i io_usage io_max_speed [path_to_tempfile]]  
  options:  
    -c cpu_usage								stress the utilization of CPU to cpu_usage  
    -m mem_usage								stress the utilization of memory to mem_usage  
    -i io_usage io_max_speed path_to_tempfile	 stress the utilization of disk io to io_usage according to io_max_speed(kB/s), the default path_to_tempfile is ./tempfile
```

