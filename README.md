# File-Word-Counter
Console application written in C to read from any file, count the number of occurrences of each word in the file, and then output each word in order of least to greatest lexicographic order along with its number of occurrences. The beginning of the output also contains the total number of words and the total number of unique words. The hash table is implemented using separate chaining.

**Program Files**
- main.c - main program.
- hash_table.h/hash_table.c - hash table interface.
- linked_list.h/linked_list.c - linked list interface used for the hash table and storing all the words into one list for output.
- status.h - header file for Boolean and Status enumerated types.
- Makefile - so that the program can be run with the command line. In this case, main.c could be modified so that the input and output file names are entered into argv[1] and argv[2] respectively instead of being hardcoded into main.

**Input and Output Files**
- bible.txt - the input file. The entire bible as a text file.
- output.txt - the output file. Contains each word in the bible, how many times it occured, the total number of words, and the total number of unique words.
- output_hash_table.txt - an output file containing the hash table representation in memory as well as a summary of its performance at the bottom of the file.
