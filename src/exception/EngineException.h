#pragma once
#include <exception>
#include <string>

class EngineException : public std::exception
{
public:
	EngineException( int line,const char* file ) noexcept;
	const char* what() const noexcept override;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};
