#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <vector>
#include <string>

class Database {
    private:
        char* filename;
        sqlite3* db;

    public:
        struct Operation {
            int id;
            int date;
            double amount;
            int category;
            std::string description;
        };

        struct Category {
            int id;
            std::string name;
            std::string color;
            int vault;
            int type;
            std::string description;
        };

        struct Vault {
            int id;
            std::string name;
            std::string description;
        };

        Database();
        
        static int callback_vault(void* data, int argc, char** argv, char** colName);
        static int callback_category(void* data, int argc, char** argv, char** colName);
        static int callback_operation(void* data, int argc, char** argv, char** colName);

        int open(const char* filename);
        void close();
        int init_base_table();

        int create_vault(const char* name, const char* description = nullptr);
        int create_category(const char* name, const char* color, int vault, int type, const char* description = nullptr);
        int create_balance(int id);
        int insert_operation(int date, double amount, int category, const char* description = "");

        std::vector<Vault> get_vaults();
        std::vector<Category> get_categories(int id);
        std::vector<Operation> get_operations_by_vault(int id);
        std::vector<Operation> get_operations_by_category(int id);
        Operation get_operation(int id);
        Category get_category(int id);
        double get_balance(int id);

        int update_operation(int id, int date, double amount, int category, char* description);
        int update_category(int id, const char* name, const char* color, const char* description);
        int update_vault(int id, const char* name, const char* description);

        void remove_operation(int id);
        void remove_operation_by_category(int id);
        void remove_category(int id);
        void remove_operations_by_vault(int id);
        void remove_categories_by_vault(int id);
        void remove_vault(int id);

        void change_balance(int id, double amount);
        void fix_diff(int curr_category_id, double curr_amount, Operation prev_operation);
};

#endif // DATABASE_HPP