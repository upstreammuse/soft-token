#include <QApplication>
#include "softtoken.h"

int main(int argc, char* argv[])
{
   QApplication app(argc, argv);
   st::SoftToken st;
   st.show();

   return app.exec();
}
