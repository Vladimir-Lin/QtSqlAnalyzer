#include <qtsqlanalyzer.h>

N::SqlParser:: SqlParser(Sql & s)
{
  sql = s ;
}

N::SqlParser:: SqlParser(void)
{
}

N::SqlParser::~SqlParser(void)
{
}

int N::SqlParser::count(void)
{
  return Columns . count ( ) ;
}

QString N::SqlParser::syntax(int index)
{
  QString s                        ;
  QString b = " "                  ;
  QString column = Columns[index]  ;
  s  = column                      ;
  s += b                           ;
  s += TypeMaps[column]            ;
  if (Paraments.contains(column))  {
    s += "("                       ;
    s += Paraments[column]         ;
    s += ")"                       ;
  }                                ;
  if (SignMaps.contains(column))   {
    s += b                         ;
    s += SignMaps[column]          ;
  }                                ;
  if (Attributes.contains(column)) {
    s += b                         ;
    s += Attributes[column]        ;
  }                                ;
  if (Extras.contains(column))     {
    s += b                         ;
    s += Extras[column]            ;
  }                                ;
  return s                         ;
}

int N::SqlParser::limits(void)
{
  return Conditions.count() ;
}

QString N::SqlParser::boundary(int index)
{
  QString name   = Conditions [ index ]          ;
  QString symbol = Symbols    [ name  ]          ;
  QString s      = ""                            ;
  if (symbol=="unique")                          {
    s = QString("unique key `%1`").arg(name)     ;
  } else                                         {
    s = QString("%1 `%2`").arg(symbol).arg(name) ;
  }                                              ;
  if (Remains.contains(name))                    {
    s += " "                                     ;
    s += Remains [ name ]                        ;
  }                                              ;
  return s                                       ;
}

bool N::SqlParser::isForeign(int index)
{
  QString name = Conditions[ index ]           ;
  nKickOut ( !Symbols.contains(name) , false ) ;
  QString s    = Symbols   [ name  ]           ;
  nKickOut ( s != "constraint"       , false ) ;
  return Remains .contains ( name )            ;
}

bool N::SqlParser::Parse(QString database,QString table,QString creation)
{
  QString c = creation                    ;
  QString v = N::Reverse(c)               ;
  int     l = c.indexOf('(')              ;
  int     r = v.indexOf(')')              ;
  int     z = c.length()                  ;
  Database  = database                    ;
  Table     = table                       ;
  Creation  = creation                    ;
  z        -= l                           ;
  z        -= r                           ;
  z        -= 2                           ;
  Inside    = c.mid(l+1,z)                ;
  Tails     = c.right(r-1)                ;
  Inside    = Inside.replace("\n","")     ;
  Inside    = Inside.replace("\r","")     ;
  QStringList in = Split(Inside)          ;
  QStringList cs                          ;
  Lines . clear ()                        ;
  Lines = in                              ;
  cs << "primary"                         ;
  cs << "unique"                          ;
  cs << "key"                             ;
  cs << "index"                           ;
  cs << "constraint"                      ;
  cs << "check"                           ;
  for (int i=0;i<in.count();i++)          {
    QString     L = in[i]                 ;
    QStringList S = L . split (' ')       ;
    if (S.count()>0)                      {
      QString syntax = S[0]               ;
      syntax = syntax . toLower ( )       ;
      if (cs.contains(syntax))            {
        Constraints << L                  ;
      } else                              {
        Items       << L                  ;
      }                                   ;
    }                                     ;
  }                                       ;
  for (int i=0;i<Items.count();i++)       {
    Analyze    ( Items[i]       )         ;
  }                                       ;
  for (int i=0;i<Constraints.count();i++) {
    Constraint ( Constraints[i] )         ;
  }                                       ;
  return true                             ;
}

bool N::SqlParser::Analyze(QString item)
{
  int     stage  = 1                             ;
  QString column = ""                            ;
  QString s      = ""                            ;
  int     length = item.length()                 ;
  int     index  = 0                             ;
  int     to     = -1                            ;
  int     attr   = 0                             ;
  QChar   c                                      ;
  while (stage>0)                                {
    c = item.at(index)                           ;
    switch (stage)                               {
      case 1                                     : // column
        switch (c.toLatin1())                    {
          case ' '                               :
            index ++                             ;
          break                                  ;
          case '`'                               :
            to     = tag(item,index)             ;
            to    ++                             ;
            column = item.mid(index,to-index)    ;
            index  = to                          ;
            stage  = 2                           ;
            s      = ""                          ;
            column = Undress(column,'`')         ;
            Columns << column                    ;
          break                                  ;
          default                                :
            to     = space(item,index)           ;
            column = item.mid(index,to-index)    ;
            index  = to                          ;
            stage  = 2                           ;
            s      = ""                          ;
            Columns << column                    ;
          break                                  ;
        }                                        ;
      break                                      ;
      case 2                                     : // type
        switch (c.toLatin1())                    {
          case ' '                               :
            index ++                             ;
          break                                  ;
          default                                :
            to = space(item,index)               ;
            s  = item.mid(index,to-index)        ;
            index  = to                          ;
            stage  = 3                           ;
            setType ( column , s )               ;
            s      = ""                          ;
          break                                  ;
        }                                        ;
      break                                      ;
      case 3                                     :
        switch (c.toLatin1())                    {
          case ' '                               :
            index ++                             ;
          break                                  ;
          default                                :
            to = space(item,index)               ;
            s  = item.mid(index,to-index)        ;
            index  = to                          ;
            index ++                             ;
            if (s!="CURRENT_TIMESTAMP")          {
              s  = s . toLower ()                ;
            }                                    ;
            if (s=="unsigned")                   {
              SignMaps   [ column ] = s          ;
              s = ""                             ;
            } else
            if (s=="not")                        {
              Attributes [ column ] = s          ;
              s = ""                             ;
            } else
            if (s=="null")                       {
              if (Attributes.contains(column))   {
                Attributes [ column ] += " "     ;
                Attributes [ column ] += s       ;
              } else                             {
                Attributes [ column ]  = s       ;
                s = ""                           ;
              }                                  ;
            } else
            if (s=="default")                    {
              Attributes [ column ]  = s         ;
              s = ""                             ;
            } else
            if (s=="auto_increment")             {
              Extras [ column ]  = s             ;
              s = ""                             ;
            } else                               {
              if (attr==0)                       {
                if (Attributes.contains(column)) {
                  Attributes [ column ] += " "   ;
                  Attributes [ column ] += s     ;
                } else                           {
                 Attributes [ column ]  = s      ;
                 s = ""                          ;
                }                                ;
              } else                             {
                if (Extras.contains(column))     {
                  Extras [ column ] += " "       ;
                  Extras [ column ] += s         ;
                } else                           {
                  Extras [ column ]  = s         ;
                  s = ""                         ;
                }                                ;
              }                                  ;
              attr++                             ;
            }                                    ;
          break                                  ;
        }                                        ;
      break                                      ;
    }                                            ;
    if (index>=length)                           {
      stage = 0                                  ;
    }                                            ;
  }                                              ;
  return true                                    ;
}

bool N::SqlParser::Constraint(QString constraint)
{
  int     stage  = 1                                ;
  QString symbol = ""                               ;
  QString name   = ""                               ;
  QString s      = ""                               ;
  int     length = constraint.length()              ;
  int     index  = 0                                ;
  int     to     = -1                               ;
  int     attr   = 0                                ;
  QChar   c                                         ;
  while (stage>0)                                   {
    c = constraint.at(index)                        ;
    switch (stage)                                  {
      case 1                                        : // symbol
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          default                                   :
            to     = space(constraint,index)        ;
            symbol = constraint.mid(index,to-index) ;
            symbol = symbol . toLower ()            ;
            index  = to                             ;
            stage  = 2                              ;
            s      = ""                             ;
            if ( symbol == "primary"    )           {
              stage = 4                             ;
            } else
            if ( symbol == "unique"     )           {
              stage = 3                             ;
            } else
            if ( symbol == "key"        )           {
              stage = 2                             ;
            } else
            if ( symbol == "index"      )           {
              stage = 2                             ;
            } else
            if ( symbol == "constraint" )           {
              stage = 6                             ;
            } else
            if ( symbol == "check"      )           {
              stage = 9                             ;
            }                                       ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 2                                        : // name
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '`'                                  :
            to     = tag(constraint,index)          ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            name   = Undress(s,'`')                 ;
            Conditions    << name                   ;
            Symbols [name] = symbol                 ;
            stage          = 8                      ;
            s              = ""                     ;
          break                                     ;
          default                                   :
            to     = space(constraint,index)        ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            Conditions    << name                   ;
            Symbols [name] = symbol                 ;
            stage          = 8                      ;
            s              = ""                     ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 3                                        : // unique key + name
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '`'                                  :
            to     = tag(constraint,index)          ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            stage  = 2                              ;
            s      = ""                             ;
          break                                     ;
          default                                   :
            to     = space(constraint,index)        ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            stage  = 2                              ;
            s      = ""                             ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 4                                        : // primary
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '`'                                  :
            to     = tag(constraint,index)          ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            stage  = 5                              ;
            s      = ""                             ;
          break                                     ;
          default                                   :
            to     = space(constraint,index)        ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            stage  = 5                              ;
            s      = ""                             ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 5                                        : // primary key (`id`)
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '('                                  :
            to     = bracket(constraint,index)      ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            setPrimary (s)                          ;
            index  = length                         ;
            stage  = 10                             ;
            s      = ""                             ;
          break                                     ;
          default                                   :
            index ++                                ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 6                                        : // constraint
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '`'                                  :
            to     = tag(constraint,index)          ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            name   = Undress(s,'`')                 ;
            Conditions    << name                   ;
            Symbols [name] = symbol                 ;
            stage          = 7                      ;
            s              = ""                     ;
          break                                     ;
          default                                   :
            to     = space(constraint,index)        ;
            s      = constraint.mid(index,to-index) ;
            index  = to                             ;
            Conditions    << name                   ;
            Symbols [name] = symbol                 ;
            stage          = 7                      ;
            s              = ""                     ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 7                                        : // foreign key
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          default                                   :
            s  = constraint.mid(index,length-index) ;
            Remains[name] = s                       ;
            index         = length                  ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 8                                        : // remain brackets
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          default                                   :
            to     = bracket(constraint,index)      ;
            to    ++                                ;
            s      = constraint.mid(index,to-index) ;
            Remains [ name ] = s                    ;
            index            = length               ;
          break                                     ;
        }                                           ;
      break                                         ;
      case 9                                        : // check
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          default                                   :
            s  = constraint.mid(index,length-index) ;
            index   = length                        ;
            Checks << s                             ;
          break                                     ;
        }                                           ;
      break                                         ;
    }                                               ;
    if (index>=length) stage = 0                    ;
  }                                                 ;
  return true                                       ;
}

QStringList N::SqlParser::References(int index)
{
  QStringList v                                ;
  QString name = Conditions[ index ]           ;
  nKickOut ( !Symbols.contains(name)     , v ) ;
  QString s    = Symbols   [ name  ]           ;
  nKickOut ( s != "constraint"           , v ) ;
  nKickOut ( !Remains .contains ( name ) , v ) ;
  return References ( Remains [ name ]       ) ;
}

QStringList N::SqlParser::References(QString foreign)
{
  QStringList v                                     ;
  QStringList l                                     ;
  int         stage  = 1                            ;
  QString     s      = ""                           ;
  int         length = foreign.length()             ;
  int         index  = 0                            ;
  int         to     = -1                           ;
  QChar       c                                     ;
  while (stage>0)                                   {
    c = foreign.at(index)                           ;
    switch (stage)                                  {
      case 1                                        : // symbol
        switch (c.toLatin1())                       {
          case ' '                                  :
            index ++                                ;
          break                                     ;
          case '('                                  :
            to     = bracket(foreign,index)         ;
            to    ++                                ;
            s      = foreign.mid(index,to-index)    ;
            l     << s                              ;
            s      = ""                             ;
            index  = to                             ;
          break                                     ;
          default                                   :
            to     = space(foreign,index)           ;
            s      = foreign.mid(index,to-index)    ;
            l     << s                              ;
            s      = ""                             ;
            index  = to                             ;
          break                                     ;
        }                                           ;
      break                                         ;
    }                                               ;
    if (l.count()>=6)                               {
      s = foreign.mid(index,length-index)           ;
      if (!completeSpace(s)) l << s                 ;
      index = length                                ;
    }                                               ;
    if (index>=length) stage = 0                    ;
  }                                                 ;
  if (l.count()<6) return v                         ;
  v << l[2]                                         ;
  v << l[4]                                         ;
  v << l[5]                                         ;
  if (l.count()>6) v << l[6]                        ;
  return v                                          ;
}

void N::SqlParser::setPrimary(QString items)
{
  QStringList s = Members(items)        ;
  for (int i=0;i<s.count();i++)         {
    QString d = s[i]                    ;
    if (Columns.contains(d))            {
      if (Attributes.contains(d))       {
        Attributes[d] += " primary key" ;
      }                                 ;
    }                                   ;
  }                                     ;
}

void N::SqlParser::setType(QString column,QString type)
{
  bool isBracket                                           ;
  isBracket = ( type.contains('(') && type.contains(')') ) ;
  QStringList paraments                                    ;
  paraments << "decimal"                                   ;
  paraments << "numeric"                                   ;
  paraments << "char"                                      ;
  paraments << "varchar"                                   ;
  paraments << "enum"                                      ;
  paraments << "set"                                       ;
  if (isBracket)                                           { // something inside
    int     bi = type.indexOf('(')                         ;
    QString t  = type.left   (bi )                         ;
    QString b  = type.mid    (bi+1,type.length()-bi-2)     ;
    t          = t.toLower()                               ;
    if (paraments.contains(t))                             {
      TypeMaps  [ column ] = t                             ;
      Paraments [ column ] = b                             ;
    } else                                                 {
      if (t=="int") t = "integer"                          ;
      TypeMaps  [ column ] = t                             ;
    }                                                      ;
  } else                                                   {
    TypeMaps [ column ] = type                             ;
  }                                                        ;
}

QString N::SqlParser::Undress(QString item,QChar escape)
{
  nKickOut ( item . length() <= 0      , ""   ) ;
  nKickOut ( item . at ( 0 ) != escape , item ) ;
  QString s      = ""                           ;
  int     length = item.length()                ;
  int     index  = 1                            ;
  QChar   c                                     ;
  while (index<length)                          {
    c  = item . at ( index )                    ;
    if (c==escape) return s                     ;
    s     += c                                  ;
    index ++                                    ;
  }                                             ;
  return s                                      ;
}

bool N::SqlParser::completeSpace(QString & s)
{
  for (int i=0;i<s.length();i++) {
    QChar c = s.at(i)            ;
    if (c!=' ') return false     ;
  }                              ;
  return true                    ;
}

int N::SqlParser::space(QString & s,int from)
{
  int index = s.indexOf(' ',from) ;
  if (index<0) return s.length()  ;
  return index                    ;
}

int N::SqlParser::bracket(QString & s,int from)
{
  int index = s.indexOf('(',from+1) ;
  int end   = s.indexOf(')',from+1) ;
  nKickOut ( index < 0   , end )    ;
  nKickOut ( index > end , end )    ;
  do                                {
    index = s.indexOf('(',end+1)    ;
    end   = s.indexOf(')',end+1)    ;
    nKickOut ( index < 0 , end )    ;
  } while ( index <= end )          ;
  return end                        ;
}

int N::SqlParser::tag(QString & s,int from)
{
  return s.indexOf('`',from+1) ;
}

int N::SqlParser::quote(QString & s,int from)
{
  bool  drop = false    ;
  int   to   = from + 1 ;
  QChar c               ;
  do                    {
    c = s.at(to)        ;
    if (c=='\\')        {
      to++              ;
      to++              ;
    } else
    if (c=='"')         {
      drop = true       ;
    } else              {
      to++              ;
    }                   ;
  } while (!drop)       ;
  return to             ;
}

int N::SqlParser::squote(QString & s,int from)
{
  bool  drop = false    ;
  int   to   = from + 1 ;
  QChar c               ;
  do                    {
    c = s.at(to)        ;
    if (c=='\\')        {
      to++              ;
      to++              ;
    } else
    if (c=='\'')        {
      drop = true       ;
    } else              {
      to++              ;
    }                   ;
  } while (!drop)       ;
  return to             ;
}

QStringList N::SqlParser::Split(QString inside)
{
  int         state  = 1                 ;
  int         length = inside.length()   ;
  int         index  = 0                 ;
  int         from   = -1                ;
  int         to     = -1                ;
  QString     in     = inside            ;
  QString     s      = ""                ;
  QChar       c                          ;
  QStringList r                          ;
  while (state>0)                        {
    c = inside.at(index)                 ;
    switch (state)                       {
      case 1                             :
        switch (c.toLatin1())            {
          case ' '                       :
            index++                      ;
          break                          ;
          case '\t'                      :
            index++                      ;
          break                          ;
          case '`'                       :
            from  = index                ;
            to    = tag(in,from)         ;
            to   ++                      ;
            s     = in.mid(from,to-from) ;
            index = to                   ;
            from  = to                   ;
            state = 2                    ;
          break                          ;
          case '"'                       : // obviously, wrong syntax
          case '('                       :
          case ','                       :
          case '\''                      :
          break                          ;
          default                        :
            from  = index                ;
            to    = index                ;
            state = 2                    ;
          break                          ;
        }                                ;
      break                              ;
      case 2                             :
        switch (c.toLatin1())            {
          case '`'                       :
            to    = tag(in,to)           ;
            to   ++                      ;
            s    += in.mid(from,to-from) ;
            index = to                   ;
            from  = to                   ;
          break                          ;
          case '('                       :
            to    = bracket(in,to)       ;
            to   ++                      ;
            s    += in.mid(from,to-from) ;
            index = to                   ;
            from  = to                   ;
          break                          ;
          case '"'                       :
            to    = quote(in,to)         ;
            to   ++                      ;
            s    += in.mid(from,to-from) ;
            index = to                   ;
            from  = to                   ;
          break                          ;
          case '\''                      :
            to    = squote(in,to)        ;
            to   ++                      ;
            s    += in.mid(from,to-from) ;
            index = to                   ;
            from  = to                   ;
          break                          ;
          case ','                       :
            r    << s                    ;
            index ++                     ;
            from  = index                ;
            to    = index                ;
            state = 1                    ;
            s     = ""                   ;
          break                          ;
          default                        :
            s     += c                   ;
            index ++                     ;
            to    ++                     ;
            from   = to                  ;
          break                          ;
        }                                ;
      break                              ;
    }                                    ;
    if (index>=length)                   {
      if (s.length()>0)                  {
        r << s                           ;
        s  = ""                          ;
      }                                  ;
      state = 0                          ;
    }                                    ;
  }                                      ;
  return r                               ;
}

QStringList N::SqlParser::Members(QString lists)
{
  QString     r = N::Reverse(lists)                      ;
  QStringList l                                          ;
  QStringList dress                                      ;
  int         index = lists.indexOf( '(' , 0           ) ;
  int         final = r.length()-1-r.indexOf( ')' , 0  ) ;
  QString     s     = lists.mid( index+1,final-index-1 ) ;
  dress = s.split(',')                                   ;
  for (int i=0;i<dress.count();i++)                      {
    QString d = dress[i]                                 ;
    d  = d . replace ( "`" , "" )                        ;
    d  = d . replace ( " " , "" )                        ;
    l << d                                               ;
  }                                                      ;
  return l                                               ;
}
