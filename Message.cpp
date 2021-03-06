#include "Message.h"

Message::Message()
  : mRaw(HeaderLength, '\0'), mBody(1, '\0')
{
}

Message::Message(const std::string& str)
  : mRaw(HeaderLength, '\0'), mBody(str.length() + 1, '\0')
{
  // trim to max size if necessary
  std::string out(str);
  if (out.length() > MaxLength)
    {
      mBody.resize(MaxLength + 1);
      out = out.substr(0, MaxLength);
    }
  
  memcpy(&mBody[0], (char*)out.c_str(), out.length());
}

Message::~Message()
{
}

char* Message::raw()
{
  return &mRaw[0];
}

int Message::rawLength()
{
  return mRaw.size();
}

char* Message::rawBody()
{
  if (mRaw.size() > HeaderLength)
    return &mRaw[HeaderLength];
  return NULL;
}

int Message::rawBodyLength()
{
  return mRaw.size() - HeaderLength;
}

char* Message::body()
{
  if (mBody.size() > 0)
    return &mBody[0];
  return NULL;
}

int Message::length()
{
  return mBody.size();
}

bool Message::encode()
{
  using namespace std;
  
  // maybe eventually base64 encode
  char* encoded = &mBody[0];
  int len = mBody.size();

  // resize the raw string appropriately
  mRaw.resize(HeaderLength + len);

  // encodes the header
  char header[HeaderLength + 1] = "";
  sprintf(header, "%4d", len);
  memcpy(&mRaw[0], header, HeaderLength);

  // copies the encoded message
  if (len > 0)
    {
    memcpy(&mRaw[HeaderLength], encoded, len);
    return true;
    }
  
  return false;
}

bool Message::decode()
{
  using namespace std;

  // maybe eventually base64 encode
  char* decoded = &mRaw[HeaderLength];
  int length = mRaw.size() - HeaderLength;

  // resizes the body to the appropriate size
  mBody.resize(length);

  // copies the data to the body
  if (length > 0)
    {
    memcpy(&mBody[0], decoded, length);
    return true;
    }

printf("TEST FAIL\n");
  return false;
}

bool Message::decodeHeader()
{
  // extracts the header
  char header[HeaderLength + 1] = "";
  strncat(header, &mRaw[0], HeaderLength);

  // resizes the raw buffer to the specified size
  mRaw.resize(atoi(header) + HeaderLength);

  return true;
}
