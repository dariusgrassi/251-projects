/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:     Darius Grassi
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

	//
	// input the plates and fines:
	//
	cout << "Reading '" << infilename << "'..." << endl;

	string fine;
	string plate;

	getline(infile, fine);

	//
	// for each pair (fine, license plate), hash and store/update fine:
	//
	
  
  while (!infile.eof())
  {
    getline(infile, plate);
	
    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  }
}

//Swap helper function for selection sort of vectors
void swap(int& t1, int& t2){
	int temp = 0;
	
	temp = t1;
	t1 = t2;
	t2 = temp;
}


int main()
{
	int    N;        // = 10000;
	string basename; // = "tickets1";

	cout << "Enter hashtable size> ";
	cin >> N;

	hashtable<string, int>  ht(N);
	ILplates                hashplates(ht);

	cout << "Enter base filename> ";
	cin >> basename;
	cout << endl;

	string outfilename = basename + "-output.txt";
	ofstream outfile(outfilename);

	//
	// process input file of fines and license plates:
	//
	hashInput(basename, hashplates);

	//
	// debugging:
	//
	
	int minPosition = 0;
	
	vector<string> plates = ht.Keys();
	vector<int> amounts = ht.Values();
	string min = plates.at(0);
	int size = plates.size() ;
	string temp;

	cout << "Sorting..." << endl;
	for(int i = 0; i < size-1; i++){
		minPosition = i;
		
		
		for(int j = i+1; j < size; j++){
			if(plates[j].compare(plates[minPosition]) < 0){
				minPosition = j;
			}
		}
		
		//swap the smallest value to its correct position
		swap(plates[minPosition], plates[i]);
		swap(amounts[minPosition], amounts[i]);
	}
	
	//Write to output file here
	cout << "Writing '" << outfilename << "'..." << endl;  
	for (size_t i = 0; i < plates.size(); ++i)
	{
		outfile << "\"" << plates[i] << "\"" << " $" << amounts[i] << endl;
	}

  //
  // done:
  //
  return 0;
}