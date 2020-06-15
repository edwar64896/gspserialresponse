#include <gspserialresponse.h>

gspGrouped* gspSerialResponse::firstInstance=nullptr;

char * gspSerialResponse::getLastResponse() {
    return szResponse; 
}

void gspSerialResponse::update() {
    switch (operationmode) {
        case GSP_SR_MODE_CALLBACK:
            cbProcessor(szResponse);
        break;
        case GSP_SR_MODE_CALLBACK_NONSTD:
            cbProcessorNonstd(szResponse);
        break;        
        case GSP_SR_MODE_REPLACE:
            strcpy(*_replaceMe,szResponse);
        break;
    }    
}

char gspSerialResponse::getChar()// Get a character from the serial buffer
{
  while (!Serial.available()); // wait for data
  return ((char)Serial.read()); // Thanks Doug
}

void gspSerialResponse::getChars(uint8_t nChars) {
  uint8_t i=0;
  for (i = 0; i < nChars;  i++) {
    szResponse[i] = getChar();
  }
  szResponse[i]=0;
}

bool gspSerialResponse::check() {

    if (parseWin){
        getChars(iCharsToGrab);
        update();
        reset();   
        return false;     
    }


    // take a peek at the incoming source.
    char incoming=Serial.peek();

    // anything there?
    if (incoming==-1) {
        return false; // force a reset of the check sequence.
    }

    // start matching the buffer
    if (incoming != (*(szHeader+(cursor++))) ) {
        cursor=0;
    }

    // if we get to here we have matched the buffer so far.
    // we need to do this twice to win the prize.

    // if we are the last instance in the list, then we need to "read" one byte from the serial input as we are done peeking
    if (getNextInstance()==nullptr)
        Serial.read();

    //only one instance should get this far.
    //TODO: work out how to do a global reset 
    if (cursor==2) {
        parseWin=1;
        return true;
    }

    return true; // continue.
}

// reset the cursor and the parse status.
// then we can have another go next time around.
void gspSerialResponse::reset() {
    cursor=0;
    parseWin=0;
}

//gspSerialResponse::gspSerialResponse(const char* _szHeader, uint8_t _iCharsToGrab,void (* _cbProcessor)(char *))
//:gspGrouped() {
//    szHeader=_szHeader;
//    iCharsToGrab=_iCharsToGrab;
//    cbProcessor=_cbProcessor;
//    cursor=0;
//    operationmode=GSP_SR_MODE_CALLBACK;
//    szResponse[0]=0;
//}

gspSerialResponse::gspSerialResponse(const char* _szHeader, uint8_t _iCharsToGrab,nonstd::function<void (char *)> _callback)
:gspGrouped() {
    szHeader=_szHeader;
    iCharsToGrab=_iCharsToGrab;
    cbProcessorNonstd=_callback;
    cursor=0;
    operationmode=GSP_SR_MODE_CALLBACK_NONSTD;
    szResponse[0]=0;
}

gspSerialResponse::gspSerialResponse(const char* _szHeader, uint8_t _iCharsToGrab,char ** replaceMe)
:gspGrouped(),_replaceMe(replaceMe) {
    szHeader=_szHeader;
    iCharsToGrab=_iCharsToGrab;
    cursor=0;
    operationmode=GSP_SR_MODE_REPLACE;
    szResponse[0]=0;
}

gspSerialResponse::gspSerialResponse():gspGrouped() {}
gspSerialResponse::~gspSerialResponse(){}
