To Build:
make

To Run:
v0/mc0
v1/mc1
v2/mc2

TESTING
v0 and v1 where tested by hand
v2 was tested by running ./mc2 < test.in > test.out
In addition there are some C asserts. I initially wanted to do test-driven-development for this but lost sight of that goal.

ABOUT
Author: Turner J Robbins tjrobbins@wpi.edu

The design is pretty simple, the main function in main.c takes care of handling I/O and uses the helper functions in util.c. util.c also contains a bunch of static constants for keeping track of background processes, numbers of jobs, etc...

Keeping track of user commands:
I used an array of arrays of character pointers.

Keeping track of background commands:
there is an array that holds the PIDs of all processses running in the background. Cleaning up this array is a bit of a length process so it's only done in batches. In addition there is an array of integers that hold information about which 'user commands' are actually supposed to be run in the background. 

In the future I would like to implement a more user-friendly data stucture for handling all of the background and process data. The current implementation isn't very easily extended, and is kind of a mess. If I had another week I would probably use a struct to hold all of the data for each process, that way I could easily access more information about each process when I remove that process. 
