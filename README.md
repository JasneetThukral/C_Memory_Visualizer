# C_Memory_Visualizer

## Summary:
The program is to be run with users code to generate visualization of memory leaks

## Running the program:
To compile the sample test type "make main test". This will generate the targets "main" and "test" <br />
To run our sample test run "./main test -q queue_mod -v vector_mode -s stack_mod -a array_mod -n 100" <br />
<br />
The format of the command like argument is: ./main <exe> with options -q, -v, -s, -a -n <br />
exe = executable generated from file with memory leaks <br />
-q = name of user defined queues (if any) <br />
-v = name of user defined queues (if any) <br />
-q = name of user defined queues (if any) <br />
-q = name of user defined queues (if any) <br />
-n = maximum size of datastructure user creates in file that generates exe
  
## Sample Usage (Use the following code to run our premade example): 
### The Output will be sent to "output.html", which you can open in a browser to view
```bash
make clean
make main test 
./main test -q queue_mod -v vector_mode -s stack_mod -a array_mod -n 100
```

## Relevent Files
main.c <br />
creator_methods.h <br />
creator_methods.c <br />
visualizer.c 
  
## Important Notes: 
Our program uses "interface" in order to understand which user datastructures correspond to "general" data types 
(ie stack, vector, queue, array). More information about this can be found in the "creator_methods.h" file. 

## Contributors 
Jasneet Thukral - all of main.c and creator_methods.c <br />
Alan - created makefile and imported stack, queue, vector, array files <br /> 
Haowei - all of visualizer.c

## Research 
[Final Report - Inspiration, Design Process, Future](https://docs.google.com/document/d/1WfU8wjXfTwxeStbrrXw7TCKy-CWMiCwE897xCzVXLqc/edit?usp=sharing)






