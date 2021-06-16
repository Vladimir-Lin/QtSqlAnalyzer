#include <qtsqlanalyzer.h>

N::SqlConstraint:: SqlConstraint (void)
{
}

N::SqlConstraint:: SqlConstraint (const SqlConstraint & constraint)
{
  ME = constraint ;
}

N::SqlConstraint::~SqlConstraint (void)
{
}

N::SqlConstraint & N::SqlConstraint::operator = (const SqlConstraint & constraint)
{
  nMemberCopy ( constraint , descriptions ) ;
  return ME                                 ;
}

QString N::SqlConstraint::syntax(Sql & sql)
{
  return "" ;
}
