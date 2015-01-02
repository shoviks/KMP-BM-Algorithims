/*******************************************************
Author: Shovik Shyamsundar
Class: CSCI 2720
Project 2
******************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <set>

using namespace std;

string text;
string pattern;

string getFileContent(const char *filename)
{
  ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
    {
      string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      return(contents);
    }
  throw(errno);
}
int** CreateKMPskipArray(string search)
{
  //Getting unique characters from string using a set
  char* temp = (char*)search.c_str();
  set<char> sa(temp, temp + search.length());
  set<char>::iterator iter;
  
  int count=0;
  char array[sa.size()];

  //Put unique characters into an array
  for(iter = sa.begin(); iter!=sa.end(); iter++)
    {
      array[count] = (*iter);
      count++;
    }
  
  //Create KMPskipArray dimensions
  int oneD =  sa.size() + 1;
  int twoD = search.length() + 1;
  
  int **KMPskipArray = new int* [oneD];
  for (int i = 0; i < oneD; i++)
    KMPskipArray[i] = new int [twoD];

  //Convert unique characters into ASCII code then insert into first column of KMPskipArray
  for (int i = 0; i < oneD; i++)
    {
      int ASCII = array[i];
      KMPskipArray[i][0] = ASCII;
    }
  KMPskipArray[oneD - 1][0] = 0;

  //Insert values into KMPskipArray
  for (int i = 0; i < oneD; i++)
    {
      for (int j = 1; j < twoD; j++)
	{
	  int patternCode = search.at(j - 1);
	  //Insert 0 whenever match occurs
	  if (KMPskipArray[i][0] == patternCode)
	    KMPskipArray[i][j] = 0;
	  else
	    {
	      bool dExists = false;//if d exists
	      //Finds smallest d
	      for (int d = 0; d <= (j-1); d++)
		{
		  patternCode = search.at(j-1-d);//p[i-d]
		  if (patternCode == KMPskipArray[i][0])//checks if p[i-d] == c
		    {
		      dExists = true;
		      if ((j-d-2) >= 0)//checks if i-d-1 >= 0
			{
			  count = 0;
			  //count keeps track of each p[j] == p[j+d] 
			  for (int k = 0; k <= (j-d-2); k++)
			    {
			      if (search.at(k)==search.at(k+d))
				count++;
			    }
			  //if p[j] == p[j+d] for all 0 <= j <= i-d-1
			  if (count == (j-1-d))
			    {
			      KMPskipArray[i][j] = d;
			    }
			  else
			    KMPskipArray[i][j] = j;
			}
		      else
			{
			  KMPskipArray[i][j] = d;    
			}
		    }
		}
	      if (!dExists)
		KMPskipArray[i][j] = j;
	    }
	}
    }
  
  return KMPskipArray;
}
int KMPsearch(string textSearch, int **skipArray)
{

  //Getting unique characters from string using a set
  char* temp = (char*)pattern.c_str();
  set<char> sa(temp, temp + pattern.length());
  set<char>::iterator iter;
  
  int oneD =  sa.size() + 1;//KMPskipArray height
  int twoD = pattern.length() + 1;//KMPskipArray width
  int patLength = pattern.length();
  int textLength = textSearch.length();
  int matchCount = 0;
  int textIndex = 0;
  int patIndex = 0;
  int textASCII;
  int skips;
  int firstMatch;
  bool other;

  while ((matchCount < patLength) & (patIndex < patLength) & (textIndex < textLength))
    {
      other = true;
      if (pattern.at(patIndex) == textSearch.at(textIndex))
	{
	  
	  matchCount++;
	  if (matchCount == 1)
	    firstMatch = textIndex;
	  patIndex++;
	  textIndex++;
	
	}
      else
	{
	  textASCII = textSearch.at(textIndex);
	  for (int i = 0; i < (oneD - 1); i++)
	    {
	      if (textASCII == skipArray[i][0])
		{
		  other = false;
		  skips = skipArray[i][patIndex + 1];
		  if (matchCount > 0)
		    textIndex+=(skips-1);
		  else
		    textIndex+=(skips);
		  patIndex = 0;
		  break;
		}
	    }
	  if (other)
	    {
	      skips = skipArray[oneD - 1][patIndex + 1];
	      textIndex+=(skips);
	      patIndex = 0;
	    }
	  matchCount = 0;
	}
    }
  if (matchCount == patLength)
    return firstMatch;
  return -1;
}
int** CreateBMskipArray(string search)
{
  //Getting unique characters from string using a set
  char* temp = (char*)search.c_str();
  set<char> sa(temp, temp + search.length());
  set<char>::iterator iter;
  
  int count=0;
  char array[sa.size()];

  //Put unique characters into an array
  for(iter = sa.begin(); iter!=sa.end(); iter++)
    {
      array[count] = (*iter);
      count++;
    }
  
  //Create BMskipArray dimensions
  int oneD =  sa.size() + 1;
  int twoD = search.length() + 1;
  
  int **BMskipArray = new int* [oneD];
  for (int i = 0; i < oneD; i++)
    BMskipArray[i] = new int [twoD];

  //Convert unique characters into ASCII code then insert into first column of KMPskipArray
  for (int i = 0; i < oneD; i++)
    {
      int ASCII = array[i];
      BMskipArray[i][0] = ASCII;
    }
  BMskipArray[oneD - 1][0] = 0;

  int max;

  for (int row = 0; row < oneD; row++)
    {
      for (int col = 1; col < twoD; col++)
	{
	  int i = col-1;
	  int patternCode = search.at(i);
	  if (BMskipArray[row][0] == patternCode)//inserts -1 instead of nothing
	    BMskipArray[row][col] = -1;
	  else 
	    {
	      bool dExists = false;//no such d exists that satistfies conditions
	      for (int d = 0; d <= i; d++)
		{
		  if (search.at(i-d) == BMskipArray[row][0])//checks if p[i-d] == c
		    {
		      dExists = true;
		      count = 0;
		      if (d > (i+1))
			max = d;
		      else
			max = i + 1;
		      for (int j = max; j < search.length(); j++)
			{
			  if (search.at(j)==search.at(j-d))
			    count++;
			}
		      if (max > (search.length() - 1))
			{
			  BMskipArray[row][col] = d;
			  break;
			}
		      //if p[j]== p[j-d] for all max(i+1, d) <= j < search.length()
		      else if (count == search.length() - max)
			{
			  BMskipArray[row][col] = d;
			}
		      else
			BMskipArray[row][col] = search.length();
		    }
		}
	      if (!dExists)
		BMskipArray[row][col] = search.length();
	    }
	}
    }

  return BMskipArray;
	  
}
int BMsearch(string textSearch, int** skipArray)
{
  //Getting unique characters from string using a set
  char* temp = (char*)pattern.c_str();
  set<char> sa(temp, temp + pattern.length());
  set<char>::iterator iter;
  
  int oneD =  sa.size() + 1;//KMPskipArray height
  int twoD = pattern.length() + 1;//KMPskipArray width
  int patLength = pattern.length();
  int textLength = textSearch.length();
  int matchCount = 0;
  int textIndex = patLength - 1;
  int patIndex = patLength - 1;
  int textASCII;
  int skips;
  int lastMatch;
  bool other;
  
  while ((matchCount < patLength) & (patIndex >= 0) & (textIndex < textLength) & (0 <= textIndex))
    {
      other = true;
      if (pattern.at(patIndex) == textSearch.at(textIndex))
	{
	  matchCount++;
	  if (matchCount == patLength)
	    lastMatch = textIndex;
	  if (patIndex > 0 && textIndex > 0)
	    {
	      patIndex--;
	      textIndex--;   
	    }
	}
      else
	{
	  // matchCount = 0;
	  textASCII = textSearch.at(textIndex);
	  for (int i = 0; i < (oneD - 1); i++)
	    {
	      if (textASCII == skipArray[i][0])
		{
		  other = false;
		  skips = skipArray[i][patIndex + 1];
		  if (matchCount > 0)
		    textIndex+=(skips+1);
		  else
		    textIndex+=skips;
		  patIndex = patLength - 1;
		  break;
		}
	    }
	  if (other)
	    {
	      skips = skipArray[oneD - 1][patIndex + 1];
	      textIndex+=(skips);
	      patIndex = patLength - 1;
	    }
	  matchCount = 0;
	}
      
    }
  if (matchCount == patLength)
    return lastMatch;
  return -1;
    
}
void SkipArrayPrint(int** skipArray)
{
  //Getting unique characters from string using a set
  char* temp = (char*)pattern.c_str();
  set<char> sa(temp, temp + pattern.length());
  set<char>::iterator iter;
  
  int oneD =  sa.size() + 1;//KMPskipArray height
  int twoD = pattern.length() + 1;//KMPskipArray width
  char ASCII;
  int value;
  cout << "\t";
  for (int i = 0; i < pattern.length(); i++)
    cout << temp[i] << "\t";
  cout << endl;
  for (int i = 0; i < oneD; i++)
    {
      for (int j = 0; j < twoD; j++)
	{
	  if (j == 0)
	    {
	      if (i == (oneD - 1))
		cout << "Other\t";
	      else
		{
		  value = skipArray[i][j];
		  ASCII = static_cast<char>(value);
		  cout << ASCII << "\t";
		}
	    }
	  else
	    cout << skipArray[i][j] << "\t";
	}
      cout << endl;
      }
}

int main(int argc, char *argv[]) {
  if (argc != 4)
    {
      cout << "Usage: " << argv[0] << " <Filename> <Filename> <Type of Search: K or B>" << endl;
      exit(1);
    }
  char *searchString = argv[1];
  char *searchText = argv[2];
  char *searchType = argv[3];

  ifstream infile(searchString);

  text =  getFileContent(searchText);
    
  while(!infile.eof())
    {
      getline(infile,pattern);
      cout << "" << pattern << "\n";
      if (searchType[0] == 'K' || searchType[0] == 'k')
	{
	  int **KMPskipArray = CreateKMPskipArray(pattern);
	  SkipArrayPrint(KMPskipArray);
	  cout << endl;
	  if (KMPsearch(text, KMPskipArray) == -1)
	    cout << "not found" << endl;
	  else
	    cout << KMPsearch(text, KMPskipArray) << endl;
	}
      else if (searchType[0] == 'B' || searchType[0] == 'b')
        {
          int **BMskipArray = CreateBMskipArray(pattern);
          SkipArrayPrint(BMskipArray);
          cout << endl;
          if (BMsearch(text, BMskipArray) == -1)
            cout << "not found" << endl;
          else
            cout << BMsearch(text, BMskipArray) << endl;
	}
      
    }
  
  return 0;
  
}


