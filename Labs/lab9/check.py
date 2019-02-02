#!/usr/bin/python

from __future__ import print_function
from collections import deque
import sys, re

def     sys_error(s):
    print ("Error: " + s)
    sys.exit(1)

# not used
def     print_help():
    print("Useage: check.py")
    sys.exit(0)

ptn_id = re.compile('^(Computer|Printer)\s+(\d+)\s');
ptn_job = re.compile('job\s+(\d+)');
ptn_summary = re.compile('\s(\d+)\s+jobs');

ptn_config = re.compile('(jobs|computers|printers|size)=(\d+)');

n_lines = 0

num_jobs = num_computers = num_printers = q_size = 0
computer_total = printer_total = 0
job_count = 0
for line in sys.stdin:
    n_lines += 1

    # config lines
    if n_lines < 5: 
        # check parameters
        m = re.search(ptn_config, line)
        if m:
            key = m.group(1)
            v = int(m.group(2), 0)
            if key == 'jobs': 
                num_jobs = v
            elif key == 'computers':
                num_computers = v
            elif key == 'printers':
                num_printers = v
            else:
                q_size = v
                assert num_computers > 0
                assert num_printers > 0
                assert num_jobs > 0
                assert q_size > 0
                computers = [0] * num_computers
                printers = [0] * num_printers
                q = deque([])
        else:
            sys_error("Not a configuration line.\n"+line)
        continue
            
    m = re.search(ptn_id, line)
    if not m:
        sys_error("Not a computer/printer activity.")

    mj = re.search(ptn_job, line)
    ms = re.search(ptn_summary, line)
    pcid = int(m.group(2))
    job = -1
    total = -1
    if m.group(1) == 'Computer':
        if mj:
            job = int(mj.group(1))
            if job != job_count :
                sys_error("Job {} has not been submitted yet.\nline {}:{}{}".
                        format(job_count, n_lines, line, q))
            q.append(job)
            computers[pcid] = 0
            job_count += 1
        elif ms: 
            computer_total += int(ms.group(1))
            continue
        else:  # wait
            if len(q) < q_size and computers[pcid] == 0: 
                sys_error("Computer should not wait.\nline {}:{}{}".format(n_lines, line, q))
            computers[pcid] = 1
    else:
        if mj:
            job = int(mj.group(1))
            job2 = q.popleft() 
            if job != job2:
                sys_error("Printer did not fetch the first job in the queue.\nLine {}: {}".
                        format(n_lines, line))
            printers[pcid] = 0
        elif ms:
            printer_total += int(ms.group(1))
            continue
        else:
            if len(q) > 0 and printers[pcid] == 0:
                sys_error("Printer should not wait.\nline {}:{}{}".format(n_lines, line, q))
            printers[pcid] = 1
    if job > 0 :
        print("line {}:{}{}".format(n_lines, line, q))
    if len(q) > q_size :
        sys_error("The queue has more than {} elements.\n".format(q_size))

#sanity check
print(len(q), num_jobs, computer_total, printer_total)
assert len(q) == 0
assert job_count == num_jobs
assert computer_total == num_jobs
assert printer_total == num_jobs
