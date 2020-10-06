#pragma once
#ifndef dataH
#define dataH

#include <boost/serialization/variant.hpp>
#include <boost/serialization/vector.hpp>
#include "global.h"

//数据类型 数据序列化和反序列化

#define bufferSize 5000

class Data_Student {
public:
	std::vector<int>check;
	double attendence;
	double score;
	std::vector<int>work;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& attendence;
		ar& score;
		ar& work;
	}
};

class Data_login {
public:
	std::string sid;
	std::string pwd;
	std::string position;
	Data_login(std::string sid, std::string pwd, std::string position);
	Data_login();
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& sid;
		ar& pwd;
		ar& position;
	}
};

class Data_Message {
public:
	int status;
	int broadcast;
	std::string message;
	std::string name;
	Data_Message() {
		status = 1;
		broadcast = 0;
		message = "";
	}
	Data_Message(int status, int broadcast, std::string message, std::string name) :
		status(status), broadcast(broadcast), message(message), name(name) {}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& status;//用于是否接受临时会话，目前一定接受
		ar& broadcast;
		ar& message;
	}
};

class Data_Question {
public:
	std::string sid;
	std::string name;
	std::string position;
	std::string content;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& sid;
		ar& name;
		ar& position;
		ar& content;
	}
};

typedef boost::variant<Data_login, Data_Student, Data_Question, Data_Message> DataT;
class Data {
public:
	static const int Login = 0;
	static const int Student = 1;
	static const int Question = 2;
	static const int Message = 3;
	DataT payload;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& payload;
	}
};



template <class T>
class Data_Array {
public:
	std::vector<T>S;
	Data_Array(T* arr, int n) {
		for (int i = 0;i < n;i++) {
			S.push_back(arr[i]);
		}
	}
	Data_Array() {}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& S;
	}
};


//Data_Array Data

std::string DataSerialize(Data& data);

Data DataDeserialize(std::string string);

template<typename T>
std::string DataArraySerialize(Data_Array<T>& data);

template<typename T>
int DataArrayDeserialize(std::string string, Data_Array<T>& data);

#endif // !dataH
