# File-Word-Counter
Console application written in C to read from any file, count the number of occurrences of each word, and then output each word in order of least to greatest lexicographic order along with the number of occurrences. The beginning of the output also contains the total number of words and the total number of unique words.

Video Demo: https://www.youtube.com/watch?v=UB65W-8PRIs


**Program Files**
- main.c - main program.
- hash_table.h/hash_table.c - hash table interface.
- linked_list.h/linked_list.c - linked list interface used for the hash table separate chaining and storing all the words into one list for output.
- status.h - header file for Boolean and Status enumerated types.
- Makefile - so that the program can be run with the command line in Linux.

**Input and Output Files**
- bible.txt - the input file. The entire bible as a text file.
- output.txt - the output file. Contains each word in the bible, how many times it occured, the total number of words, and the total number of unique words.
- output_hash_table.txt - an output file containing the hash table representation in memory as well as a summary of its performance at the bottom of the file.
