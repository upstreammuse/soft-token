#ifndef SOFTTOKEN_H
#define SOFTTOKEN_H

#include <QWidget>

namespace st { class Otp; }
namespace st { class SoftToken; }
namespace Ui { class SoftToken; }

class st::SoftToken : public QWidget
{
   Q_OBJECT

public:
   explicit SoftToken(QWidget* parent = 0);
   ~SoftToken();

private slots:
   void generateInterval();
   void setKey(const QString& key);

private:
   QByteArray fromBase32(const QString& str);

private:
   Ui::SoftToken* ui;
   Otp* m_otp;
   QTimer* m_timer;
};

#endif
