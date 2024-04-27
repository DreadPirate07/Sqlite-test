#include <sqlite3.h>
#include <iostream>

// Function to execute SQL commands
static int execute_sql(sqlite3 *db, const char *sql)
{
    char *errorMessage = nullptr;
    int res = sqlite3_exec(db, sql, nullptr, 0, &errorMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error in execution : " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    return res;
}

// Function to create the table
void create_table(sqlite3 *db)
{
    const char *sql = "CREATE TABLE IF NOT EXISTS employees (id INTEGER PRIMARY KEY,name TEXT NOT NULL, age INTEGER NOT NULL)";
    execute_sql(db, sql);
}

// Function to insert data into the table
void insert_data(sqlite3 *db, int id, const char *name, int age)
{
    char sql[256];
    sprintf(sql, "INSERT INTO employees (id, name, age) VALUES (%d, '%s', %d);", id, name, age);
    execute_sql(db, sql);
}

// Function to query and display data
void query_data(sqlite3 *db)
{
    const char *sql = "SELECT * FROM employees;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *name = sqlite3_column_text(stmt, 1);
            int age = sqlite3_column_int(stmt, 2);
            std::cout << "ID: " << id << ", Name: " << name << ", Age: " << age << std::endl;
        }
        sqlite3_finalize(stmt);
    }
}

int main()
{
    sqlite3 *db;
    if (sqlite3_open("test.db", &db) == SQLITE_OK)
    {
        create_table(db);
        insert_data(db, 1, "Bitcoin", 10);
        insert_data(db, 2, "Satoshi", 20);
        query_data(db);
    }
    else
    {
        std::cerr << "Could not open database" << std::endl;
    }
    sqlite3_close(db);
    return 0;
}
