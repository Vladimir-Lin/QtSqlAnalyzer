#include <qtsqlanalyzer.h>

N::SqlConditions:: SqlConditions         (void)
                 : QList<SqlCondition *> (    )
{
}

N::SqlConditions::~SqlConditions (void)
{
}

int N::SqlConditions::add(QString field,QString symbol)
{
  SqlCondition * sc          ;
  sc  = new SqlCondition ( ) ;
  sc -> field  = field       ;
  sc -> symbol = symbol      ;
  sc -> relate = 0           ;
  ME << sc                   ;
  return count ( )           ;
}

int N::SqlConditions::add(QString field,QString symbol,const char * value)
{
  SqlCondition * sc                       ;
  sc  = new SqlCondition ( )              ;
  sc -> field  = field                    ;
  sc -> symbol = symbol                   ;
  sc -> relate = 1                        ;
  sc -> value << QString::fromUtf8(value) ;
  ME << sc                                ;
  return count ( )                        ;
}

int N::SqlConditions::add(QString field,QString symbol,VarArgs & value)
{
  SqlCondition * sc          ;
  sc  = new SqlCondition ( ) ;
  sc -> field  = field       ;
  sc -> symbol = symbol      ;
  sc -> relate = 2           ;
  sc -> value  = value       ;
  ME << sc                   ;
  return count ( )           ;
}

QStringList N::SqlConditions::syntaxes(Sql & sql)
{
  QStringList s                  ;
  for (int i=0;i<count();i++)    {
    s << ME [ i ] -> syntax(sql) ;
  }                              ;
  return s                       ;
}
