
// A computer knows its id.
// Computers are numbered from 0 to c - 1.
void computer()
{
    int     done = 0;
    int     njobs = 0;

    while (! done) {
        // computer works and prepares a job
        int     job = work();
        int     r;

        // wait until queue is ready
        r = q_can_submit(jq);
        while (r == 0) {
            r = q_can_submit(jq);
        }

        // r < 0 means all jobs are done
        if (r < 0) {
            done = 1;
            continue;
        }

        // submit a job to the queue
        // id is the id of this computer
        q_submit_job(jq, job, id);

        njobs ++;
    }
    // print out how many jobs this computer has submitted
    printf("Computer %2d submitted %d jobs.\n", id, njobs);
}


// A printer knows its id.
// Printers are numbered from 0 to p - 1.
void printer()
{
    int done = 0;
    int njobs = 0;

    while (! done) {
        int     r;

        // wait until a job is available
        r = q_job_available(jq);
        while (r == 0)  {
            r = q_job_available(jq);
        }

        // r < 0 means all jobs have started
        if (r < 0) {
            done = 1;
            continue;
        }

        // fetch a job
        int job = q_fetch_job(jq, id);

        njobs ++;

        // print the job
        print_job(job);
    }

    // print out how many jobs this printer has submitted
    printf("Printer  %2d completed %d jobs.\n", id, njobs);
}

