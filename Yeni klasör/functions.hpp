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
		this->wordsStartPoints[this->wordArraySize] = j;
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

	File(int wordArraySize, int wordSize) {
		this->wordArraySize = wordArraySize;
		this->wordCounts = (int*)malloc(sizeof(int) * this->wordArraySize);
		this->wordsStartPoints = (int*)malloc(sizeof(int) * this->wordArraySize + 1);
		this->wordsStartPoints[this->wordArraySize] = wordSize;
		this->wordArray = (char*)malloc(sizeof(char) * this->wordsStartPoints[this->wordArraySize]);
		for (int i = 0; i < this->wordArraySize; i++) {
			this->wordCounts[i] = 0;
		}
	}

	File(int wordArraySize, int wordSize, int sumOfWords, int sumOfChars) {
		this->wordArraySize = wordArraySize;
		this->wordCounts = (int*)malloc(sizeof(int) * this->wordArraySize);
		this->wordsStartPoints = (int*)malloc(sizeof(int) * this->wordArraySize + 1);
		this->wordsStartPoints[this->wordArraySize] = wordSize;
		this->wordArray = (char*)malloc(sizeof(char) * sumOfChars);
		for (int i = 0; i < this->wordArraySize; i++) {
			this->wordCounts[i] = 0;
		}
	}

};

class Folder {
public:
	char* wordArray = NULL;
	int wordArraySize = 0;
	int* wordsStartPoints = NULL;
	int* wordCounts = NULL;
	int fileSize = 0;
	int* fileStartPoints = NULL;

	Folder() {
		this->wordArray = NULL;
		this->wordArraySize = 0;
		this->wordsStartPoints = NULL;
		this->wordCounts = NULL;
		this->fileSize = 0;
		this->fileStartPoints = NULL;
	}

	Folder(list<list<string>> wordList) {
		this->fileSize = wordList.size();
		this->fileStartPoints = (int*)malloc(sizeof(int) * this->fileSize + 1);//sonuncusu bitis noktasi
		int i = 0, j = 0;
		for (list<list<string>>::iterator itr = wordList.begin(); itr != wordList.end(); ++itr) {
			this->wordArraySize += itr->size();
			this->fileStartPoints[i] = j;
			for (list<string>::iterator itr2 = itr->begin() ; itr2 != itr->end(); ++itr2) {
				//this->wordArraySize += itr2->size();
				j += itr2->size();
			}
			i++;
		}
		this->fileStartPoints[i] = j;
		
		this->wordCounts = (int*)malloc(sizeof(int) * this->wordArraySize);
		this->wordsStartPoints = (int*)malloc(sizeof(int) * this->wordArraySize + 1);
		i = 0; j = 0;
		for (list<list<string>>::iterator itr = wordList.begin(); itr != wordList.end(); ++itr) {
			for (list<string>::iterator itr2 = itr->begin(); itr2 != itr->end(); ++itr2) {
				this->wordCounts[i] = 0;
				this->wordsStartPoints[i] = j;				
				i++;
				j += itr2->size();
			}
		}
		this->wordsStartPoints[this->wordArraySize] = j;

		i = 0;
		this->wordArray = (char*)malloc(sizeof(char) * this->wordsStartPoints[this->wordArraySize]);
		for (list<list<string>>::iterator itr = wordList.begin(); itr != wordList.end(); ++itr) {
			for (list<string>::iterator itr2 = itr->begin(); itr2 != itr->end(); ++itr2) {
				char* temp = _strdup(itr2->c_str());
				for (j = 0; j < itr2->size(); j++) {
					this->wordArray[i] = temp[j];
					i++;
				}				
			}
		}
	}

	list<File> folderToFileList() {
		list<File> fileList{};
		if (this->wordArray != NULL &&
			this->wordArraySize != 0 &&
			this->wordsStartPoints != NULL &&
			this->wordCounts != NULL &&
			this->fileSize != 0 &&
			this->fileStartPoints != NULL) {

			int wordArraySize = 0, wordSize = 0, wordArraySizeSum = 0, wordCountSum = 0, wordSizeSum = 0;
			for (int i = 0; i < this->fileSize; i++) {
				for (int j = wordArraySize; j < this->wordArraySize; j++) {
					/*if (this->wordsStartPoints[j + 1] < this->fileStartPoints[i + 1]) {
						wordSize = this->wordsStartPoints[j + 1];
					}*/
					if (this->wordsStartPoints[j + 1] == this->fileStartPoints[i + 1]) {
						wordArraySize = j + 1 - wordArraySizeSum;
						wordArraySizeSum += wordArraySize;
						wordSize = this->wordsStartPoints[j + 1] - wordSizeSum;
						wordSizeSum += wordSize;
						break;
					}
				}
				
				//cout << wordArraySize << " " << wordSize << endl << endl;

				/*File f(wordArraySize, wordSize);
				
				int k = 0;
				for (int j = this->fileStartPoints[i]; j < this->fileStartPoints[i] + wordSize; j++) {
					f.wordArray[k] = this->wordArray[j];
					cout << f.wordArray[k];
					k++;
				}
				cout << " ";*/

				File f(wordArraySize, wordSize, this->wordArraySize, this->wordsStartPoints[this->wordArraySize]);
				for (int j = 0; j < this->wordArraySize; j++) {
					f.wordArray[j] = this->wordArray[j];
				}

				for (int j = 0; j < wordArraySize; j++) {
					f.wordCounts[j] = this->wordCounts[wordCountSum];
					f.wordsStartPoints[j] = this->wordsStartPoints[wordCountSum];
					//cout << f.wordCounts[j] << " " << f.wordsStartPoints[j] << endl;
					++wordCountSum;
				}
				f.wordsStartPoints[wordArraySize] = this->wordsStartPoints[wordCountSum];
				//cout << f.wordsStartPoints[wordArraySize] << endl;

				//cout << wordCountSum << endl;

				fileList.push_back(f);
			}

		}
		return fileList;
	}

};


File countWords(string filePath);
void printFile(File file);

Folder countWordsFromFolder(string folderPath);
void printFolder(Folder folder);