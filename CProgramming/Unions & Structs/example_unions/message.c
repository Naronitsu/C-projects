#include "message.h"
#include <string.h>
#include <stdio.h>

struct Message make_message(enum MessageType type, const union MessageData *data) {
    struct Message m;
    m.type = type;
    switch (type) {
    case ERROR:
        m.data.errorCode = data->errorCode;
        break;
    case TEMPERATURE:
        m.data.temperature = data->temperature;
        break;
    case TEXT:
        strncpy(m.data.text, data->text, sizeof m.data.text - 1);
        m.data.text[sizeof m.data.text - 1] = '\0';
        break;
    }
    return m;
}

void print_message(struct Message message) {
    switch (message.type) {
    case ERROR:
        printf("[ERROR CODE] %d\n", message.data.errorCode);
        break;
    case TEMPERATURE:
        printf("[TEMPERATURE] %f\n", message.data.temperature);
        break;
    case TEXT:
        printf("[TEXT MESSAGE] %s\n", message.data.text);
        break;
    }
}