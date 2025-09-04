#include <filesystem>
#include <log_c/log.h>
#include <unistd.h>
#include <cstdlib>
#include <pwd.h>





#include "utils.hpp"




bool Utils::is_root()
{
    if(getuid() != 0)
        return false;
        
    return true;
}

std::string Utils::get_home_dir()
{
    //return std::getenv("HOME");
    const char* sudo_user = std::getenv("SUDO_USER");
        const char* home_env = std::getenv("HOME");
    
        if (sudo_user) {
            struct passwd* pw = getpwnam(sudo_user);
            if (pw) {
                return pw->pw_dir;  // real home directory of original user
            }
        }
    
        return home_env ? home_env : "/"; // fallback
}

void Utils::setup_dirs()
{
    std::ostringstream psensor_dir_path;
    
    psensor_dir_path << get_home_dir() << "/" << PSENSOR_DIR;
    
    try
    {
        if(std::filesystem::create_directory(psensor_dir_path.str()))
        {
            log_info("psensor directory created (%s)", psensor_dir_path.str().c_str());
        }
        else
        {
            log_warn("psensor directory already exists or could not be created");
        }
    }
    
    catch(const std::filesystem::filesystem_error& e)
    {
        log_error("Directory creating Error: %s", e.what());
    }
}