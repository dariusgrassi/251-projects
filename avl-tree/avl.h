/*avl.h*/

#pragma once

#include <iostream>
#include <algorithm>  // std::max
#include <cstdlib>    // std::abs
#include <stack>
#include <vector>
#include <cassert>

using namespace std;

template<typename TKey, typename TValue>
class avltree
{
private:
  struct NODE
  {
    TKey   Key;
    TValue Value;
    int    Height;
    NODE*  Left;
    NODE*  Right;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)

  // 
  // _inorder does the actual inorder traversal and output 
  // to console.  Each key is output to the console followed
  // by " ", including the last key.
  //
  void _inorder(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder(cur->Left);
      cout << "(" << cur->Key << "," << cur->Value << "," << cur->Height << ")" << " ";
      _inorder(cur->Right);
    }
  }

  void _inorder_keys(NODE* cur, std::vector<TKey>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_keys(cur->Left, V);
      V.push_back(cur->Key);
      _inorder_keys(cur->Right, V);
    }
  }

  void _inorder_values(NODE* cur, std::vector<TValue>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_values(cur->Left, V);
      V.push_back(cur->Value);
      _inorder_values(cur->Right, V);
    }
  }

  void _inorder_heights(NODE* cur, std::vector<int>& V)
  {
    if (cur == nullptr)
      return;
    else
    {
      _inorder_heights(cur->Left, V);
      V.push_back(cur->Height);
      _inorder_heights(cur->Right, V);
    }
  }
  
  //Helper function used for destructor and clear functions
  //Uses postorder traversal method to delete each node in tree
  void _postorder(NODE* cur) {
	  if (cur == nullptr)
		  return;
		  
	  _postorder(cur->Left);
	  _postorder(cur->Right);
	  
	  delete cur;
  }

  //
  // _copytree:
  //
  // Given a tree, inserts the (key, value) pairs into *this* tree.
  //
  void _copytree(NODE* cur)
  {
    if (cur == nullptr)
      return;
    else
    {
      //
      // to copy and build an exact copy, we need to "visit" cur
      // first and insert that (key, value) pair first.  That way
      // all the nodes to the left will properly go to the left,
      // all the node to the right will properly go to the right.
      //
      insert(cur->Key, cur->Value);

      _copytree(cur->Left);
      _copytree(cur->Right);
    }
  }

  //
  // _RightRotate
  // _LeftRotate
  //
  // Rotates the tree around the node N, where Parent is N's parent.  Note that
  // Parent could be null, which means N is the root of the entire tree.  If 
  // Parent denotes a node, note that N could be to the left of Parent, or to
  // the right.  You'll need to take all this into account when linking in the
  // new root after the rotation.  Don't forget to update the heights as well.
  //
  void _RightRotate(NODE* Parent, NODE* N)
  {
    assert(N != nullptr);  // pre-conditions: N and L must exist
    assert(N->Left != nullptr);

    NODE* L = N->Left;
    NODE* B = L->Right;
    
    N->Left = B;
	L->Right = N;
    
    if (Parent == nullptr)
       Root = L;
    else if (L->Key < Parent->Key)
       Parent->Left = L;
    else
       Parent->Right = L;
       
    //update heights
    //checks if both N children are null
    if(N->Left != nullptr && N->Right != nullptr) {
	   N->Height = 1 + max(N->Left->Height, N->Right->Height); //if both exist, compute height
	}
    else if(N->Left == nullptr && N->Right != nullptr) {  //checks if only left child exists
       N->Height = 1 + N->Right->Height;
	}   
    else if(N->Right == nullptr && N->Left != nullptr) {   //checks if only right child exists
       N->Height = 1 + N->Left->Height;
	}
    else {
		 N->Height = 0;
	}
        
    //checks if both L children are null
	if(L->Left == nullptr)  
       L->Height = 1 + L->Right->Height;
    else
       L->Height = 1 + max(L->Left->Height, L->Right->Height);
  }

  void _LeftRotate(NODE* Parent, NODE* N)
  {
    assert(N != nullptr);  // pre-conditions: N and L must exist
    assert(N->Right != nullptr);

    NODE* R = N->Right;
    NODE* B = R->Left;
    
    N->Right = B;
	R->Left = N;
    
    if (Parent == nullptr)
       Root = R;
    else if (R->Key < Parent->Key)
       Parent->Left = R;
    else
       Parent->Right = R;
       
    //update heights
    //checks if both N children are null
    if(N->Left == nullptr && N->Right == nullptr)
       N->Height = 0;
    else if(N->Left == nullptr && N->Right != nullptr)   //checks if only left child exists
       N->Height = 1 + N->Right->Height;
    else if(N->Right == nullptr && N->Left != nullptr)   //checks if only right child exists
       N->Height = 1 + N->Left->Height;
    else
       N->Height = 1 + max(N->Left->Height, N->Right->Height); //if both exist, compute height
        
    if(R->Right == nullptr)  //checks if both L children are null
       R->Height = 1 + R->Left->Height;
    else
       R->Height = 1 + max(R->Left->Height, R->Right->Height);
  }
  
  //If the tree height is broken at a certain node, _RotateToFix is called
  //to re-balance the tree. Handles all 4 cases.
  void _RotateToFix(NODE* Parent, NODE* N)
  {
	  assert(N != nullptr);
	  
	  int hLN = (N->Left == nullptr) ? -1 : N->Left->Height;
	  int hRN = (N->Right == nullptr) ? -1 : N->Right->Height;
	  	  
	  //Case 1 or 2: Tree is leaning left
	  if(hLN - hRN > 1){
		  int hL1 = (N->Left->Left == nullptr) ? -1 : N->Left->Left->Height;
		  int hR1 = (N->Left->Right == nullptr) ? -1 : N->Left->Right->Height;
		  
		  if(hL1 - hR1 >= 1) {
			  _RightRotate(Parent, N);    //Case 1
			}
		  else {
			  _LeftRotate(N, N->Left);    //Case 2
			  _RightRotate(Parent, N);
		  }
	  }
	  else {
		  int hL2 = (N->Right->Left == nullptr) ? -1 : N->Right->Left->Height;
		  int hR2 = (N->Right->Right == nullptr) ? -1 : N->Right->Right->Height;
		  
		  if(hL2-hR2 >= 1) {
			  _RightRotate(N, N->Right);    //Case 3
			  _LeftRotate(Parent, N);
		  }
		  else {
			  _LeftRotate(Parent, N);    //Case 4
		  }
	  }
   }
   
   //Helper function that returns the distance from cur to the node
   //in the tree holding TKey key. 
   int _findDistanceToNode(TKey key, NODE* cur)
   {
	   int distance = 0;
	   
	   //moves cur to track the node in the tree
	   //keeps track of distance it takes to find the node from cur.
	   while(cur != nullptr)
	   {
		   if(key == cur->Key)
			   return distance;
		   
		   if(key < cur->Key)
			   cur = cur->Left;
		   else
			  cur = cur->Right;
			  
		   distance++;
	   }
	   
	   return -1;
   }


public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  avltree()
  {
    Root = nullptr;
    Size = 0;
  }
  
  //
  // destructor :
  //
  virtual ~avltree(){
	  _postorder(Root);
  }

  //
  // copy constructor:
  //
  avltree(avltree& other)
  {
    Root = nullptr;
    Size = 0;

    _copytree(other.Root);
  }

  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  int size()
  {
    return Size;
  }

  //
  // height
  //
  // Returns the height of tree; the height of an empty tree is
  // defined as -1.
  //
  int height()
  {
    if (Root == nullptr)
      return -1;
    else
      return Root->Height;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning a pointer to the 
  // value if found and nullptr if not.  We return a pointer so the caller
  // may update the underlying value if needed.
  //
  TValue* search(TKey key)
  {
    NODE* cur = Root;
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
      {
        return &cur->Value;
      }

      if (key < cur->Key)  // search left:
      {
        cur = cur->Left;
      }
      else
      {
        cur = cur->Right;
      }
    }//while  

    // if get here, not found
    return nullptr;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the tree; if the key has 
  // already been inserted then the function returns without changing 
  // the tree.
  //
  void insert(TKey key, TValue value)
  {
    NODE* prev = nullptr;
    NODE* cur = Root;

    //
    // stack the nodes we visit so we can walk back up
    // the search path later, adjusting heights:
    //
    stack<NODE*> nodes;

    //
    // 1. Search to see if tree already contains key:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key)  // already in tree
        return;

      nodes.push(cur);  // stack so we can return later:

      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
        cur = cur->Right;
      }
    }//while

    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 
    NODE* newNode;
    newNode = new NODE();
    newNode->Key = key;
    newNode->Value = value;
    newNode->Height = 0;  // leaf node -> sub-tree of height 0:
    newNode->Left = nullptr;
    newNode->Right = nullptr;

    //
    // 3. link in the new node:
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //
    if (prev == nullptr)
      Root = newNode;
    else if (key < prev->Key)
      prev->Left = newNode;
    else
      prev->Right = newNode;

    // 
    // 4. update size:
    //
    Size++;

    //
    // 5. walk back up tree using stack and update heights.
    //
    //
    while (!nodes.empty())
    {
      cur = nodes.top();
      nodes.pop();
	  
	  //If cur->Left or cur->Right is nullptr, height is -1
	  //else, the height is calculated normally
      int hL = (cur->Left == nullptr) ? -1 : cur->Left->Height;
      int hR = (cur->Right == nullptr) ? -1 : cur->Right->Height;
      int hCur = 1 + std::max(hL, hR);

      if (cur->Height == hCur)  // didn't change, so no need to go further:
        break;
      else  // height changed, update and keep going:
        cur->Height = hCur;
		
	  //checks if tree heights are broken
	  if(abs(hL - hR) > 1) {
		  if(nodes.empty()) {  //if cur is root
		    _RotateToFix(nullptr, cur);
		  }
		  else {//if cur is not root
			_RotateToFix(nodes.top(), cur);
		  }
	  }
    }//while
	
    //
    // done!
    //
    return;
  }
  
  //Calculates distance between two nodes in the tree
  int distance(int k1, int k2)
  {
	  int total_distance = 0;
	  NODE* cur = Root;
	  
	  //Condition to handle if either of the keys don't exist in the tree
	  if(search(k1) != nullptr && search(k2) != nullptr){ 
		  //If they do exist and the keys are the same, just return 0
		  //since there is no distance between the same node.
		  if(k1 == k2) {
			  return 0;
		  }		  
	  }
	  else {
		  return -1;
	  }
	  
	  //Loop to move cur if nodes are on either the right or left of the root
	  while(cur != nullptr) 
	  {
		  //If k1 and k2 are on opposite sides of cur
		  if((k1 <= cur->Key && k2 >= cur->Key) || (k1 >= cur->Key && k2 <= cur->Key))
		  {
			  int root_to_k1 = _findDistanceToNode(k1, cur);
			  int root_to_k2 = _findDistanceToNode(k2, cur);

			  //add distance from root to each node 
			  total_distance = root_to_k1 + root_to_k2;
			  return total_distance;
		  }
		  //If k1 and k2 are both left of cur
		  else if(k1 < cur->Key && k2 < cur->Key)
		  {
			  cur = cur->Left;
			  continue;
		  }
		  //If k1 and k2 are both right of cur
		  else if(k1 > cur->Key && k2 > cur->Key)
		  {
			  cur = cur->Right;
			  continue;
		  }
	  }
	  
	  //random return value; should never be triggered
	  return -999;
  }

  //
  // inorder:
  //
  // Performs an inorder traversal of the tree, outputting
  // the keys to the console.
  //
  void inorder()
  {
    cout << "Inorder: ";

    _inorder(Root);

    cout << endl;
  }

  std::vector<TKey> inorder_keys()
  {
    std::vector<TKey>  V;

    _inorder_keys(Root, V);

    return V;
  }

  std::vector<TValue> inorder_values()
  {
    std::vector<TValue>  V;

    _inorder_values(Root, V);

    return V;
  }

  std::vector<int> inorder_heights()
  {
    std::vector<int>  V;

    _inorder_heights(Root, V);

    return V;
  }

  // clear: resets a tree to empty
  void clear(){
	  _postorder(Root);
	  
	  Root = nullptr;
	  Size = 0;
  }
};
