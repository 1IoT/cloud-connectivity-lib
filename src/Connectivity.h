#include <Arduino.h>
#include <Message.h>

#ifndef _CONNECTIVITY_H_
#define _CONNECTIVITY_H_

// Defines the default status codes
#define STATUS_OK 20
#define STATUS_WARN 40
#define STATUS_ERROR 50

// Defines the levels of the logger
#define LOG_LEVEL_DEBUG "DEBUG"
#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_WARN "WARN"
#define LOG_LEVEL_ERROR "ERROR"

class Connectivity {
    public:
        Connectivity();

        void send_state_change_double(int itemId, double value, int statusCode);

        void send_state_change_boolean(int itemId, boolean value, int statusCode);

        void send_item_status(int itemId, int statusCode);

        void send_device_status(int statusCode);

        void send_item_log(int itemId, String level, String message);

        void send_device_log(String level, String message);

        /*********************************************************
         * Handle Incoming Messages
         ********************************************************/

        Message get_incoming_message();

        

    private:
        void send_to_cloud(String message);

        String get_change_double_state_message(int itemId, double value, int statusCode);

        String get_change_boolean_state_message(int itemId, boolean value, int statusCode);

        String get_change_state_message(int itemId, String value, String type, int statusCode);

        String get_change_item_status_message(int itemId, int statusCode);

        String get_change_device_status_message(int statusCode);

        /*********************************************************
         * Handle Incoming Messages
         ********************************************************/

        void readSerial();

        void resetSerial();

        Message handle_state_change(char* message);

        bool handle_notification();

        bool set_state_for_item(int itemNumber, char* value);
};

#endif