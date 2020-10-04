/*test.cpp*/
 
//
// Lab week 07:
// 
// AVL unit tests based on Catch framework.  We are assuming the AVL class is now
// balancing the trees in these tests.
//
 
#include <iostream>
#include <vector>
 
#include "avl.h"
 
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
 
using namespace std;
 
TEST_CASE("(1) AVL tree with 4 nodes") 
{
  avltree<int, int>  avl;
 
  //
  // build a simple BST tree with 4 nodes:
  //
  avl.insert(100, -100);
  avl.insert(50, -50);
  avl.insert(150, -150);
  avl.insert(125, -125);
 
  // 
  // Tests:
  //
  vector<int>  keys, values, heights;
 
  keys = avl.inorder_keys();
  values = avl.inorder_values();
  heights = avl.inorder_heights();
 
  REQUIRE(avl.size() == 4);
  REQUIRE(avl.height() == 2);
 
  REQUIRE(keys[0] == 50);
  REQUIRE(values[0] == -50);
  REQUIRE(heights[0] == 0);
 
  REQUIRE(keys[1] == 100);
  REQUIRE(values[1] == -100);
  REQUIRE(heights[1] == 2);
 
  REQUIRE(keys[2] == 125);
  REQUIRE(values[2] == -125);
  REQUIRE(heights[2] == 0);
 
  REQUIRE(keys[3] == 150);
  REQUIRE(values[3] == -150);
  REQUIRE(heights[3] == 1);
}
 
TEST_CASE("(2) empty tree") 
{
  avltree<int, int>  avl;
 
  REQUIRE(avl.size() == 0);
  REQUIRE(avl.height() == -1);
}
 
TEST_CASE("(3) just one node") 
{
  avltree<int, int>  avl;
     
    avl.insert(123, 88);
 
  REQUIRE(avl.size() == 1);
  REQUIRE(avl.height() == 0);
     
    vector<int>  keys, values, heights;
 
  keys = avl.inorder_keys();
  values = avl.inorder_values();
  heights = avl.inorder_heights();
 
    REQUIRE(keys[0] == 123);
    REQUIRE(values[0] == 88);
  REQUIRE(heights[0] == 0);
}
 
TEST_CASE("(4) build a small tree that triggers AVL case 1") 
{
  avltree<int, int>  avl;
 
  //
  // build a simple BST tree with 3 nodes:
  //
  avl.insert(100, -100);
  avl.insert(50, -50);
  avl.insert(25, -25);  // triggers right rotate at the root:
 
  // 
  // Tests:
  //
  vector<int>  keys, values, heights;
 
  keys = avl.inorder_keys();
  values = avl.inorder_values();
  heights = avl.inorder_heights();
 
  REQUIRE(avl.size() == 3);
  REQUIRE(avl.height() == 1);
 
  REQUIRE(keys[0] == 25);
  REQUIRE(values[0] == -25);
  REQUIRE(heights[0] == 0);
 
  REQUIRE(keys[1] == 50);
  REQUIRE(values[1] == -50);
  REQUIRE(heights[1] == 1);
 
  REQUIRE(keys[2] == 100);
  REQUIRE(values[2] == -100);
  REQUIRE(heights[2] == 0);
}