#include "file.h"

void currentStateToFile(const char *fileName) {
    if(checkAllReady() == -1) {printf("VM and LAC not initiated");}
    FILE *outFile;
    if((outFile=fopen(fileName, "wb")) == NULL) {
        exception(__FILE__, __LINE__, "I do not know why");
    }
    fwrite(lacTable, sizeof(int), LACSIZE, outFile);
    fwrite(vmTable, sizeof(int), VMSIZE, outFile);
    fwrite(&registerFile.pc, sizeof(int), 1, outFile);
    fwrite(dataStackMemory, sizeof(int), STACKSIZE, outFile);
    fclose(outFile);
    return;
}

void fileToState(const char *fileName) {
    if(checkAllReady() == -1) {printf("VM and LAC not initiated");}
    FILE *file;
    if((file=fopen(fileName, "rb")) == NULL) {
        exception(__FILE__, __LINE__, "Fail to open the file");
    }
    fread(lacTable, sizeof(int), LACSIZE, file);
    fread(vmTable, sizeof(int), VMSIZE, file);
    fread(&registerFile.pc, sizeof(int), 1, file);
    fread(dataStackMemory, sizeof(int), STACKSIZE, file);
    return;
}