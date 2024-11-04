#include "EngineException.h"
#include <sstream>

#include "../common/Logger.h"


EngineException::EngineException(int line, const char *file) noexcept
    : line(line),
      file(file) {
}

const char *EngineException::what() const noexcept {
    return GetOriginString().c_str();
}

int EngineException::GetLine() const noexcept {
    return line;
}

const std::string &EngineException::GetFile() const noexcept {
    // Verifica se la parte finale della stringa corrisponde a srcDir
    size_t pos = file.rfind("/src/");
    if (pos != std::string::npos) {
        return *new std::string(file.substr(pos, file.length() - pos));
    }
    return file;
}

std::string EngineException::GetOriginString() const noexcept {
    std::ostringstream oss;
    oss << GetFile() << ":" << GetLine();
    return oss.str();
}
