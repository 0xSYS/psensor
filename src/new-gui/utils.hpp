#ifndef UTILS_HPP
#define UTILS_HPP


#include <string>



#define PSENSOR_DIR ".config/psensor"



class Utils {
public:
    static bool is_root();
    static std::string get_home_dir();
    static void setup_dirs();
};

#endif