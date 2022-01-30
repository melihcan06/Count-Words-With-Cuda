#include"functions.hpp"
#include"helper.cuh"

void printFile(File file) {    
    char* wordArray = file.wordArray;
    int wordArraySize = file.wordArraySize;
    int* wordsStartPoints = file.wordsStartPoints;
    int* wordCounts = file.wordCounts;
    for (int i = 0; i < wordArraySize; i++) {
        if (wordCounts[i] != 0) {
            for (int j = wordsStartPoints[i]; j < wordsStartPoints[i + 1]; j++) {
                cout << wordArray[j];
            }
            cout << " " << wordCounts[i] << endl;
        }
    }
    cout << endl ;
}

void printIntList(int* intList, int size) {
    for (int i = 0; i < size; i++) {
        cout << intList[i] << endl;
    }
}

void printStringList(list<string> stringList) {
    for (string str : stringList) {
        cout << str << endl;
    }
}

void printStringList(list<string> stringList, int mode) {
    if (mode == 0) {
        for (string str : stringList) {
            cout << str << endl;
        }
    }
    else {
        for (string str : stringList) {
            cout << str << " ";
        }
        cout << endl;
    }
}

void printStringArray(char* wordArray, int wordArraySize, int* wordsStartPoints) {    
    int j = 1;
    for (int i = 0; i < wordsStartPoints[wordArraySize]; i++) {
        cout << wordArray[i];
        if (i+1 == wordsStartPoints[j]) {
            cout << " ";
            j++;
        }
    }
}

list<string> split(string text, string space_delimiter) {
    list<string> words{};
    size_t pos = 0;
    while ((pos = text.find(space_delimiter)) != string::npos) {
        words.push_back(text.substr(0, pos));
        text.erase(0, pos + space_delimiter.length());
    }
    words.push_back(text);
    return words;
}

list<string> readFile(string path) {
    ifstream myfile;
    list<string> files{};
    string line;
    list<string> words;

    myfile.open(path);
    if (myfile.is_open()) {
        while (!myfile.eof()) {
            getline(myfile, line);
            words = split(line, " ");
            for (string word : words) {
                files.push_back(word);
            }
        }
        myfile.close();
    }

    return files;
}

list<list<string>> readFolder(string path) {
    list<list<string>> files;
    for (const auto& entry : fs::directory_iterator(path)) {
        //sonu txt ise kontrolu koy!
        files.push_back(readFile(entry.path().u8string()));
    }

    return files;
}

File countWords(string filePath) {//1 tane txt icin, kontrolleri yap
    list<string> wordList = readFile(filePath);    
    File file(wordList);    
    countFileWordsHost(file.wordArray, file.wordArraySize, file.wordsStartPoints, file.wordCounts);
    return file;
}