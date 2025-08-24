#include <unistd.h>





#include "utils.hpp"




bool Utils::is_root()
{
    if(getuid() != 0)
        return false;
        
    return true;
}