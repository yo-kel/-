
#ifndef MYSQLH
#define MYSQLH

#include <stdlib.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "global.h"
#include "data.h"

using namespace sql;

extern sql::Driver* driver;
extern sql::Connection* con;

const std::string mysqlAddress = "tcp://127.0.0.1:3306";
const std::string dataBase = "class_management_system";

int MysqlInit();

int FindStudentPwd(std::string sid,std::string &pwd);

int SaveClassroomSubject(std::string cid, std::string* arr ,int n);

int SaveSubjectStudent(std::string subjectId, std::string*arr, int n);

std::string StringConvert(std::string string);

#endif // !MYSQLH

