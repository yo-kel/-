#pragma once
#ifndef dataH
#define dataH

#include <boost/serialization/variant.hpp>
#include "global.h"



#define bufferSize 5000


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




typedef boost::variant<Data_login> DataT;
class Data {
public:
	static const int Login = 0;
	DataT payload;
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& payload;
	}
};





std::string DataSerialize(Data data);

Data DataDeserialize(std::string string);
#endif // !dataH
