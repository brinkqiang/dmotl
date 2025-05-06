#define OTL_ODBC_MYSQL // Compile OTL 4/ODBC/MySQL

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

#pragma warning(push)
#pragma warning(disable: 5272)
#include <otlv4.h>
#pragma warning(pop)

#include "dmflags.h"

class DatabaseHandler {
private:
    otl_connect db;
    static constexpr const char* DEFAULT_DB = "test_db";
    static constexpr const char* TABLE_NAME = "test_tab";

    void executeDirect(const std::string& sql) {
        otl_cursor::direct_exec(db, sql.c_str(), otl_exception::enabled);
    }

public:
    DatabaseHandler() { otl_connect::otl_initialize(); }
    
    ~DatabaseHandler() {
        try {
            if (db.connected) db.logoff();
        } catch (...) {} // Destructors should not throw
    }

    void connect(const std::string& user, 
                 const std::string& password,
                 const std::string& server = "127.0.0.1",
                 int port = 3306) {
        std::ostringstream conn_str;

        std::string driver = "DRIVER={MySQL ODBC 9.2 Unicode Driver};";

        //std::string driver = "DSN=mysql;";

        conn_str << driver
                 << "UID=" << user << ";"
                 << "PWD=" << password << ";"
                 << "SERVER=" << server << ";"
                 << "PORT=" << port << ";"
                 << "CHARSET=utf8mb4;";
        
        try {
            db.rlogon(conn_str.str().c_str());
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Connection failed");
        }
    }

    void initializeDatabase() {
        try {
            executeDirect("CREATE DATABASE IF NOT EXISTS " + std::string(DEFAULT_DB));
            executeDirect("USE " + std::string(DEFAULT_DB));
            executeDirect(
                "CREATE TABLE IF NOT EXISTS " + std::string(TABLE_NAME) + 
                "(id INT PRIMARY KEY, name VARCHAR(255), "
                "modified_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)"
            );
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Database initialization failed");
        }
    }
    void dropDatabase() {
        try {
            executeDirect("DROP DATABASE IF EXISTS " + std::string(DEFAULT_DB));
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Database initialization failed");
        }
    }
    void insertData(int num_records) {
        try {
            otl_stream insert_stream(
                1, // Buffer size 1 = no buffering
                ("INSERT INTO " + std::string(TABLE_NAME) + 
                " (id, name) VALUES(:id<int>, :name<char[255]>)").c_str(),
                db
            );

            for (int i = 1; i <= num_records; ++i) {
                std::string name = "Record_" + std::to_string(i);
                insert_stream << i << name.c_str();
            }
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Data insertion failed");
        }
    }

    void updateRecord(int id, const std::string& new_name) {
        try {
            otl_stream update_stream(
                1,
                ("UPDATE " + std::string(TABLE_NAME) + 
                " SET name = :name<char[255]> WHERE id = :id<int>").c_str(),
                db
            );
            
            if (new_name.empty()) {
                update_stream << otl_null() << id;
            } else {
                update_stream << new_name.c_str() << id;
            }
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Update operation failed");
        }
    }

    void displayRecords(int threshold) {
        try {
            std::ostringstream query;
            query << "SELECT id, name, modified_at FROM " << TABLE_NAME 
                  << " WHERE id >= " << threshold 
                  << " ORDER BY modified_at DESC";

            otl_stream select_stream(1, query.str().c_str(), db);
            
            int id;
            char name[255];
            otl_datetime timestamp;

            while (!select_stream.eof()) {
                select_stream >> id >> name >> timestamp;
                std::cout << "ID: " << id
                          << "\tName: " << (select_stream.is_null() ? "NULL" : name)
                          << "\tLast Modified: " << formatTimestamp(timestamp)
                          << std::endl;
            }
        } catch (const otl_exception& e) {
            handleException(e);
            throw std::runtime_error("Query failed");
        }
    }

private:
    static void handleException(const otl_exception& e) {
        std::cerr << "Database Error:\n"
                  << "Message: " << e.msg << "\n"
                  << "SQL State: " << e.sqlstate << "\n"
                  << "Statement: " << e.stm_text << "\n"
                  << "Variable: " << e.var_info << "\n"
                  << "Code: " << e.code << "\n";
    }
    static std::string formatTimestamp(const otl_datetime& dt) {
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << dt.year << "-"
            << std::setw(2) << dt.month << "-"
            << std::setw(2) << dt.day << " "
            << std::setw(2) << dt.hour << ":"
            << std::setw(2) << dt.minute << ":"
            << std::setw(2) << dt.second;
        return oss.str();
    }

};

DEFINE_uint64(PORT, 3306, "mysql port");
DEFINE_string(HOST, "localhost", "mysql host");
DEFINE_string(USER, "root", "mysql username");
DEFINE_string(PASS, "pass", "mysql password");

int main(int argc, char** argv) {
    DMFLAGS_INIT(argc, argv);
    
    try {
        DatabaseHandler db;
        db.connect(FLAGS_USER, FLAGS_PASS, FLAGS_HOST, FLAGS_PORT);
        db.dropDatabase();
        db.initializeDatabase();
        
        db.insertData(10);
        db.updateRecord(5, "Updated_Record");
        db.updateRecord(6, "");  // Set to NULL
        db.displayRecords(4);
        
    } catch (const std::exception& e) {
        std::cerr << "Application Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
