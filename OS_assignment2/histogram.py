import matplotlib.pyplot as plt

def read_execution_time(file_path):
    with open(file_path, 'r') as file:
        execution_time = float(file.readline().strip())
    return execution_time


fifo_execution_time = read_execution_time('fifo_execution_time.txt')
rr_execution_time = read_execution_time('rr_execution_time.txt')
other_execution_time = read_execution_time('other_execution_time.txt')
arr = [fifo_execution_time,rr_execution_time,other_execution_time]
arr.sort()
fifo_execution_time=arr[0]
rr_execution_time=arr[1]
other_execution_time=arr[2]
print("FIFO : "+str(fifo_execution_time)+" seconds")
print("RR : "+ str(rr_execution_time)+"  seconds")
print("OTHER : "+ str(other_execution_time)+"  seconds")
colors = ['cyan', 'yellow', 'purple']
plt.bar(['SCHED_FIFO', 'SCHED_RR', 'SCHED_OTHER'], 
        [fifo_execution_time, rr_execution_time, other_execution_time], 
        color=colors)

plt.xlabel('Scheduling Policy')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Times for Different Scheduling Policies')

plt.yscale('linear')
plt.savefig('plot.png')

plt.show()
plt.close()


