#pragma once
#ifndef dataH
#define dataH

#include <boost/serialization/variant.hpp>
#include <boost/serialization/vector.hpp>
#include "global.h"

//�������� �������л��ͷ����л�

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

class Data_Ans {
public:
	static const int unchecked = -1;
	static const int correct = -2;
	static const int wrong = -3;
	std::string title;
	std::string type;//"choice" "blank" "short" "file"
	std::string content;
	int checked;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& title;
		ar& type;
		ar& content;
		ar& checked;
	}
};

class Data_Hmwk {
public:
	std::string title;
	std::string type;
	LL ddl;
	int score;
	std::string content;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& title;
		ar& type;
		ar& ddl;
		ar& score;
		ar& content;
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
	Data_Message(int status, int broadcast, std::string message,std::string name) :
		status(status), broadcast(broadcast), message(message),name(name) {}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		//ar& boost::serialization::base_object<Data>(*this);
		ar& status;//�����Ƿ������ʱ�Ự��Ŀǰһ������
		ar& broadcast;
		ar& message;
		ar& name;
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

typedef boost::variant<Data_login,Data_Student,Data_Question,Data_Message,Data_Ans,Data_Hmwk> DataT;
class Data {
public:
	static const int Login = 0;
	static const int Student = 1;
	static const int Question = 2;
	static const int Message = 3;
	static const int Ans = 4;
	static const int Hmwk = 5;
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
	Data_Array(){}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& S;
	}
};


//Data_Array Data

std::string DataSerialize(Data &data);

Data DataDeserialize(std::string string);

template<typename T>
std::string DataArraySerialize(Data_Array<T>& data);

template<typename T>
int DataArrayDeserialize(std::string string, Data_Array<T>& data);

#endif // !dataH
