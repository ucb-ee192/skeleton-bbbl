#ifndef _LINE_LOGGER_H_
#define _LINE_LOGGER_H_

#include <cstdint>
#include <ctime>
#include <cstdio>
#include <stdexcept>

class LineLogger {
public:
  LineLogger() {
    fp_ = NULL;

    time_t t = time(NULL);
    struct tm tm_struct = *localtime(&t);

    snprintf(filename_, sizeof(filename_), "linescans.%d%02d%02d-%02d%02d%02d.csv",
        tm_struct.tm_year + 1900, tm_struct.tm_mon + 1, tm_struct.tm_mday, tm_struct.tm_hour, tm_struct.tm_min, tm_struct.tm_sec);

    fp_ = fopen(filename_, "w");
    if (fp_ == NULL) {
      throw std::runtime_error("LineLogger file open failed");
    }
    fprintf(fp_, "time (us), linescan, velocity\n");  // write header
  }

  void write_row(uint64_t time_us, uint16_t line[], float velocity) {
    if (fp_ == NULL) {
      throw std::runtime_error("LineLogger file not open");
    }

    fprintf(fp_, "%lld, ", time_us);
    fprintf(fp_, "\"[");
    for (size_t i=0; i<127; i++) {  // only read 127 because the last element doesn't need the trailing comma
      fprintf(fp_, "%d, ", line[i]);
    }
    fprintf(fp_, "%d", line[127]);
    fprintf(fp_, "]\", ");
    fprintf(fp_, "%f", velocity);

    fprintf(fp_, "\n");
  }

  void close() {
    fclose(fp_);
    fp_ = NULL;
  }

  const char* get_filename() {
    return filename_;
  }

protected:
  static constexpr size_t kFilenameLength = 128;

  char filename_[kFilenameLength];
  FILE* fp_;
};

#endif
