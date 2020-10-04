#pragma once
#ifndef dataH
#define dataH

#include "global.h"


#define bufferSize 5000

class DataBytes {
public:
	int type;
	std::string buf;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& type;
		ar& buf;
	}
	Data BytesToData() {
		switch (type) {
		case Data::Login:

		}
	}
};

class Data {
public:
	static const int Login = 1;
	int type;
	DataBytes DataToBytes() {

	}
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version){
		ar& type;
	}
};


class Data_login :public Data {
public:
	std::string sid;
	std::string pwd;
	std::string position;
	Data_login(std::string sid, std::string pwd, std::string position);
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& boost::serialization::base_object<Data>(*this);
		ar& sid;
		ar& pwd;
		ar& position;
	}
};



template <typename T>
void DataSerialize(T data, std::string& res);

template <typename T>
void DataDeserialize(std::string string, T& data);
#endif // !dataH
