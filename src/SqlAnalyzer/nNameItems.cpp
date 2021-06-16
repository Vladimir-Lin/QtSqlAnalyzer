#include <qtsqlanalyzer.h>

N::NameItems:: NameItems        (void)
             : QList<NameItem *>(    )
{
  clear ( ) ;
}

N::NameItems:: NameItems        (const NameItems & item)
             : QList<NameItem *>(                      )
{
  assign ( item ) ;
}

N::NameItems::~NameItems(void)
{
}

QStringList N::NameItems::StringList(void)
{
  QStringList S               ;
  for (int i=0;i<count();i++) {
    S << ME[i] -> String ( )  ;
  }                           ;
  return S                    ;
}

int N::NameItems::indexOf(SUID uuid,int language)
{
  for (int i=0;i<count();i++)               {
    if ( ( uuid     == ME[i] -> uuid    )  &&
         ( language == ME[i] -> language) ) {
      return i                              ;
    }                                       ;
  }                                         ;
  return -1                                 ;
}

int N::NameItems::indexOf(SUID uuid)
{
  for (int i=0;i<count();i++)     {
    if ( ( uuid == ME[i]->uuid) ) {
      return i                    ;
    }                             ;
  }                               ;
  return -1                       ;
}

N::NameItems & N::NameItems::assign (const NameItems & items)
{
  clear ( )                         ;
  for (int i=0;i<items.count();i++) {
    append ( items [ i ] )          ;
  }                                 ;
  return ME                         ;
}

N::NameItems & N::NameItems::operator = (const NameItems & items)
{
  return assign ( items ) ;
}

int N::NameItems::obtain(SqlConnection & SC)
{
  NameItem * NI = new NameItem ( )  ;
  while ( NI -> next ( SC ) )       {
    append(NI)                      ;
    NI = new NameItem ( )           ;
  }                                 ;
  delete NI                         ;
  return count ( )                  ;
}

int N::NameItems::query(SqlConnection & SC,QString Q)
{
  if (!SC.Query(Q)) return count() ;
  return obtain ( SC )             ;
}

int N::NameItems::where           (
      SqlConnection & SC          ,
      QString         Table       ,
      QString         WhereString ,
      QString         OrderString ,
      QString         LimitString )
{
  QString Q                 ;
  Q = SC . sql . SelectFrom (
        SC.Columns          (
          6                 ,
          "id"              ,
          "uuid"            ,
          "language"        ,
          "length"          ,
          "flags"           ,
          "name"          ) ,
        Table               ,
        WhereString         ,
        OrderString         ,
        LimitString       ) ;
  return query ( SC , Q )   ;
}

int N::NameItems::equal           (
      SqlConnection & SC          ,
      QString         Table       ,
      QString         Name        ,
      QString         OrderString ,
      QString         LimitString )
{
  QString Q                               ;
  Q = SC . sql . SelectFrom               (
        SC.Columns                        (
          6                               ,
          "id"                            ,
          "uuid"                          ,
          "language"                      ,
          "length"                        ,
          "flags"                         ,
          "name"                        ) ,
        Table                             ,
        SC.sql.Where(1,"name")            ,
        OrderString                       ,
        LimitString                     ) ;
  SC . Prepare ( Q                      ) ;
  SC . Bind    ( "name" , Name.toUtf8() ) ;
  if ( ! SC.Exec() ) return count ( )     ;
  return obtain ( SC )                    ;
}

int N::NameItems::like            (
      SqlConnection & SC          ,
      QString         Table       ,
      QString         Pattern     ,
      QString         OrderString ,
      QString         LimitString )
{
  QString Q                                  ;
  Q = SC . sql . SelectFrom                  (
        SC.Columns                           (
          6                                  ,
          "id"                               ,
          "uuid"                             ,
          "language"                         ,
          "length"                           ,
          "flags"                            ,
          "name"                           ) ,
        Table                                ,
        "where `name` like :NAME"            ,
        OrderString                          ,
        LimitString                        ) ;
  SC . Prepare ( Q                         ) ;
  SC . Bind    ( "name" , Pattern.toUtf8() ) ;
  if ( ! SC.Exec() ) return count ( )        ;
  return obtain ( SC )                       ;
}

int N::NameItems::regexp          (
      SqlConnection & SC          ,
      QString         Table       ,
      QString         RegExp      ,
      QString         OrderString ,
      QString         LimitString )
{
  QString Q                                 ;
  Q = SC . sql . SelectFrom                 (
        SC.Columns                          (
          6                                 ,
          "id"                              ,
          "uuid"                            ,
          "language"                        ,
          "length"                          ,
          "flags"                           ,
          "name"                          ) ,
        Table                               ,
        "where `name` regexp :NAME"         ,
        OrderString                         ,
        LimitString                       ) ;
  SC . Prepare ( Q                        ) ;
  SC . Bind    ( "name" , RegExp.toUtf8() ) ;
  if ( ! SC.Exec() ) return count ( )       ;
  return obtain ( SC )                      ;
}

int N::NameItems::fromUuid        (
      SqlConnection & SC          ,
      QString         Table       ,
      SUID            uuid        ,
      QString         OrderString ,
      QString         LimitString )
{
  return where                     (
           SC                      ,
           Table                   ,
           SC . WhereUuid ( uuid ) ,
           OrderString             ,
           LimitString           ) ;
}

int N::NameItems::fromUuid        (
      SqlConnection & SC          ,
      QString         Table       ,
      SUID            uuid        ,
      int             language    ,
      QString         OrderString ,
      QString         LimitString )
{
  QString W                                                   ;
  W = QString  ( "where `uuid` = %1 and `language` = %2"      )
      . arg    ( uuid                                         )
      . arg    ( language                                   ) ;
  return where ( SC , Table , W , OrderString , LimitString ) ;
}
