#include "mysql.h"

sql::Driver* driver;
sql::Connection* con;

int FindStudentPwd(std::string sid,std::string &pwd) {
	try {
		std::string table = "student";

		sql::Statement* stmt;
		stmt = con->createStatement();

		sql::ResultSet* res;
		std::string statement = "SELECT pwd from " + table+" WHERE sid="+sid;
		std::cout << statement << std::endl;
		res = stmt->executeQuery(statement.c_str());
		delete stmt;
		while (res->next()) {
			pwd = res->getString("pwd").c_str();
			//std::cout << pwd << std::endl;
			return 0;
		}
		return -1;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

int SaveClassroomSubject(std::string cid, std::string* arr, int n) {
	std::string table = "classroom";
	for (int i = 0;i < n;i++) {
		InsertSubject(arr[i]);
	}

	Data_Array<std::string> data(arr, n);
	std::string serial = DataArraySerialize<std::string>(data);
	serial = StringConvert(serial);
	sql::Statement* stmt;
	stmt = con->createStatement();

	std::string statement = "UPDATE " + table + " SET subject=" + serial+" WHERE cid="+cid;
	std::cout << statement << std::endl;
	stmt->execute(statement.c_str());
	delete stmt;
	return 0;
}

int SaveSubjectStudent(std::string subjectId, std::string* arr, int n) {

	std::string table = "subject";


	Data_Array<std::string> data(arr, n);

	std::string serial = DataArraySerialize<std::string>(data);

	serial = StringConvert(serial);
	sql::Statement* stmt;
	stmt = con->createStatement();

	std::string statement = "UPDATE " + table + " SET student=" + serial + " WHERE subject_name=" + StringConvert(subjectId);
	std::cout << statement << std::endl;
	stmt->executeUpdate(statement.c_str());
	delete stmt;
	return 0;
}


std::string StringConvert(std::string string) {
	return "'" + string + "'";
}



int MysqlInit() {
	try {
		driver = get_driver_instance();
		//sql::SQLString sqlArress(mysqlAddress);
		std::cout << mysqlAddress << std::endl;
		//std::cout << sqlArress << std::endl;
		con = driver->connect(mysqlAddress.c_str(), "root", "123456");//应从配置文件中读取密码
		if (!con->isValid()) {
			std::cout << "failed to connect mysql" << std::endl;
			return -1;
		}
		std::cout << "connect to mysql" << std::endl;

		con->setSchema(dataBase.c_str());
		if (!con->isValid()) {
			std::cout << "failed to connect table" << std::endl;
			return -1;
		}
		return 1;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}

}

int QueryStudentId(std::string subjectId, std::string sid,int &studentId) {
	try {
		std::string table = "subject";

		sql::Statement* stmt;
		stmt = con->createStatement();

		sql::ResultSet* res;
		std::string statement = "SELECT student from " + table + " WHERE subject_name=" + StringConvert(subjectId);

		std::cout << statement << std::endl;
		res = stmt->executeQuery(statement.c_str());
		delete stmt;

		//puts("OK");
		while (res->next()) {
			std::string rawData = res->getString("student").c_str();
			Data_Array<std::string> data;
			DataArrayDeserialize<std::string>(rawData, data);

			int n = data.S.size();
			for (int i = 0;i < n;i++) {
				if (sid == data.S[i]) {
					studentId = i;
					return 0;
				}
			}
			return 0;
		}
		return -1;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

int QueryClassroomSubject(Data_Array<std::string>& subject) {
	std::string table = "classroom";
	sql::Statement* stmt;
	stmt = con->createStatement();

	sql::ResultSet* res;
	std::string statement = "SELECT subject from " + table;
	std::cout << statement << std::endl;
	res = stmt->executeQuery(statement.c_str());
	delete stmt;
	while (res->next()) {
		std::string rawData = res->getString("subject").c_str();
		//std::cout << rawData << std::endl;

		DataArrayDeserialize<std::string>(rawData,subject);
		return 0;
	}
	return -1;
}

int QuerySubjectStudent(std::string subjectId, Data_Array<std::string>& student) {
	std::string table = "subject";
	sql::Statement* stmt;
	stmt = con->createStatement();

	sql::ResultSet* res;
	std::string statement = "SELECT student from " + table+" WHERE subject_name="+StringConvert(subjectId);
	std::cout << statement << std::endl;
	res = stmt->executeQuery(statement.c_str());
	delete stmt;
	while (res->next()) {
		std::string rawData = res->getString("student").c_str();
		//std::cout << rawData << std::endl;

		DataArrayDeserialize<std::string>(rawData, student);
		return 0;
	}
	return -1;

}


template<typename T>
int QueryStudentCheckInOut(std::string classId,Data_Array<T> &checkInOut, std::string inOut) {
	try {
		std::string table = "class";

		sql::Statement* stmt;
		stmt = con->createStatement();

		sql::ResultSet* res;
		std::string statement = "SELECT " + inOut + " from " + table + " WHERE class_name=" + StringConvert(classId);
		std::cout << statement << std::endl;
		res = stmt->executeQuery(statement.c_str());
		delete stmt;
		//puts("finish");
		while (res->next()) {
			std::string rawData = res->getString(inOut.c_str()).c_str();
			//std::cout << rawData << std::endl;

			DataArrayDeserialize<T>(rawData, checkInOut);
			return 0;
		}
		return -1;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
template int QueryStudentCheckInOut<int>(std::string classId, Data_Array<int>& checkInOut, std::string inOut);
template int QueryStudentCheckInOut<LL>(std::string classId, Data_Array<LL>& checkInOut, std::string inOut);
template int QueryStudentCheckInOut<__int64>(std::string classId, Data_Array<__int64>& checkInOut, std::string inOut);

template<typename T>
int QueryStudentCheckInOut(std::string subjectId,std::string classId, std::string sid, T &checkInOut, std::string inOut) {
	int studentId;
	QueryStudentId(subjectId, sid, studentId);
	Data_Array<T> data;
	QueryStudentCheckInOut<T>(classId, data, inOut);
	if (data.S.size() <= studentId)return -1;
	checkInOut = data.S[studentId];
	return 0;
}
template int QueryStudentCheckInOut<LL>(std::string subjectId, std::string classId, std::string sid, LL& checkInOut, std::string inOut);
template int QueryStudentCheckInOut<int>(std::string subjectId, std::string classId, std::string sid, int& checkInOut, std::string inOut);

int UpdateStudentCheck(std::string subjectId, std::string classId, std::string sid, int status) {
	//负数状态表示由老师确定，不再在登出时自动修改
	std::string table = "class";
	int studentId;
	if (QueryStudentId(subjectId, sid, studentId) != 0)return -1;

	sql::Statement* stmt;
	stmt = con->createStatement();
	//std::cout << "asdas:" << studentId << std::endl;

	Data_Array<int>dataCheck;
	//puts("finish");
	QueryStudentCheckInOut<int>(classId, dataCheck, "chk");
	if (dataCheck.S.size() <= studentId)return -1;//建立课堂后不能再加新人
	dataCheck.S[studentId] = status;
	std::string rawData = StringConvert(DataArraySerialize<int>(dataCheck));

	std::string statement = "UPDATE " + table + " SET chk=" + rawData + " WHERE class_name=" +StringConvert( classId);
	std::cout << statement << std::endl;
	int res=stmt->executeUpdate(statement.c_str());
	delete stmt;
	if (res != 0)return -1;
	return 0;
}

int UpdateStudentCheckinOut(std::string subjectId,std::string classId, std::string sid, LL timestamp,std::string inOut) {
	int studentId;
	if (QueryStudentId(subjectId, sid, studentId) != 0)return -1;
	std::string table = "class";
	
	std::cout << "timestamp: " << timestamp << std::endl;
	sql::Statement* stmt;
	stmt = con->createStatement();

	Data_Array<LL>dataCheck;
	QueryStudentCheckInOut<LL>(classId, dataCheck, inOut);

	if (dataCheck.S.size() <= studentId)return -1;//建立课堂后不能再加新人
	dataCheck.S[studentId] = timestamp;
	std::string rawData = StringConvert(DataArraySerialize(dataCheck));
	std::string statement = "UPDATE " + table + " SET "+inOut+"="+ rawData + " WHERE class_name=" +StringConvert( classId);
	std::cout << statement << std::endl;
	int res = stmt->executeUpdate(statement.c_str());
	delete stmt;
	if (res != 0)return -1;
	return 0;
}

int QuerySubjectClass(std::string subjectId, Data_Array<std::string>& classes) {
	std::string table = "subject";
	sql::Statement* stmt;
	stmt = con->createStatement();

	sql::ResultSet* res;
	std::string statement = "SELECT class from " + table + " WHERE subject_name=" + StringConvert( subjectId);
	std::cout << statement << std::endl;
	res = stmt->executeQuery(statement.c_str());
	delete stmt;
	while (res->next()) {
		std::string rawData = res->getString("class").c_str();
		std::cout <<"data:"<< rawData << std::endl;
		if (rawData == "") {
			puts("no data");
			continue;
		}
		//puts("OK");
		DataArrayDeserialize<std::string>(rawData, classes);
		
		return 0;
	}
	return -1;
}

int UpdateSubjectClass(std::string subjectId, Data_Array<std::string>& classes) {
	std::string table = "subject";
	sql::Statement* stmt;
	stmt = con->createStatement();

	std::string rawData = StringConvert(DataArraySerialize<std::string>(classes));

	std::string statement = "UPDATE " + table + " SET class=" + rawData + " WHERE subject_name=" + StringConvert(subjectId);
	std::cout << statement << std::endl;
	int res = stmt->executeUpdate(statement.c_str());
	delete stmt;
	return 0;
}

int QueryStudent(std::string subjectId, std::string classId, std::string sid, Data_Student& data) {
	Data_Array<std::string>classes;
	QuerySubjectClass(subjectId, classes);
	for (int i = 0;i < classes.S.size();i++) {
		std::cout << classes.S[i] << std::endl;
	}
	int studentId;
	QueryStudentId(subjectId, sid, studentId);

	int n = classes.S.size();
	int attendCnt = 0;
	double totalScore=0.0;
	for (int i = 0;i < n;i++) {
		Data_Array<int>checkData;
		QueryStudentCheckInOut<int>(classes.S[i], checkData, "chk");
		if (checkData.S[studentId] == 1 || checkData.S[studentId] == -1)attendCnt++;
		data.check.push_back(checkData.S[studentId]);

		QueryStudentCheckInOut<int>(classes.S[i], checkData, "score");
		totalScore += checkData.S[studentId];

		QueryStudentCheckInOut<int>(classes.S[i], checkData, "work");
		data.work.push_back(checkData.S[studentId]);
	}
	data.attendence = 1.0 * attendCnt / n;
	return 0;
}

int InsertSubject(std::string subjectId) {
	std::string table = "subject";
	sql::Statement* stmt;
	stmt = con->createStatement();
	std::string statement = "INSERT INTO " + table + "(subject_name)" + " VALUES(" + StringConvert(subjectId) + ")";
	std::cout << statement << std::endl;
	int res = stmt->executeUpdate(statement.c_str());
	if (res != 0)return -1;
	return 0;
}

int InsertClass(std::string subjectId, std::string classId) {
	try
	{
		//还应更新subject中的class
		std::string table = "class";
		sql::Statement* stmt;
		stmt = con->createStatement();
		//sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO class(class_name) VALUES (?)");
		//sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO " + table + " (class_name)" + " VALUES (?)");
		//pstmt->setString(1, StringConvert(classId));
		//pstmt->executeUpdate();
		//delete pstmt;

		std::string statement = "INSERT INTO " + table + "(class_name)" + " VALUES(" + StringConvert(classId) + ")";
		std::cout << statement << std::endl;
		int res = stmt->executeUpdate(statement.c_str());
		//delete stmt;

		Data_Array<std::string>classes;
		QuerySubjectClass(subjectId, classes);
		classes.S.push_back(classId);
		UpdateSubjectClass(subjectId, classes);

		Data_Array<std::string>student;
		QuerySubjectStudent(subjectId, student);
		int n = student.S.size();


		Data_Array<LL>tmpLL;
		Data_Array<int>tmpInt;
		for (int i = 0;i < n;i++) {
			tmpLL.S.push_back(0);
			tmpInt.S.push_back(0);
		}

		stmt = con->createStatement();
		statement = "UPDATE " + table +
			" SET chk=" + StringConvert(DataArraySerialize<int>(tmpInt)) +
			", checkin=" + StringConvert(DataArraySerialize<LL>(tmpLL)) +
			", checkout=" + StringConvert(DataArraySerialize<LL>(tmpLL)) +
			", score=" + StringConvert(DataArraySerialize<int>(tmpInt)) +
			", work=" + StringConvert(DataArraySerialize<int>(tmpInt)) +
			" WHERE class_name=" + StringConvert(classId);

		//std::string statement = "UPDATE " + table + " SET chk=" + "'1'" + " WHERE class_name=" + StringConvert(classId);
		std::cout << statement << std::endl;
		 res = stmt->executeUpdate(statement.c_str());
		delete stmt;
		if (res != 0)return -1;
		return 0;
	}
	catch (sql::SQLException& e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}