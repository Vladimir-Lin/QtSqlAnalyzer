#include <qtsqlanalyzer.h>

#pragma pack(push,1)

typedef struct     {
  TUID    id       ;
  SUID    uuid     ;
  int32_t language ;
  int32_t length   ;
  int32_t flags    ;
} NameItemPacket   ;

#pragma pack(pop)

#define NipSize sizeof(NameItemPacket)
#define ReqSize ( length + NipSize )

N::NameItem:: NameItem(void)
{
  clear ( ) ;
}

N::NameItem:: NameItem(const NameItem & item)
{
  assign ( item ) ;
}

N::NameItem::~NameItem(void)
{
}

void N::NameItem::clear(void)
{
  id       = 0         ;
  uuid     = 0         ;
  language = 0         ;
  length   = 0         ;
  flags    = 0         ;
  name     . clear ( ) ;
}

N::NameItem & N::NameItem::assign(const NameItem & item)
{
  nMemberCopy ( item , id       ) ;
  nMemberCopy ( item , uuid     ) ;
  nMemberCopy ( item , language ) ;
  nMemberCopy ( item , length   ) ;
  nMemberCopy ( item , flags    ) ;
  nMemberCopy ( item , name     ) ;
  return ME                       ;
}

N::NameItem & N::NameItem::operator = (const NameItem & item)
{
  return assign ( item ) ;
}

QString N::NameItem::String(void)
{
  return QString :: fromUtf8 ( name ) ;
}

QString N::NameItem::String(QTextCodec & codec)
{
  return codec . toUnicode ( name ) ;
}

bool N::NameItem::isFlag(int Mask)
{
  return ( ( Mask & flags ) == Mask ) ;
}

QByteArray N::NameItem::toByteArray (void)
{
  QByteArray     B                               ;
  NameItemPacket NIP                             ;
  NIP . id       = id                            ;
  NIP . uuid     = uuid                          ;
  NIP . language = (int32_t)language             ;
  NIP . length   = (int32_t)length               ;
  NIP . flags    = (int32_t)flags                ;
  B   . append  ( (const char *)&NIP , NipSize ) ;
  B   . append  ( name                         ) ;
  return B                                       ;
}

N::NameItem & N::NameItem::operator = (const QByteArray & packet)
{
  if ( packet . size ( ) < NipSize ) return ME           ;
  NameItemPacket * NIP = (NameItemPacket *)packet.data() ;
  id       = NIP->id                                     ;
  uuid     = NIP->uuid                                   ;
  language = NIP->language                               ;
  length   = NIP->length                                 ;
  flags    = NIP->flags                                  ;
  name     . clear ( )                                   ;
  if ( length <= 0 ) return ME                           ;
  if ( ReqSize > packet.size() ) return ME               ;
  name = packet.mid(NipSize,length)                      ;
  return ME                                              ;
}

N::NameItem & N::NameItem::operator = (const QString & string)
{
  name   = string . toUtf8 ( ) ;
  length = name   . size   ( ) ;
  return ME                    ;
}

bool N::NameItem::obtain(SqlConnection & SC)
{
  id       = SC . Tuid      ( 0 ) ;
  uuid     = SC . Uuid      ( 1 ) ;
  language = SC . Int       ( 2 ) ;
  length   = SC . Int       ( 3 ) ;
  flags    = SC . Int       ( 4 ) ;
  name     = SC . ByteArray ( 5 ) ;
  return true                     ;
}

bool N::NameItem::next(SqlConnection & SC)
{
  if (!SC.Next()) return false ;
  return obtain ( SC )         ;
}
