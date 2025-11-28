#ifndef MESSAGE_H
#define MESSAGE_H

enum MessageType { ERROR, TEMPERATURE, TEXT };

union MessageData {
    int errorCode;
    float temperature;
    char text[50];
};

struct Message {
    enum MessageType type;
    union MessageData data;
};

struct Message make_message(enum MessageType type, const union MessageData *data);
void print_message(struct Message message);

#endif 