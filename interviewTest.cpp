#include <fstream>
#include <iostream>
#include <cstring>
#include <ctype.h>
#include <regex>

using namespace std;

ifstream inPrev ("previous.in");
ifstream inCurr ("current.in");
ifstream inIgnr ("gitignore.in");
ofstream outAdd ("added.out");
ofstream outMod ("modified.out");
ofstream outDel ("deleted.out");

/// structure that will help me memorize the contents of the files
struct git{
    char* initial[100];
    char* normalized[100];
    int dateModified[100];
    int total;
};

/// structure that will help me memorize the contents that will be ignored
struct ignore{
    char* strings[100];
    int nStrings;
};

/// function used to read the contents of previous version files
void readInfoPrev1(git &x){
    char input[100];
    int index=0;
    int date;
    while (inPrev>>input){
        int len=strlen(input);

        /// allocating the memory we need in order to store the string in our structure
        x.initial[index]=new char[len+1];
        x.normalized[index]=new char[len+1];

        /// copying the normal string into our specified component of our structure
        strcpy(x.initial[index],input);

        /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
        /// in order to be able to compare and find substrings as we are using case insensitive strings
        strcpy(x.normalized[index],input);

        for (int i=0; i<len; i++){
            x.normalized[index][i]=toupper(x.normalized[index][i]);
        }
        inPrev>>date;
        x.dateModified[index]=date;

        index++;
    }
    x.total=index;
}

/// function used to read the contents of current version files
void readInfoCurr1(git &x){
    char input[100];
    int index=0;
    int date;
    while (inCurr>>input){
        int len=strlen(input);

        /// allocating the memory we need in order to store the string in our structure
        x.initial[index]=new char[len+1];
        x.normalized[index]=new char[len+1];

        /// copying the normal string into our specified component of our structure
        strcpy(x.initial[index],input);

        /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
        /// in order to be able to compare and find substrings as we are using case insensitive strings
        strcpy(x.normalized[index],input);

        for (int i=0; i<len; i++){
            x.normalized[index][i]=toupper(x.normalized[index][i]);
        }
        inCurr>>date;
        x.dateModified[index]=date;

        index++;
    }
    x.total=index;
}

/// general function used to read the contents of the input files
void readInfo1(git &gitPrev, git &gitCurr){
    readInfoPrev1(gitPrev);
    readInfoCurr1(gitCurr);
}

/// function meant to solve the first task
void task1(){
    git gitPrev, gitCurr;
    readInfo1(gitPrev, gitCurr);

    /// we will now go through each member of gitPrev.normalized, respectively gitCurr.normalized
    /// and compare the date modified, and provide specific outputs for each case
    int i=0, j=0;
    while (i<gitPrev.total&&j<gitCurr.total){
        if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])==0){
            if (gitPrev.dateModified[i]<gitCurr.dateModified[j]){
                outMod<<gitPrev.initial[i]<<'\n';
            }
            i++;
            j++;
        }
        else{
            if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])<0){
                outDel<<gitPrev.initial[i]<<'\n';
                i++;
            }
            else{
                if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])>0){
                    outAdd<<gitCurr.initial[j]<<'\n';
                    j++;
                }
            }
        }
    }

    ///treating the cases in which some strings are left unprocessed
    while (i<gitPrev.total){
        outDel<<gitPrev.initial[i]<<'\n';
        i++;
    }

    while (j<gitCurr.total){
        outAdd<<gitCurr.initial[j]<<'\n';
        j++;
    }

    /// freeing the memory we used
    for (int i=0; i<gitPrev.total; i++){
        delete []gitPrev.initial;
        delete []gitPrev.normalized;
    }
    for (int i=0; i<gitCurr.total; i++){
        delete []gitCurr.initial;
        delete []gitCurr.normalized;
    }
}

/// function used to read the contents of ignore files
void readInfoIgn(struct ignore &ignStr){
    char input[100];
    ignStr.nStrings=0;

    while(inIgnr>>input){
        int len=strlen(input);
        for (int i=0; i<len; i++){
            input[i]=toupper(input[i]);
        }

        ignStr.strings[ignStr.nStrings]=new char [len+1];
        strcpy(ignStr.strings[ignStr.nStrings],input);

        ignStr.nStrings++;
    }
}

/// function used to check if a certain input should be ignored or not
bool checkIgnore(char *input, struct ignore x){
    for (int i=0; i<x.nStrings; i++){
        if (strstr(input,x.strings[i])!=NULL){
            return true;
        }
    }
    return false;
}

/// function used to read the contents of previous version files
void readInfoPrev2(git &x, struct ignore y){
    char input[100];
    int index=0;
    int date;
    while (inPrev>>input){
        /// checking whether this input should be ignored or not
        char inputNorm[100];
        int len=strlen(input);
        strcpy(inputNorm,input);
        for (int i=0; i<len; i++){
            inputNorm[i]=toupper(inputNorm[i]);
        }

        if (!checkIgnore(inputNorm, y)){


            /// allocating the memory we need in order to store the string in our structure
            x.initial[index]=new char[len+1];
            x.normalized[index]=new char[len+1];

            /// copying the normal string into our specified component of our structure
            strcpy(x.initial[index],input);

            /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
            /// in order to be able to compare and find substrings as we are using case insensitive strings
            strcpy(x.normalized[index],inputNorm);

            inPrev>>date;
            x.dateModified[index]=date;

            index++;
        }
        else{
            inPrev>>date;
        }
    }
    x.total=index;
}

/// function used to read the contents of current version files
void readInfoCurr2(git &x, struct ignore y){
    char input[100];
    int index=0;
    int date;
    while (inCurr>>input){
        /// checking whether this input should be ignored or not
        char inputNorm[100];
        int len=strlen(input);
        strcpy(inputNorm,input);
        for (int i=0; i<len; i++){
            inputNorm[i]=toupper(inputNorm[i]);
        }

        if (!checkIgnore(inputNorm, y)){


            /// allocating the memory we need in order to store the string in our structure
            x.initial[index]=new char[len+1];
            x.normalized[index]=new char[len+1];

            /// copying the normal string into our specified component of our structure
            strcpy(x.initial[index],input);

            /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
            /// in order to be able to compare and find substrings as we are using case insensitive strings
            strcpy(x.normalized[index],inputNorm);

            inCurr>>date;
            x.dateModified[index]=date;

            index++;
        }
        else{
            inCurr>>date;
        }
    }
    x.total=index;
}

/// general function used to read the contents of the input files
void readInfo2(git &gitPrev, git &gitCurr, struct ignore &ignStr){
    readInfoIgn(ignStr);
    readInfoPrev2(gitPrev, ignStr);
    readInfoCurr2(gitCurr, ignStr);
}

/// function meant to solve the second task
void task2(){
    git gitPrev, gitCurr;
    struct ignore ignStr;
    readInfo2(gitPrev, gitCurr, ignStr);

    /// we will now go through each member of gitPrev.normalized, respectively gitCurr.normalized
    /// and compare the date modified, and provide specific outputs for each case
    int i=0, j=0;
    while (i<gitPrev.total&&j<gitCurr.total){
        if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])==0){
            if (gitPrev.dateModified[i]<gitCurr.dateModified[j]){
                outMod<<gitPrev.initial[i]<<'\n';
            }
            i++;
            j++;
        }
        else{
            if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])<0){
                outDel<<gitPrev.initial[i]<<'\n';
                i++;
            }
            else{
                if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])>0){
                    outAdd<<gitCurr.initial[j]<<'\n';
                    j++;
                }
            }
        }
    }

    ///treating the cases in which some strings are left unprocessed
    while (i<gitPrev.total){
        outDel<<gitPrev.initial[i]<<'\n';
        i++;
    }

    while (j<gitCurr.total){
        outAdd<<gitCurr.initial[j]<<'\n';
        j++;
    }

    /// freeing the memory we used
    for (int i=0; i<gitPrev.total; i++){
        delete []gitPrev.initial[i];
        delete []gitPrev.normalized[i];
    }
    for (int i=0; i<gitCurr.total; i++){
        delete []gitCurr.initial[i];
        delete []gitCurr.normalized[i];
    }
    for (int i=0; i<ignStr.nStrings; i++){
        delete []ignStr.strings[i];
    }
}

/// function used to check if a certain input should be ignored or not
bool checkIgnore3(char *input, struct ignore x){
    string aux(input);
    for (int i=0; i<x.nStrings; i++){
        regex expression(x.strings[i]);
        if (regex_match(input,expression)){
            return true;
        }
    }
    return false;
}

/// function used to read the contents of previous version files
void readInfoPrev3(git &x, struct ignore y){
    char input[100];
    int index=0;
    int date;
    while (inPrev>>input){
        /// checking whether this input should be ignored or not
        char inputNorm[100];
        int len=strlen(input);
        strcpy(inputNorm,input);
        for (int i=0; i<len; i++){
            inputNorm[i]=toupper(inputNorm[i]);
        }

        if (!checkIgnore3(inputNorm, y)){


            /// allocating the memory we need in order to store the string in our structure
            x.initial[index]=new char[len+1];
            x.normalized[index]=new char[len+1];

            /// copying the normal string into our specified component of our structure
            strcpy(x.initial[index],input);

            /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
            /// in order to be able to compare and find substrings as we are using case insensitive strings
            strcpy(x.normalized[index],inputNorm);

            inPrev>>date;
            x.dateModified[index]=date;

            index++;
        }
        else{
            inPrev>>date;
        }
    }
    x.total=index;
}

/// function used to read the contents of current version files
void readInfoCurr3(git &x, struct ignore y){
    char input[100];
    int index=0;
    int date;
    while (inCurr>>input){
        /// checking whether this input should be ignored or not
        char inputNorm[100];
        int len=strlen(input);
        strcpy(inputNorm,input);
        for (int i=0; i<len; i++){
            inputNorm[i]=toupper(inputNorm[i]);
        }

        if (!checkIgnore3(inputNorm, y)){


            /// allocating the memory we need in order to store the string in our structure
            x.initial[index]=new char[len+1];
            x.normalized[index]=new char[len+1];

            /// copying the normal string into our specified component of our structure
            strcpy(x.initial[index],input);

            /// now we are going to normalize the text, meaning we will transform each lowercase to uppercase
            /// in order to be able to compare and find substrings as we are using case insensitive strings
            strcpy(x.normalized[index],inputNorm);

            inCurr>>date;
            x.dateModified[index]=date;

            index++;
        }
        else{
            inCurr>>date;
        }
    }
    x.total=index;
}

/// general function used to read the contents of the input files
void readInfo3(git &gitPrev, git &gitCurr, struct ignore &ignStr){
    readInfoIgn(ignStr);
    readInfoPrev3(gitPrev, ignStr);
    readInfoCurr3(gitCurr, ignStr);
}

/// function meant to solve the bonus task
void task3(){
    git gitPrev, gitCurr;
    struct ignore ignStr;
    readInfo3(gitPrev, gitCurr, ignStr);

    /// we will now go through each member of gitPrev.normalized, respectively gitCurr.normalized
    /// and compare the date modified, and provide specific outputs for each case
    int i=0, j=0;
    while (i<gitPrev.total&&j<gitCurr.total){
        if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])==0){
            if (gitPrev.dateModified[i]<gitCurr.dateModified[j]){
                outMod<<gitPrev.initial[i]<<'\n';
            }
            i++;
            j++;
        }
        else{
            if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])<0){
                outDel<<gitPrev.initial[i]<<'\n';
                i++;
            }
            else{
                if (strcmp(gitPrev.normalized[i],gitCurr.normalized[j])>0){
                    outAdd<<gitCurr.initial[j]<<'\n';
                    j++;
                }
            }
        }
    }

    ///treating the cases in which some strings are left unprocessed
    while (i<gitPrev.total){
        outDel<<gitPrev.initial[i]<<'\n';
        i++;
    }

    while (j<gitCurr.total){
        outAdd<<gitCurr.initial[j]<<'\n';
        j++;
    }

    /// freeing the memory we used
    for (int i=0; i<gitPrev.total; i++){
        delete []gitPrev.initial[i];
        delete []gitPrev.normalized[i];
    }
    for (int i=0; i<gitCurr.total; i++){
        delete []gitCurr.initial[i];
        delete []gitCurr.normalized[i];
    }
    for (int i=0; i<ignStr.nStrings; i++){
        delete []ignStr.strings[i];
    }
}

int main () {
    cout<<"Choose your task:\n1 for Task 1;\n2 for Task 2;\n3 for Bonus Task.\n";
    int task;
    cin>>task;
    if (task==1){
        task1();
    }
    if (task==2){
        task2();
    }
    if (task==3){
        task3();
    }
    return 0;
}
