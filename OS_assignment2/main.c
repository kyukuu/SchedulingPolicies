#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <time.h>

int main()
{
   // SCHED_OTHER
   struct timespec total_time_start;
   struct timespec total_time_end;
   struct timespec start_time_sched_other, end_time_sched_other;

   clock_gettime(CLOCK_MONOTONIC, &total_time_start);
   clock_gettime(CLOCK_MONOTONIC, &start_time_sched_other);
   pid_t pid_other = fork();
   if (pid_other == 0)
   {
      setpgid(0, 0);
      int scheduling_policy = SCHED_OTHER;
      sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 0});
      if (sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 0}) == -1)
      {
         printf("Failed to set SCHED_OTHER scheduling policy");
      }
      execl("./sched_other", "child", NULL);
      perror("Exec command failed\n"); // if execl fails, program execution reaches here
   }
   else if (pid_other < 0)
   {
      printf("Fork failed\n"); // error for fork failed
   }
   else if (pid_other > 0)
   {
      // SCHED_FIFO
      struct timespec start_time_sched_fifo, end_time_sched_fifo;

      clock_gettime(CLOCK_MONOTONIC, &start_time_sched_fifo);
      pid_t pid_fifo = fork();
      if (pid_fifo == 0)
      {
         setpgid(0, 0);
         int scheduling_policy = SCHED_FIFO;
         sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 50});
         if (sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 0}) == -1)
         {
            printf("Failed to set SCHED_OTHER scheduling policy");
         }
         execl("./sched_fifo", "child", NULL);
         perror("Exec command failed\n"); // if execl fails, program execution reaches here
      }
      else if (pid_fifo < 0)
      {
         printf("Fork failed\n"); // error for fork failed
      }
      else if (pid_fifo > 0)
      {
         // SCHED_RR
         struct timespec start_time_sched_rr, end_time_sched_rr;

         clock_gettime(CLOCK_MONOTONIC, &start_time_sched_rr);
         pid_t pid_rr = fork();
         if (pid_rr == 0)
         {
            setpgid(0, 0);
            int scheduling_policy = SCHED_RR;
            sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 50});
            if (sched_setscheduler(0, scheduling_policy, &(struct sched_param){.sched_priority = 0}) == -1)
            {
               printf("Failed to set SCHED_OTHER scheduling policy");
            }
            execl("./sched_rr", "child", NULL);
            perror("Exec command failed\n"); // if execl fails, program execution reaches here
         }
         else if (pid_rr < 0)
         {
            printf("Fork failed\n"); // error for fork failed
         }
         else if (pid_rr > 0)
         {
            for (int i = 0; i < 3; i++)
            {
               pid_t pid = wait(NULL);
               if (pid == pid_fifo)
               {
                  // fifo process ends
                  waitpid(pid_fifo, NULL, 0);
                  clock_gettime(CLOCK_MONOTONIC, &end_time_sched_fifo);

                  // Calculate execution time for SCHED_FIFO
                  double cpu_time_used_sched_fifo = (end_time_sched_fifo.tv_sec - start_time_sched_fifo.tv_sec) +
                                                    (end_time_sched_fifo.tv_nsec - start_time_sched_fifo.tv_nsec) / 1e9;
            
                  FILE *output_file_fifo = fopen("fifo_execution_time.txt", "w");
                  if (output_file_fifo == NULL)
                  {
                     perror("Failed to open output file");
                     return 1;
                  }
                  fprintf(output_file_fifo, "%.6f\n", cpu_time_used_sched_fifo); // Write execution time to file
                  fclose(output_file_fifo);
               }
               else if (pid == pid_rr)
               {
                  // rr process ends
                  waitpid(pid_rr, NULL, 0);
                  clock_gettime(CLOCK_MONOTONIC, &end_time_sched_rr);

                  // Calculate execution time for SCHED_RR
                  double cpu_time_used_sched_rr = (end_time_sched_rr.tv_sec - start_time_sched_rr.tv_sec) +
                                                  (end_time_sched_rr.tv_nsec - start_time_sched_rr.tv_nsec) / 1e9;
         
                  FILE *output_file_rr = fopen("rr_execution_time.txt", "w");
                  if (output_file_rr == NULL)
                  {
                     perror("Failed to open output file");
                     return 1;
                  }
                  fprintf(output_file_rr, "%.6f\n", cpu_time_used_sched_rr); // Write execution time to file
                  fclose(output_file_rr);
               }
               else if (pid = pid_other)
               {
                  // sched_other process ends
                  waitpid(pid_other, NULL, 0);
                  clock_gettime(CLOCK_MONOTONIC, &end_time_sched_other);

                  // Calculate execution time for SCHED_OTHER
                  double cpu_time_used_sched_other = (end_time_sched_other.tv_sec - start_time_sched_other.tv_sec) +
                                                     (end_time_sched_other.tv_nsec - start_time_sched_other.tv_nsec) / 1e9;
   
                  FILE *output_file_other = fopen("other_execution_time.txt", "w");
                  if (output_file_other == NULL)
                  {
                     perror("Failed to open output file");
                     return 1;
                  }
                  fprintf(output_file_other, "%.6f\n", cpu_time_used_sched_other); // Write execution time to file
                  fclose(output_file_other);
               }
            }
         }
      }
   }
   clock_gettime(CLOCK_MONOTONIC, &total_time_end);
   double total_time = (total_time_end.tv_sec - total_time_start.tv_sec) +
                       (total_time_end.tv_nsec - total_time_start.tv_nsec) / 1e9;
   printf("Total running time: %.6f seconds\n", total_time);
   return 0;
}
