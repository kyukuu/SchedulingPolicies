all: sched_other sched_rr sched_fifo main
plot: histogram

sched_other: sched_other.c
	gcc sched_other.c -o sched_other

sched_fifo: sched_fifo.c
	gcc sched_fifo.c -o sched_fifo

sched_rr: sched_rr.c
	gcc sched_rr.c -o sched_rr

main: main.c
	gcc main.c -o main

histogram: histogram.py
	python3 histogram.py

clean:
	rm -f main
	rm -f sched_fifo
	rm -f sched_other
	rm -f sched_rr