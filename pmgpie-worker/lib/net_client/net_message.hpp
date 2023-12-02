/**
 * Network client Message object
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 02/12/2023
 */

#include <cstdint>
#include <vector>
#include <iostream>
#include <iomanip>

namespace net
{
    // Message types
    enum class MessageType : uint32_t
    {
        RegisterWorker,
        RegisterWorkerACK,
        DispatchWorkUnit,
        DispatchWorkUnitACK,
        DispatchWorkUnitNEG,
        SubmitWorkUnitResult,
        Goodbye,
    };

    // Message bodies
    struct DispatchWorkUnitBody
    {
        uint16_t thread;   // Thread to dispatch to
        uint64_t start;    // Start digit
        uint16_t n_digits; // Number of digits
    };

    struct DispatchWorkUnitNEGBody
    {
        std::string reason;
    };

    // Actual message object
    struct MessageHeader
    {
        MessageType type; // Message type
        uint32_t length;  // Length of message body
    };

    struct Message
    {
        MessageHeader header;
        std::vector<uint8_t> body;

        // Gets size of message body
        size_t size() const
        {
            return body.size();
        }

        template <typename DataType>
        friend Message &operator<<(Message &msg, const DataType &data)
        {
            // Check that the type of the data being pushed is trivially copyable
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

            // Cache current size of vector, as this will be the point we insert the data
            size_t i = msg.body.size();

            // Resize the vector by the size of the data being pushed
            msg.body.resize(msg.body.size() + sizeof(DataType));

            // Physically copy the data into the newly allocated vector space
            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

            // Recalculate the message size
            msg.header.length = msg.size();

            // Return the target message so it can be "chained"
            return msg;
        }

        template <typename DataType>
        friend Message &operator>>(Message &msg, DataType &data)
        {
            // Check that the type of the data being pushed is trivially copyable
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

            // Cache the location towards the end of the vector where the pulled data starts
            size_t i = msg.body.size() - sizeof(DataType);

            // Physically copy the data from the vector into the user variable
            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            // Shrink the vector to remove read bytes, and reset end position
            msg.body.resize(i);

            // Recalculate the message size
            msg.header.length = msg.size();

            // Return the target message so it can be "chained"
            return msg;
        }

        friend std::ostream &operator<<(std::ostream &os, const Message &msg)
        {
            os << "Type:" << int(msg.header.type) << " Length:" << msg.header.length << "\n";

            os << "Body: ";
            for (int i = 0; i < msg.body.size(); i++)
            {

                std::stringstream stream;
                stream << std::hex << std::setfill('0') << std::setw(2) << (int)msg.body[i];
                std::string result(stream.str());
                os << result << " " << std::dec;
            }

            os << std::endl;

            return os;
        }
    };
};
