# TODO
- Fast inverse sqrt (because why not)
- Approximating Float Square Roots Using Bit Manipulation

Arr[N]
Get max num of threads, max_threads
Divide, N/max_threads, to get start and end of a thread. N/max_threads
- start = x0, end = (n/max_threads)-1
Create a thread with the array, start, and end

N = 64, max_threads 8, [start, end][0,7][8,15],[16,23],[24,31][32,39][40,47][48,55][56,63], s = s+8 = s+max_threads


[i+max_threads-1, (i = 0) ? max_threads-1 : max_threads]