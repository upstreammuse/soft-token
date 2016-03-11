#include "softtoken.h"

#include <QDateTime>
#include <QTimer>
#include "otp.h"
#include "ui_softtoken.h"

using namespace st;

// public

SoftToken::SoftToken(QWidget* parent) :
   QWidget(parent),
   ui(new Ui::SoftToken),
   m_otp(new Otp(QByteArray("testkey"), this)),
   m_timer(new QTimer(this))
{
   ui->setupUi(this);

   // generate intervals
   connect(m_timer, SIGNAL(timeout()), this, SLOT(generateInterval()));
   m_timer->start(100);

   // display OTPs
   connect(m_otp, SIGNAL(oneTimePassword(QString)),
           ui->otp, SLOT(display(QString)));
}

SoftToken::~SoftToken()
{
   delete ui;
}

// private slots

void SoftToken::generateInterval()
{
   qint64 msec(QDateTime::currentMSecsSinceEpoch());
   m_otp->setInterval(msec / 30000);
   ui->timeLeft->setValue(30000 - msec % 30000);
}

void SoftToken::setKey(const QString& key)
{
   m_otp->setKey(fromBase32(key + QString((8 - key.size() % 8) % 8, '=')));
}

// private

QByteArray SoftToken::fromBase32(const QString& str)
{
   const QString table("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=");

   QByteArray key;
   for (int i(0); i < str.size(); i += 8)
   {
      quint8 in[8];
      for (int j(0); j < 8; ++j)
      {
         in[j] = table.indexOf(str[i + j], 0, Qt::CaseInsensitive);
      }

      quint8 out[5];
      out[0] = in[0] << 3 | in[1] >> 2;
      out[1] = in[1] << 6 | in[2] << 1 | in[3] >> 4;
      out[2] = in[3] << 4 | in[4] >> 1;
      out[3] = in[4] << 7 | in[5] << 2 | in[6] >> 3;
      out[4] = in[6] << 5 | in[7];

      if (str[i + 2] == '=')
      {
         key.append(reinterpret_cast<char*>(out), 1);
      }
      else if (str[i + 4] == '=')
      {
         key.append(reinterpret_cast<char*>(out), 2);
      }
      else if (str[i + 5] == '=')
      {
         key.append(reinterpret_cast<char*>(out), 3);
      }
      else if (str[i + 7] == '=')
      {
         key.append(reinterpret_cast<char*>(out), 4);
      }
      else
      {
         key.append(reinterpret_cast<char*>(out), 5);
      }
   }

   return key;
}
