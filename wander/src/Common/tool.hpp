#ifndef PRECISEINTEREST_TOOL_HPP_
#define PRECISEINTEREST_TOOL_HPP_

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <zconf.h>

namespace Common {
  class tools {
    public:
      struct FileInfo {
            public:
                std::string fileName = "";
                int size = 0;
                int time = 0;
      };
      static std::vector<std::string> split(std::string str, std::string pattern) {
          std::vector<std::string> result;
          char* s = const_cast<char*>(str.c_str());
          const char *d = pattern.c_str();
          char *p;
          p = strtok(s,d);
          while(p) {
              result.push_back(p);
              p=strtok(NULL,d);
          }
          return result;
      }

      static std::vector<char*> split(char* str, const char* pattern) {
          std::vector<char*> result;
          char* s = str;
          const char *d = pattern;
          char *p;
          p = strtok(s,d);
          while(p) {
              result.push_back(p);
              p=strtok(NULL,d);
          }
          return result;
      }

      static const char* stringToConstChar(std::string str) {
          return str.c_str();
      }

      static std::string constCharToString(const char* constChar) {
          std::string str(constChar);
          return str;
      }


      static struct FileInfo getNewFile(std::string filePath) {
          DIR *d;
          struct dirent *file;
          struct stat buf;
          struct FileInfo newFile;

          char* path = const_cast<char*>(filePath.c_str());
          if(!(d = opendir(path))) {
              printf("error opendir %s!!!\\n",path);
              return newFile;
          }
          chdir(path);
          int time = 0;
          while((file = readdir(d)) != NULL) {

              if(strncmp(file->d_name, ".", 1) == 0)
              continue;
              if(stat(file->d_name, &buf) >= 0 && !S_ISDIR(buf.st_mode)) {

                  if(buf.st_mtime > time) {
                      time = buf.st_mtime;

                      newFile.fileName = file->d_name;
                      newFile.size = buf.st_size;
                      newFile.time = buf.st_mtime;

                      //newFile = file->d_name;
                  }
                  //printf("%s\n",file->d_name);
                  //printf("\tfile size=%d\n",buf.st_size);
                  //printf("\tfile last modify time=%d\n",buf.st_mtime);
              }

          }
          closedir(d);
          return newFile;
    }



  };


//char* constCharToChar(const char* constChar) {
//}
}



#endif
