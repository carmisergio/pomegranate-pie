
#pragma once

#include <string>
#include <cinttypes>
#include <iomanip>
#include <cstring>

struct NetFrame
{
    uint32_t length;
    std::string payload;

    // Constructor
    NetFrame(std::string &data)
    {
        this->payload = data;

        // Set length
        length = data.length();
    }
    NetFrame(){};

    // Resizes payload to length
    void auto_resize()
    {
        this->payload.resize(this->length);
    };

    // Get message as bytes
    std::string as_bytes()
    {
        std::string bytes;
        bytes.resize(sizeof(this->length) + payload.size());

        // Copy length
        std::memcpy((char *)bytes.data(), &length, sizeof(uint32_t));
        // Copy payload
        std::memcpy((char *)bytes.data() + sizeof(uint32_t), payload.data(), payload.size());

        return bytes;
    };

    friend std::ostream &operator<<(std::ostream &os, const NetFrame &frame)
    {
        os << "NetFrame[length: " << frame.length << ", payload: ";

        for (int i = 0; i < frame.length; i++)
        {
            os << std::hex << std::setfill('0') << std::setw(2) << (int)frame.payload[i];

            if (i != frame.length - 1)
                os << ", ";
        }

        os << "]";

        return os;
    }
};