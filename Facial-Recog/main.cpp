/* 
 * File:   main.cpp
 * Author: Chris Kozma
 * Purpose: To identify facial expressions by comparing vectors
 * Created on November 26, 2013, 5:29 PM
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5
/*
 * 
 */
//Read input files from template or display error message
vector<vector<float> > inputFile(const char* pFileName)
{
        vector<vector<float> > pResults;
	string sLine;
	try
	{
		cout << "Reading input file: " << pFileName << endl;
		ifstream input(pFileName);
		while (!input.eof())
		{
			vector<float> pLineResults;
			getline(input, sLine);
			istringstream line(sLine);
			while (!line.eof())
			{
				float fResult;
				line >> fResult;
				pLineResults.push_back(fResult);
			}
			pResults.push_back(pLineResults);
		}
		input.close();
	}
	catch(...)
	{
		//error
		cout << "Error loading file";
	}
	return pResults;
}


//similarity function x is query y is template
vector<float> similar(vector<vector<float> > x, vector<vector<float> > y)
{
	float numerator;
	float numeratorAdded = 0;
	float denominatorX;
	float denominatorY;
	float denominatorXAdded = 0;
	float denominatorYAdded = 0;
	float temp;
	vector<float> s;
		
	//step through every row in the template file given, t
	for (vector<vector<float> >::size_type u = 0; u < y.size() - 1; u++)
	{
		//cout << "YOU MADE IT TO ROW " << u + 1 << ": ";
		//step through every item in the row
		for (vector<float>::size_type v = 0; v < y[u].size(); v++)
		{
			numerator = (x[0][v] * y[u][v]);
			denominatorX = x[0][v] * x[0][v];
			denominatorY = y[u][v] * y[u][v];
			numeratorAdded = numeratorAdded + numerator;
			denominatorXAdded = denominatorXAdded + denominatorX;
			denominatorYAdded = denominatorYAdded + denominatorY;
		}
		temp = numeratorAdded / (sqrt(denominatorXAdded) * sqrt(denominatorYAdded));
		s.push_back(temp);
		//cout << "Number after adding : " << added << endl;
		//reset variables since we are going to a different row
		numerator = 0;
		denominatorX = 0;
		denominatorY = 0;
		numeratorAdded = 0;
		denominatorXAdded = 0;
		denominatorYAdded = 0;
		temp = 0;
	}
	return s;
}


template <class random_iterator>
class IndexedComparison
{
   public:
      IndexedComparison (random_iterator begin,
         random_iterator end)
         : p_begin (begin), p_end (end) {}
      bool operator () (unsigned int a, unsigned int b) const
         { return *(p_begin + a) > *(p_begin + b); }

   private:
      random_iterator const p_begin;
      random_iterator const p_end;
};

//function to sort the vector s and chop to top 10 results, then output to screen
void sortAndOutput(vector<float> toBeSorted)
{
	vector<float> indexes(toBeSorted.size());
        //initialize original index locations
	for (int i = 0; i != indexes.size(); ++i) indexes[i] = i;
        //sort index vector descending
        sort(indexes.begin(), indexes.end(), IndexedComparison<vector<float>::const_iterator>
                (toBeSorted.begin(), toBeSorted.end()));
	//sort vector descending
        sort(toBeSorted.begin(), toBeSorted.end(), greater<float>());
	//only keep top 10 results after being sorted
	toBeSorted.erase(toBeSorted.begin() + 10, toBeSorted.end());
	//for fun to show whats on what line -- temporary --
	for (unsigned int i = 0; i < toBeSorted.size(); i++)
	{
		cout << toBeSorted[i] << " - ";
		cout << "Index: " << indexes[i] << endl;
	}
	cout << endl;
}

void work()
{
	vector<float> temp;
	char data[100];
	char templates[100];
        //dont know how to fix these warnings without breaking program
	char* datas[3] = { "data/%03d_AU01_query.dat", "data/%03d_AU12_query.dat", "data/%03d_AU17_query.dat" };
	for (int i = 1; i < 48; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			sprintf(templates, "data/%03d_template.dat", i);   // create the name of the template 1-47
			sprintf(data, datas[j], i);
			temp = similar(inputFile(data), inputFile(templates));
			sortAndOutput(temp);
		}
	}
}

int main(void)
{
    //start execute time clock
    clock_t tStart = clock();
    work();
    //output execute time
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}
