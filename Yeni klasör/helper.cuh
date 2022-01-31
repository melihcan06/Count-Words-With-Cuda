#include<iostream>
#include<conio.h>
#include<cuda.h>
//#include<ctime>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>
#include<device_functions.h>
#include<string.h>

__global__ void assingNumberKernel(int* numbers, int size, int number);
__device__ bool cmpStringKernel(char* wordArray, int firstStart, int firstEnd, int secondStart, int secondEnd);
__device__ int findWordIdxKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int startIdx);
__device__ int findWordIdxForFolderKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int startIdx, int fileSize, int* fileStartPoints);
__global__ void countFileWordsKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts);
__global__ void countFolderWordsKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts, int fileSize, int* fileStartPoints);
void countFileWordsHost(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts);
void countFolderWordsHost(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts, int fileSize, int* fileStartPoints);
