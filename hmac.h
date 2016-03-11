#ifndef HMAC_H
#define HMAC_H

#include <QCryptographicHash>

namespace st { class Hmac; }

class st::Hmac
{
public:
   enum
   {
      BLOCK_SIZE = 64,
      INNER_PADDING = 0x36,
      OUTER_PADDING = 0x5C
   };

public:
   Hmac(const QByteArray& key);
   ~Hmac();
   void addData(const QByteArray& data);
   void reset();
   QByteArray result();
   void setKey(const QByteArray& key);

private:
   QCryptographicHash m_hash;
   QByteArray m_key;
   QByteArray m_result;
};

#endif
