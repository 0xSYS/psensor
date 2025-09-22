#ifndef SENSOR_PROPERTIES_HPP
#define SENSOR_PROPERTIES_HPP

#include "sensor_list.hpp"


#define SENSOR_PROPERTIES_PATH "/.config/psensor/sensor_properties.json"


void saveSensorProperties(std::vector<ui_sensor>& sens_props);
std::vector<ui_sensor> loadSensorProperties();
#endif