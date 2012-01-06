/***************************************************************************************************************
 **
 ** Copyright 2011 Tuukka Lahtela. All rights reserved.
 ** 
 ** Redistribution and use in source and binary forms, with or without modification, are
 ** permitted provided that the following conditions are met:
 ** 
 **    1. Redistributions of source code must retain the above copyright notice, this list of
 **       conditions and the following disclaimer.
 ** 
 **    2. Redistributions in binary form must reproduce the above copyright notice, this list
 **       of conditions and the following disclaimer in the documentation and/or other materials
 **       provided with the distribution.
 ** 
 ** THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
 ** WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 ** FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 ** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 ** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 ** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ** ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ** 
 ** The views and conclusions contained in the software and documentation are those of the
 ** authors and should not be interpreted as representing official policies, either expressed
 ** or implied, of Tuukka Lahtela
 **
 ****************************************************************************************************************/


#ifndef ELISANETWORKSERVICE_H
#define ELISANETWORKSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QStringList>
#include <QList>
#include <QTimer>

#include "elisadatamodel.h"
#include "elisadataparser.h"

class BooleanReply;
class ChannelsReply;
class RecordingsReply;
class EpgReply;
class ProgramReply;
class FoldersReply;

class ElisaNetworkService : public QObject
{
    Q_OBJECT

public:
    static ElisaNetworkService* instance();

    BooleanReply* login(const QString& userName, const QString& passWord);
    BooleanReply* recordProgram(const QString& id);
    BooleanReply* removeProgram(const QString& id);
    BooleanReply* removeRecording(const QString& id);

    ChannelsReply* channelList();
    EpgReply* epgWeek();
    RecordingsReply* recordings();
    RecordingsReply* topList();
    ProgramReply* programInfo(const QString& programId);
    FoldersReply* folderListing(const QString& folderId="", const QString& destination="");

private:
    ElisaNetworkService(QObject* parent=0);
    QNetworkReply* makeRequest(QString uri);


private:
    static ElisaNetworkService *mInstance;  
    QNetworkAccessManager* mNetworkManager;  
};

class NetworkReader : public QObject
{
    Q_OBJECT

public:
    virtual ~NetworkReader();

    virtual void parseData(const QByteArray& data) = 0;
    virtual void networkError() = 0;

public slots:
    void readData();
    void networkTimeout();

protected:
    NetworkReader(QNetworkReply* reply, QObject* parent=0);

protected:
    ElisaDataParser mParser;
    QNetworkReply* mReply;
    QTimer mTimer;
};

class BooleanReply : public NetworkReader
{
    Q_OBJECT
public:
    BooleanReply(QNetworkReply* reply, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void requestDone(bool loggedIn, bool networkError);
};

class ChannelsReply : public NetworkReader
{
    Q_OBJECT
public:
    ChannelsReply(QNetworkReply* reply, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void channelList(QStringList list, bool networkError);
};

class RecordingsReply : public NetworkReader
{
    Q_OBJECT
public:
    enum Type{
        REC,
        TOP
    };
    RecordingsReply(QNetworkReply* reply, Type type, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void recordings(QList<Recording>, bool networkError);

private:
    Type mType;
};

class EpgReply : public NetworkReader
{
    Q_OBJECT
public:
    EpgReply(QNetworkReply* reply, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void epg(QHash<QString,QList<Program> >, bool networkError);
};

class ProgramReply : public NetworkReader
{
    Q_OBJECT
public:
    ProgramReply(QNetworkReply* reply, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void programInfo(Program, bool networkError);
};

class FoldersReply : public NetworkReader
{
    Q_OBJECT
public:
    FoldersReply(QNetworkReply* reply, QObject* parent=0);
    void parseData(const QByteArray& data);
    void networkError();

signals:
    void folderListing(QPair<QList<Folder>, QList<Recording> >, bool networkError);
};

#endif

