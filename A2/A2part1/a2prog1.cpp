#include <iostream>
#include <glibc>
#include <unistd.h>
using namespace std;

int main() {
    mkdir("folder1");
    mkdir("folder2");
    mkdir("folder3");
    int fd = open("folder1/file1.txt", O_CREAT);
    int fd = open("folder1/file2.txt", O_CREAT);
    int fd = open("folder2/file3.txt", O_CREAT);
    int fd = open("folder2/file4.txt", O_CREAT);
    chdir("folder3");
    int symlink(const char *link5, const char *file4.txt);
    chdir("..");
    return 0;
}