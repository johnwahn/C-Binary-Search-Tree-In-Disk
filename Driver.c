/* 
 * Name: John Ahn
 * CSE12, Winter 19
 * 03/13/2019
 * cs12xab
 * Assignment Nine
 * -----------------------------------------------------------------------------
 * File Name: Driver.c
 * Description: Ths file contains our main function that will test the
 * functionality of our binary tree in the disk.
 *
 * Functions:
 *
 *      main - Our main function that will test our binary tree and TNode
 *      implementations that are in the disk. 
 * ---------------------------------------------------------------------------*/

#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"
#include <fstream>

using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
        return stream << "name:  " << stu.name
                << " with studentnum:  " << stu.studentnum;
}
/** 
 * Our main function where we will test our binary tree with binary and ASCII
 * inputs as well as testing our TNode implementations 
 * */
int main (int argc, char * const * argv) {
        char buffer[BUFSIZ]; //changing buffer to BUFSIZ which is 16
        char command;
        long number;
        char option;
        istream* is = &cin; //declaring an istream
        ostream* os = &cout;//declaring an ostream
        
        SymTab<UCSDStudent>::Set_Debug_Off ();

        while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
                case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                        break;
                }       
        }

        SymTab<UCSDStudent> ST("Driver.datafile");
        ST.Write (cout << "Initial Symbol Table:\n" );


        while (cin) {
                //checking if it is end of file
                if (!*is) {
                   
                        // checking if it is not from keyboard
                        if (is!=&cin) {
                                
                                // delete is and os and reset both
                                delete is;
                                is = &cin;

                                delete os; 
                                os = &cout; 
                        }

                }
                command = NULL;         // reset command each time in loop
                *os << "Please enter a command ((f)ile, (i)nsert, "
                        << "(l)ookup, (r)emove, (w)rite):  ";
                *is >> command;

                switch (command) {

                case 'f': {
                        
                        *os <<"Please enter file name for commands: ";
                        *is >> buffer;
                        
                        //if input is not from keyboard 
                        if (is!=&cin) {
                                
                                //deleting is and os 
                                delete is;
                                delete os;

                        }
                        
                        //resetting is and os 
                        is = new ifstream(buffer);
                        os = new ofstream("/dev/null");
                        
                        break;
                }
                
                case 'i': {
                        *os << "Please enter UCSD student name to insert:  ";
                        *is >> buffer;  // formatted input

                        *os << "Please enter UCSD student number:  ";
                        *is >> number;

                        UCSDStudent stu (buffer, number);

                        // create student and place in symbol table
                        ST.Insert (stu);
                        break;
                }
                case 'l': { 
                        unsigned long found;    // whether found or not

                        *os << "Please enter UCSD student name to lookup:  ";
                        *is >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        found = ST.Lookup (stu);
                        
                        if (found)
                                cout << "Student found!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                        }
                case 'r': { 
                        unsigned long removed;

                        *os << "Please enter UCSD student name to remove:  ";
                        *is >> buffer;  // formatted input

                        UCSDStudent stu (buffer, 0);
                        removed = ST.Remove(stu);

                        if (removed)
                                cout << "Student removed!!!\n" << stu << "\n";
                        else
                                cout << "student " << buffer << " not there!\n";
                        break;
                }
                case 'w':
                        ST.Write (cout << "The Symbol Table contains:\n");
                }
        }
        /*Reveals the result of the total cost and operation at 
         * the end of the program*/ 
        ST.Write (cout << "\nFinal Symbol Table:\n");
        
        //when operation is incremented
        if (ST.GetOperation() !=0) {
           
           cout << "\nCost of operations: ";
           cout << ST.GetCost();
           cout << " tree acesses";

           cout << "\nNumber of operations: ";
           cout << ST.GetOperation();

           cout << "\nAverage cost: ";
           cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
           cout << " tree accesses/operation\n:";
        }
        //when operations is not incremented
        else {
           cout << "\nNo cost information available.\n";
        }


}
