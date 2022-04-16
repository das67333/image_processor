#pragma once

#include <stdexcept>
#include <string>

class InvalidCmdArgument : public std::invalid_argument {
public:
    InvalidCmdArgument(const std::string& s) : std::invalid_argument(s) {
    }
};

class HelpMessage : public std::runtime_error {
public:
    static const std::string HELP_MESSAGE;

    HelpMessage() : std::runtime_error(HELP_MESSAGE) {
    }
};

class FilesystemError : public std::runtime_error {
public:
    FilesystemError(const std::string& s) : std::runtime_error(s) {
    }
};

class IncompatibleFileError : public std::runtime_error {
public:
    IncompatibleFileError(const std::string& s) : std::runtime_error(s) {
    }
};

class FileCorruptionError : public std::runtime_error {
public:
    FileCorruptionError(const std::string& s) : std::runtime_error(s) {
    }
};
