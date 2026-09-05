#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class LogCommand
{
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string &message) = 0;
};

class ConsoleLogCommand
    : public LogCommand
{
public:
    void print(const std::string &message) override {
        std::cout << message << '\n';
    }
};

class FileLogCommand
    : public LogCommand
{
public:
    explicit FileLogCommand(std::string path)
        : path_(std::move(path)) {}

    void print(const std::string &message) override {
        std::ofstream file(path_, std::ios::app);
        if(!file) {
            throw std::runtime_error("Cannot open log file: " + path_);
        }
        file << message << '\n';
    }

private:
    std::string path_;
};

void print(LogCommand &command, const std::string &message) {
    command.print(message);
}

void task3_1()
{
    ConsoleLogCommand console;
    FileLogCommand file("command.log");

    print(console, "Message written to the console");
    print(file, "Message written to the file");
}
