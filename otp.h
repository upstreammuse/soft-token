#ifndef OTP_H
#define OTP_H

#include <QObject>
#include "hmac.h"

namespace st { class Otp; }

class st::Otp : public QObject
{
   Q_OBJECT

public:
   explicit Otp(const QByteArray& key, QObject* parent = 0);

signals:
   void oneTimePassword(const QString& otp);

public slots:
   void setInterval(quint64 interval);
   void setKey(const QByteArray& key);

private:
   Hmac m_hmac;
};

#endif
