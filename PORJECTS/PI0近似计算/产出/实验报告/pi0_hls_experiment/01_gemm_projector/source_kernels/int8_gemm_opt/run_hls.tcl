open_project int8_gemm_opt_prj
set_top int8_gemm_opt_kernel
add_files kernel.cpp
add_files -tb tb.cpp
open_solution "solution1" -flow_target vivado
set_part {xcvu9p-flga2104-2-i}
create_clock -period 10 -name default
csim_design
csynth_design
exit
