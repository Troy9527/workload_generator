# Description
The repository contains a system workload generator and a system monitor  


# Monitor  

## compile
use comannd `make monitor` to build the system monitor

## usage
run `./monitor.out`

# Workload  

## compile
use comannd `make load` to build the system workload generator

## usage
usage: workload [-c cpu_usage] [-m mem_usage] [-i io_usage io_max_speed path_to_tempfile]  
  options:  
    -c cpu_usage				stress the utilization of CPU to cpu_usage  
    -m mem_usage				stress the utilization of memory to mem_usage  
    -i io_usage io_max_speed path_to_tempfile	stress the utilization of disk io to io_usage according to io_max_speed  


