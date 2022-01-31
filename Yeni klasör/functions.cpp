#include"functions.hpp"
#include"helper.cuh"

void printIntList(int* intList, int size) {
    for (int i = 0; i < size; i++) {
        cout << intList[i] << endl;
    }
    cout << endl;
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
    cout << endl;
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

Folder countWordsFromFolder(string folderPath) {
    list<list<string>> wordList = readFolder(folderPath);
    Folder folder(wordList);
    //printIntList(folder.fileStartPoints, folder.fileSize+1);
    //printIntList(folder.wordsStartPoints, folder.wordArraySize + 1);
    //printStringArray(folder.wordArray, folder.wordArraySize, folder.wordsStartPoints);
    countFolderWordsHost(folder.wordArray, folder.wordArraySize, folder.wordsStartPoints, folder.wordCounts, folder.fileSize, folder.fileStartPoints);   
    return folder;
}

void printFile(File file) {
    char* wordArray = file.wordArray;//genel
    int wordArraySize = file.wordArraySize;//yerel
    int* wordsStartPoints = file.wordsStartPoints;//yerel
    int* wordCounts = file.wordCounts;//yerel
    for (int i = 0; i < wordArraySize; i++) {
        if (wordCounts[i] != 0) {
            for (int j = wordsStartPoints[i]; j < wordsStartPoints[i + 1]; j++) {
                cout << wordArray[j];
            }
            cout << " " << wordCounts[i] << endl;
        }
    }
    cout << endl;
}

void printFolder2(Folder folder) {
    list<File> fileList = folder.folderToFileList();
    for (list<File>::iterator itr = fileList.begin(); itr != fileList.end(); ++itr) {
        printFile((*itr));
    }
}

void printFolder(Folder folder) {    
    int k = 0;
    for (int i = 0; i < folder.fileSize; i++) {        
        for (int j = folder.fileStartPoints[i]; j < folder.fileStartPoints[i + 1]; j++) {
            if (folder.wordCounts[k] != 0) {
                cout << folder.wordArray[j];
                if (j + 1 == folder.wordsStartPoints[k+1]) {
                    cout << " " << folder.wordCounts[k] << endl;
                    ++k;
                }
            }
            else {
                j = folder.wordsStartPoints[k+1]-1;
                ++k;
            }
        }
        cout << endl << endl;
    }
}
