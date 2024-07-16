// json_utils.h
#ifndef JSON_UTILS_H
#define JSON_UTILS_H

char *prepare_json_data(float speed, double latitude, double longitude, float heading, float distance2lot);

char *prepare_sensor_data(float humid, float temperature);

#endif /* JSON_UTILS_H */