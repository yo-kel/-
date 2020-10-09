#include "data.h"

Data_login::Data_login(std::string sid, std::string pwd, std::string position) {
    this->sid = sid;
    this->pwd = pwd;
    this->position = position;
}
Data_login::Data_login() {
    sid = "";
    pwd = "";
    position = "";
}

std::string DataSerialize(Data &data) {
    //std::string res = "";
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << data;
    return archive_stream.str();
}

Data DataDeserialize(std::string string) {
    std::istringstream archive_stream(string);
    boost::archive::text_iarchive archive(archive_stream);
    Data data;
    archive >> data;
    return data;
}

template<typename T>
std::string DataArraySerialize(Data_Array<T> &data) {
    std::ostringstream archive_stream;
    boost::archive::text_oarchive archive(archive_stream);
    archive << data;
    std::string serial = archive_stream.str();
    return serial;
}

template std::string DataArraySerialize<int>(Data_Array<int>& data);
template std::string DataArraySerialize<std::string>(Data_Array<std::string>& data);
template std::string DataArraySerialize<LL>(Data_Array<LL>& data);

template<typename T>
int DataArrayDeserialize(std::string string, Data_Array<T>& data) {
    std::istringstream archive_stream(string);
    boost::archive::text_iarchive archive(archive_stream);
    archive >> data;
    return 0;
}


template int DataArrayDeserialize<int>(std::string string, Data_Array<int>& data);
template int DataArrayDeserialize<std::string>(std::string string, Data_Array<std::string>& data);
template int DataArrayDeserialize<LL>(std::string string, Data_Array<LL>& data);