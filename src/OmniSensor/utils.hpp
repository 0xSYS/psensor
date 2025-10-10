#ifndef UTILS_HPP
#define UTILS_HPP


#include <string>
#include <vector>

#include <sfd.h>



#define PSENSOR_DIR ".config/psensor"

inline sfd_Options temp_fd;



class Utils {
public:
    static bool is_root();
    static std::string get_home_dir();
    static void setup_dirs();
    static std::vector<std::string> get_color_themes_files();
};

#endif