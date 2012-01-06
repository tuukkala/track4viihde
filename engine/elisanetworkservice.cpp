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

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QNetworkCookieJar>

#include "elisanetworkservice.h"
#include "track4viihdeengine.h"

ElisaNetworkService *ElisaNetworkService::mInstance = 0;

const char* const BASE_URI         = "http://elisaviihde.fi/etvrecorder";
const char* const LOGIN_URI        = "/default.sl?username=%1&password=%2&ajax";
const char* const CHANNELS_URI     = "/ajaxprograminfo.sl?channels";
const char* const REC_URI          = "/recordings.sl?ajax";
const char* const EPG_URI          = "/ajaxprograminfo.sl";
const char* const PROGRAM_INFO     = "/program.sl?programid=%1&ajax";
const char* const FOLDERS_LIST     = "/ready.sl?folderid=%1&ajax";
const char* const FOLDERS_LIST_POS = "/ready.sl?folderid=XXX&ppos=YYY&ajax";
const char* const RECORD_PROGRAM   = "/program.sl?record=%1&ajax";
const char* const REMOVE_RECORDING = "/program.sl?remover=%1&ajax";
const char* const REMOVE_PROGRAM   = "/program.sl?removep=%1&ajax";
const char* const TOP_URI          = "/channels.sl?ajax";

ElisaNetworkService::ElisaNetworkService(QObject* parent)
    :QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
}

ElisaNetworkService* ElisaNetworkService::instance()
{
    if(mInstance == 0){
        mInstance = new ElisaNetworkService();        
    }
    return mInstance;
}

/**
 * Request recordings. Caller responsible for deletion.
 * Call is asynchronous so wait for epg signal.
 */
RecordingsReply* ElisaNetworkService::recordings()
{
    return new RecordingsReply(makeRequest(QString(REC_URI)), RecordingsReply::REC);
}

RecordingsReply* ElisaNetworkService::topList()
{
    return new RecordingsReply(makeRequest(QString(TOP_URI)), RecordingsReply::TOP);
}

/**
 * Sends a login request to the server. Returns a BooleanReply object which will provide
 * the response.
 */
BooleanReply* ElisaNetworkService::login(const QString& userName, const QString& passWord)
{    
    return new BooleanReply(makeRequest(QString(LOGIN_URI).arg(userName).arg(passWord)));
}

BooleanReply* ElisaNetworkService::recordProgram(const QString& id)
{
    return new BooleanReply(makeRequest(QString(RECORD_PROGRAM).arg(id)));
}
BooleanReply* ElisaNetworkService::removeProgram(const QString& id)
{
    return new BooleanReply(makeRequest(QString(REMOVE_PROGRAM).arg(id)));
}
BooleanReply* ElisaNetworkService::removeRecording(const QString& id)
{
    return new BooleanReply(makeRequest(QString(REMOVE_RECORDING).arg(id)));
}

/**
 * Request detailed information on a program.  
 */
ProgramReply* ElisaNetworkService::programInfo(const QString& programId)
{
    return new ProgramReply(makeRequest(QString(PROGRAM_INFO).arg(programId)));
}

/**
 * Request channels list.
 */
ChannelsReply* ElisaNetworkService::channelList()
{    
    return new ChannelsReply(makeRequest(CHANNELS_URI));
}

/**
 * Get program info for week.
 */
EpgReply* ElisaNetworkService::epgWeek()
{
    return new EpgReply(makeRequest(EPG_URI));
}

/**
 * Get folder details. If no folder given then default folder returned.
 */
FoldersReply* ElisaNetworkService::folderListing(const QString& folderId, const QString& destination)
{
    if(!destination.isEmpty()){        
        return new FoldersReply(makeRequest(QString(FOLDERS_LIST_POS).arg(folderId).arg(destination)));
    }
    else{
        return new FoldersReply(makeRequest(QString(FOLDERS_LIST).arg(folderId)));
    }
}

QNetworkReply* ElisaNetworkService::makeRequest(QString uri)
{
    uri = BASE_URI + uri;
    return mNetworkManager->get(QNetworkRequest(QUrl(uri)));
}

NetworkReader::NetworkReader(QNetworkReply* reply, QObject* parent)
    :QObject(parent)
{
    mReply = reply;
    connect(mReply, SIGNAL(finished()), this, SLOT(readData()));
    mTimer.setSingleShot(true);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT( networkTimeout()));
    mTimer.start(20000);
}

NetworkReader::~NetworkReader()
{
    //make sure that we do not delete if something is still being processed
    mReply->deleteLater();
    mReply = 0;
}

void NetworkReader::readData()
{
    mTimer.stop();
    if(mReply->error() == QNetworkReply::NoError){
        parseData(mReply->readAll());
    }
    else{
        qDebug() << "Network error: " << mReply->error();
        networkError();
    }
    deleteLater();//suicide
}

void NetworkReader::networkTimeout()
{    
    networkError();
    deleteLater();//suicide
}

BooleanReply::BooleanReply(QNetworkReply* reply, QObject* parent)
    :NetworkReader(reply, parent)
{}

void BooleanReply::parseData(const QByteArray& data)
{    
    if((QString(data) == "TRUE")){
        emit requestDone(true, false);
    }
    else{
        emit requestDone(false, false);
    }   
}

void BooleanReply::networkError()
{
    emit requestDone(false, true);
}

ChannelsReply::ChannelsReply(QNetworkReply* reply, QObject* parent)
    :NetworkReader(reply, parent)
{}

void ChannelsReply::parseData(const QByteArray& data)
{
    emit channelList(mParser.parseChannelList(data), false);
}

void ChannelsReply::networkError()
{
    emit channelList(QStringList(), true);
}

RecordingsReply::RecordingsReply(QNetworkReply* reply, Type type, QObject* parent)
    :NetworkReader(reply, parent)
{
    mType = type;
}

void RecordingsReply::parseData(const QByteArray& data)
{
    if(mType == REC){
        emit recordings(mParser.parseRecordings(data), false);
    }
    else{
        emit recordings(mParser.parseTopList(data), false);
    }
}

void RecordingsReply::networkError()
{
    emit recordings(QList<Recording>(), true);
}


EpgReply::EpgReply(QNetworkReply* reply, QObject* parent)
    :NetworkReader(reply, parent)
{
}

void EpgReply::parseData(const QByteArray& data)
{
  emit epg(mParser.parseProgramWeek(data), false);
}

void EpgReply::networkError()
{
    emit epg(QHash<QString,QList<Program> >(), true);
}

ProgramReply::ProgramReply(QNetworkReply* reply, QObject* parent)
    :NetworkReader(reply, parent)
{}

void ProgramReply::parseData(const QByteArray& data)
{
    Program program;
    mParser.parseProgramInfo(data, program);
    emit programInfo(program, false);
}   

void ProgramReply::networkError()
{
    emit programInfo(Program(), true);
}

FoldersReply::FoldersReply(QNetworkReply* reply, QObject* parent)
    :NetworkReader(reply, parent)
{
}

void FoldersReply::parseData(const QByteArray& data)
{
    emit folderListing(mParser.parseFolderList(data), false);
}

void FoldersReply::networkError()
{
    emit folderListing(QPair<QList<Folder>, QList<Recording> >(), true);
}
