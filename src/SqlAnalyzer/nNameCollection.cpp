#include <qtsqlanalyzer.h>

N::NameCollection:: NameCollection(void)
{
}

N::NameCollection:: NameCollection(const NameCollection & item)
{
  Names = item . Names ;
  Items = item . Items ;
  Maps  = item . Maps  ;
}

N::NameCollection::~NameCollection(void)
{
}

void N::NameCollection::clear(void)
{
  Names . clear ( ) ;
  Items . clear ( ) ;
  Maps  . clear ( ) ;
}

N::NameCollection & N::NameCollection::operator = (const NameCollection & item)
{
  Names = item . Names ;
  Items = item . Items ;
  Maps  = item . Maps  ;
  return ME            ;
}

bool N::NameCollection::OwnerNames(SUID owner,UUIDs & Uuids,NAMEs & Names)
{
  UUIDs U                                  ;
  SUID  u                                  ;
  //////////////////////////////////////////
  if (owner<=0)                            {
    for (int i=0;i<Maps.count();i++)       {
      U << Maps[i]->name                   ;
    }                                      ;
  } else                                   {
    Maps . fromOwner ( owner , U )         ;
  }                                        ;
  //////////////////////////////////////////
  foreach (u,U)                            {
    int index                              ;
    index = Items.indexOf(u)               ;
    if (index>=0)                          {
      Names [ u ] = Items[index]->String() ;
      if (!Uuids.contains(u))              {
        Uuids << u                         ;
      }                                    ;
    }                                      ;
  }                                        ;
  //////////////////////////////////////////
  return ( Uuids.count() > 0 )             ;
}

bool N::NameCollection::FetchNames(SqlConnection & SC,QString table,SUID uuid,int language)
{
  return ( language <= 0 )                                                       ?
         Names . fromUuid ( SC , table , uuid ,            SC.OrderByAsc("id") ) :
         Names . fromUuid ( SC , table , uuid , language , SC.OrderByAsc("id") ) ;
}

bool N::NameCollection::FetchItems (
       SqlConnection & SC          ,
       QString         table       ,
       SUID            uuid        ,
       int             Limit       ,
       QString         column      ,
       Qt::SortOrder   sorting     )
{
  return Items . fromUuid               (
           SC                           ,
           table                        ,
           uuid                         ,
           SC . OrderBy(column,sorting) ,
           SC . sql . Limit(0,Limit)  ) ;
}

bool N::NameCollection::ObtainNames (
       SqlConnection & SC           ,
       QString         table        ,
       int             Limit        ,
       QString         column       ,
       Qt::SortOrder   sorting      )
{
  for (int i=0;i<Maps.count();i++)      {
    if ( Maps[i]->name > 0 )            {
      FetchItems                        (
        SC                              ,
        table                           ,
        Maps[i]->name                   ,
        Limit                           ,
        column                          ,
        sorting                       ) ;
    }                                   ;
  }                                     ;
  return ( Items.count() > 0 )          ;
}

bool N::NameCollection::FetchMaps(SqlConnection & SC,QString table,SUID uuid)
{
  return Maps . where ( SC , table , uuid ) ;
}

bool N::NameCollection::FetchBoth (
       SqlConnection & SC         ,
       QString         nameTable  ,
       QString         mapsTable  ,
       SUID            uuid       )
{
  FetchNames  ( SC , nameTable , uuid ) ;
  FetchMaps   ( SC , mapsTable , uuid ) ;
  ObtainNames ( SC , nameTable        ) ;
  return ( ( Names.count() > 0 )       ||
           ( Items.count() > 0 )       ||
           ( Maps .count() > 0 )      ) ;
}
