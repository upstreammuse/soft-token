#include "otp.h"

#include <QtEndian>

using namespace st;

// public

Otp::Otp(const QByteArray& key, QObject* parent) :
   QObject(parent),
   m_hmac(key)
{
}

// public slots

void Otp::setInterval(quint64 interval)
{
   // interval converts to a big-endian byte array
   QByteArray otpTimeBlock(8, 0);
   qToBigEndian(interval, reinterpret_cast<uchar*>(otpTimeBlock.data()));

   // hash it
   m_hmac.reset();
   m_hmac.addData(otpTimeBlock);
   QByteArray hash(m_hmac.result());

   // find offset from last nibble of hash
   quint8 offset(hash.right(1)[0] & 0xF);

   // OTP comes out of big-endian hash array
   quint32 otpHash(qFromBigEndian<quint32>(reinterpret_cast<const uchar*>(
                                              hash.constData()) + offset));

   // remove sign and modulo for fixed-digit OTP
   otpHash &= 0x7FFFFFFF;
   otpHash %= 1000000;
   emit oneTimePassword(QString("%1").arg(otpHash, 6, 10, QChar('0')));
}

void Otp::setKey(const QByteArray& key)
{
   m_hmac.setKey(key);
}
