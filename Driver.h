/* 
 * Name: John Ahn
 * CSE12, Winter 19
 * 03/12/2019
 * cs12xab
 * Assignment Nine
 * -----------------------------------------------------------------------------
 * File Name: Driver.c
 * Description: Ths file contains definitions of a UCSDStudent object which we
 * will insert, as a TNode, in our binary tree which will be located in a Disk
 * -----------------------------------------------------------------------------
 *  
 *  class UCSDStudent
 *
 *  Description: This class contains data fields, functions and constructors
 *  that define a UCSDStudent object that takes in two parameters; a name and
 *  student number. We will use our operator overload functions in order in our
 *  Tree.c in order to implement a recursive binary tree
 *
 *  Data Fields:
 *      
 *      ostream- used to help print our UCSDStudent object
 *      name- name of the student
 *      studentnum- student's number 
 *
 *  Functions:
 *      
 *      UCSDStudent - this object has a default constructor, a constructor that 
 *      takes in a char* and long as a parameter in order to give the 
 *      UCSDStudent object a name and a number, and also has a constructor that 
 *      takes in a UCSDStudent object as a parameter
 *
 *      operator const char* - gets the name of the student 
 *
 *      long operator == - compares two UCSDStudent objects, checks if they are
 *      equal or not
 *
 *      long operator > - compares two UCSDStudent objects' names and checks 
 *      which name is greater than the other alphabetically.
 *  */

#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[16];
        long studentnum;
public: 
        /** 
         * A default constructor for UCSDStudent object*/
        UCSDStudent(void) : studentnum (0) {
                memset (name, '\0', sizeof (name));
        }
        
        /** 
         * This constructor takes in a UCSDStudent to initialize 
         * a new duplicate UCSDStudent object
         * @param student - a UCSDStudent object*/
        UCSDStudent (const UCSDStudent & student) {
                memset (name, '\0', sizeof (name));
                strcpy (name, student.name);
                studentnum = student.studentnum;
        }

        /** 
         * This constructor initializes a UCSDStudent object
         * @param nm - name of the student 
         * @param val - student's school number*/
        UCSDStudent (char * nm, long val = 0) : studentnum (val) {
                //initializing name and studentnum
                memset (name, '\0', sizeof (name));
                strcpy (name, nm);
        }
        /** 
         * Getter function that gets the name of the student
         * @return returns the name of the student */
        operator const char * (void) const {
                return name;
        }
        /** 
         * An overloaded operator that compares two UCSDStudent objects
         * @return true or false depending if two objects are equal or not*/
        long operator == (const UCSDStudent & sss) const {
                return ! strcmp (name, sss.name);
        }
        
        /** 
         * Another overloaded operator that determines which USCDStudent object
         * is greater
         * @return true or false if the calling object is greater than the
         * UCSDStudent object passed in as the parameter of this method*/
        long operator > (const UCSDStudent & sss) const {
                return (strcmp (name,sss.name) > 0) ? 1 : 0;
        }
};

#endif
