/****************************************************************************
 *                                                                          *
 * Copyright (C) 2001~2016 Neutrino International Inc.                      *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_SQLANALYZER_H
#define QT_SQLANALYZER_H

#include <QtCore>
#include <Essentials>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_SQLANALYZER_LIB)
#      define Q_SQLANALYZER_EXPORT Q_DECL_EXPORT
#    else
#      define Q_SQLANALYZER_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_SQLANALYZER_EXPORT
#endif

namespace N
{

class Q_SQLANALYZER_EXPORT SqlCondition   ;
class Q_SQLANALYZER_EXPORT SqlConditions  ;
class Q_SQLANALYZER_EXPORT SqlConstraint  ;
class Q_SQLANALYZER_EXPORT SqlConstraints ;
class Q_SQLANALYZER_EXPORT SqlTemplates   ;
class Q_SQLANALYZER_EXPORT SqlParser      ;
class Q_SQLANALYZER_EXPORT SqlClone       ;
class Q_SQLANALYZER_EXPORT NameItem       ;
class Q_SQLANALYZER_EXPORT NameItems      ;
class Q_SQLANALYZER_EXPORT NameMapItem    ;
class Q_SQLANALYZER_EXPORT NameMapItems   ;
class Q_SQLANALYZER_EXPORT NameCollection ;

typedef QMap < QString , SqlParser > SqlParsers ;

class Q_SQLANALYZER_EXPORT SqlCondition
{
  public:

    QString field  ;
    QString symbol ;
    int     relate ;
    VarArgs value  ;

    explicit       SqlCondition (void);
    explicit       SqlCondition (QString field) ;
    explicit       SqlCondition (QString field,QString symbol,VarArgs & value) ;
                   SqlCondition (const SqlCondition & condition) ;
    virtual       ~SqlCondition (void) ;

    SqlCondition & operator =   (const SqlCondition & condition) ;

    QString        syntax       (Sql & sql) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlConditions : public QList<SqlCondition *>
{
  public:

    explicit    SqlConditions (void) ;
    virtual    ~SqlConditions (void) ;

    int         add           (QString field,QString symbol) ;
    int         add           (QString field,QString symbol,const char * value) ;
    int         add           (QString field,QString symbol,VarArgs & value) ;

    QStringList syntaxes      (Sql & sql) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlConstraint
{
  public:

    VarArgs descriptions ;

    explicit        SqlConstraint (void);
                    SqlConstraint (const SqlConstraint & constraint) ;
    virtual        ~SqlConstraint (void) ;

    SqlConstraint & operator =    (const SqlConstraint & constraint) ;

    QString         syntax        (Sql & sql) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlConstraints : public QList<SqlConstraint *>
{
  public:

    explicit    SqlConstraints (void) ;
    virtual    ~SqlConstraints (void) ;

    QStringList syntaxes      (Sql & sql) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlTemplates
{
  public:

    SqlConditions  fields      ;
    SqlConstraints constraints ;

    explicit SqlTemplates (void) ;
    virtual ~SqlTemplates (void) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlParser
{
  public:

    Sql         sql         ;
    QString     Database    ;
    QString     Table       ;
    QString     Creation    ;
    QString     Inside      ;
    QStringList Lines       ;
    QStringList Items       ;
    QStringList Constraints ;
    QString     Tails       ;

    QStringList Columns     ;
    KMAPs       TypeMaps    ;
    KMAPs       SignMaps    ;
    KMAPs       Paraments   ;
    KMAPs       Attributes  ;
    KMAPs       Extras      ;
    KMAPs       Comments    ;

    QStringList Conditions  ;
    KMAPs       Symbols     ;
    KMAPs       Remains     ;
    QStringList Checks      ;

    explicit SqlParser        (Sql & sql) ;
    explicit SqlParser        (void) ;
    virtual ~SqlParser        (void) ;

    int         count         (void) ;
    QString     syntax        (int index) ;
    int         limits        (void) ;
    QString     boundary      (int index) ;
    bool        isForeign     (int index) ;
    QStringList References    (int index) ;
    QStringList References    (QString ForeignKey) ;

    bool        Parse         (QString database,QString table,QString creation) ;

    bool        Analyze       (QString item) ;
    bool        Constraint    (QString constraint) ;

    void        setType       (QString column,QString type) ;
    void        setPrimary    (QString items) ;

    QString     Undress       (QString item,QChar escape) ;
    QStringList Split         (QString inside) ;
    QStringList Members       (QString lists) ;

    int         space         (QString & s,int from) ;
    int         bracket       (QString & s,int from) ;
    int         tag           (QString & s,int from) ;
    int         quote         (QString & s,int from) ;
    int         squote        (QString & s,int from) ;
    bool        completeSpace (QString & s) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT SqlClone
{
  public:

    explicit SqlClone (void) ;
    virtual ~SqlClone (void) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT NameItem
{
  public:

    TUID       id       ;
    SUID       uuid     ;
    int        language ;
    int        length   ;
    int        flags    ;
    QByteArray name     ;

    explicit NameItem      (void) ;
             NameItem      (const NameItem & item) ;
    virtual ~NameItem      (void) ;

    void       clear       (void) ;
    QByteArray toByteArray (void) ;
    QString    String      (void) ;
    QString    String      (QTextCodec & codec) ;
    bool       isFlag      (int Mask) ;

    NameItem & assign      (const NameItem   & item  ) ;
    NameItem & operator  = (const NameItem   & item  ) ;
    NameItem & operator  = (const QByteArray & packet) ;
    NameItem & operator  = (const QString    & string) ;

    bool       obtain      (SqlConnection & Connection) ;
    bool       next        (SqlConnection & Connection) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT NameItems : public QList<NameItem *>
{
  public:

    explicit    NameItems   (void) ;
                NameItems   (const NameItems & item) ;
    virtual    ~NameItems   (void) ;

    QStringList StringList  (void) ;
    virtual int indexOf     (SUID uuid,int language) ;
    virtual int indexOf     (SUID uuid) ;

    NameItems & assign      (const NameItems & items) ;
    NameItems & operator  = (const NameItems & items) ;

    int         obtain      (SqlConnection & Connection) ;
    int         query       (SqlConnection & Connection,QString Q) ;
    int         where       (SqlConnection & Connection         ,
                             QString         Table              ,
                             QString         WhereString        ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;
    int         equal       (SqlConnection & Connection         ,
                             QString         Table              ,
                             QString         Name               ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;
    int         like        (SqlConnection & Connection         ,
                             QString         Table              ,
                             QString         Pattern            ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;
    int         regexp      (SqlConnection & Connection         ,
                             QString         Table              ,
                             QString         RegExp             ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;
    int         fromUuid    (SqlConnection & Connection         ,
                             QString         Table              ,
                             SUID            uuid               ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;
    int         fromUuid    (SqlConnection & Connection         ,
                             QString         Table              ,
                             SUID            uuid               ,
                             int             language           ,
                             QString         OrderString = ""   ,
                             QString         LimitString = "" ) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT NameMapItem
{
  public:

    TUID id     ; // SQL sequence id
    SUID uuid   ; // parent uuid
    SUID name   ; // name uuid
    int  prefer ; // name preference order
    int  usage  ; // enum N::Dynamic::NameUsages

    explicit      NameMapItem (void) ;
                  NameMapItem (const NameMapItem & item) ;
    virtual      ~NameMapItem (void) ;

    void          clear       (void) ;
    QByteArray    toByteArray (void) ;

    NameMapItem & assign      (const NameMapItem & item  ) ;
    NameMapItem & operator  = (const NameMapItem & item  ) ;
    NameMapItem & operator  = (const QByteArray  & packet) ;
    bool          operator == (const NameMapItem & item) ;

    bool          obtain      (SqlConnection     & Connection) ;
    bool          next        (SqlConnection     & Connection) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT NameMapItems : public QList<NameMapItem *>
{
  public:

    explicit       NameMapItems (void) ;
                   NameMapItems (const NameMapItems & items) ;
    virtual       ~NameMapItems (void) ;

    NameMapItems & assign       (const NameMapItems & items) ;
    NameMapItems & operator  =  (const NameMapItems & items) ;

    bool           fromOwner    (SUID owner,UUIDs & Uuids) ;
    int            assignPrefer (int startFrom = 1) ;
    void           assignUsage  (int usage) ;

    int            obtain       (SqlConnection & Connection) ;
    int            query        (SqlConnection & Connection,QString Q) ;
    int            where        (SqlConnection & Connection         ,
                                 QString         Table              ,
                                 QString         WhereString        ,
                                 QString         OrderString = ""   ,
                                 QString         LimitString = "" ) ;
    int            where        (SqlConnection & Connection                     ,
                                 QString         Table                          ,
                                 SUID            uuid                           ,
                                 QString         column  = "prefer"             ,
                                 Qt::SortOrder   sorting = Qt::AscendingOrder ) ;
    bool           remove       (SqlConnection & Connection                     ,
                                 QString         Table                        ) ;
    bool           store        (SqlConnection & Connection                     ,
                                 QString         Table                        ) ;

    void           sort         (void) ;

  protected:

  private:

};

class Q_SQLANALYZER_EXPORT NameCollection
{
  public:

    NameItems    Names ;
    NameItems    Items ;
    NameMapItems Maps  ;

    explicit         NameCollection (void) ;
                     NameCollection (const NameCollection & item) ;
    virtual         ~NameCollection (void) ;

    void             clear          (void) ;

    NameCollection & operator =     (const NameCollection & collection) ;
    bool             OwnerNames     (SUID owner,UUIDs & Uuids,NAMEs & Names) ;

    bool             FetchNames     (SqlConnection & Connection                       ,
                                     QString         nameTable                        ,
                                     SUID            uuid                             ,
                                     int             language = 0                   ) ;
    bool             FetchItems     (SqlConnection & Connection                       ,
                                     QString         nameTable                        ,
                                     SUID            uuid                             ,
                                     int             Limit    = 1                     ,
                                     QString         column   = "id"                  ,
                                     Qt::SortOrder   sorting  = Qt::DescendingOrder ) ;
    bool             ObtainNames    (SqlConnection & Connection                       ,
                                     QString         nameTable                        ,
                                     int             Limit    = 1                     ,
                                     QString         column   = "id"                  ,
                                     Qt::SortOrder   sorting  = Qt::DescendingOrder ) ;
    bool             FetchMaps      (SqlConnection & Connection                       ,
                                     QString         mapsTable                        ,
                                     SUID            uuid                           ) ;
    bool             FetchBoth      (SqlConnection & Connection                       ,
                                     QString         nameTable                        ,
                                     QString         mapsTable                        ,
                                     SUID            uuid                           ) ;

  protected:

  private:

};

}

Q_DECLARE_METATYPE(N::SqlCondition)
Q_DECLARE_METATYPE(N::SqlConditions)
Q_DECLARE_METATYPE(N::SqlConstraint)
Q_DECLARE_METATYPE(N::SqlConstraints)
Q_DECLARE_METATYPE(N::SqlTemplates)
Q_DECLARE_METATYPE(N::SqlParser)
Q_DECLARE_METATYPE(N::SqlParsers)
Q_DECLARE_METATYPE(N::SqlClone)
Q_DECLARE_METATYPE(N::NameItem)
Q_DECLARE_METATYPE(N::NameItems)
Q_DECLARE_METATYPE(N::NameMapItem)
Q_DECLARE_METATYPE(N::NameMapItems)
Q_DECLARE_METATYPE(N::NameCollection)

QT_END_NAMESPACE

#endif
