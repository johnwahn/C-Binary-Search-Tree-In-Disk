/* 
 * Name: John Ahn
 * CSE12, Winter 19
 * 03/13/2019
 * cs12xab
 * Assignment Nine
 * -----------------------------------------------------------------------------
 * File Name: Tree.c
 * Description: Ths file contains a program that implements a recursive binary
 * tree class, utilizing the TNode struct.The binary tree will be in the disk.
 * -----------------------------------------------------------------------------
 *  
 *  struct TNode
 *
 *  Description: This struct contains constructors, datafields, and functions
 *  that implements a TNode object. ThisTNode object has its own datafileds
 *  called left and right which are both offsets which we can locate where the 
 *  left and rigth child of that TNode is in the disk. Also contains data, 
 *  this_position(current location),balance, and height as its data fields. 
 *
 *  Data Fields
 *      data - data of the UCSDStudent passed in by the user
 *      left - location where the left child is in the disk
 *      right - location where the right child is in the disk
 *      this_position - location where the current node is in the disk
 *      balance - balance value of the node
 *      height - height value of the node
 *  
 *  Functions
 *      TNode - there are four TNode constructors, one default TNode constructor
 *      , TNode's Remove constructor, that takes in Whatever & used in Tree's
 *      Remove, TNode's Write constructor, that takes in 3 parameters, used to
 *      write a new TNode to the disk, and TNode's Read constructor, takes in
 *      two parameters, used to read an existing TNode on the disk. 
 *
 *
 *      Lookup - looks up for the element in the tree recursively 
 *
 *      Insert - called from Tree's insert and inserts the TNodes into the tree
 *      recursively 
 *
 *      ReplaceAndRemoveMax- called from TNode's Remove in order to find the 
 *      predeccsor node which will become the root 
 *
 *      Remove- called from Tree's remove and removes a TNode on the tree
 *      recirsively 
 *
 *      SetHeightAndBalance - a function that updates the height and balance for
 *      each node from bottom to top
 *
 *      Read - reads an existing TNode in the disk, used in TNode's read
 *      constructor
 *
 *      Write - writes a new TNode to the disk, used in TNode's write
 *      constructor
 *
 *      ostream & Write_AllTNodes - prints out all the TNodes in order
 *
 *
 *  */

/*******************************************************************************
 * class Tree
 * Description: This class contains datafields, constructor, deconstructor, and
 * functions that implement a binary tree object. There will be TNode objects
 * within the Tree object which would be the branch and leaves for the tree.The
 * entire Tree and its data will be stored in the disk. 
 *
 * Functions
 *      Tree - allocates and initializes a new Tree object
 *     
 *     ~Tree - deconstructor for the tree
 *      
 *      GetCost - gets the current value of cost
 *
 *      GetOperation - get the current value of operation
 *
 *      IncrementCost - increments the value of cost
 *
 *      IncrementOperation - increments the value of operation
 *      
 *      Set_Debug_On - turns debugger on
 *      
 *      Set_Debug_off - turns debugger off
 *      
 *      IsEmpty - checks if the tree is empty
 *      
 *      Write - prints out the content of the Tree object
 *
 *      ostream & operator << - prints out element in the tree
 *      
 *      Insert - inserts the element into the Tree, inserts the root node if
 *      tree is initially empty
 *      
 *      Remove - removes the element from the Tree
 *      
 *      Lookup - looks up an element from the Tree
 *      */

#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2 
#define DEFAULT_HEIGHT -1 //used in SHAB function

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

template <class Whatever>
struct  TNode {
// friends:

// data fields:
        Whatever data;
        long height;
        long balance;
        offset left;
        offset right;
        offset this_position;   // current position

// function fields:
        TNode () : height (0), balance (0), left (0), right (0), 
                this_position (0) {}

        // to declare the working TNode in Tree's Remove
        TNode (Whatever & element) : data (element), height (0), balance (0),
                left (0), right (0), this_position (0) {}
        
        TNode (Whatever &, fstream *, long &);  // to add new node to disk
        TNode (const offset &, fstream *);      // to read node from disk
        
        unsigned long Insert (Whatever &, fstream *, long &, offset &);
        // optional recursive Lookup declaration would go here
        unsigned long Lookup (Whatever & element,fstream *) const;
        void Read (const offset &, fstream *);  // read node from disk
        unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
                long fromSHB = FALSE);
        void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
        void SetHeightAndBalance (fstream *, offset &);
        void Write (fstream *) const;           // update node to disk

        ostream & Write_AllTNodes (ostream &, fstream *) const;
};
/*------------------------------------------------------------------------------
Function Name:          Set_Debug_Off
Description:            Turn debug off
parameter:              none
------------------------------------------------------------------------------*/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off (void) {
   debug_on = 0;
}

/*------------------------------------------------------------------------------
Function Name:          Set_Debug_On
Description:            Turn debug on
parameter:              none
------------------------------------------------------------------------------*/

template <class Whatever>
void Tree<Whatever> :: Set_Debug_On (void) {
   debug_on = 1;
}
/*------------------------------------------------------------------------------
Function Name:          Insert (Tree)

Description:            Inserts an element into the tree. Inserts the root node
                        if the tree is first empty, if not then it will call
                        TNode's Insert function and returns the return value of
                        TNode's Insert function with a variable.

parameter:              element - the data we want to insert into the tree

Return:                 Returns true if insertion happens and false otherwise
------------------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element) {
   
   Tree<Whatever>::IncrementOperation(); //incrementing operation
   unsigned long status = 0; //used in order to save the return value 

   if (occupancy == 0) {
      
      TNode<Whatever> insertRoot(element,fio,occupancy); //inserting root node

      return 1;
   }
   
   else {
      
      //the calling object that will delegate TNode's Insert
      TNode<Whatever>readRootNode(root,fio);
      
      //delegating TNode's Insert at the root node
      status = readRootNode.Insert(element, fio, occupancy, root); 
      
      return status; //returning value of TNode's insert
   } 
 
}
/*----------------------------------------------------------------------
Function Name:          ReplaceAndRemoveMax
        
Description:            This function is called from TNode's remove in
                        order to find the predecessor node in order to 
                        remove a node with two existing children. We
                        go left from our targetTNode then go right as
                        far as we can, done recursively, until right is
                        null. So the furthest right node will become the
                        predecessor node to replace the target node.
        
Parameter:              targetTNode - reference to the TNode that we want to 
                        remove
                                
                        fio - filestream corresponding to the datafield where 
                        the Tree is stored in the Disk

                        PositionInParent - reference to the TNode position in
                        the parent TNode to get the current TNode's offset in 
                        the disk
        ----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
        fstream * fio, offset & PositionInParent) {
   
   //if right child does not exist
   if (!right) {
      
      /*setting reference in order to replace target's data
       * with the this data*/
      targetTNode.data = data;
      PositionInParent = left; //setting PositionInParent to left offset
   }
   //if right child exists
   else {
      //made a calling object to recursively call ReplaceAndRemoveMax
      TNode<Whatever>rightNode(right,fio);
      
      //the furthest right node will do the recursive call
      rightNode.ReplaceAndRemoveMax(targetTNode,fio,right);
      
      SetHeightAndBalance(fio,PositionInParent); //updating SHB
   }
}
/*----------------------------------------------------------------------
Function Name:          Remove (TNode)
        
Description:            This function is called from Tree's remove in
                        order to remove a TNode from the tree. If the 
                        node we want to remove has two children then it
                        will delegate ReplaceAndRemoveMax. The function 
                        will recursively go through the whole tree in
                        order to find the elementTNode that we want to 
                        delete. The nodes will be deleted differently 
                        depending on which pointer, right or left, it
                        contains.And after a removal it will cal SHB as
                        long as this function was not called from the
                        SHB function, since SHB also delegates TNode's 
                        remove. 
        
Parameter:              elementTNode - the reference to the TNode that we
                        want to remove
                                
                        PointerInParent - a reference to the TNode 
                        pointer in parent which we use to get to the 
                        current node

                        fio - filestream corresponding to the datafield where
                        the Tree exists in the disk

                        occupancy - reference to the occupancy in the Tree

                        PositionInParent - a reference to the TNode position in
                        the parent TNode used to get to the current TNode's 
                        offset in the datafile
                        
                        fromSHB - indicator whether if the function was
                        called from SHB or not
        
Return:                 True or false depending if the remove was 
                        sucessful or not
----------------------------------------------------------------------*/
                                
template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
        fstream * fio, long & occupancy, offset & PositionInParent,
        long fromSHB) {
    
   unsigned long returnValue; //saves return value of Remove
   
   //if the node we want to remove is found from the disk
   if (data == elementTNode.data) {
      
      //setting predcessor node as the root
      elementTNode.data = this -> data;
      
      //deleting the root and leaf
      if (!left && !right) {
        
        PositionInParent=0; //setting PIP to null
      }
      
      //deleting node with no right child
      else if (left && !right) {
         
         PositionInParent = left; //setting PIP to left
      }
      
      //deleting node with no left child
      else if (!left && right) {
         
         PositionInParent = right; //setting PIP to right
      }
      
      //deleting node with two children
      else {
          
         TNode<Whatever>leftNode(left,fio); //used to call RARM

         //calling RARM on the left child
         leftNode.ReplaceAndRemoveMax(*this,fio,left); 
      }
      returnValue=1; //indicates that the remove is successful 
   }

   else if (data>elementTNode.data) {
      
      if (!left) {
         
         return 0; //element not in the Tree
      }

      else {
         
         TNode<Whatever> leftNode(left,fio); //used to make recursive call 
         //recursively calls Remove when left is not null
         returnValue = leftNode.Remove(elementTNode,fio,occupancy,left);
      }
   }

   else {
      
      if (!right) {
         
         return 0; //does not exist in Tree
      }
      
      else {
         
         TNode<Whatever> rightNode(right,fio); //used to make recursive call
         //recursively calls Remove when right is not null
         returnValue = rightNode.Remove(elementTNode,fio,occupancy,right);
      }
   }
   
   //if not called from SHB then call SHB
   if(!fromSHB) {
      SetHeightAndBalance(fio,PositionInParent);
   }
   
   //or else update the changes to the disk
   else {
      Write(fio);
   }

   return returnValue; //returns back to Tree's Remove
}
/*------------------------------------------------------------------------------
Function Name:          Remove (Tree)

Description:            First checks if the tree is empty by checking if 
                        occupancy is zero then, it will call TNode's remove 
                        function to find the element in the tree recursively.If
                        the element is found then it will be removed and if not
                        it will just return false

Parameter:              element - data we want to remove from the tree

Return:                 True or false depending if remove was successful or not
------------------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {

   Tree<Whatever>::IncrementOperation();//increments operation
   unsigned long status = 0; //used to save return value
   
   if (occupancy==0) {
      
      return 0;
   }

   TNode<Whatever>temp(element);//temp TNode used to delegate TNode's remove
   
   //calling object used to call TNode's remove
   TNode<Whatever>readRootNode(root,fio);
   
   //delegating TNode's remove
   status = readRootNode.Remove(temp,fio,occupancy,root);
   
   element = temp.data; //setting element to temp's data
   
   //if root node is removed then call ResetRoot
   if (occupancy==0) {
      ResetRoot();
   }
   
   //if a remove was successful decrement occupancy
   if (status) {
      occupancy--;
   }
   return status;
}
/*----------------------------------------------------------------------
Function Name:          SetHeightAndBalance
        
Description:            This function is called whenever an insertion or
                        removal occurs so that we can update the SHB for
                        the entire tree. If the balance of a certain 
                        node cause over the threshold then we will
                        remove that node with TNode's remove and insert
                        the same node back into the tree.
        
Parameter:              fio - filestream that corresponds to the datafile
                        where the Tree exists in the disk
                        
                        PositionInParent - a reference to the TNode 
                        pointer in parent which we use to get to the 
                        current node
----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
        offset & PositionInParent) {
   
   //both default height for right and left is -1
   long leftHeight = DEFAULT_HEIGHT;
   long rightHeight = DEFAULT_HEIGHT;
   
   //checks if left node exists
   if (left) {
      TNode<Whatever> temp(left, fio);
      leftHeight = temp.height;
   }
   
   //checks if right node exists 
   if (right) {
      TNode<Whatever> temp(right, fio);
      rightHeight = temp.height;
   }
   
   //setting current node's height to the greater height
   if (leftHeight < rightHeight) {
      
      height = rightHeight+1;//when right height is greater
   }

   else {
      
      height = leftHeight+1;//when left height is greater
   }

   balance = leftHeight - rightHeight; //updating balance

   //when balance goes beyond threshold
   if (abs(balance) > THRESHOLD) {
      
      long randomOccupancy = 0; //used to create a temp TNode
      
      TNode<Whatever>value(data); //used to call TNode's Remove using this data
      
      value.data=data; //making sure that value's data is equal to this data
      
      //removing node and setting from SHB to 1
      Remove(value,fio,randomOccupancy,PositionInParent,1);
      
      TNode<Whatever>temp(PositionInParent,fio); //used to call TNode's insert
      
      //reinserting the removed node back into the tree
      temp.Insert(value.data,fio,randomOccupancy,PositionInParent); 

   }
   //if balance does not go beyond threshold write to disk
   else {
      
      Write(fio);
   }
}
/*------------------------------------------------------------------------------
Function Name:          GetCost

Description:            Returns the current value of cost

parameter:              none

Returns:                The value (long) of the current cost
------------------------------------------------------------------------------*/
template <class Whatever>
long Tree <Whatever> :: GetCost () {
   return cost;
}

/*------------------------------------------------------------------------------
Function Name:          GetOperation

Description:            Returns the current value of operation

parameter:              none

Returns:                The value (long) of the current operation
------------------------------------------------------------------------------*/
template <class Whatever>
long Tree <Whatever> :: GetOperation () {
   return operation;
}

/*------------------------------------------------------------------------------
Function Name:          IncrementCost

Description:            Increments cost

parameter:              none
------------------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: IncrementCost () {
   cost++; //whenever Write and Read is called
}
/*------------------------------------------------------------------------------
Function Name:          IncrementOperation

Description:            Increments operation

parameter:              none
------------------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: IncrementOperation () {
   operation++; //whenever remove,insert,lookup is called
}
/*------------------------------------------------------------------------------
Function Name:          ResetRoot

Description:            Called in Tree's remove when the root node, the last
                        node in the Tree, is removed by setting the root offset
                        to the end of the disk

parameter:              none
------------------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: ResetRoot () {
   
   fio -> seekp(0,ios::end);
   root = fio->tellp(); //setting root offset to the end of the disk
}
/*----------------------------------------------------------------------
Function Name:          Lookup (TNode)
        
Description:            Looks for the element data in the tree. If the 
                        element is found in the tree then we set the 
                        element reference equal to data. If not it will
                        recursively go through the whole binary tree to 
                        search for the element. If it ends up at a null
                        left or right child then it will return false
                        for failure but if it found a match it will 
                        return true for success.
        
Parameter:              element - the data we want to look for in the 
                        tree

                        fio - filestream corresponding to the datafile where the
                        tree is located in the disk

Return:                 Returns true or false depending if the search is
                        successful or not
        ----------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever>:: Lookup(Whatever & element, fstream * fio) const{
   
   unsigned long returnValue; //used to save the return values
   
   //when element we are searching for is found
   if (data == element) {
      
      element = data; //setting element to data
      return 1;
   }
   
   //when element is less than current data
   else if (data > element) {
      
      //if left is empty then element does not exist in tree
      if (!left) {
         
         return 0;
      }
      TNode<Whatever>leftNode(left,fio); //used to call TNode's Lookup
      
      //if left exists then call Lookup at left
      returnValue = leftNode.Lookup(element,fio);
   }

   else {
      
      //if right is empty then element does not exist in tree
      if (!right) {
         
         return 0;
      }
      TNode<Whatever>rightNode(right,fio); //used to call TNode's Lookup
      
      //if right exists then call Lookup at right 
      returnValue = rightNode.Lookup(element,fio);
   }

   return returnValue; //returning back to Tree's lookup 

}

/*------------------------------------------------------------------------------
Function Name:          Insert (TNode)

Description:            Inserts an element into the tree, recursively. If an 
                        insertion has taken place then the function will call
                        SHB to update the height and balance. If a duplicate
                        insertion occurs then the newest element will replace 
                        the old. The recursion will continue until we find a 
                        null right or left child to insert the TNode. 

Parameter:              Element - the data we want to insert into the tree

                        fio - filestream corresponding to the datafile where the
                        Tree is stored on disk

                        occupancy - reference to the occupancy of Tree
                        
                        PointerInParent - a reference to the TNode pointer in
                        the parent TNode used to get to the current TNode

Return:                 Returns true or false indicating the success of the
                        insert. 
------------------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
        long & occupancy, offset & PositionInParent) {
   
   if (data==element) {
      
      //duplicate insert
      data = element; 

      //Write to the disk at this_position in order to update data
      fio -> seekp(this_position);
      Write(fio); 
      
      return 1;
   }

   else if (data > element) {
      
      //insert when left is null
      if (!left) {
         
         //inserting newTNode to the left
         TNode<Whatever> newTNode(element,fio,occupancy);
         //updating left's offset
         left = newTNode.this_position;
      }

      else {
         
         //used to recursively call Insert
         TNode<Whatever>leftTNode(left,fio);
         
         //calling the function on the left child
         leftTNode.Insert(element,fio,occupancy,left);
      }
   }

   else {
      
      //insert when right is null
      if (!right) {
         
         //inerting newTNode on the right
         TNode<Whatever>newTNode(element,fio,occupancy);
         //updating right's offset 
         right = newTNode.this_position;
      }

      else {
         //used to recursively call Insert
         TNode<Whatever>rightTNode(right,fio);
         //calling the function on the right child
         rightTNode.Insert(element,fio,occupancy,right);
      }
   }
   
   //updating SHB
   SetHeightAndBalance(fio,PositionInParent);
   return 1;
}
/*------------------------------------------------------------------------------
Function Name:          Lookup (Tree)

Description:            First checks if the tree is empty then, it will call
                        TNode's lookup function to find the element in the tree
                        recursively.Then returns the return value of TNode's
                        lookup in order to check if the search failed or
                        succeeded.

Parameter:              element - data we want to lookup in the tree

Return:                 True or false, depending if seach was successful or not
------------------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {
   
   Tree<Whatever>::IncrementOperation();
   
   unsigned long status = 0;//used to save the return value

   if (occupancy==0) {
      
      return 0;
   }
   
   TNode<Whatever> rootNode(root,fio); //used to deletegate TNode's lookup
   
   status = rootNode.Lookup (element,fio); //delegating TNode's lookup

   return status; //returning the return value of TNode's lookup
}

/*------------------------------------------------------------------------------
Function Name:          Read

Description:            Seeks the position then reads from position in order to
                        read the TNode that is present in the datafile into
                        memory. Called from TNode's read constructor

parameter:              position - offset in the datafile to the position of the
                        TNode we want to read into memory

                        fio - filestream corresponding to the datafile where the
                        Tree is stored on disk
------------------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) {
   
   Tree<Whatever>::IncrementCost(); //increments cost
   
   fio -> seekg(position);//goes to position
   
   //reads TNode into memory
   fio -> read((char*)this,sizeof(TNode<Whatever>));

   if(Tree<Whatever>::debug_on) {
      
      cerr<<COST_READ<<(const char*)data<<"]\n";
   }
}
/*------------------------------------------------------------------------------
Function Name:          TNode (TNode's read constructor)

Description:            Calls the Read function

parameter:              position - offset in the datafile to the position of the
                        TNode we want to read into memory

                        fio - filestream corresponding to the datafile where the
                        Tree is stored on disk
------------------------------------------------------------------------------*/
template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) {
   
   Read(position,fio);
}

/*------------------------------------------------------------------------------
Function Name:          TNode(TNode's write constructor)

Description:            Called when we want to insert a new TNode to the disk.

parameter:              element - data to be stored in the TNode we want to 
                        insert

                        occupancy - reference to occupancy of the tree, which
                        will be incremented when insertion occurs

                        fio - filestream corresponding to the datafile where the
                        Tree is stored on disk
------------------------------------------------------------------------------*/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy): 
                        data (element), height (0), balance (0), left (0), 
                        right (0) {
   //to make sure we insert a new node at the end of the file
   fio -> seekp(0,ios::end);
   
   this_position = fio -> tellp(); //in order to initialize this_position
   
   Write(fio); //writing to disk
   
   occupancy++; //incrementing occupancy
}

/*------------------------------------------------------------------------------
Function Name:          Write

Description:            Writes this TNode to the disk at this_position.

parameter:              fio -filestream corresponding to the datafile where the
                        Tree is stored on disk
------------------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const {
   
   Tree<Whatever>::IncrementCost(); //increment cost
   
   fio -> seekp(this_position); //seek this_position
   
   //writes out the node to the disk
   fio -> write((const char*)this,sizeof(TNode<Whatever>));

   if (Tree<Whatever>::debug_on) {
      
      cerr<<COST_WRITE<<(const char*)data<<"]\n";
   }

}
/*------------------------------------------------------------------------------
Function Name:          Tree Constructor 

Description:            First checks if the tree is empty. If it is then it will
                        write out its root and occupancy to the disk. If not
                        then it will move to the beginning of the file and read
                        in root and occupancy

parameter:              datafile - used to initialize the fstream
------------------------------------------------------------------------------*/
template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
        fio (new fstream (datafile, ios :: out | ios :: in)),
        occupancy(0),root(0) {
   
      
   static unsigned long count = 0; //used to store tree_count
   tree_count = ++count; //incrementing count
  
   //getting the beginning and end's offsets
   fio -> seekp(0,ios::beg);
   offset beginning = fio -> tellp(); 
   fio -> seekp(0,ios::end);
   offset ending = fio -> tellp();
   
   if(debug_on) {
      cerr<<TREE<<tree_count<<ALLOCATE;
   }
   
   //if file is empty
   if (beginning == ending) {
      
      fio -> seekp(0,ios::beg); //seeking to beginning of the file
      root = sizeof(root) + sizeof(occupancy); //setting root to 16
      
      //writing out the root and occupancy to disk 
      fio -> write((const char*)&root,sizeof(root)); 
      fio -> write((const char*)&occupancy,sizeof(occupancy));
   }

   else {
      //reads the root and occupancy from the disk at the beginning 
      fio -> seekg(0,ios::beg);
      fio -> read((char*)&root,sizeof(root));
      fio -> read((char*)&occupancy, sizeof(occupancy));

   }
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{  
   fio -> seekp(0,ios::beg);
   
      fio -> write((const char*)&root,sizeof(root));
      fio -> write((const char*)&occupancy, sizeof(occupancy));
      delete fio; 

  
  if (Tree<Whatever>::debug_on) {
      cerr<<TREE<<tree_count<<DEALLOCATE;
   }
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}

template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This funtion will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        long old_cost = cost;

        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        fio->seekg (0, ios :: end);
        offset end = fio->tellg ();

        // check for new file
        if (root != end) {
                TNode<Whatever> readRootNode (root, fio);
                readRootNode.Write_AllTNodes (stream, fio);
        }

        // ignore cost when displaying nodes to users
        cost = old_cost;

        return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
        if (left) {
                TNode<Whatever> readLeftNode (left, fio);
                readLeftNode.Write_AllTNodes (stream, fio);
        }
        stream << *this;
        if (right) {
                TNode<Whatever> readRightNode (right, fio);
                readRightNode.Write_AllTNodes (stream, fio);
        }

        return stream;
}

