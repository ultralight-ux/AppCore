#pragma once
#include <AppCore/Defines.h>
#include <Ultralight/platform/Logger.h>
#include <Ultralight/String.h>
#include <fstream>

namespace ultralight {

class FileLogger : public Logger {
  std::ofstream log_file_;
public:
  FileLogger(const String& log_path);
  virtual ~FileLogger();

  virtual void LogMessage(LogLevel log_level, const String16& message) override;
};

}  // namespace ultralight
