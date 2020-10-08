#include "fileio.h"

int CreateDir(std::string path) {
    try {
        path = "." + path;
        boost::filesystem::path dir(path);
        if (!(boost::filesystem::exists(dir))) {
            std::cout << dir << " Doesn't Exists" << std::endl;

            if (boost::filesystem::create_directories(dir))std::cout << dir << " Successfully Created !" << std::endl;
            else return -1;
        }
        return 0;

    }
    catch (const  boost::filesystem::filesystem_error& e) {
        std::cout << e.what() << '\n';
    }
}

int WriteToFile(std::string dir, std::string fileName, std::string content) {
    if (CreateDir(dir) != 0)return -1;
    std::ofstream myfile;
    myfile.open(dir +"/"+ fileName,std::ios::out);
    if (myfile.is_open()) {
        myfile << content << std::endl;
        myfile.close();
        return 0;
    }
    else return -1;
}

int ReadFromFile(std::string dir, std::string fileName, std::string& content) {
    std::ifstream myfile;
    myfile.open(dir + "/" + fileName);
    if (myfile.is_open()) {
        while(getline(myfile, content));
        return 0;
    }
    else return -1;
}


std::string SplicePathString(int count,...) {
    va_list ap;
    va_start(ap, count);
    std::string res = "";
    for (int i = 0;i < count;i++) {
        res += "/" + va_arg(ap, std::string);
    }
    va_end(ap);
    return res;
}

void AllFileNames(std::string path, std::vector<std::string>& S) {
    try {
        boost::filesystem::path dir(path);
        for (auto i = boost::filesystem::directory_iterator(dir); i != boost::filesystem::directory_iterator(); i++) {
            S.push_back(i->path().filename().string());
            std::cout << i->path().filename().string() << std::endl;
        }

    }
    catch (const  boost::filesystem::filesystem_error& e) {
        std::cout << e.what() << '\n';
    }
}