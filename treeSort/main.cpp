//
//  main.cpp
//  Project4
//
//  Created by Zach Manno on 4/14/15.
//  This program uses a balanced binary tree sort to sort a list of names
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

void loadTree (struct node*,string); //to load tree
void printInorder (struct node *root); //to print tree inorder(debugging)
void balanceCalc (struct node *root, struct node *newNode); //calculate balance
void imbalanceLeft(struct node *root, struct node *newNode); //rebalance tree with left imbalance
void imbalanceRight(struct node *root, struct node *newNode); //rebalance tree with right imbalance
void printInorderTabular (struct node *root); //printing function

struct node {
    
    string value;   // value of node
    short height;    // node height
    short balance;  // node balance
    struct node *leftPtr;    // pointer to left
    struct node *rightPtr; // pointer to right
};

int main()
{
    string name; //name of value to be stored in tree
    string fileName; //user inputted file name
    
    struct node *root; //to store the root node
    
    cout << "Enter the name of the input file\n";
    cin >> fileName;
    ifstream myfile (fileName);
    
    myfile >> name; //read in first entry
    node *newPtr = new node; //make a new node for the root node
    
    root = newPtr;
    root -> value = name; //set the first string as the root node
    root -> height = 1; //set height as 1
    
    myfile >> name; //read in next name
    loadTree (root,name); //load the tree with the new name
    
    while(!myfile.eof())
    {
        myfile >> name; //read in each additional name
        loadTree (root,name); //load each name
    }
    cout << left << setw(10) <<  "\nNode" << setw(10) << "Left" << setw(10) << "Right" << right << setw(10) << "Height" << right << setw(10) << "Balance" << endl;
    
    printInorderTabular(root); //function call to print inorder in tabular format
    
    return 0;
    
}//end main

void loadTree (struct node *root,string name) {
    
    /*Inorder Load:
     
     Left
     Node
     Right
     
     */
    
    string rootValue; //for storing the value of the root to compare to the name string
    
    if(root != NULL)
        rootValue = root -> value;
    
    if(root == NULL) return; //base case
    
    if( name[0] < rootValue[0]) loadTree(root->leftPtr,name); //call left
    
    node *newNode = new node;
    newNode -> value = name;
    
    if(root->leftPtr == NULL)
    {
        if(name[0] < rootValue[0])
        {
            //set new node as root's left pointer, calculate balance
            newNode -> value = name;
            newNode -> height = 1;
            root -> leftPtr = newNode;
            balanceCalc(root,newNode);
            return;
        }
    }
    
    if(root->rightPtr == NULL)
    {
        if(name[0] > rootValue[0])
        {
            //set new node as root's right pointer, calculate balance
            newNode -> height = 1;
            root -> rightPtr = newNode;
            balanceCalc(root,newNode);
            return;
        }
    }
    
    if( name[0] > rootValue[0]) loadTree(root -> rightPtr,name); //call right
    
    balanceCalc(root,newNode); //balance calculation before returning
    
    return;
}

void printInorder (struct node *root) { //This function prints the progress of the tree, used only for debugging
    
    /*Preorder:
     
     Left
     Right
     Node
     
     */
    
    struct node *auxPtr;
    
    if(root == NULL) return; //base case
    
    printInorder(root->leftPtr); //call left
    
    cout << "Value: " << root -> value << endl; //Print node
    cout << "Height: " << root -> height << endl; //Print height
    cout << "Balance: " << root -> balance << endl; //Print balance
    if( (abs(root -> balance)) > 1) cout << "*IMBALANCE*" << endl; //Print if theres an imbalance
    
    if(root -> leftPtr != NULL) //Print Left Ptr
    {
        auxPtr = root -> leftPtr;
        cout << "Left Ptr: " << auxPtr -> value << endl;
    }
    else cout << "Left Ptr: NULL" << endl;
    
    if(root -> rightPtr != NULL) //Print Right Ptr
    {
        auxPtr = root -> rightPtr;
        cout << "Right Ptr: " << auxPtr -> value << endl << endl;
    }
    else cout << "Right Ptr: NULL" << endl << endl;
    
    printInorder(root -> rightPtr); //call right
    
    return;
    
}

void balanceCalc (struct node *root, struct node *newNode) { //To calculate the balance of a node
    
    int max = 0; //max height between left and right ptr
    int rootBalance; //balance of the root
    int auxPtr1Height = 0; //left ptr height
    int auxPtr2Height = 0; //right prt height
    struct node *auxPtr1; //points to node's left ptr
    struct node *auxPtr2; //points to node's right ptr
    
    if(root == NULL) return; //base case
    
    if((root -> rightPtr == NULL) && (root -> leftPtr ==NULL)) //if node is a leaf, height is 1
    {
        root -> height = 1;
        // return;
    }
    else
    {
        if( root -> leftPtr != NULL) //if the node has a left pointer, set leftPtr height
        {
            auxPtr1 = root -> leftPtr;
            //debugging
            /*   if(auxPtr1 -> rightPtr != NULL)
             {
             auxPtr3 = auxPtr1 -> rightPtr;
             
             cout << "TEST\n";
             cout << root -> value << endl;
             cout << auxPtr1 -> value << endl;
             cout << auxPtr3 -> value << endl;
             }
             */
            
            auxPtr1Height = auxPtr1 -> height;
        }
        
        if(root -> rightPtr != NULL) //same for right ptr
        {
            auxPtr2 = root -> rightPtr;
            auxPtr2Height = auxPtr2 -> height;
        }
        
        //finding max between left and right ptr height and adding 1
        if( auxPtr1Height < auxPtr2Height)
        {
            max = auxPtr2Height;
            root -> height = max+1;
        }
        if( auxPtr1Height > auxPtr2Height)
        {
            max = auxPtr1Height;
            root -> height = max+1;
        }
        if (auxPtr1Height == auxPtr2Height)
        {
            max = auxPtr1Height;
            root -> height = max+1;
        }
        
    }
    
    rootBalance = auxPtr1Height - auxPtr2Height; //equation for root balance
    root -> balance = rootBalance; //setting root balance
    
    if( (abs(rootBalance)) > 1) //if there's an imbalance, call left or right imbalance function accordingly
    {
        // cout << "Imbalance detected" << endl;
        if(auxPtr1Height > auxPtr2Height)
        {
            //cout << "Left imbalance" << endl;
            imbalanceLeft(root,newNode);
            
        }
        if(auxPtr2Height > auxPtr1Height)
        {
            //cout << "Right imbalance" << endl;
            imbalanceRight(root,newNode);
        }
    }
    return;
}

void imbalanceLeft (struct node *root, struct node *newNode) { //to balance the tree when a left imbalance occurs
    
    bool LL = false; //if it is a left left imbalance
    bool LR = false; //if it is a left right imbalance
    string temp = "";
    
    /*cout << "*********************\n";
     cout << "Imbalanced node = " << root -> value << endl;
     cout << "Balance: " << root -> balance << endl;
     cout << "New node = " << newNode -> value << endl;
     cout << "*********************\n";*/
    
    struct node *auxPtrLLcheck; //node's left pointer
    
    if(root -> leftPtr != NULL) auxPtrLLcheck = root -> leftPtr;
    
    if(auxPtrLLcheck -> leftPtr != NULL)
    {
        //if the node's leftPtr's leftPtr value = the new node value, it is a LL imbalance
        if ((auxPtrLLcheck -> leftPtr -> value) == (newNode -> value)) LL = true;
        
    }
    
    if(auxPtrLLcheck -> rightPtr != NULL)
    {
        //if the node's leftPtr's rightPtr value = the new node value, it is a LR imbalance
        if ((auxPtrLLcheck -> rightPtr -> value) == (newNode -> value)) LR = true;
        
    }
    
    if(LL)
    {
        //cout << "Imbalance type: Left Left\n\n";
        
        //step1
        temp = root -> value;
        root -> value = auxPtrLLcheck -> value;
        auxPtrLLcheck -> value = temp;
        
        //step2
        root -> rightPtr = auxPtrLLcheck;
        root -> leftPtr = newNode;
        
        //step 3
        auxPtrLLcheck -> leftPtr = newNode -> rightPtr;
        
        //recalculating balances for the effected nodes
        balanceCalc(root-> rightPtr, newNode);
        //cout << "root leftptr balance calc for " << root -> leftPtr -> value << endl;
        balanceCalc(root-> leftPtr, newNode);
        //cout << "root balance calc for " << root -> value << endl;
        balanceCalc(root, newNode);
    }
    
    if(LR)
    {
        //cout << "Imbalance type: Left Right\n\n";
        temp = newNode -> value;
        newNode -> value = root -> value;
        root -> value = temp;
        
        //step 2 (LLcheck = aux2)
        auxPtrLLcheck -> rightPtr = newNode -> leftPtr;
        
        //step 3
        newNode -> rightPtr = root -> rightPtr;
        
        //step 4
        root -> rightPtr = newNode;
        
        //recalculate balances for the effected nodes
        balanceCalc(root-> rightPtr, newNode);
        balanceCalc(root-> leftPtr, newNode);
        balanceCalc(root, newNode);
        
        //printInorder(root);
    }
    return;
}

void imbalanceRight (struct node *root, struct node *newNode) { //to balance the tree when a right imbalance occurs
    
    //Same logic as imbalance left function but in reverse
    bool RR = false;
    bool RL = false;
    string temp = "";
    
    /*cout << "*********************\n";
     cout << "Imbalanced node = " << root -> value << endl;
     cout << "Balance: " << root -> balance << endl;
     cout << "New node = " << newNode -> value << endl;
     cout << "*********************\n";*/
    
    struct node *auxPtrRRcheck;
    
    if(root -> rightPtr != NULL) auxPtrRRcheck = root -> rightPtr;
    if(auxPtrRRcheck -> rightPtr != NULL)
    {
        if ((auxPtrRRcheck -> rightPtr -> value) == (newNode -> value)) RR = true;
        
    }
    
    if(auxPtrRRcheck -> leftPtr != NULL)
    {
        if ((auxPtrRRcheck -> leftPtr -> value) == (newNode -> value)) RL = true;
        
    }
    
    if(RR)
    {
        //cout << "Imbalance type: Right Right\n\n";
        
        //step1
        temp = root -> value;
        root -> value = auxPtrRRcheck -> value;
        auxPtrRRcheck -> value = temp;
        
        //step2
        root -> leftPtr = auxPtrRRcheck;
        root -> rightPtr = newNode;
        auxPtrRRcheck -> rightPtr = newNode -> leftPtr;
        
        //recalcuate the balance for the effected nodes
        balanceCalc(root-> leftPtr, newNode);
        balanceCalc(root-> rightPtr, newNode);
        balanceCalc(root, newNode);
        
        //printInorder(root);
    }
    
    if(RL)
    {
        //cout << "Imbalance type: Right Left\n\n";
        
        //step 1
        temp = newNode -> value;
        newNode -> value = root -> value;
        root -> value = temp;
        
        //step 2 (LLcheck = aux2)
        auxPtrRRcheck -> leftPtr = newNode -> rightPtr;
        
        //step 3
        newNode -> leftPtr = root -> leftPtr;
        
        //step 4
        root -> leftPtr = newNode;
        
        //recalcuate the balance for the effected nodes
        balanceCalc(root-> leftPtr, newNode);
        balanceCalc(root-> rightPtr, newNode);
        balanceCalc(root, newNode);
        
        //printInorder(root);
    }
    return;
}

void printInorderTabular (struct node *root){ //To print the tree inorder in a tabular format
    
    /*Inorder:
     
     Left
     Node
     Right
     
     */
    
    if(root == NULL) return; //base case
    
    printInorderTabular(root->leftPtr); //call left
    
    //Print node information
    cout << left << setw(10) << root -> value;
    
    if(root -> leftPtr != NULL) cout << setw(10) << root -> leftPtr -> value;
    else cout << setw(10) << "NULL";
    
    if(root -> rightPtr != NULL) cout << setw(10) << root -> rightPtr -> value;
    else cout << setw(10) << "NULL";
    
    cout << right << setw(10) << root -> height << setw(10) << root -> balance << endl;
    
    printInorderTabular(root -> rightPtr); //call right
    
    return;
}
