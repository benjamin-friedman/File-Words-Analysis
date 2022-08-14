# File-Words-Analysis
Console application written in C to read from any file, count the number of occurrences of each word, and then output each word in order of least to greatest lexicographic order along with the number of occurrences. The beginning of the output also contains the total number of words and the total number of unique words. The program is done using a hash table with separate chaining, and a second output file shows the contents of the hash table as well as a summary of its performance. To demonstrate the program's capabilities, a text file containing the entire bible was used since it's a very long book.

**Video Demo:** https://www.youtube.com/watch?v=l1XjpQryBoI

**Program Files**
- Main.c - Main program.
- HashTable.h/HashTable.c - Hash table interface.
- LinkedList.h/LinkedList.c - Linked list interface used for the hash table separate chaining and storing all the words into one list for the output.
- Status.h - Header file for Boolean and Status enumerated types.
- Makefile - For compiling the program in Linux.
- bible.txt - The entire bible as a text file (used as the input file).

**Output Files**
- output_words.txt - An output file ontaining each word in the bible, how many times it occured, the total number of words, and the total number of unique words.
- output_hash_table.txt - An output file containing the hash table representation in memory as well as a summary of its performance at the bottom of the file. Note that the performance of the hash table is contingent on its initial capacity which can be changed (10,000 was used in the program).
