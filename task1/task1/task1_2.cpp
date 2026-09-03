#include <string>
#include <vector>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <map>

namespace {
    class SqlSelectQueryBuilder
    {
    public:
        SqlSelectQueryBuilder &AddColumn(const std::string &column)
        {
            m_columns.push_back(column);

            return *this;
        }

        SqlSelectQueryBuilder &AddFrom(const std::string &from)
        {
            m_from = from;

            return *this;
        }

        SqlSelectQueryBuilder &AddWhere(const std::string &col, const std::string &val)
        {
            m_wheres.push_back({ col, val });

            return *this;
        }

        SqlSelectQueryBuilder &AddWhere(const std::map<std::string, std::string> &kv) noexcept
        {
            m_wheres.insert(m_wheres.end(), kv.begin(), kv.end());

            return *this;
        }

        SqlSelectQueryBuilder &AddColumns(const std::vector<std::string> &columns) noexcept
        {
            m_columns.insert(m_columns.end(), columns.begin(), columns.end());

            return *this;
        }

        std::string BuildQuery()
        {
            if(!m_from) {
                throw std::logic_error("no 'from' statement");
            }

            std::stringstream ss;
            ss << "SELECT ";
            if(m_columns.empty()) {
                ss << "* ";
            } else {
                for(size_t i = 0; i < m_columns.size(); ++i) {
                    ss << m_columns[i];
                    if(i != m_columns.size()-1) {
                        ss << ", ";
                    } else {
                        ss << " ";
                    }
                }
            }

            ss << "FROM " << *m_from << " ";

            if(!m_wheres.empty()) {
                ss << "WHERE ";
                for(size_t i = 0; i < m_wheres.size(); ++i) {
                    ss << m_wheres[i].first << "=" << m_wheres[i].second;
                    if(i != m_wheres.size() - 1) {
                        ss << " AND ";
                    }
                }
            }

            ss << ";";

            return ss.str();
        }

        void Clear()
        {
            m_columns.clear();
            m_from.reset();
            m_wheres.clear();
        }

    private:
        std::vector<std::string> m_columns;
        std::optional<std::string> m_from;
        std::vector<std::pair<std::string, std::string>> m_wheres;
    };
}


void task1_2()
{
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumns({ "name", "phone" });
    query_builder.AddFrom("students");
    query_builder.AddWhere({ { "id", "42" }, { "name", "John" } });

    assert(query_builder.BuildQuery() == "SELECT name, phone FROM students WHERE id=42 AND name=John;");

    std::cout << query_builder.BuildQuery();
}
