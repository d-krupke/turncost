# Tours with Turn Costs

This repository contains the code for an approximated and exact solver for cycle covers and tours with turn costs in grid graphs. Further, additional material such as experimental results and test instances.

See also the example instances for how to code the instance files (obvious .csv files with one pixel per line).

Note that this code needs additional libraries to work: The commercial CPLEX library and two further freely available libraries for which you get download instructions when compiling with CPLEX.

The code emerged from a (nearly 2 year long but with interuptions) try-and-error process for finding the most efficient way to solve the problems. Thus, the code was subject to constant major code changes which in combination with the notorious lack of time of a PhD-student led to some ugly but working constructs in the code. This project throughly improved my moral to write good tests because they give your project stability that your experimental code cannot have.

The efficiency optimizations focused mainly on the cycle cover problem as it showed to be the primary challenge. For tours one could still get some additional speed ups for the approximation as well as the integer program. However, there probably wouldn't be any significant changes considering the solvable instance sizes. Hence, the necessary additional work is probably not worth it. If you are still interested, I can of course give you some details on potential improvements.


The following external libraries are used:
* [Blossom V Matching Algorithm](http://pub.ist.ac.at/~vnk/software.html) (Commercial usage could need a licence)
* [pcst_fast](https://github.com/fraenkel-lab/pcst_fast/tree/master/src)
* [IBM CPLEX](https://www.ibm.com/analytics/cplex-optimizer) (Note that this is a commercial software but it is much more powerful than the open source alternatives)
