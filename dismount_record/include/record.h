#ifndef _RECORD_H_
#define _RECORD_H_
#define RECORDFILEPATH "/var/tmp/RecordFileNum"
#define HeadLength 9

#define EndLength 13
/* #define HeadLength 6 */
/* #define EndLength 5 */
int IsHead(char*);
int IsEnd(char*);
int RecordFileInit();
int ReadRecordFileNum(char *RecordNumASC);
int WriteRecordFileNum(int RecordFileNum);
int WriteDataToRecordFile(FILE *file, char *data, int length);
int ReadDataFromRecordFile(FILE *file, char *data, int length);
FILE* OpenDataRecordFile(int);
int GenerateRecordFileName(char *FileName);
int WriteARecord(char* data, int length);
#endif

