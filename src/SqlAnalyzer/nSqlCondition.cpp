#include <qtsqlanalyzer.h>

N::SqlCondition:: SqlCondition (void)
{
  field  = "" ;
  symbol = "" ;
  relate = 0  ;
}

N::SqlCondition:: SqlCondition (QString f)
                : field        (        f)
                , symbol       (""       )
                , relate       (0        )
{
}

N::SqlCondition:: SqlCondition (QString f,QString s,VarArgs & v)
                : field        (        f                      )
                , symbol       (                  s            )
                , relate       (2                              )
                , value        (                              v)
{
}

N::SqlCondition::SqlCondition (const SqlCondition & condition)
{
  ME = condition ;
}

N::SqlCondition::~SqlCondition (void)
{
}

N::SqlCondition & N::SqlCondition::operator = (const SqlCondition & condition)
{
  nMemberCopy ( condition , field  ) ;
  nMemberCopy ( condition , symbol ) ;
  nMemberCopy ( condition , relate ) ;
  nMemberCopy ( condition , value  ) ;
  return ME                          ;
}

QString N::SqlCondition::syntax(Sql & sql)
{
  QStringList s                  ;
  s << field                     ;
  s << symbol                    ;
  switch (relate)                {
    case 0                       :
    case 1                       :
      if (value.count()>0)       {
        s << value[0].toString() ;
      }                          ;
    break                        ;
    case 2                       :
    break                        ;
  }                              ;
  return s.join(' ')             ;
}
