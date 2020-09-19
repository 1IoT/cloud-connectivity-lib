#include <Arduino.h>
#include <Connectivity.h>


// Required for processing incoming messages
const int numChars = 1000; 
char receivedChars[numChars];
bool newDataAvailable = false;

/*********************************************************
 * Public
 ********************************************************/

Connectivity::Connectivity(Serial *serialToUse) {
    this.pSerial = serialToUse;
    this.pSerial.begin(115200);
}

void Connectivity::send_state_change_double(int itemId, double value, int statusCode) {
    send_to_cloud(get_change_double_state_message(itemId, value, statusCode));
}

void Connectivity::send_state_change_boolean(int itemId, boolean value, int statusCode) {
    send_to_cloud(get_change_boolean_state_message(itemId, value, statusCode));
}

void Connectivity::send_state_change_int(int itemId, int value, int statusCode) {
    send_to_cloud(get_change_int_state_message(itemId, value, statusCode));
}

void Connectivity::send_item_status(int itemId, int statusCode) {
    send_to_cloud(get_change_item_status_message(itemId, statusCode));
}

void Connectivity::send_device_status(int statusCode) {
    send_to_cloud(get_change_device_status_message(statusCode));
}

void Connectivity::send_item_log(int itemId, String level, String message) {
    send_to_cloud("N:LOG:" + String(itemId)  + ":" + level + ":" + message);
}

void Connectivity::send_device_log(String level, String message) {
    send_to_cloud("N:LOG:D:" + level + ":" + message);
}

/*********************************************************
 * Private
 ********************************************************/

void Connectivity::send_to_cloud(String message) {
    this.pSerial.println(message);
}

String Connectivity::get_change_double_state_message(int itemId, double value, int statusCode){
  return get_change_state_message(itemId, String(value), "DOUBLE", statusCode);
}

String Connectivity::get_change_boolean_state_message(int itemId, boolean value, int statusCode) {
  String valueStr = value ? "1" : "0";
  return get_change_state_message(itemId, valueStr, "BOOLEAN", statusCode);
}

String Connectivity::get_change_int_state_message(int itemId, int value, int statusCode) {
    return get_change_state_message(itemId, String(value), "INT", statusCode);
}

String Connectivity::get_change_state_message(int itemId, String value, String type, int statusCode) {
  return ("S:" + String(itemId) + ":" + String(statusCode) + ":" + type + ":" + value);
}

String Connectivity::get_change_item_status_message(int itemId, int statusCode) {
  return ("S:" + String(itemId) + ":" + String(statusCode));
}

String Connectivity::get_change_device_status_message(int statusCode) {
  return ("S:D:" + statusCode);
}

// Handle Incoming Messages

Message Connectivity::get_incoming_message() {
    Message item;
    readSerial();
    
    if(newDataAvailable){
        this.pSerial.println(receivedChars);
        //Serial.println(strcat("Received: ", receivedChars));
        item = handle_state_change(receivedChars);
        resetSerial();
        
        return item;
    }

    return {};
}

void Connectivity::resetSerial() {
    newDataAvailable = false;
}

void Connectivity::readSerial() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    while (this.pSerial.available() > 0 && newDataAvailable == false) {
        rc = this.pSerial.read();
        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        } else {
            // Terminate the string
            receivedChars[ndx] = '\0';
            ndx = 0;
            newDataAvailable = true;
        }
    }
}

int get_item_id(int i, char* token) {
    // TODO This might be a little bit dangerous, because in the case that token is not an int, 
    // it returns 0 (which is referencing the power item then) -> Have to look for mor robust str to int implementations (e.g. checking with #isHexadecimalDigit()
   
    if(i == 2) {
         return atoi(token);
    }
}

char* get_item_state_value(int i, char* token) {
    if(i == 5) {
        return token;
    }
}

Message Connectivity::handle_state_change(char* input) {
    char* token = strtok(input, ":");
    Message message;

    //Assert that message is of the form timestamp:S/N:(.*) where (.*) is of the form pin_number:state_value:type:value for incoming state changes
    for (int i = 0; i <= 5; i++) {
        if(token == NULL) {
            send_device_log(LOG_LEVEL_ERROR, "Device could not process the incoming message");
            message.messageContent = MESSAGE_CONTENT_ERROR;
            return message;
        }

        if(i == 1) {
            if (strcmp(token, "N") == 0) {
                message.messageContent = MESSAGE_CONTENT_NOTIFICATION;
                return message;
            }

        }

        message.messageContent = MESSAGE_CONTENT_ITEM;
        message.itemId = get_item_id(i, token);
        message.value = get_item_state_value(i, token);
        
        token = strtok(NULL, ":");
    }

    if(token != NULL) {
        send_device_log(LOG_LEVEL_ERROR, "Device could not process the incoming message");
        message.messageContent = MESSAGE_CONTENT_ERROR;
        return message;
    }

    return message;
}

bool Connectivity::set_state_for_item(int itemNumber, char* value) {
  return true;
}

bool Connectivity::handle_notification() {
  //NOT IMPLEMENTED HERE
  //YOU SHOULD RETURN A PONG FOR A INCOMING PING HERE FOR EXAMPLE
  return false;    
}