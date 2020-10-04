//
// Darius Grassi
// U. of Illinois, Chicago
// CS 251: Fall 2019
//
// Project #02: inputs movies and reviews, allowing the user to search
// by movie ID, movie Name, or review ID.  Uses binary search trees for 
// fast search; no other data structures are allowed.
//
 
#include <iostream>
#include <fstream>
#include <string>
#include "bst.h"
 
using namespace std;
 
//Holds the data for the number of reviews read in from the text files
struct MovieData
{
    string name;
    int PubYear = 0;
    int Num5Stars = 0;
    int Num4Stars = 0;
    int Num3Stars = 0;
    int Num2Stars = 0;
    int Num1Stars = 0;
};


//
// trim
//
// trims whitespace from beginning and end of string.
//
// Ref: https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
// Modified: to handle empty strings, and strings of all blanks.
//
string trim(const string& str)
{
  size_t first = str.find_first_not_of(" \t\n\r");
  size_t last = str.find_last_not_of(" \t\n\r");
 
  if (first == string::npos && last == string::npos)  // empty, or all blanks:
    return "";
  else if (first == string::npos)  // no blanks at the start:
    first = 0;                     // 0-based
  else if (last == string::npos)   // no blanks trailing at end:
    last = str.length() - 1;       // 0-based
 
  return str.substr(first, (last - first + 1));
}
 
 
//
// InputMovies
//
// Inputs the data from the "movies" file and "reviews" file, which contains N>0 lines, where
// each line contains:
//     id pubYear name
// or:
//     reviewID movieID numStars
//
void InputMovies(string moviesFilename, string reviewsFilename, binarysearchtree<int, MovieData>& movieIDs,
        binarysearchtree<string, int>& movieNames, int& reviewCounter) {

    // **********************************************
    //          Read in movies File here
    // **********************************************
    ifstream moviesFile(moviesFilename);
    int id, pubYear;
    string name;
    MovieData ID_data;

    if (!moviesFile.good()) {
        cout << "**Error: unable to open movies file '" << moviesFilename << "', exiting" << endl;
        return;
    }

    moviesFile >> id;  // get first ID, or set EOF flag if file empty:

    while (!moviesFile.eof()) {
        // we have more data, so input publication year and movie name:
        moviesFile >> pubYear;
        getline(moviesFile, name);  // movie name fills rest of input line:

        // trim potential whitespace from both ends of movie name:
        name = trim(name);
        ID_data.PubYear = pubYear;
        ID_data.name = name;

        movieIDs.insert(id, ID_data);
        movieNames.insert(name, id);


        moviesFile >> id;  // get next ID, or set EOF flag if no more data:
    }

    // **********************************************
    //          Read in reviews File here
    // **********************************************
    ifstream reviewsFile(reviewsFilename);
    int reviewID, movieID, numStars;

    if (!reviewsFile.good()) {
        cout << "**Error: unable to open movies file '" << reviewsFilename << "', exiting" << endl;
        return;
    }

    reviewsFile >> reviewID;  // get first ID, or set EOF flag if file empty:

    while (!reviewsFile.eof()) {
        reviewsFile >> movieID;
        reviewsFile >> numStars;

        reviewCounter++;

        //finds MovieData struct stored in value at movieID
        MovieData* data = movieIDs.search(movieID);

        //Checks rating given in review and increments accordingly
        if (numStars == 1)
            data->Num1Stars++;
        else if (numStars == 2)
            data->Num2Stars++;
        else if (numStars == 3)
            data->Num3Stars++;
        else if (numStars == 4)
            data->Num4Stars++;
        else
            data->Num5Stars++;

        reviewsFile >> reviewID;  // get next ID, or set EOF flag if no more data:
    }
}

//Function to check if user input is a string or integer
//Returns true if input is an integer
bool checkInput(string input)
{
    for (int i = 0; i < input.length(); i++)
        if (isdigit(input[i]) == false)
            return false;

    return true;
}
 
//
// main
//
int main()
{
  string moviesFilename = "movies1.txt";
  string reviewsFilename = "reviews1.txt";

  cout << "movies file?> ";
  cin >> moviesFilename;
 
  cout << "reviews file?> ";
  cin >> reviewsFilename;
 
  string junk;
  int reviewCount = 0;
  string userInput;
  getline(cin, junk);  // discard EOL following last input:

  //Creates two BSTs
  //First one holds <movieIDs, MovieData data structs>
  //Second one holds <movieNames, movieIDs>
  binarysearchtree<int, MovieData> movieIDs;
  binarysearchtree<string, int> movieNames;
  MovieData* data;

  //Reads in data from movies1.txt and reviews1.txt
  InputMovies(moviesFilename, reviewsFilename, movieIDs, movieNames, reviewCount);

  cout << "Num movies: " << movieIDs.size() << endl;
  cout << "Num reviews: " << reviewCount << endl;

  cout << "\nTree by movie id: size=" << movieIDs.size() << ", height=" << movieIDs.height() << endl;
  cout << "Tree by movie name: size=" << movieNames.size() << ", height=" << movieNames.height() << endl;

  //userInput loop to search for movies by name or ID
  while(userInput != "#")
  {
      cout << "\nEnter a movie id or name (or # to quit)> ";
      getline(cin, userInput);

      //checks if input is for a name or an ID
      bool isInt = checkInput(userInput);
      if(isInt) {
          int movieID = stoi(userInput.c_str());
          data = movieIDs.search(movieID);
          if (data == nullptr) {
              cout << "not found..." << endl;
              continue;
          }
          cout << "Movie ID: " << movieID << endl;
          cout << "Movie Name: " << data->name << endl;
      }
      else{
          int* inputID = movieNames.search(trim(userInput));
          if(inputID == NULL && userInput != "#"){
              cout << "not found..." << endl;
              continue;
          }
          if(userInput == "#")
              return 0;
          data = movieIDs.search(*inputID);
          cout << "Movie ID: " << *inputID << endl;
          cout << "Movie Name: " << data->name << endl;
      }

      //Calculates average from data stored in MovieData struct for specific node
      double sum = (data->Num1Stars * 1) + (data->Num2Stars * 2) + (data->Num3Stars * 3) + (data->Num4Stars * 4) + (data->Num5Stars * 5);
      int numReviews = data->Num1Stars + data->Num2Stars + data->Num3Stars + data->Num4Stars + data->Num5Stars;
      double avg;
      if(numReviews != 0)
          avg = sum / numReviews;
      else
          avg = 0;

      //Prints out reviews info
      cout << "Avg rating: " << avg << endl;
      cout << "5 stars: " << data->Num5Stars << endl;
      cout << "4 stars: " << data->Num4Stars << endl;
      cout << "3 stars: " << data->Num3Stars << endl;
      cout << "2 stars: " << data->Num2Stars << endl;
      cout << "1 star: " << data->Num1Stars << endl;
  }
    // done:
  return 0;
}
