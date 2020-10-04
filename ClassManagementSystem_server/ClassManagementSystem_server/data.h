#pragma once
#ifndef DATAH
#define DATAH

#include "global.h"


#define bufferSize 5000

class Data {
public:
	static const int Login = 1;

	int type;
	char buf[bufferSize];
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& type;
		ar& buf;
	}
};


class Data_login :public Data {
public:
	std::string sid;
	std::string pwd;
	std::string position;
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


#endif // !data
