#pragma once
#include <sstream>
#include <string>

#define THROW_WINDOW_EXCEPTION( msg ) Exception( __LINE__, __FILE__, msg )

class Exception : public std::exception {
    int line;
    std::string file;
    std::string message;

public:
    Exception(const int line, const std::string &file, const std::string &message) {
        this->line = line;
        this->file = file;
        this->message = message;
    }



    [[nodiscard]] std::string Message() const {
        return message;
    }

    void SetMessage(const std::string &m) {
        this->message = m;
    }

    [[nodiscard]] virtual std::string Print() const {
        std::ostringstream oss;
        oss << file << ":" << line << " - " << message;
        return oss.str();
    };
};
