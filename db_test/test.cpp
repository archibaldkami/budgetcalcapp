#include "database.hpp"
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int main() {
    Database db;
    db.create_vault("Vault1");

    db.create_category("Category 1", "FFFFFF", 1, 1);

    db.insert_operation(time(NULL), 200.52, 1, "Mreow 1");
    db.insert_operation(time(NULL), 50, 1, "Mreow 2");
    db.insert_operation(time(NULL), 768, 1, "Mreow 3");
    db.insert_operation(time(NULL), 200.52, 1, "Mreow 4");
    db.insert_operation(time(NULL), 50, 1, "Mreow 5");
    db.insert_operation(time(NULL), 768, 1, "Mreow 6");

    db.create_category("Category 2", "FFFFFF", 1, 2);

    db.insert_operation(time(NULL), 200.52, 2, "Mreow 1");
    db.insert_operation(time(NULL), 50, 2, "Mreow 2");
    db.insert_operation(time(NULL), 768, 2, "Mreow 3");
    db.insert_operation(time(NULL), 200.52, 2, "Mreow 4");
    db.insert_operation(time(NULL), 50, 2, "Mreow 5");
    db.insert_operation(time(NULL), 768, 2, "Mreow 6");

    db.create_vault("Vault 2");

    db.create_category("Category 3", "FFFFFF", 2, 1);

    db.insert_operation(time(NULL), 200.52, 3, "Mreow 1");
    db.insert_operation(time(NULL), 50, 3, "Mreow 2");
    db.insert_operation(time(NULL), 768, 3, "Mreow 3");
    db.insert_operation(time(NULL), 200.52, 3, "Mreow 4");
    db.insert_operation(time(NULL), 50, 3, "Mreow 5");
    db.insert_operation(time(NULL), 768, 3, "Mreow 6");

    db.remove_category(1);
    
    db.remove_operation(15);
    
    vector<Database::Vault> vaults = db.get_vaults();
    vector<Database::Category> categories;
    vector<Database::Operation> operations;

    
    for (auto& vault : vaults) {
        cout << "\033[33mVault\033[0m " << vault.id << "\t" << vault.name << "\t" << vault.description << endl << db.get_balance(vault.id) << endl << endl;

        categories = db.get_categories(vault.id);
        for (auto& category : categories) {
            cout << "\033[34mCategory\033[0m " << category.id << "\t" << category.name << "\t" << category.color << "\t" << category.vault << category.type << category.description << endl << endl; 
            operations = db.get_recent_operations_by_category(category.id);
        
            for (auto& operation : operations) {
                cout << "\033[35mOperation\033[0m " << operation.id << "\t" << operation.date << "\t" << operation.amount << "\t" << operation.category << operation.description << endl;
            }
            cout << endl << endl;
        }
    }

    return 0;
}