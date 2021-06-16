#include <qtsqlanalyzer.h>

#pragma pack(push,1)

typedef struct      {
  TUID    id        ;
  SUID    uuid      ;
  SUID    name      ;
  int32_t prefer    ;
  int32_t usage     ;
} NameMapItemPacket ;

#pragma pack(pop)

#define NipSize sizeof(NameMapItemPacket)
#define ReqSize ( length + NipSize )

N::NameMapItem:: NameMapItem(void)
{
  clear ( ) ;
}

N::NameMapItem:: NameMapItem(const NameMapItem & item)
{
  assign ( item ) ;
}

N::NameMapItem::~NameMapItem(void)
{
}

void N::NameMapItem::clear(void)
{
  id     = 0                ;
  uuid   = 0                ;
  name   = 0                ;
  prefer = 0                ;
  usage  = Dynamic::Default ;
}

QByteArray N::NameMapItem::toByteArray (void)
{
  QByteArray        B                             ;
  NameMapItemPacket NMIP                          ;
  NMIP . id     = id                              ;
  NMIP . uuid   = uuid                            ;
  NMIP . name   = name                            ;
  NMIP . prefer = prefer                          ;
  NMIP . usage  = usage                           ;
  B    . append ( (const char *)&NMIP , NipSize ) ;
  return B                                        ;
}

N::NameMapItem & N::NameMapItem::assign(const NameMapItem & item)
{
  nMemberCopy ( item , id     ) ;
  nMemberCopy ( item , uuid   ) ;
  nMemberCopy ( item , name   ) ;
  nMemberCopy ( item , prefer ) ;
  nMemberCopy ( item , usage  ) ;
  return ME                     ;
}

N::NameMapItem & N::NameMapItem::operator = (const NameMapItem & item)
{
  return assign ( item ) ;
}

N::NameMapItem & N::NameMapItem::operator = (const QByteArray  & packet)
{
  if ( packet.size() < NipSize ) return ME    ;
  NameMapItemPacket * NMIP                    ;
  NMIP   = (NameMapItemPacket *)packet.data() ;
  id     = NMIP -> id                         ;
  uuid   = NMIP -> uuid                       ;
  name   = NMIP -> name                       ;
  prefer = NMIP -> prefer                     ;
  usage  = NMIP -> usage                      ;
  return ME                                   ;
}

bool N::NameMapItem::operator == (const NameMapItem & item)
{
  if ( uuid != item.uuid ) return false ;
  if ( name != item.name ) return false ;
  return true                           ;
}

bool N::NameMapItem::obtain(SqlConnection & SC)
{
  id     = SC . Tuid ( 0 ) ;
  uuid   = SC . Uuid ( 1 ) ;
  name   = SC . Uuid ( 2 ) ;
  prefer = SC . Int  ( 3 ) ;
  usage  = SC . Int  ( 4 ) ;
  return true              ;
}

bool N::NameMapItem::next(SqlConnection & SC)
{
  if (!SC.Next()) return false ;
  return obtain ( SC )         ;
}
