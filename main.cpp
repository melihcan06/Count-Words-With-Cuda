﻿#include<iostream>
#include"functions.hpp"

using namespace std;


int main() {	
	string folderPath = "C:\\Users\\user\\source\\repos\\paralel3\\paralel3\\dosyalar";
	string file1Path = "C:\\Users\\user\\source\\repos\\paralel3\\paralel3\\dosyalar\\dosya1.txt";
	string file2Path = "C:\\Users\\user\\source\\repos\\paralel3\\paralel3\\dosyalar\\dosya2.txt";

	File numbers = countWords(file1Path);
	printFile(numbers);

	File numbers2 = countWords(file2Path);
	printFile(numbers2);

	return 0;
}