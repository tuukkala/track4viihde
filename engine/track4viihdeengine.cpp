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

#include <QDebug>
#include <QSettings>
#include <QProcess>

#include "track4viihdeengine.h"

Track4ViihdeEngine* Track4ViihdeEngine::mInstance = 0;
const char* const sfhgsfhg = "45l2n3";
//not really needed but will make it a bit more difficult to crack the password 
//single user system so no need to regenerate
const char* const SUOLAA = "liherkgh496875460tu35otn2lk4nf09jotn2lk3t4nofh3o4rn1l34nrt2334olt13o4iuf1304r134oi";

const char* const USER_KEY = "user";
const char* const PASS_KEY = "pass";
const char* const NETWORK_ERROR = "Connection error!";


Track4ViihdeEngine* Track4ViihdeEngine::instance()
{
    if(mInstance == 0){
        mInstance = new Track4ViihdeEngine();
        qDebug() << "Engine created";
    }        
    return mInstance;
}

Track4ViihdeEngine::~Track4ViihdeEngine()
{
    delete mCrypter;
}


Track4ViihdeEngine::Track4ViihdeEngine()
{    
    mUserLogged = false;
    mLogginInProgress = false;
    mCurrentChannel = 0;
    mEpgModel = new ProgramListModel(this);
    mRecordingModel = new RecordingListModel(this);
    mTopTenModel = new RecordingListModel(this);
    mRecordedShows = new RecordingListModel(this);
    mFoldersModel = new FolderListModel(this);
    mChannelModel = new QStringListModel(this);
    mProgramDateModel = new QStringListModel(this);
    mProgramDetailsModel = new ProgramDetailsModel(this);
    QString str(bcd);
    str.append(sfhgsfhg);
    bool ok;
    quint64 avain = str.toULongLong(&ok, 36);
    mCrypter = new SimpleCrypt(avain);
    mCrypter->setCompressionMode(SimpleCrypt::CompressionNever);
    mCrypter->setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);
    mNetworkManager = ElisaNetworkService::instance();

    if(loadLoginCredentials()){
        logIn(mUserName, mPassword);
    }   
}


void Track4ViihdeEngine::storeLoginDetails()
{
    if(mUserName.isEmpty() || mPassword.isEmpty()){
        return;
    }

    QString eUserName = mCrypter->encryptToString(mUserName);
    QString ePassword = mCrypter->encryptToString(mPassword+SUOLAA);    
    QSettings userSettings;
    userSettings.setValue(USER_KEY,eUserName);
    userSettings.setValue(PASS_KEY,ePassword);
    userSettings.sync();
}

bool Track4ViihdeEngine::loadLoginCredentials()
{
    bool ok = false;
    QSettings userSettings;
    if(userSettings.contains(USER_KEY) && userSettings.contains(PASS_KEY) ){
        mUserName = mCrypter->decryptToString(userSettings.value(USER_KEY).toString());
        mPassword = mCrypter->decryptToString(userSettings.value(PASS_KEY).toString());
        mPassword.remove(SUOLAA);
        ok = true;
    }
    return ok;
}

bool Track4ViihdeEngine::isLoginInProgress()
{
    return mLogginInProgress;
}

bool Track4ViihdeEngine::isUserLogged()
{
    return mUserLogged;
}

void Track4ViihdeEngine::logIn(const QString& userName, const QString& password)
{    
    mLogginInProgress = true;
    mUserName = userName;
    mPassword = password;
    BooleanReply* reply = mNetworkManager->login(userName, password);
    connect(reply, SIGNAL(requestDone(bool, bool)), this, SLOT(loginDone(bool, bool)));
}

void Track4ViihdeEngine::getChannelList()
{    
    ChannelsReply* reply = mNetworkManager->channelList();
    connect(reply, SIGNAL(channelList(QStringList, bool)), this, SLOT(channelList(QStringList,bool)));
}

QString Track4ViihdeEngine::currentChannel()
{
    if(!mChannelModel->stringList().isEmpty()){
        return mChannelModel->stringList().at(mCurrentChannel);
    }
    else{
        return QString("");
    }
}

void Track4ViihdeEngine::getEpg()
{        
    EpgReply* reply = mNetworkManager->epgWeek();
    connect(reply, SIGNAL(epg(QHash<QString,QList<Program> >, bool)), this, SLOT(epgDone(QHash<QString,QList<Program> >, bool)));
}

void Track4ViihdeEngine::getRecordings()
{ 
    RecordingsReply* reply = mNetworkManager->recordings();
    connect(reply, SIGNAL(recordings(QList<Recording>, bool)), this, SLOT(recordings(QList<Recording>, bool)));
}

void Track4ViihdeEngine::getFolderList(const QString& folderId, const QString& destination)
{ 
    if(!folderId.isEmpty()){
        mFoldersModel->setCurrentFolder(folderId);
    }
    FoldersReply* reply = mNetworkManager->folderListing(folderId, destination);
    connect(reply, SIGNAL(folderListing(QPair<QList<Folder>, QList<Recording> >, bool)),
                this, SLOT(folderListing(QPair<QList<Folder>, QList<Recording> >, bool)));
}

void Track4ViihdeEngine::programDetails(const QString& programId)
{
    if(programId.isEmpty()){
        mErrorMessage = tr("Invalid program");
        qDebug("Programdetails error");
        emit errorRecord();
    }
    else{
        ProgramReply* reply = mNetworkManager->programInfo(programId);
        connect(reply, SIGNAL(programInfo(Program, bool)), this, SLOT(programInfo(Program, bool)));
    }
}

void Track4ViihdeEngine::programInfo(Program program, bool networkError)
{
     if(!networkError){
         qDebug() << program.toString();
         mProgramDetailsModel->setProgram(program);
         emit programDetailsReady();
     }
     else{
         mErrorMessage = tr(NETWORK_ERROR);
         qDebug("Programinfo error");
         emit error();
     }
}

void Track4ViihdeEngine::recordPogram(const QString& programId)
{
    qDebug() << "record " << programId;
    BooleanReply* reply = mNetworkManager->recordProgram(programId);
    connect(reply, SIGNAL(requestDone(bool, bool)), this, SLOT(recordReply(bool, bool)));
}

void Track4ViihdeEngine::recordReply(bool result, bool networkError)
{
    if(!networkError){
        if(result){
            getRecordings();
            emit recordingDone();
        }
        else{
            mErrorMessage = tr("Error when updating recording details.");
            qDebug("Record details update error");
            emit errorRecord();
        }
    }
    else{
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Record reply error");
        emit errorRecord();
    }
}


void Track4ViihdeEngine::removeReply(bool result, bool networkError)
{
    if(!networkError){
        if(result){
            getFolderList(mFoldersModel->currentFolder());
            getRecordings();
            emit removed();
        }
        else{
            mErrorMessage = tr("Could note remove program.");
            qDebug() << mErrorMessage;
            emit errorRecord();
        }
    }
    else{
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Remove reply error");
        emit errorRecord();
    }
}

void Track4ViihdeEngine::playVideo(const QString& streamUrl)
{
    qDebug() << "try to launch video " << streamUrl;
    QStringList args;
    args << streamUrl;
    if(!QProcess::startDetached("vlc", args)){
        mErrorMessage = tr("Could not launch video. Make sure vlc is installed");
        emit errorRecord();
    }
}

/**
  * Removes upcoming recording.
  */
void Track4ViihdeEngine::removeRecording(const QString& recordId)
{
    BooleanReply* reply = mNetworkManager->removeRecording(recordId);
    connect(reply, SIGNAL(requestDone(bool, bool)), this, SLOT(recordReply(bool, bool)));
}
/**
  * existing program
  */
void Track4ViihdeEngine::removeProgram(const QString& programViewId)
{
    qDebug() << "poista " << programViewId;
    BooleanReply* reply = mNetworkManager->removeProgram(programViewId);
    connect(reply, SIGNAL(requestDone(bool, bool)), this, SLOT(removeReply(bool, bool)));
}

void Track4ViihdeEngine::folderListing(QPair<QList<Folder>, QList<Recording> > folderPair, bool networkError)
{    
    if(!networkError){
        if(!folderPair.first.isEmpty()){
            mFoldersModel->setFolders(folderPair.first);
            mFoldersModel->setCurrentFolder(tr("Default"));
        }
        if(!folderPair.second.isEmpty()){
            mRecordedShows->setRecordings(folderPair.second);
            emit recordedShowReady();
        }
        else{
            mErrorMessage = tr("No recordings");
            emit errorFolders();
        }
    }
    else{
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Folderlist err");
        emit errorFolders();
    }
}

FolderListModel* Track4ViihdeEngine::folderListModel()
{
    return mFoldersModel;
}

void Track4ViihdeEngine::channelList(QStringList list, bool networkError)
{
    if(!networkError){               
        mChannelModel->setStringList(list);
        getRecordings();
    }
    else{
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Channel list");
        emit error();
    }
}

void Track4ViihdeEngine::loginDone(bool logged, bool networkError)
{    
    mLogginInProgress = false;
    if(logged){        
        mUserLogged = true;
        storeLoginDetails();
        emit userLogged();
        getChannelList();
    }
    else{        
        if(networkError) {
            mErrorMessage = tr(NETWORK_ERROR);
        }
        else{
            mErrorMessage = tr("Login failed!");
        }
        emit loginFailed();
    }
    //cleanup the reply object
    disconnect(sender(), 0, this, 0);
}

bool Track4ViihdeEngine::isEpgReady()
{    
    if(mEpg.isEmpty()) return false;
    else return true;
}

void Track4ViihdeEngine::changeChannel(const QString& channel)
{
    if(mChannelModel->stringList().contains(channel)){
        fillEpgModel(channel);
    }
}

void Track4ViihdeEngine::nextEpgChannel()
{    
    mCurrentChannel++;
    if(mCurrentChannel >= mChannelModel->stringList().count()){
        mCurrentChannel = 0;
    }
    fillEpgModel(mChannelModel->stringList().at(mCurrentChannel));
}

void Track4ViihdeEngine::previousEpgChannel()
{
    if(mCurrentChannel == 0){
        mCurrentChannel = mChannelModel->stringList().count() - 1;
    }
    else{
        mCurrentChannel--;
    }
    fillEpgModel(mChannelModel->stringList().at(mCurrentChannel));
}

void Track4ViihdeEngine::fillEpgModel(const QString& channel)
{
    if(mEpg.contains(channel)){ 
        QList<Program> programs = mEpg.value(channel);
        mEpgModel->setPrograms(programs);
        mCurrentChannel = mChannelModel->stringList().indexOf(channel);
        QStringList dates;
        QString previous;
        foreach(Program program, programs){
            QDateTime startTime = program.startTime();
            if( previous != startTime.toString("ddd dd.MM")){
                QString dateStr = startTime.toString("ddd dd.MM");
                dates.append(dateStr);
                previous = dateStr;
            }
        }
        mProgramDateModel->setStringList(dates);
    }    
    emit epgReady();
}

void Track4ViihdeEngine::epgDone(QHash<QString,QList<Program> > epg, bool networkError)
{    
    if(!networkError){
        mEpg = epg;
        if(mEpg.isEmpty()){
           mErrorMessage = tr(NETWORK_ERROR);
           qDebug("Epg empty");
           emit errorEpg();
        }
        else{
            setRecordedPrograms();
            if(!mChannelModel->stringList().isEmpty()){
                if(mChannelModel->stringList().count() > mCurrentChannel){
                    fillEpgModel(mChannelModel->stringList().at(mCurrentChannel));
                }
                else{
                    mCurrentChannel = 0;
                    fillEpgModel(mChannelModel->stringList().at(mCurrentChannel));
                }
            }
            else{
                fillEpgModel(mEpg.keys().first());
            }
        }
        getFolderList();
    }
    else{        
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Epg error");
        emit errorEpg();
    } 
}

ProgramListModel* Track4ViihdeEngine::epgModel()
{
    return mEpgModel;
}

void Track4ViihdeEngine::refreshAll()
{
    if(loadLoginCredentials()){
        logIn(mUserName, mPassword);
    }
    getChannelList();
}

void Track4ViihdeEngine::recordings(QList<Recording> recordings, bool networkError)
{
    if(!networkError){        
        if(!recordings.isEmpty()){
            mRecordingModel->setRecordings(recordings);
            mNewRecordingIds.clear();
            foreach(Recording rec, recordings){
                mNewRecordingIds.append(rec.getProgramId());
            }
            emit recordingsReady();
        }
        else{
            mErrorMessage = tr("No future recordings");
            emit errorRecordings();
        }
    }
    else{                        
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("Recordings err");
        emit errorRecordings();
    } 
    getEpg();
}

void Track4ViihdeEngine::topTen(QList<Recording> recordings, bool networkError)
{    
    if(!networkError){
        mTopTenModel->setRecordings(recordings);
    }
    else{
        mErrorMessage = tr(NETWORK_ERROR);
        qDebug("top ten error");
        emit error();
    }
}

RecordingListModel* Track4ViihdeEngine::recordingModel()
{
    return mRecordingModel;
}

RecordingListModel* Track4ViihdeEngine::topTenModel()
{
    return mTopTenModel;
}

RecordingListModel* Track4ViihdeEngine::recordedShowsModel()
{
    return mRecordedShows;
}

QStringListModel* Track4ViihdeEngine::channelModel()
{
    return mChannelModel;
}

QStringListModel* Track4ViihdeEngine::programDatesModel()
{
    return mProgramDateModel;
}

QString Track4ViihdeEngine::lastErrorMessage()
{
    return mErrorMessage;
}

QString Track4ViihdeEngine::getEpgDate()
{
    return mEpgDate;
}

ProgramDetailsModel* Track4ViihdeEngine::programDetailsModel()
{
    return mProgramDetailsModel;
}

void Track4ViihdeEngine::newEpgDate(QString date)
{
    if(date != mEpgDate){
        mEpgDate = date;
        emit epgDateChanged();
    }
}

void Track4ViihdeEngine::setRecordedPrograms()
{
    QMutableHashIterator<QString, QList<Program> > ite(mEpg);
    while (ite.hasNext()) {
        ite.next();
        QList<Program> &programs = ite.value();
        QMutableListIterator<Program> i(programs);
        while (i.hasNext()){
            Program &program = i.next();
            if(mNewRecordingIds.contains(program.id())){
                program.setToBeRecorded(true);
            }
        }
    }
}

