## Files

1. `main.c` - The main C program that manages and measures the execution times of child processes running with different scheduling policies.
2. `sched_fifo.c`, `sched_rr.c`, and `sched_other.c` - These C programs perform computational tasks and serve as child processes with different scheduling policies.
3. `histogram.py` - A Python script to generate a histogram comparing the execution times of the different scheduling policies.
4. `Makefile` - A Makefile for compiling the C programs and generating the histogram.

## How to Use

1. **Compile the C Programs:**

    Use the provided Makefile to compile the C programs. Simply run the following command in your terminal

    make all

    This will compile all the C programs (`main.c`, `sched_fifo.c`, `sched_rr.c`, and `sched_other.c`) and generate the executable files.

2. **Run the Main Program:**

    Execute the main program to initiate child processes with different scheduling policies and measure their execution times:

    ./main

    The program will output the execution times for each scheduling policy and write these times to corresponding output files (`fifo_execution_time.txt`, `rr_execution_time.txt`, `other_execution_time.txt`).

    For a detailed explanation of what `main.c` does, refer to the [description of main.c](#mainc---scheduling-policy-comparison).

3. **Generate Execution Time Comparison:**

    Use the Makefile to generate a histogram comparing the execution times of the different scheduling policies:

    make plot

    This will execute the `histogram.py` script, which creates a `plot.png` file that visually represents the execution times for each scheduling policy.

4. **Review Results:**

    You can analyze the generated `plot.png` to compare the execution times between SCHED_FIFO, SCHED_RR, and SCHED_OTHER scheduling policies. This visualization can help you understand the performance differences.

## Cleaning Up

To remove the compiled executable files and any generated output files, you can use the Makefile's `clean` target:

make clean


This will remove the `main`, `sched_fifo`, `sched_other`, and `sched_rr` executables, as well as the `fifo_execution_time.txt`, `rr_execution_time.txt`, `other_execution_time.txt`, and `plot.png` files.

## Dependencies

- The C programs require a C compiler, such as GCC.
- The Python script `histogram.py` requires Matplotlib, a Python plotting library. You can install it using:

    pip install matplotlib

## Notes

- This project assumes a Unix-like operating system for scheduling policies.
- Ensure that the C programs (`sched_fifo.c`, `sched_rr.c`, and `sched_other.c`) contain the desired computational tasks for testing purposes.

### `main.c` - Scheduling Policy Comparison

The `main.c` file is the central component of the project, responsible for managing and measuring the execution times of child processes running under different scheduling policies in a Unix-like operating system. Here's a step-by-step breakdown of its functionality:

1. **Initialization**:
   - The program begins by including necessary header files for system calls (`stdio.h`, `stdlib.h`, `unistd.h`, `sys/types.h`, `sys/wait.h`, `sched.h`) and time-related functions (`time.h`).
   - It defines structs (`struct timespec`) to hold timing information for measuring execution times.

2. **Time Measurement**:
   - It starts measuring the total execution time of the program by calling `clock_gettime` and storing the current time in the `total_time_start` struct.

3. **SCHED_OTHER Process Creation**:
   - The program creates a child process for the `SCHED_OTHER` scheduling policy (the default policy).
   - It sets the child process's scheduling policy to `SCHED_OTHER` and priority to 0 using `sched_setscheduler`.
   - The child process then executes the "sched_other" program.
   - If any errors occur during process creation or scheduling policy setup, appropriate error messages are printed.

4. **SCHED_FIFO Process Creation**:
   - Similar to the previous step, the program creates a child process for the `SCHED_FIFO` scheduling policy.
   - It assigns a priority of 50 to the child process.
   - The child process executes the "sched_fifo" program.
   - Error handling is performed for process creation and scheduling policy setup.

5. **SCHED_RR Process Creation**:
   - This step is nearly identical to the previous two steps, but it creates a child process for the `SCHED_RR` scheduling policy.
   - The child process is assigned a priority of 50.
   - The child process executes the "sched_rr" program.
   - Error handling is in place for process creation and scheduling policy setup.

6. **Execution Time Measurement and Logging**:
   - After creating the child processes, the program waits for each child process to finish using `waitpid`.
   - For each scheduling policy (FIFO, RR, OTHER), it records the execution time by measuring the time before and after each child process's execution.
   - The execution times are printed to the console and written to corresponding output files (`fifo_execution_time.txt`, `rr_execution_time.txt`, `other_execution_time.txt`).

7. **Total Running Time Calculation**:
   - Finally, the program measures the total running time of the entire program by recording the time after all child processes have completed.
   - It calculates and prints the total execution time of the program to the console.

8. **Cleanup**:
   - The program finishes by returning 0 to indicate successful execution.

#Why the order FIFO<RR<OTHER?
    TLDR; 
    - `SCHED_FIFO` provides uninterrupted CPU access until the process voluntarily yields, resulting in the shortest execution times.
    - `SCHED_RR` introduces a time-sharing mechanism with minimal overhead, leading to slightly longer execution times due to context switching.
    - `SCHED_OTHER` uses dynamic priorities and can be preempted by higher-priority processes, potentially causing waiting times and longer execution times in busy environments.


    1. **SCHED_FIFO (First-Come-First-Served)**:
    - `SCHED_FIFO` follows a strict "first-come-first-served" scheduling policy. Once a process with `SCHED_FIFO` is scheduled, it runs until it voluntarily yields the CPU or a higher-priority process becomes ready.
    - Since there is no preemption within the same priority level, a `SCHED_FIFO` process can potentially monopolize the CPU until it explicitly relinquishes control.
    - In your code, you've assigned a relatively high priority (50) to the `SCHED_FIFO` process. This means that it will have a higher chance of being scheduled ahead of other processes, resulting in quick execution.
    - The key point here is that `SCHED_FIFO` processes do not voluntarily give up CPU control, making them execute without interruption until they complete their tasks.

    2. **SCHED_RR (Round Robin)**:
    - `SCHED_RR` is also a real-time scheduling policy, but it introduces a time-sharing mechanism among processes with the same priority.
    - Processes with `SCHED_RR` share the CPU in a round-robin fashion, meaning each process is given a time quantum (a fixed amount of time) to execute before being moved to the back of the queue.
    - After each time quantum, the scheduler selects the next `SCHED_RR` process in the queue to run.
    - While `SCHED_RR` ensures fairness among processes with the same priority, it incurs some overhead due to context switching. Context switching involves saving and restoring process state, which consumes some CPU time.
    - Consequently, `SCHED_RR` processes experience a small amount of overhead, resulting in slightly longer execution times compared to `SCHED_FIFO` processes.

    3. **SCHED_OTHER (Normal Scheduling)**:
    - `SCHED_OTHER` is the default scheduling policy for most user-level processes in a typical Linux system.
    - It uses a dynamic priority mechanism based on various factors, including the process's behavior and CPU load.
    - Processes with `SCHED_OTHER` priorities can be preempted by higher-priority processes, and they may also voluntarily yield the CPU.
    - The dynamic priority adjustment ensures that CPU time is distributed among processes in a way that balances system responsiveness and fairness.
    - However, this dynamic nature means that `SCHED_OTHER` processes may experience waiting times when the CPU is busy with higher-priority tasks or when they yield the CPU voluntarily.
    - As a result, `SCHED_OTHER` processes may have longer execution times compared to `SCHED_FIFO` and `SCHED_RR` processes that have fixed or higher priorities.


    The exact execution times can also be affected by factors such as the specific workload, system load, and scheduler behavior on the host system.