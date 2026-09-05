#include <fstream>
#include <iostream>
#include <string>
#include <memory>

enum class Type
{
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage
{
public:
    LogMessage(Type type, const std::string &message)
        : type_(type)
        , message_(message)
    {}

    Type type() const { return type_; }
    const std::string &message() const { return message_; }

private:
    Type type_;
    std::string message_;
};

class LogHandler
{
public:
    virtual ~LogHandler() = default;

    LogHandler &setNext(std::unique_ptr<LogHandler> next) {
        next_ = std::move(next);
        return *next_;
    }

    virtual void handle(const LogMessage &message) const {
        if(next_) {
            next_->handle(message);
        }
    }

protected:
    std::unique_ptr<LogHandler> next_;
};

class FatalErrorHandler
    : public LogHandler
{
public:
    void handle(const LogMessage &message) const override {
        if(message.type() == Type::FatalError) {
            throw std::runtime_error(message.message());
        }
        LogHandler::handle(message);
    }
};

class ErrorHandler
    : public LogHandler
{
public:
    explicit ErrorHandler(const std::string &path)
        : path_(path) {}

    void handle(const LogMessage &message) const override {
        if(message.type() != Type::Error) {
            LogHandler::handle(message);
            return;
        }

        std::ofstream file(path_, std::ios::app);
        if(!file) {
            throw std::runtime_error("Cannot open log file: " + path_);
        }
        file << "Error: " << message.message() << '\n';
    }

private:
    std::string path_;
};

class WarningHandler
    : public LogHandler
{
public:
    void handle(const LogMessage &message) const override {
        if(message.type() == Type::Warning) {
            std::cout << "Warning: " << message.message() << '\n';
            return;
        }
        LogHandler::handle(message);
    }
};

class UnknownMessageHandler
    : public LogHandler
{
public:
    void handle(const LogMessage &message) const override {
        if(message.type() == Type::Unknown) {
            throw std::runtime_error("Unhandled message (" + message.message() + ")");
        }
        LogHandler::handle(message);
    }
};

void task3_3() {
    auto chain = std::make_unique<FatalErrorHandler>();
    LogHandler &errorHandler = chain->setNext(
        std::make_unique<ErrorHandler>("chain-errors.log"));
    LogHandler &warningHandler = errorHandler.setNext(
        std::make_unique<WarningHandler>());
    warningHandler.setNext(std::make_unique<UnknownMessageHandler>());

    chain->handle(LogMessage(Type::Warning, "Warning"));
    chain->handle(LogMessage(Type::Error, "Error"));

    try {
        chain->handle(LogMessage(Type::FatalError, "Fatal Error"));
    } catch(const std::exception &exception) {
        std::cout << "Exception: " << exception.what() << '\n';
    }

    try {
        chain->handle(LogMessage(Type::Unknown, "Unexpected event"));
    } catch(const std::exception &exception) {
        std::cerr << "Exception: " << exception.what() << '\n';
    }
}
