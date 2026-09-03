
#include <map>
#include <string>
#include <iostream>

class VeryHeavyDatabase
{
public:
    std::string GetData(const std::string &key) const noexcept {
        return "Very Big Data String: " + key;
    }
};

class OneShotDB : VeryHeavyDatabase
{
public:
    explicit OneShotDB(VeryHeavyDatabase *real_object, size_t shots = 1)
        : shots_(shots)
        , real_db_(real_object)
    {
    }

    std::string GetData(const std::string &key) const noexcept {
        if(key_used_times_[key] < shots_) {
            ++key_used_times_[key];
            return "Very Big Data String: " + key;
        }

        return "error";
    }

private:
    size_t shots_;
    VeryHeavyDatabase *real_db_;

    mutable std::map<std::string, size_t> key_used_times_;
};

void task2_2()
{
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
}