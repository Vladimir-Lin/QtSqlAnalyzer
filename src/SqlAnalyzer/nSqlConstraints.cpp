#include <qtsqlanalyzer.h>

N::SqlConstraints:: SqlConstraints         (void)
                  : QList<SqlConstraint *> (    )
{
}

N::SqlConstraints::~SqlConstraints (void)
{
}

QStringList N::SqlConstraints::syntaxes(Sql & sql)
{
  QStringList s               ;
  for (int i=0;i<count();i++) {
    s << ME[i] -> syntax(sql) ;
  }                           ;
  return s                    ;
}
