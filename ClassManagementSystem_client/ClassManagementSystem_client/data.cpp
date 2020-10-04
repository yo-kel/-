#include "data.h"

Data_login::Data_login(std::string sid, std::string pwd, std::string position) {
    this->type = Data::Login;
    this->sid = sid;
    this->pwd = pwd;
    this->position = position;
}

template <typename T>

void DataSerialize(T data,std::string &res) {
    //std::string res = "";
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << data;
    res = archive_stream.str();
}

template <typename T>
void DataDeserialize(std::string string, T& data) {
    std::istringstream archive_stream(string);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> data;
}