Documentation for Warmup Assignment 1
=====================================

+-------+
| BUILD |
+-------+
In order to generate executable listtest and warmup1, you should type "make" under the directory which contained all the required files (cs402.h, listtest.c, Makefile, my402list.c, my402list.h, warmup1.c).
Comments: There is no warning or error messages executing "make" when I tried in my account on nunki.

+---------+
| GRADING |
+---------+

(A) Doubly-linked Circular List : 
	My program works fine with this part, when execute ./listtest for 20 times, it produce nothing.

(B.1) Sort (file) : 
	My program works fine with this part, it read and print the file properly. "diff" command generate no output.

(B.2) Sort (stdin) : ? out of 30 pts
	My program works fine with this part, it read and print the input properly. "diff" command generate no output.

Missing required section(s) in README file : No
Cannot compile : Can compile
Compiler warnings : No warnings
"make clean" : Can remove all *.o and executable files
Segmentation faults : No
Separate compilation : I used sepearte compilation lines in the Makefile.
Malformed input : 
	My program can recgnoise the malformed input in those files and quit the process properly.
Too slow : My program generate correct results in several seconds.
Bad commandline : 
	My program can recgnoise the bad commandlines listed in this section and print reasonable error message. THE FIRST AND LAST TWO TYPE.
Bad behavior for random input : Can response properly to random input.
Did not use My402List and My402ListElem to implement "sort" in (B) : Using My402List and My402ListElem to implement.

+------+
| BUGS |
+------+

No bugs. Works fine with all tests listed in grading guidelines.

+-------+
| OTHER |
+-------+

No.
