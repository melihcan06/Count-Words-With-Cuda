#pragma once
#include<iostream>
#include<conio.h>
#include<cuda.h>
#include<ctime>
#include<vector>
#include<iomanip>
#include<string>
#include<list>
#include<fstream>
#include<queue>
#include <filesystem>
#define MAX_WORD_CHAR 100


namespace fs = std::filesystem;
using namespace std;


void printIntList(int* intList, int size);
void printStringList(list<string> stringList);
void printStringList(list<string> stringList, int mode);
void printStringArray(char* wordArray, int wordArraySize, int* wordsStartPoints);

list<string> split(string text, string space_delimiter);

list<string> readFile(string path);
list<list<string>> readFolder(string path);

class File {
public:
	char* wordArray = NULL;
	int wordArraySize = 0;
	int* wordsStartPoints = NULL;
	int* wordCounts = NULL;

	File() {
		this->wordArray = NULL;
		this->wordArraySize = 0;
		this->wordsStartPoints = NULL;
		this->wordCounts = NULL;
	}

	File(list<string> wordList) {
		this->wordArraySize = wordList.size();

		//seri ancak paralel yapýlabilir

		this->wordCounts = (int*)malloc(sizeof(int) * this->wordArraySize);
		this->wordsStartPoints = (int*)malloc(sizeof(int) * this->wordArraySize + 1);
		int i = 0, j = 0;
		for (list<string>::iterator itr = wordList.begin(); itr != wordList.end(); ++itr) {			
			this->wordsStartPoints[i] = j;
			this->wordCounts[i] = 0;
			//cout << this->wordsStartPoints[i] << endl;
			i++;
			j += itr->size();
		}
		this->wordsStartPoints[wordArraySize] = j;
		//cout << this->wordsStartPoints[i] << endl;
		//cout << endl << j <<" " << wordArraySize << endl;		
		
		i = 0;
		this->wordArray = (char*)malloc(sizeof(char) * this->wordsStartPoints[this->wordArraySize]);
		for (list<string>::iterator itr = wordList.begin(); itr != wordList.end(); ++itr) {
			char* temp = _strdup(itr->c_str());
			for (j = 0; j < itr->size(); j++) {
				this->wordArray[i] = temp[j];
				i++;
			}
		}	
	}
};

File countWords(string filePath);
void printFile(File file);