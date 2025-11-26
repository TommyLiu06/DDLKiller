#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

struct Modification;

class DatabaseManager {
public:
    DatabaseManager(const std::string& dbPath);

    // SQLite::Database 不可拷贝
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    void addModification(
        const std::string& userId,
        const int& operationId,
        const std::string& operation,
        const int& targetId = 0,
        const std::string& title = "",
        const std::string& description = "",
        const std::string& dueDate = "",
        int deleteFlag = 0,
        int completedFlag = 0
    );

    std::vector<Modification> getModificationsByUser(const std::string& userId);

private:
    SQLite::Database db;
    SQLite::Statement insert;
    SQLite::Statement select;

};
