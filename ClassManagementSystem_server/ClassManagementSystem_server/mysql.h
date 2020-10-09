
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
//查询学生密码

int SaveClassroomSubject(std::string cid, std::string* arr ,int n);
//保存教室的科目有哪些

int QueryClassroomSubject(Data_Array<std::string>& subject);
//查询教室的科目有哪些

int SaveSubjectStudent(std::string subjectId, std::string*arr, int n);
//保存科目的学生有哪些

int QuerySubjectStudent(std::string subjectId, Data_Array<std::string>&student);

template<typename T>
int QueryStudentCheckInOut(std::string classId, Data_Array<T>& checkInOut, std::string inOut);
//更新学生的check（checkin,checkout）

int UpdateStudentCheckinOut(std::string subjectId, std::string classId, std::string sid, LL timestamp, std::string inOut);
//更新学生的check

int QueryStudentId(std::string subjectId, std::string sid, int& studentId);
//查询学生在科目的编号

template<typename T>
int QueryStudentCheckInOut(std::string classId, Data_Array<T>& checkInOut, std::string inOut);
//查询课堂的check

template<typename T>
int QueryStudentCheckInOut(std::string subjectId, std::string classId, std::string sid, T& checkInOut, std::string inOut);
//查询学生的check

int UpdateStudentCheck(std::string subjectId, std::string classId, std::string sid, int status);
//更新学生的check

int QuerySubjectClass(std::string subjectId, Data_Array<std::string>& classes);
//查询科目的课堂有哪些

int QuerySubjectStudent(std::string subjectId, Data_Array<std::string>& student);
//查询科目的学生有哪些



int UpdateSubjectClass(std::string subjectId, Data_Array<std::string>& classes);
//更新科目的课堂有哪些

int QueryStudent(std::string subjectId, std::string classId, std::string sid, Data_Student& data);
//查询学生各项课堂情况

int InsertSubject(std::string subjectId);
//添加新课程

int InsertClass(std::string subjectId, std::string classId);
//添加新课堂

std::string StringConvert(std::string string);

#endif // !MYSQLH

