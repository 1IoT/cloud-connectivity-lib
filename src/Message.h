// Defines the content of the message
#define MESSAGE_CONTENT_ITEM 1
#define MESSAGE_CONTENT_NOTIFICATION 2
#define MESSAGE_CONTENT_ERROR -1

class Message {
    public:
        int itemId;
        char* value;
        int messageContent;
};