# CS4348-Project1
 Class: CS 4348.006 Professor: Subbarayan Venkatesan  
 
 Project 1: Multiplying 2 (large) numbers via multithreading 
 
 Group Members: Jonny Schroeter - jas170005, Caleb Munson - ccm170000 
 Date: 25 March 2021 
 

 Filename: thread.cc 
 Compile using command: g++ thread.cc -lpthread  

 Thread model: posix 
 gcc version: 4.8.5 20150623 (Red Hat 4.8.5-44) (GCC) 

 Description: The program takes in an input file accepting 3 numbers on 3 separate lines. The first line is the number of threads, with the max being 255. The second and third lines are the two numbers to be multiplied, with the first number being the multiplicand, and the second number being the multiplier. The threads split up the work by using a digit of the multiplier and multiplying all of the digits of the multiplicand. The result of each operation is then put into an array where the answer is stored. Once the operations are complete, the final answer is printed to an output file.  Notes: The input file must be input.txt. The output file is output.txt
