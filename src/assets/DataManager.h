#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

//#include <stdint.h>

namespace xen
{
    struct DataBuffer {
        uint8_t* buffer;
        uint64_t length;
        uint64_t r_length;
        bool disposable;
    };

    class DataManager {
        public:
            DataManager();
            ~DataManager();
            static DataManager* GetInstance();

            static DataBuffer ReadAllData(const std::string filename);
            std::string ReadAllText(const std::string filename);

        private:
            static DataManager* instance;
    };
}

#endif //DATA_MANAGER_H