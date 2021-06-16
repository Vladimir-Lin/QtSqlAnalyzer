#include <qtsqlanalyzer.h>

N::NameMapItems:: NameMapItems         (void)
                : QList<NameMapItem *> (    )
{
  clear ( ) ;
}

N::NameMapItems:: NameMapItems         (const NameMapItems & items)
                : QList<NameMapItem *> (                          )
{
  assign ( items ) ;
}

N::NameMapItems::~NameMapItems(void)
{
}

N::NameMapItems & N::NameMapItems::assign(const NameMapItems & items)
{
  clear ( )                         ;
  for (int i=0;i<items.count();i++) {
    append ( items [ i ] )          ;
  }                                 ;
  return ME                         ;
}

N::NameMapItems & N::NameMapItems::operator = (const NameMapItems & items)
{
  return assign ( items ) ;
}

bool N::NameMapItems::fromOwner(SUID owner,UUIDs & Uuids)
{
  for (int i=0;i<count();i++)   {
    if ( owner == ME[i]->uuid ) {
      if (ME[i]->name>0)        {
        Uuids << ME[i]->name    ;
      }                         ;
    }                           ;
  }                             ;
  return ( Uuids.count() > 0 )  ;
}

int N::NameMapItems::assignPrefer(int startFrom)
{
  for (int i=0;i<count();i++)  {
    ME[i]->prefer = startFrom  ;
    startFrom++                ;
  }                            ;
  return startFrom             ;
}

void N::NameMapItems::assignUsage(int usage)
{
  for (int i=0;i<count();i++) {
    ME[i]->usage = usage      ;
  }                           ;
}

int N::NameMapItems::obtain(SqlConnection & SC)
{
  NameMapItem * NMI = new NameMapItem ( ) ;
  while ( NMI->next(SC) )                 {
    if (!contains(NMI))                   {
      append(NMI)                         ;
      NMI = new NameMapItem ( )           ;
    }                                     ;
  }                                       ;
  delete NMI                              ;
  return count ( )                        ;
}

int N::NameMapItems::query(SqlConnection & SC,QString Q)
{
  if (!SC.Query(Q)) return count() ;
  return obtain ( SC )             ;
}

int N::NameMapItems::where        (
      SqlConnection & SC          ,
      QString         Table       ,
      QString         WhereString ,
      QString         OrderString ,
      QString         LimitString )
{
  QString Q                 ;
  Q = SC . sql . SelectFrom (
        SC.Columns          (
          5                 ,
          "id"              ,
          "uuid"            ,
          "name"            ,
          "prefer"          ,
          "usage"         ) ,
        Table               ,
        WhereString         ,
        OrderString         ,
        LimitString       ) ;
  return query ( SC , Q )   ;
}

int N::NameMapItems::where    (
      SqlConnection & SC      ,
      QString         Table   ,
      SUID            uuid    ,
      QString         column  ,
      Qt::SortOrder   sorting )
{
  return where                                     (
           SC                                      ,
           Table                                   ,
           SC . WhereUuid ( uuid                 ) ,
           SC . OrderBy   ( column , sorting ) ) ;
}

bool N::NameMapItems::remove (
       SqlConnection & SC    ,
       QString         Table )
{
  if (count()<=0) return false                        ;
  QString Q                                           ;
  QString W                                           ;
  for (int i=0;i<count();i++)                         {
    W = QString ( "where `uuid` = %1 and `name` = %2" )
        . arg   ( ME[i]->uuid                         )
        . arg   ( ME[i]->name                       ) ;
    Q = SC.sql.DeleteFrom(Table,W)                    ;
    SC . Query ( Q )                                  ;
  }                                                   ;
  return true                                         ;
}

bool N::NameMapItems::store(SqlConnection & SC,QString Table)
{
  if (count()<=0) return false                ;
  QString Q                                   ;
  Q = SC . sql . InsertInto                   (
        Table                                 ,
        4                                     ,
        "uuid"                                ,
        "name"                                ,
        "prefer"                              ,
        "usage"                             ) ;
  for (int i=0;i<count();i++)                 {
    SC . Prepare ( Q                        ) ;
    SC . Bind    ( "uuid"   , ME[i]->uuid   ) ;
    SC . Bind    ( "name"   , ME[i]->name   ) ;
    SC . Bind    ( "prefer" , ME[i]->prefer ) ;
    SC . Bind    ( "usage"  , ME[i]->usage  ) ;
    SC . Exec    (                          ) ;
  }                                           ;
  return true                                 ;
}

static bool NameMapPreferLessThan(const N::NameMapItem * s1,const N::NameMapItem * s2)
{
  return s1->prefer < s2->prefer ;
}

void N::NameMapItems::sort(void)
{
  qSort ( begin() , end () , NameMapPreferLessThan ) ;
}
