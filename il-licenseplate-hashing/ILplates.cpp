/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// Darius Grassi
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>

#include "ILplates.h"

using namespace std;

bool allCaps(string plate){
	for(size_t i = 0; i < plate.length(); i++){
		
		if(!(isupper(plate[i]) || isspace(plate[i]))){
			return false;
		}
		
	}
	
	return true;
}

bool allDigits(string plate){
	for(size_t i = 0; i < plate.length(); i++){
		
		if(!(isdigit(plate[i]))){
			return false;
		}
		
	}
	
	return true;
}

//Helper function to concatenate a string of ascii values of characters
//and return them as a string
int convertCharacters(string plate){
	int converted = 0;
	
	for(char c : plate){
		converted += (c - 'A' + 1);
	}
	
	return converted;
}
//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH
//
int ILplates::Hash(string plate)
{
	unsigned long long index = -1;
	int numSpaces = 0;
			
	//if length is out of bounds
	if((plate.length() > 8) || (plate.length() == 0)){
		return -1;
	}
	
	//loop to check individual characters of plate
	for(char c : plate){
		
		//if there is a space, increment numSpaces
		if(isspace(c) && (c != plate[0])){
			numSpaces++;
		}
		
		//if character is not uppercase letter and not a digit and not a space
		if(!(isupper(c)) && !(isdigit(c)) && !(isspace(c))){
			return -1;
		}
		
		//if there is more than one space
		if(numSpaces > 1){
			return -1;
		}
	}
	
	//if it is a personalized plate
	string capitals, digits;
	
	if(numSpaces == 1){
		for(size_t i = 0; i < plate.length(); i++){
			
			if(isspace(plate[i])){
				
				//Split string into capitals and digits
				capitals = plate.substr(0, i);
				digits = plate.substr(i+1, plate.length()-1);
				
				
				//Checks if left is all capitals and right is all digits, if not return -1
				if(!(allDigits(digits) && allCaps(capitals))){
					return -1;
				}
				
				//if personalized capitals arent 1-5 letters and digits are 1-99
				//or personalized capitals arent 1-6 letters and digits are 1-9
				if((capitals.length() >= 1 && capitals.length() <= 5) && (digits.length() > 2)){
					return -1;
				}
				else if((capitals.length() < 1 || capitals.length() > 6) && (digits.length() > 1)){
					return -1;
				}
			}
		}
		
		int processedCapitals = convertCharacters(capitals);
		//values are multiplied randomly to scatter 
		index = ((stoi(digits)) * 1500) + (processedCapitals * 2);
	}
	
	//Else it is a vanity plate
	else {
		//if plate isnt all digits or all caps
		if(!(allDigits(plate) || allCaps(plate))){
			return -1;
		}

		//if length of string is greater than 7
		if(plate.length() > 7){
			return -1;
		}
		
		//if the plate is only digits 0-999
		if(allDigits(plate)){
			if((plate.length() < 1) || (plate.length() > 3)){
				return -1;
			}
			
			index = stoi(plate);
		}
		
		//if the plate is only characters 1000 - 1999
		if(allCaps(plate)){
			if((plate.length() < 1) || (plate.length() > 7)){
				return -1;
			}
			
			index = 1500 + (convertCharacters(plate)*43);
		}
	}
	
	return index % HT.Size();
}


//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
	int index = Hash(plate);
	bool empty = false;
	string key = plate;
	int value = 0;

	if(index == -1){
		return -1;
	}
	
	//loop to check each index
	while(!empty){
		HT.Get(index, empty, key, value);
		
		if(key == plate){
			return value;
		}
		
		index = (index + 1) % HT.Size();
	}

	return -1;
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
	int goalIndex = Hash(plate);
	
	if(goalIndex == -1 || goalIndex > HT.Size()){
		return;
	}
	
	int probed = 0;
	string key = plate;
	int value = 0;
	bool empty = false;
		
	while(probed < HT.Size()){				
		HT.Get(goalIndex, empty, key, value);
		
		if(empty || plate == key){
			HT.Set(goalIndex, plate, newValue);
			return;
		}
		
		goalIndex = (goalIndex + 1) % HT.Size();
		probed++;
	}
		
	return;
}
