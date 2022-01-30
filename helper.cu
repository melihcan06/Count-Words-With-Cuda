#include<iostream>
#include<cuda.h>
#include<cuda_runtime.h>
#include<device_launch_parameters.h>
#include<device_functions.h>

__global__ void assingNumberKernel(int* numbers, int size, int number) {
	int idx = threadIdx.x;//threadIdx.x + blockIdx.x*blockDim.x;
	if (idx < size) {
		numbers[idx] = number;
	}
}

__device__ bool cmpStringKernel(char* wordArray, int firstStart, int firstEnd, int secondStart, int secondEnd){
	int firstSize = firstEnd - firstStart;
	int secondSize = secondEnd - secondStart;
	
	# if __CUDA_ARCH__>=200
		//printf("%d %d\n", secondSize, firstSize);
	#endif
	
	if (firstSize != secondSize) {
		return false;
	}
	else {
		for (int i = 0; i < firstSize; i++) {
			if (wordArray[i + firstStart] != wordArray[i + secondStart]) {
				return false;
			}
		}
		return true;
	}
}

__device__ int findWordIdxKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int startIdx) {
	if (startIdx < wordArraySize) {//en sonuncu, array in bitis idx i
		int start = wordsStartPoints[startIdx];
		int end = wordsStartPoints[startIdx + 1];
		bool cmp;

		# if __CUDA_ARCH__>=200
			//printf("%d \n", end);
		#endif

		for (int i = 0; i < startIdx; i++) {
			cmp = cmpStringKernel(wordArray, start, end, wordsStartPoints[i], wordsStartPoints[i + 1]);
			if (cmp) {
				return i;
			}
		}		
	}

	return startIdx;
}

/*
wordArray: \n olmadan uc uca ekli kelimeler
wordArraySize: kac adet kelime var bir dosyada
wordsStartPoints: kelimelerin wordArray'deki baslangic konumlari
wordCounts: sonuc icin doldurulacak olan kac kelimeden kac adetin oldugunu tutan dizi
wordArraySize tane thread baslat, wordsStartPoints un uzunlugu wordArraySize+1 dir cunku sonuncusu bitis idx'i dir.
*/
__global__ void countFileWordsKernel(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts) {
	int idx = threadIdx.x;
	if (idx < wordArraySize) {
		int newIdx = findWordIdxKernel(wordArray, wordArraySize, wordsStartPoints, idx);		
		atomicAdd((wordCounts + newIdx), 1);
	}
}

void countFileWordsHost(char* wordArray, int wordArraySize, int* wordsStartPoints, int* wordCounts) {
	int* wordCountsC;
	cudaMalloc(&wordCountsC, sizeof(int) * wordArraySize);
	cudaMemcpy(wordCountsC, wordCounts, sizeof(int) * wordArraySize, cudaMemcpyHostToDevice);
	//assingNumberKernel << <1, wordArraySize >> > (wordCountsC, wordArraySize, 0);

	char* wordArrayC;
	cudaMalloc(&wordArrayC, sizeof(char) * wordsStartPoints[wordArraySize]);
	cudaMemcpy(wordArrayC, wordArray, sizeof(char) * wordsStartPoints[wordArraySize], cudaMemcpyHostToDevice);

	int* wordsStartPointsC;
	cudaMalloc(&wordsStartPointsC, sizeof(int) * (wordArraySize + 1));
	cudaMemcpy(wordsStartPointsC, wordsStartPoints, sizeof(int) * (wordArraySize + 1), cudaMemcpyHostToDevice);

	dim3 threadsPerBlock(wordArraySize, 1, 1);
	countFileWordsKernel << <1, threadsPerBlock >> > (wordArrayC, wordArraySize, wordsStartPointsC, wordCountsC);

	cudaMemcpy(wordCounts, wordCountsC, sizeof(int) * wordArraySize, cudaMemcpyDeviceToHost);

	cudaFree(wordCountsC);
	cudaFree(wordArrayC);
	cudaFree(wordsStartPointsC);
}