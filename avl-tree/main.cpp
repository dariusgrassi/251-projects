//
// HW #08: AVL left and right rotate
//

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "avl.h"

using namespace std;


int main()
{
    avltree<int, int>  avl;
    int    k;
    string rotation;

    cout << "Enter a key to insert into tree (0 to stop inserting)> ";
    cin >> k;

    while (k != 0)
    {
        avl.insert(k, k);

        cout << endl;
        cout << "Size: " << avl.size() << endl;
        cout << "Height: " << avl.height() << endl;
        avl.inorder();
        cout << endl;

        cout << "Enter a key to insert into tree (0 to stop inserting)> ";
        cin >> k;

    }
    int start, dest;

    cout << "Enter two keys to check the distance(-1 to quit): ";
    cin >> start;

    while( start != -1){

cin >> dest;
        int v = avl.distance( start, dest);

        cout << "Distance is: " << v << endl;
        cout << "Enter two keys to check the distance(-1 to quit): ";
        cin >> start;
    }



    return 0;
}