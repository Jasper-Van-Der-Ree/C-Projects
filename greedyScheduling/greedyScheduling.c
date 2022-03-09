#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Selection sort based approach to sorting jobs

int main(int argc, char* argv[]) {

    // First peek at how many jobs and timeslots there are
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }

    char buf[256];

    // Read the number of jobs to be scheduled
    if (!fscanf(fp, "%s\n", buf)) {
        perror("reading the number of jobs failed");
        exit(EXIT_FAILURE);
    }
    int jobcount = atoi(buf);

    // Next read the maximum number of timeslots
    if (!fscanf(fp, "%s\n", buf)) {
        perror("reading the number of timeslots failed");
        exit(EXIT_FAILURE);
    }
    int timeslots = atoi(buf);

    // We've read the number of timeslots so close the file and start over
    fclose(fp);

    // int* overlap_tickers;
    // overlap_tickers = (int*) malloc(timeslots * sizeof(int));

    char* job_output;
    job_output = (char*) malloc(jobcount * sizeof(char));
    unsigned int job_ticker = 0;
    unsigned int last_end;

    // printf("Input Jobs:\n");
    for (unsigned int slot=0; slot<timeslots; slot++) {
        
        FILE* fp = fopen(argv[1], "r");
        if (!fp) {
            perror("fopen failed");
            exit(EXIT_FAILURE);
        }

        char buf[256];

        // First read the number of jobs to be scheduled
        if (!fscanf(fp, "%s\n", buf)) {
            perror("reading the number of jobs failed");
            exit(EXIT_FAILURE);
        }

        // Next read the maximum number of timeslots
        if (!fscanf(fp, "%s\n", buf)) {
            perror("reading the number of timeslots failed");
            exit(EXIT_FAILURE);
        }

        unsigned int job_count = 0;
        char* job_list_job;
        job_list_job = (char*) malloc(jobcount * sizeof(char));

        int* job_list_beg;
        job_list_beg = (int*) malloc(jobcount * sizeof(int));

        int* job_list_end;
        job_list_end = (int*) malloc(jobcount * sizeof(int));

        // Now read the rest of the file
        for ( int line=0; line<jobcount; line++ ) {

            if (!fgets(buf, 256, fp)) {
                perror("reading a line for a job failed");
                exit(EXIT_FAILURE);
            }

            char job;
            unsigned int end;
            unsigned int beg;

            if (!sscanf(buf, "%c %d %d", &job, &beg, &end)) {
                perror("parsing a line for a job failed");
                exit(EXIT_FAILURE);
            }
            //If a job starts at time 0, it is invalid and should not be scheduled.
            if(beg == 0){
                continue;
            }

            //For example, "20" would mean that no jobs have an ending time greater than 20.
            if(end > timeslots){
                continue;
            }
            
            if (end==slot) {
        
                job_list_job[job_count] = job;
                job_list_beg[job_count] = beg;
                job_list_end[job_count] = end;

                job_count++;

                // printf("%c %d %d\n", job, beg, end);
            }

        }

        unsigned int held_beg = 0;
        unsigned char held_job;

        for(int i = 0; i < job_count; i++){
            //printf("%c %d %d\n", job_list_job[i], job_list_beg[i], job_list_end[i]);

            if (job_ticker != 0){
                if(job_list_beg[i] <= last_end){
                        continue;
                }
            }

            //first iterable
            if(i == 0){
                held_beg = job_list_beg[i];
                held_job = job_list_job[i];
                if(job_count > 1){
                    continue;
                }
            }
            
            // beginning is less, replace job and beginning
            if(job_list_beg[i] <= held_beg || held_beg == 0){
                held_job = job_list_job[i];
                held_beg = job_list_beg[i];
                if(i != job_count - 1){
                    continue;
                }
            }

            job_output[job_ticker] = held_job;
            last_end = job_list_end[i];
            job_ticker++;

        }

        free(job_list_job);
        free(job_list_beg);
        free(job_list_end);

        // overlap_tickers[slot] = current_jobs_ticker;
        fclose(fp);

    }
    // printf("\nJob Slot Tickers:\n");
    // for(int i = 0; i < timeslots; i++){
    //     printf("%d %d\n", (i + 1), overlap_tickers[i]);
    // }

    // printf("\nOutput Jobs\n");
    for(int i = 0; i < job_ticker; i++){
        if (job_output[i]) {
            printf("%c\n",job_output[i]);
        }
    }
    free(job_output);
    // free(overlap_tickers);
    exit(EXIT_SUCCESS);
}