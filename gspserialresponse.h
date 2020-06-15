#ifndef GSP_SERIAL_RESPONSE
#define GSP_SERIAL_RESPONSE

#include "Arduino.h"
#include "gspgrouped.h"

#define GSP_SR_MODE_REPLACE 1
#define GSP_SR_MODE_CALLBACK 2

class gspSerialResponse: public gspGrouped {

    public:

        static gspSerialResponse * makeOne(
                            const char* _szInput /*Parser input - this is the string we look for*/, 
                            uint8_t _nChars /*number of characters to pull from the serial stream after the parser input has passed */,
                            void (* _callback)(char *) /*callback to invoke upon successful parse*/
        ) {
            gspSerialResponse * instance = new gspSerialResponse(_szInput,_nChars,_callback);
            gspGrouped::register_instance(instance);
        return instance;
    }

        gspSerialResponse(  
            const char* szInput /*Parser input - this is the string we look for*/, 
            uint8_t nChars /*number of characters to pull from the serial stream after the parser input has passed */,
            void (* callback)(char *) /*callback to invoke upon successful parse*/
        );

        gspSerialResponse(  
            const char* szInput /*Parser input - this is the string we look for*/, 
            uint8_t nChars /*number of characters to pull from the serial stream after the parser input has passed */,
            char** /*reference to fill with result*/
        );

        gspSerialResponse();
        ~gspSerialResponse();

        bool check();
        void reset();
        char * getLastResponse();

        void update();

        static void checkAll() {
            gspGrouped::checkAll(gspSerialResponse::firstInstance);
        }

    protected:
        void setFirstInstance(gspGrouped * fi) {gspSerialResponse::firstInstance = fi;}
        gspGrouped * getFirstInstance() {return gspSerialResponse::firstInstance;}
        static gspGrouped * firstInstance;

    private:

        int operationmode=0;

        void (*cbProcessor)(char *) = nullptr;
        const char* szHeader=nullptr;
        char* szResponse=new char[20];
        uint8_t iCharsToGrab=0;
        uint8_t cursor=0;
        void getChars(uint8_t nChars);
        char * _lastResponse;
        char getChar();
        int parseWin=0;

        char ** _replaceMe;

};

#endif
