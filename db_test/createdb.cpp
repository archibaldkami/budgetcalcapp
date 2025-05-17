#include <iostream>
#include <sqlite3.h>
using namespace std;

const char* red = "\033[31m";
const char* green = "\033[32m";
const char* reset = "\033[0m";

int init_base_table(sqlite3* db) {	

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

	int rc = sqlite3_exec(db, sql_cat_types_op, nullptr, nullptr, &errMsg);
    	if (rc != SQLITE_OK) {
        	cerr << red << "ERR " << reset << "SQL 1 error: " << errMsg << endl;
        	sqlite3_free(errMsg);
		return 1;
    	}

	rc = sqlite3_exec(db, sql_doc_vaults, nullptr, nullptr, &errMsg);
    	if (rc != SQLITE_OK) {
        	cerr << red << "ERR " << reset << "SQL 2 error: " << errMsg << endl;
        	sqlite3_free(errMsg);
		return 1;
    	}

	rc = sqlite3_exec(db, sql_op_vault_amount, nullptr, nullptr, &errMsg);
    	if (rc != SQLITE_OK) {
        	cerr << red << "ERR " << reset << "SQL 3 error: " << errMsg << endl;
        	sqlite3_free(errMsg);
		return 1;
    	}

	rc = sqlite3_exec(db, sql_op_operation_history, nullptr, nullptr, &errMsg);
    	if (rc != SQLITE_OK) {
        	cerr << red << "ERR " << reset << "SQL 4 error: " << errMsg << endl;
        	sqlite3_free(errMsg);
		return 1;
	}

	rc = sqlite3_exec(db, sql_doc_categories, nullptr, nullptr, &errMsg);
    	if (rc != SQLITE_OK) {
        	cerr << red << "ERR " << reset << "SQL 5 error: " << errMsg << endl;
        	sqlite3_free(errMsg);
		return 1;
    	}
        cout << green << "LOG " << reset << "Table created successfully" << endl;

    	return 0;
}

int create_balance(sqlite3* db, int id) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO op_vault_amount (id, amount) VALUES (?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_double(stmt, 2, 0.0);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl; // ERR LOG
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to op_vault_amount successfully, ID: " << sqlite3_last_insert_rowid(db) << endl; // LOG
    sqlite3_finalize(stmt);
    return 0;
}

int create_vault(sqlite3* db, const char* name, const char* description = nullptr) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO doc_vaults (name, description) VALUES (?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl; // ERR LOG
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to doc_vaults successfully, ID: " << sqlite3_last_insert_rowid(db) << endl; // LOG
    sqlite3_finalize(stmt);
    create_balance(db, sqlite3_last_insert_rowid(db));
    return 0;
}

int create_category(sqlite3* db, const char* name, const char* color, int vault, int type, const char* description = nullptr) {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO doc_categories (name, color, vault, type, description) VALUES (?, ?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, color, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, vault);
    sqlite3_bind_int(stmt, 4, type);
    sqlite3_bind_text(stmt, 5, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl; // ERR LOG
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to doc_categories successfully, ID: " << sqlite3_last_insert_rowid(db) << endl; // LOG
    sqlite3_finalize(stmt);
    return 0;
}

int insert_operation(sqlite3* db, int date, double amount, int category, const char* description = "") {
    char* errMsg = nullptr;
    
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO doc_vaults (date, amount, category, description) VALUES (?, ?, ?, ?);";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    
    sqlite3_bind_int(stmt, 1, date);
    sqlite3_bind_double(stmt, 2, amount);
    sqlite3_bind_int(stmt, 3, category);
    sqlite3_bind_text(stmt, 4, description, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    
    if (rc != SQLITE_DONE) {
        cerr << red << "ERR " << reset << sqlite3_errmsg(db) << endl; // ERR LOG
        sqlite3_finalize(stmt);
        return 1;
    }
    
    cout << green << "LOG " << reset << "Record added to op_operation_history successfully, ID: " << sqlite3_last_insert_rowid(db) << endl; // LOG
    sqlite3_finalize(stmt);
    return 0;
}

struct Operation {
    int id;
    int date;
    double amount;
    int category;
    string description;
    
    Operation() : id(0), date(  ), amount(0.0), category(0), description("") {}
};

struct Category {
    int id;
    string name;
    string color;
    int vault;
    int type;
    string description;
    
    Category() : id(0), name(""), color(""), vault(0), type(0), description("") {}
};

struct Vault {
    int id;
    string name;
    string description;
    
    Vault() : id(0), name(""), description("") {}
};

static int callback_vaults_record(void* data, int argc, char** argv, char** azColName) {
    vector<VaultsRecord>* records = static_cast<vector<VaultRecord>*>(data);
    
    VaultsRecord record;
    
    for (int i = 0; i < argc; i++) {
        string colName = azColName[i];
        const char* value = argv[i] ? argv[i] : "";
        
        if (colName == "id") {
            record.id = atoi(value);
        } 
        else if (colName == "name") {
            record.name = value;
        }
        else if (colName == "size") {
            record.size = atof(value);
        }
        else if (colName == "created_date") {
            record.created_date = atoi(value);
        }
        else if (colName == "description") {
            record.description = value;
        }
    }
    
    records->push_back(record);
    return 0;
}

vector<Vault> get_vaults(sqlite3* db) {
    char* errMsg = nullptr;
    const char* sql = "SELECT * FROM doc_vaults;";
    
    vector<Vault> result;

    cout << green << "LOG " << reset << "Query result:" << endl;
    
    int rc = sqlite3_exec(db, sql, callback_vaults_record, &result, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << red << "ERR " << reset << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    }
    
    return result;
}

double get_balance(sqlite3* db, int id) {
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

int main() {
	sqlite3* db;
    	int rc;

    	rc = sqlite3_open("test_table.db", &db);
    
    	if (rc) {
        	cerr << red << "ERR " << reset << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        	return 1;
    	} else {
        	cout << green << "LOG " << reset << "Opened database successfully" << endl;
    	}
	init_base_table(db);
    	// Mreow :) 
	
	create_vault(db, "test1");
	create_vault(db, "test2", "Some description");
	get_vaults(db); // Mreow

	cout << get_balance(db, 8) << endl; //

    	sqlite3_close(db);
    
    	return 0;
}

