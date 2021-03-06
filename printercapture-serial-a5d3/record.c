#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "include/record.h"
#include "include/log.h"
#include "include/config.h"
#include "include/queue.h"
#include "include/general.h"
#define FILENAMEPREFIX "/var/printer/"

const char aibao_head[HeadLength] = {0x1b, 0x40, 0x1d, 0x76, 0x30, 0x00, 0x30, 0x00, 0x18};
const char aibao_end[EndLength] = {0x1b, 0x40, 0x1b, 0x40, 0x1b, 0x40, 0x1b, 0x40, 0x1b, 0x40, 0x1b, 0x4a, 0x48};
FILE *CurrentFile = NULL;
pthread_mutex_t FileLock;
extern int error;
int  IsHead(char *data)
{
  int count;
  for(count=0; count<HeadLength; ++count)
  {
    if(*(data+count)!=aibao_head[count])
    {
      return 0;
    }
  }
  return 1;
}
int IsEnd(char *data)
{
  int count;
  for(count=0; count<EndLength; ++count)
  {
    if(*(data+count)!=aibao_end[count])
      return 0;
  }
  return 1;
}
/* char zr_head[6] = {0x1b, 0x40, 0x1b, 0x4a, 0x00, 0x0d}; */
/* char zr_end[5] = {0x1b, 0x70, 0x0, 0x5, 0xff}; */
/* int IsHead(char *data) */
/* { */
/*     int count; */
/*     for(count=0; count<6; ++count) */
/*         { */
/*             if(*(data+count)!=zr_head[count]) */
/*                 { */
/*                     return 0; */
/*                 } */
/*         } */
/*     return 1; */
/* } */
/* int IsEnd(char *data) */
/* { */
/*     int count; */
/*     for(count=0; count<5; ++count) */
/*         { */
/*             if(*(data+count)!=zr_end[count]) */
/*                 { */
/*                     return 0; */
/*                 } */
/*         } */
/*     return 1; */
/* } */
int RecordFileInit()
{
  if (-1 == access(RECORDFILEPATH, F_OK))
  {
    if (-1 == WriteRecordFileNum(0))
    {
      perror("Creat file failed");
      return -1;
    }
  }
  pthread_mutex_init(&FileLock, NULL);
  return 0;
}
//inline static void GetFileLock( pthread_mutex_t * lock)
//{
  
//}
int ReadRecordFileNum(char *RecordNumASC)
{
  int RecordFileNum = 0;
 
    FILE *fd_Record;
    int ret = 0;
    if (NULL==(fd_Record = fopen(RECORDFILEPATH, "r")))
    {
      perror("Open Record file failed");
      return -1;
      //exit(1);
    }
    
    if (EOF==fscanf(fd_Record, "%d", &RecordFileNum))
    {
      printf("error = %d, num = %d\n", error, RecordFileNum);
      if (error != 0)
	perror("fscanf");
      // RecordFileNum = 0;
    }
    if (NULL!=RecordNumASC)
    {
      fseek(fd_Record, 0, SEEK_SET);
      if (EOF==fscanf(fd_Record, "%s", RecordNumASC))
      {
	printf("%s\n", RecordNumASC);
      }
    }
    // printf("%s eee\n", RecordNumASC);
    //LOGD("%s(%d)-%s:%s\n",__FILE__,__LINE__,__FUNCTION__, RecordNumASC);
    // printf("Record file num is %d\n", RecordFileNum);
    fclose(fd_Record);
    return RecordFileNum;
}
int WriteRecordFileNum(int RecordFileNum)
{
  FILE *fd_Record;
  if (NULL==(fd_Record = fopen(RECORDFILEPATH, "w+")))
  {
      perror("Open Record file failed");
      return -1;
      //exit(1);
  }
  fprintf(fd_Record, "%d", RecordFileNum);
  fclose(fd_Record);
  return 0;
}
int WriteDataToRecordFile(FILE *file, char *data, int length)
{
  //static int fd = 0;
  if (length!=fwrite(data, sizeof(char), length, file))
  {
    return -1;
  }
  return 0;
}
int ReadDataFromRecordFile(FILE *file, char *data, int length)
{
  int ret = 0;
  ret = fread(data, sizeof(char), length, file);
  if(length!=ret)
  {
    (0==feof(file))?(ret=-1):(ret);
  } 
  return ret;
}

static FILE* CreatDataRecordFile(char* RecordFileName)
{
  // char RecordNumASC[50];
  //int ret = 0;
  GenerateRecordFileName(RecordFileName);
  return  fopen(RecordFileName , "w+");
}
int GenerateRecordFileName(char *FileName)
{

  char timestr[20];
    time_t timer = time(NULL);
    struct tm timesample;
    localtime_r(&timer,&timesample);
    strftime(timestr, 20,"%F%H%M%S", &timesample);
    sprintf(FileName,"%s%s", FILENAMEPREFIX, timestr);
    LOGD("file name is %s", FileName);
    return 0;
    // printf("%s\n", RecordFileNumASC);
    // char FileNumAsc[30];
  // sprintf(FileNumAsc, "%d\0", FileNum);
    // sprintf(FileName, "%s%d", FILENAMEPREFIX, FileNum);
  // strcat(FileName, FileNumAsc);
    // LOGD(FileName);
  
}
int WriteARecord(char* data, int length)
{
    int ret = 0;
    char RecordNumASC[40];
    static char RecordFilename[40]; 
    static int RecordNum = 0;
    /* static time_t last_time = 0; */
    /* static time_t currentime = 0; */
  
    LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
    /* currentime = time(NULL);      /\*  *\/ */
    if (0 == length)//&&(NULL!=CurrentFile))
        {
            if (CurrentFile != NULL)
                {
                    if (0!=ftell(CurrentFile))
                        {
                            fflush(CurrentFile);
                            fclose(CurrentFile);
                            printf("File %s has been created!\n", RecordFilename);
                            /* CreatAndPushRecord(&RecordFilename[0]); */
                            CurrentFile = NULL;
                            return 0;
                        }
                }
            else
                {
                    return 0;
                }
        } 
    else if(NULL==CurrentFile)
        {  
            // GenerateRecordFileName(&RecordNumASC[0], RecordNum);
            if (NULL==(CurrentFile=CreatDataRecordFile(&RecordFilename[0])))
                {
                    LOGD("%s(%d)-%s:",__FILE__,__LINE__,__FUNCTION__);
                    perror(NULL);
                    LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
                    return -1;
                }
        }
    /* currentime = time(NULL); */
    /* if(IsHead(data)) */
    /* if ((currentime-last_time) > 5) */
    /* { */
    /*   if(0!=ftell(CurrentFile))//if the record file's offset is not the head,then creat a new file; */
    /*   { */
    /*     int ret; */
    /*     fflush(CurrentFile); */
    /*     fclose(CurrentFile);   */
      
    /*     ret = CreatAndPushRecord(&RecordFilename[0]); */
    /*     if (-1 == ret) */
    /*     { */

    /*     } */
    /*     //pthread_mutex_lock(&FileLock); //Get the number of files, plus 1, and wirte back */
    /*     //RecordNum = ReadRecordFileNum(NULL); */
    /*     //++RecordNum; */
    /*     //WriteRecordFileNum(RecordNum); */
    /*     //pthread_mutex_unlock(&FileLock); */

    /*     CurrentFile = CreatDataRecordFile(&RecordFilename[0]); */
    /*     if (NULL==CurrentFile) */
    /*     { */
    /*       LOGD("%s(%d)-%s:",__FILE__,__LINE__,__FUNCTION__); */
    /*       perror(NULL); */
    /*       return -1; */
    /*     } */
    /*     //WriteRecordFileNum(RecordNum); */
    /*   }   */
    /* }   */
    ret = fwrite(data, sizeof(char), length, CurrentFile);
    LOGD("In %s(%d)\n", __FUNCTION__, __LINE__);
    if (length!=ret)
        {
            LOGD("%s(%d)-%s:",__FILE__,__LINE__,__FUNCTION__);
            perror(NULL);
            return -1;
        }
    /* last_time = time(NULL); */
    /* if(((length>=EndLength)&&(IsEnd(data+length-EndLength)))||(length<EndLength))//the number of data is larger than EndLength and reach the end, or less than EndLength */
    /* {    */
    /*   int ret = 0; */
    /*   fflush(CurrentFile); */
    /*   fclose(CurrentFile); */
    /* LOGD("In %s(%d)\n", __FUNCTION__, __LINE__); */
    /*   ret = CreatAndPushRecord(&RecordFilename[0]); */
    /*   if (0==ret)//success to creat and push record */
    /*   { */
    /*   }  */
    /*   CurrentFile = NULL; */
    /* } */
    /* #if 0 */
    /*  else if (length<EndLength) */
    /*  { */
   
    /*    char tmp[EndLength]; */
    /*    int n = 0; */
    /*    for(n=0; n<EndLength-length; ++n) */
    /*    { */
    /*      tmp[n] = data[BUF_MAX-1-EndLength+length] */
    /*    } */
    /*    for(;n<EndLength;++n) */
    /*    { */
    /*      tmp[n]=*(data+EngLength-length-n); */
    /*    } */
   
    /*  } */
    /* #endif */
    LOGD("Out %s\r\n", __FUNCTION__);
    //LOGD("In %s\n", __FUNCTION__);
    return 0;
}
    /* int ReadARecord(FILE *fp, char *data, int length) */
    /* { */
    /*   return fread(data, length, sizeof(char), fp); */
    /* } */
    int ReadPrinterRecord(char *data, int length)
    {
        // char Filename_tmp[40];
        int ret = 0;
        static PrinterRecord *tmp=NULL;
        static FILE *fp = NULL;
        if (NULL == fp)
            {
                if (tmp!=NULL)
                    {
                        free(tmp);
                    }
                tmp = Dequeue(FileQueue);
                if(NULL == tmp)
                    {
                        return 0;
                    }
                LOGD("\n%s(%d)Filename is %s\n", __FILE__, __LINE__, tmp->FileName);
                fp = fopen(tmp->FileName , "r");
                if (fp==NULL)
                    {
                        PRINTERR("Can't open file %s!\n");
                        free(tmp);
                        //tmp = NULL;
                        return -1;
                        //exit(EXIT_FAILURE);//perror(NULL);
                    }
                // free(tmp);
            }
   
        ret = fread(data, sizeof(char),length, fp);
        PRINTERR("Get data num is ");
        printf("%d\n", ret);
        if (ret != length)
            {
                if (!feof(fp))
                    {
                        PRINTERR("Read failed failed!");
                    }
                fclose(fp);
                //            remove(tmp->FileName);
                free(tmp);
                tmp = NULL;
                fp = NULL;
            }
        return ret;
    }
