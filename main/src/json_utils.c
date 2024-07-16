#include "json_utils.h"
#include <string.h>
#include <stdio.h>
#include <cJSON.h>
#define MAX_JSON_LENGTH 256

// Function to prepare JSON data
char *prepare_json_data(float speed, double latitude, double longitude, float heading,float distance2lot)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "speed", speed);
    cJSON_AddNumberToObject(root, "lat", latitude);
    cJSON_AddNumberToObject(root, "long", longitude);
    cJSON_AddNumberToObject(root, "heading", heading);
    cJSON_AddNumberToObject(root, "Distance", distance2lot);
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    // Truncate JSON string if it exceeds MAX_JSON_LENGTH
    if (strlen(json_string) > MAX_JSON_LENGTH)
    {
        json_string[MAX_JSON_LENGTH - 1] = '\0'; // Null-terminate the string
    }

    // Create a new string with formatted values
    char formatted_json[MAX_JSON_LENGTH];
    snprintf(formatted_json, MAX_JSON_LENGTH, "{\"speed\":%f,\"Filter_lat\":%.6lf,\"Filter_long\":%.6lf,\"heading\":%.2f,\"distance\":%.6lf}", speed, latitude, longitude, heading,distance2lot);

    // Free the original JSON string
    free(json_string);

    // Allocate a new string and copy the formatted JSON data
    char *result = malloc(strlen(formatted_json) + 1);
    strcpy(result, formatted_json);

    return result;
}

char *prepare_sensor_data(float latitude, float longitude)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "lat", latitude);
    cJSON_AddNumberToObject(root, "long", longitude);
    char *json_string = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    // Truncate JSON string if it exceeds MAX_JSON_LENGTH
    if (strlen(json_string) > MAX_JSON_LENGTH)
    {
        json_string[MAX_JSON_LENGTH - 1] = '\0'; // Null-terminate the string
    }

    // Create a new string with formatted values
    char formatted_json[MAX_JSON_LENGTH];
    snprintf(formatted_json, MAX_JSON_LENGTH, "{\"GPS_lat\":%.6lf,\"GPS_long\":%.6lf}", latitude, longitude);

    // Free the original JSON string
    free(json_string);

    // Allocate a new string and copy the formatted JSON data
    char *result = malloc(strlen(formatted_json) + 1);
    strcpy(result, formatted_json);

    return result;
}