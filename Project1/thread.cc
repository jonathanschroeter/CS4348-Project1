/*
Team Members: Jonathan Schroeter (Jas170005@utdallas.edu) and Caleb Munson (ccm170000@utdallas.edu)
Work: Program was worked on at the same time by both members for all parts. No work was divided.
Date: 3/25/2021
Class: CS/SE 4348.006
*/

     

//Headers

#include <pthread.h>
#include<string>
#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include<cmath>
#include<fstream>
#include <stdio.h>
#include <bits/stdc++.h> 
#include<bits/stdc++.h> 
#include <string.h>
#include <sstream>
#include <vector>
using namespace std;

const int MAX = 255; // Maximum number of threads allowed
const int UPPER = 100;


// Global variables

int globcounter;        // variable to be incremented by each thread 
sem_t semaphore;	      // global semaphore, used for mutual exclusion
pthread_t tid[ MAX ];   // array of thread identifiers
string number1, number2;
vector<int> finalvector;
int global1 = 0;


// ***********************************************************************
// Function that each thread will execute.
// ***********************************************************************
void *
increment(void *arg )
{
	
	//make sure globacounter does not get bigger than number2.size
	if(globcounter >= number2.size()){
		return NULL;
        }

	//sem_wait( &semaphore ); // semaphore WAIT operation
	//length of the two numbers
	int number1Len = number1.size();
	int number2Len = number2.size();



	sem_wait( &semaphore ); // semaphore WAIT operation
	//loop for multiplying 
	for(int incounter = 0; incounter < number1Len; incounter++){
		//make sure globacounter does not get bigger than number2.size
		if(globcounter >= number2.size()){
			sem_post( &semaphore);
			return NULL;
		}
		
		//getting numbers from the string
		int index2 = (int)number2.at(globcounter) - 48;
		int index1 = (int)number1.at(incounter) - 48;

		//multiplication
		int result = index1 * index2;

		//making sure incounter + globcounter is not greater than finalvector.size(). If it exceeds, we are accessing data that is not the number
		if(incounter + globcounter >= finalvector.size()){
			sem_post( &semaphore);
                        return NULL;
		}

		//storing the result
		result = result + finalvector.at(incounter + globcounter);
		finalvector.at(incounter + globcounter) = result;
	}
	//for loop for doing the carries from the multiplication

	
	for(int j = 0; j < finalvector.size(); j++){
		//doing the carry
		if(finalvector[j] > 9){
			int carry = finalvector[j] / 10;
                        int remain = finalvector[j] % 10;
			finalvector[j] = remain;
			finalvector[j+1] = finalvector[j+1] + carry;
		}
	}

	//updating glob counter
	globcounter++;
	//recursive call (to have the thread do more than one operation)
 	sem_post( &semaphore);	//post operation
	if(globcounter < number2Len)
		increment(arg);

	//returning the thread
	return NULL;
}

//function to check to see if a string is a number or not
bool is_number(string s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//main function
int 
main()
{
	//variables 
	int i , no_threads = 0;
	string tempthreads;
	string threads;

	//bools for noticing negative numbers
	bool str1 = false , str2 = false;

	string line;

	//opening the input file
	ifstream myfile;
	myfile.open("input.txt");
	
	//checking to make sure the input file exists
	if(!myfile.is_open()) {
    		perror("Error open: No such file exists");
    		exit(EXIT_FAILURE);
	}
	if(!myfile){
		cout << "the file does not exist. The program will exit." << endl;
		exit(0);
	}

	//getting the number of threads
	getline(myfile,tempthreads);

	//outputting the number of threads
	cout << "The number of threads: " << tempthreads << endl;

	//checking to make sure the number of threads is a number
	if(is_number(tempthreads) == false){
                cout << "you have entered an invalid input for the number of threads. The program will exit" << endl;
                exit(0);
        }
	
	//if the number of threads entered is empty or 0
	if(tempthreads.length() <= 0 || tempthreads.at(0) == '0'){
                cout << "Your entered in <= 0 number of threads. The program will exit " << endl;
                exit(0);
        }

	
	//putting the number of threads into a int
	stringstream threader(tempthreads);
	threader >> no_threads;
	
	//getting the first number
	getline(myfile, number1);
	
	//for number 1
	cout << "The first number: " << number1 << endl;
	
	//checking if it is negative
	if(number1.at(0) == '-'){
		number1 = number1.substr(1,number1.size()-1);
		str1 = true;
	}

	//checking if it is a number
	if(is_number(number1) == false){
                cout << "you have entered an invalid input for the first number. The program will exit" << endl;
                exit(0);
        }

	//making sure that the number length is greater than 0 and less than 256
        if(number1.length() <= 0 || number1.length() > 256){
                cout << "Your first number is too large or too small. The program will exit " << endl;
                exit(0);
        }

	//getting the second number
	getline(myfile, number2);

	cout << "The second number: " << number2 << endl;
	
	//checking if it is negative
	if(number2.at(0) == '-'){
                number2 = number2.substr(1,number2.size()-1);
                str2 = true;
        }
	
	//checking if it is a number
	if(is_number(number2) == false){
                cout << "you have entered an invalid input for the second number. The program will exit" << endl;
                exit(0);
        }

	//making sure that the number length is greater than 0 and less than 256
	if(number2.length() <= 0 || number2.length() > 256){
		cout << "Your second number is too large or too small. The program will exit " << endl;
		exit(0);
	}

	//if the number of threads is greater than the length of the multiplier, make the number of threads the length of the multiplier
	if(no_threads > number2.length()){
		cout << "Number of threads in the file is greater than the length of the multiplier number. Therefore, the number of threads will be set to " << number2.length() << endl;
		no_threads = number2.length();
	}
		
	//close the input file
	myfile.close();
	
	
	// Initialize the mutual exclusion semaphore.
	sem_init( &semaphore, 0, 1 );

	// Create no_threads identical threads each one running the increment()
	// function, saving the thread-id in the tid array, and using the default 
	// attributes.  Each thread calls the function increment(), with one
	// argument "i", used as the thread number.

	//reversing the numbers
	reverse(number1.begin(),number1.end());
        reverse(number2.begin(),number2.end());

	//pushing the final vector full of 0s
	 for(int i = 0; i < (number1.size() + number2.size());i++){
                finalvector.push_back(0);
        }


	//creating the threads
	for( i = 0; i < no_threads; i++ ){ 	
		pthread_create( &tid[ i ], NULL, increment, NULL);
	}
	
	// Wait for all the threads to exit.  This suspends the calling thread,
	// i.e., the main program, until all target threads complete.
	for( i = 0; i < no_threads; i++ ){ 
		pthread_join( tid[ i ], NULL );
  	}

	//variable for leading zeros
	int zeroCount = 0;

	//opening output file
	ofstream myout("output.txt");
	
	//checking negative numbers
	myout << "The final answer is: ";
	if(str1 == true && str1 != str2){
		myout << "-";
	}
	if(str2 == true && str1 != str2){
                myout << "-";
        }

	//outputting the answer to the output file
	for(int i = 0; i < finalvector.size();i++){
		while(finalvector.at(finalvector.size() - i - 1) == 0 && zeroCount == 0){
			i++;
		}
		zeroCount++;
                myout <<  finalvector.at(finalvector.size() - i - 1);
        }

	//closing the output file
	myout.close();
	
	//closing the program
	cout << "The program has finished. The final answer has been output to the file" << endl;
	return 0;
}
