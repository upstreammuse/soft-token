#include "hmac.h"

using namespace st;

// public

Hmac::Hmac(const QByteArray& key) :
   m_hash(QCryptographicHash::Sha1)
{
   // setKey implies a reset
   setKey(key);
}

Hmac::~Hmac()
{
   // don't leave the key lying around in memory
   m_key.fill(0);
}

void Hmac::addData(const QByteArray& data)
{
   // continue the inner hash
   m_hash.addData(data);

   // invalidate the saved result
   m_result.clear();
}

void Hmac::reset()
{
   // start the inner hash
   QByteArray innerKey;
   foreach (quint8 c, m_key)
   {
      innerKey.append(c ^ INNER_PADDING);
   }
   m_hash.reset();
   m_hash.addData(innerKey);

   // invalidate the saved results
   m_result.clear();
}

QByteArray Hmac::result()
{
   // if there is a cached result, use it
   if (m_result.size() != 0)
   {
      return m_result;
   }

   // finish the inner hash
   QByteArray innerHash(m_hash.result());

   // do the outer hash
   QByteArray outerKey;
   foreach (quint8 c, m_key)
   {
      outerKey.append(c ^ OUTER_PADDING);
   }
   QCryptographicHash hash(QCryptographicHash::Sha1);
   hash.addData(outerKey);
   hash.addData(innerHash);

   // save the result
   m_result = hash.result();

   return m_result;
}

void Hmac::setKey(const QByteArray& key)
{
   // reduce key if needed
   QByteArray fixedKey(BLOCK_SIZE, 0);
   if (key.size() > BLOCK_SIZE)
   {
      QCryptographicHash hash(QCryptographicHash::Sha1);
      hash.addData(key);
      fixedKey = hash.result();
   }
   else
   {
      for (int i(0); i < key.size(); ++i)
      {
         fixedKey[i] = key[i];
      }
   }
   m_key = fixedKey;

   // use the reset method to prep the hash
   reset();
}
