#include "database.hpp"
#include <cstdio>
#include <fstream>
#include "json.hpp"
#include <iostream>

using namespace std;

const char* red = "\033[31m";
const char* green = "\033[32m";
const char* reset = "\033[0m";

int Database::callback_vault(void* data, int argc, char** argv, char** colName) {
    auto* vaults = static_cast<vector<Vault>*>(data);

    Vault vault;
    vault.id = stoi(argv[0]);
    vault.name = argv[1] ? argv[1] : "";
    vault.description = argv[2] ? argv[2] : "";

    vaults->push_back(vault);

    return 0;
}

int Database::callback_category(void* data, int argc, char** argv, char** colName) {
    auto* categories = static_cast<vector<Category>*>(data);

    Category category;
    category.id = stoi(argv[0]);
    category.name = argv[1] ? argv[1] : "Unnamed";
    category.color = argv[2] ? argv[2] : "FFFFFF";
    category.vault = stoi(argv[3]);
    category.type = stoi(argv[4]);
    category.description = argv[5] ? argv[5] : "";

    categories->push_back(category);

    return 0;
}

int Database::callback_operation(void* data, int argc, char** argv, char** colName) {
    auto* operations = static_cast<vector<Operation>*>(data);

    Operation operation;
    operation.id = stoi(argv[0]);
    operation.date = stoi(argv[1]);;
    operation.amount = stof(argv[2]);;
    operation.category = stoi(argv[3]);;
    operation.description = argv[5] ? argv[4] : "";

    operations->push_back(operation);

    return 0;
}

void Database::close() {
    sqlite3_close(db);
}

int Database::open(const char* filename) {
    int rc;

    rc = sqlite3_open(filename, &db);
    
    if (rc) {
        cerr << red << "ERR " << reset << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        cout << green << "LOG " << reset << "Opened database successfully" << endl;
    }
    return 0;
}

int Database::init_base_table() {	
    char* errMsg = nullptr;

    const char* sql_cat_types_op = "CREATE TABLE IF NOT EXISTS cat_types_op ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL"
        ");";

    const char* sql_doc_vaults = "CREATE TABLE IF NOT EXISTS doc_vaults ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "description TEXT"
        ");";

    const char* sql_op_vault_amount = "CREATE TABLE IF NOT EXISTS op_vault_amount ("
        "id INTEGER PRIMARY KEY,"
        "amount REAL NOT NULL,"
        "FOREIGN KEY (id) REFERENCES doc_vaults(id)"
        "    ON UPDATE CASCADE ON DELETE SET NULL"
        ");";

    const char* sql_op_operation_history = "CREATE TABLE IF NOT EXISTS op_operation_history ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date INTEGER NOT NULL,"
        "amount REAL NOT NULL,"
        "category INTEGER,"
        "description TEXT,"
        "FOREIGN KEY (category) REFERENCES doc_categories(id)"
        "    ON UPDATE CASCADE ON DELETE SET NULL"
        ");";
    
    const char* sql_doc_categories = "CREATE TABLE IF NOT EXISTS doc_categories ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "color TEXT NOT NULL,"
        "vault INTEGER,"
        "type INTEGER NOT NULL,"
        "description TEXT,"
        "FOREIGN KEY (type) REFERENCES cat_types_op(id)"
        "    ON UPDATE CASCADE ON DELETE RESTRICT,"
        "FOREIGN KEY (vault) REFERENCES cat_vaults(id)"
        "    ON UPDATE CASCADE ON DELETE SET NULL"
        ");";
    const char* sql_basetype_1 = "INSERT INTO cat_types_op (name) VALUES ('Income');";
    const char* sql_basetype_2 = "INSERT INTO cat_types_op (name) VALUES ('Outcome');";

    int rc = sqlite3_exec(db, sql_cat_types_op, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_cat_types_op error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    rc = sqlite3_exec(db, sql_doc_vaults, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_doc_vaults error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    rc = sqlite3_exec(db, sql_op_vault_amount, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_op_vault_amount error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    rc = sqlite3_exec(db, sql_op_operation_history, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_op_operation_history error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
    }

    rc = sqlite3_exec(db, sql_doc_categories, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_doc_categories error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    rc = sqlite3_exec(db, sql_basetype_1, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_basetype_1 error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    rc = sqlite3_exec(db, sql_basetype_2, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << red << "ERR " << reset << "SQL sql_basetype_2 error: " << errMsg << endl;
            sqlite3_free(errMsg);
        return 1;
        }

    cout << green << "LOG " << reset << "Table created successfully" << endl;

    return 0;
}

int Database::create_balance(int id) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO op_vault_amount (id, amount) VALUES (?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error create_balance: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_double(stmt, 2, 0.0);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to op_vault_amount successfully, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    return 0;
}

int Database::create_vault(const char* name, const char* description) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO doc_vaults (name, description) VALUES (?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error create_vault: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to doc_vaults successfully, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    create_balance(sqlite3_last_insert_rowid(db));
    return 0;
}

int Database::create_category(const char* name, const char* color, int vault, int type, const char* description) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO doc_categories (name, color, vault, type, description) VALUES (?, ?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error create_category: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, color, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, vault);
    sqlite3_bind_int(stmt, 4, type);
    sqlite3_bind_text(stmt, 5, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to doc_categories successfully, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    return 0;
}

int Database::insert_operation(int date, double amount, int category, const char* description) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO op_operation_history (date, amount, category, description) VALUES (?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error insert_operation: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_int(stmt, 1, date);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int(stmt, 3, category);
    sqlite3_bind_text(stmt, 4, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to op_operation_history successfully, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    return 0;
}

vector<Database::Vault> Database::get_vaults() {
    vector<Vault> result;
    char* errMsg = nullptr;
    const char* sql = "SELECT * FROM doc_vaults;";
    
    cout << green << "LOG " << reset << "Query result:" << endl;
    
    int rc = sqlite3_exec(db, sql, callback_vault, &result, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error get_vaults: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return result;
}

double Database::get_balance(int id) {
    char* errMsg = nullptr;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT * FROM op_vault_amount WHERE id = ?;";
    double amount;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        amount = sqlite3_column_double(stmt, 1);
    }
    sqlite3_finalize(stmt);
    
    return amount;
}

vector<Database::Operation> Database::get_operations_by_vault(int id) {
    vector<Operation> result;
    char* errMsg = nullptr;
    char sql[255];
    sprintf(sql, "SELECT o.* FROM op_operation_history o JOIN doc_categories c ON o.category = c.id JOIN doc_vaults v ON c.vault = v.id WHERE v.id = %d;", id);
    int rc = sqlite3_exec(db, sql, callback_operation, &result, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error get_operations_by_vault: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return result;
}

vector<Database::Operation> Database::get_operations_by_category(int id) {
    vector<Operation> result;
    char* errMsg = nullptr;
    char sql[128];
    sprintf(sql, "SELECT * FROM op_operation_history WHERE category = %d;", id);
    int rc = sqlite3_exec(db, sql, callback_operation, &result, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error get_operations_by_category: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return result;
}

vector<Database::Category> Database::get_categories(int id) {
    vector<Category> result;
    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "SELECT * FROM doc_categories WHERE vault = %d;", id);
    int rc = sqlite3_exec(db, sql, callback_category, &result, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error get_categories: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    return result;
}

void Database::change_balance(int id, double amount) {
    char* errMsg = nullptr;
    char sql[128];
    sprintf(sql, "UPDATE op_vault_amount SET amount = %f WHERE id = %d;", amount, id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error change_balance: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_operation(int id) {
    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "DELETE FROM op_operation_history WHERE id = %d;", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_operation: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_operation_by_category(int id) {
    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "DELETE FROM op_operation_history WHERE category = %d;", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_operation: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_category(int id) {
    remove_operation_by_category(id);

    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "DELETE FROM doc_categories WHERE id = %d;", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_category: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_operations_by_vault(int id) {
    char* errMsg = nullptr;
    char sql[150];
    sprintf(sql, "DELETE FROM op_operation_history WHERE category IN (SELECT c.id FROM doc_categories c JOIN doc_vaults v ON c.vault = v.id WHERE v.id = %d);", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_category: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_categories_by_vault(int id) {
    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "DELETE FROM doc_categories WHERE vault = %d;", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_operation: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

void Database::remove_vault(int id) {
    remove_operations_by_vault(id);
    remove_categories_by_vault(id);

    char* errMsg = nullptr;
    char sql[64];
    sprintf(sql, "DELETE FROM doc_vaults WHERE id = %d;", id);
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error remove_vault: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
}

Database::Operation Database::get_operation(int id) {
    char* errMsg = nullptr;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT * FROM op_operation_history WHERE id = ?;";
    Operation operation;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        operation.date = sqlite3_column_int(stmt, 1);
        operation.amount = sqlite3_column_double(stmt, 2);
        operation.category = sqlite3_column_int(stmt, 3);
    }
    sqlite3_finalize(stmt);
    
    return operation;
}

Database::Category Database::get_category(int id) {
    char* errMsg = nullptr;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT * FROM doc_categories WHERE id = ?;";
    Category category;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        category.vault = sqlite3_column_int(stmt, 3);
        category.type = sqlite3_column_int(stmt, 4);
    }
    sqlite3_finalize(stmt);
    
    return category;
}

void Database::fix_diff(int curr_category_id, double curr_amount, Operation prev_operation) {
    Category curr_category = get_category(curr_category_id);
    Category prev_category = get_category(prev_operation.category);

    int curr_mod = curr_category.type-1 ? -1 : 1;
    int prev_mod = prev_category.type-1 ? -1 : 1;
    double bal = get_balance(curr_category.vault);
    bal += curr_amount * curr_mod - prev_operation.amount * prev_mod;
    change_balance(curr_category.vault, bal);
}

int Database::update_operation(int id, int date, double amount, int category, char* description) {
    Operation prev_operation = get_operation(id);
    sqlite3_stmt* stmt;
    char* errMsg = nullptr;
    const char* sql = "UPDATE op_operation_history SET date = ?, amount = ?, category = ?, description = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, date);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int(stmt, 3, category);
    sqlite3_bind_text(stmt, 4, description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, id);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "update_operation success, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    
    fix_diff(category, amount, prev_operation);
    
    return 0;
}

int Database::update_category(int id, const char* name, const char* color, const char* description) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE doc_categories SET name = ?, color = ?, description = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, color, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, id);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "update_category success, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    
    return 0;
}

int Database::update_vault(int id, const char* name, const char* description) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE doc_vaults SET name = ?, description = ? WHERE id = ?";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "update_vault success, ID: " << sqlite3_last_insert_rowid(db) << endl;
    sqlite3_finalize(stmt);
    
    return 0;
}

Database::Database() {
    ifstream file("config.json");
    if (!file) {
        cout << green << "LOG " << reset << "Config is not found. Creating..." << endl;

        nlohmann::json j;

        j["name"] = "user1";
        j["theme"] = "default_light";
        j["database"] = "database.db";

        ofstream file("config.json");

        file << j.dump(4);
        file.close();

        cout << green << "LOG " << reset << "Base config creation success" << endl;
    }
    
    ifstream config_file("config.json");
    nlohmann::json config_json;
    config_file >> config_json;
    
    string temp_str = config_json["database"];
    const char* filename = temp_str.c_str();
    
    ifstream file_db(filename);
    if (!file_db) {
        open(filename);
        init_base_table();
    } else {
        open(filename);
    }
}

int main() {
    Database mreow;
    mreow.close();
    return 0;
}