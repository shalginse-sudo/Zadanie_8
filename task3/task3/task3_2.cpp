#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string &message) {}
    virtual void onError(const std::string &message) {}
    virtual void onFatalError(const std::string &message) {}
};

class Observable
{
public:
    void addObserver(Observer *observer) {
        observers_.emplace_back(observer);
    }

    void warning(const std::string &message) const {
        for(auto observer : observers_) {
            observer->onWarning(message);
        }
    }

    void error(const std::string &message) const {
        for(auto observer : observers_) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string &message) const {
        for(auto observer : observers_) {
            observer->onFatalError(message);
        }
    }

    mutable std::vector<Observer *> observers_;
};

class WarningObserver
    : public Observer
{
public:
    void onWarning(const std::string &message) override {
        std::cout << "Warning: " << message << '\n';
    }
};

class ErrorObserver
    : public Observer
{
public:
    explicit ErrorObserver(std::string path)
        : path_(std::move(path)) {}

    void onError(const std::string &message) override {
        std::ofstream file(path_, std::ios::app);
        if(!file) {
            throw std::runtime_error("Cannot open log file: " + path_);
        }
        file << message << '\n';
    }

private:
    std::string path_;
};

class FatalErrorObserver
    : public Observer
{
public:
    explicit FatalErrorObserver(std::string path)
        : path_(std::move(path)) {}

    void onFatalError(const std::string &message) override {
        std::cout << message << '\n';

        std::ofstream file(path_, std::ios::app);
        if(!file) {
            throw std::runtime_error("Cannot open log file: " + path_);
        }
        file << message << '\n';
    }

private:
    std::string path_;
};

void task3_2()
{
    Observable logger;

    WarningObserver warningObserver;
    ErrorObserver errorObserver("errors.log");
    FatalErrorObserver fatalObserver("fatal-errors.log");

    logger.addObserver(&warningObserver);
    logger.addObserver(&errorObserver);
    logger.addObserver(&fatalObserver);

    logger.warning("Warning");
    logger.error("Error");
    logger.fatalError("FatalError");
}
