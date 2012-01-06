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


#ifndef TRACK4VIIHDEENGINE_H
#define TRACK4VIIHDEENGINE_H

#include <QObject>
#include <QVariant>
#include <QWeakPointer>
#include <QStringListModel>

#include "simplecrypt.h"
#include "elisadatamodel.h"
#include "elisanetworkservice.h"
#include "modelviews/programlistmodel.h"
#include "modelviews/recordinglistmodel.h"
#include "modelviews/folderlistmodel.h"
#include "modelviews/programdetailsmodel.h"

const char * const bcd = "mfnllk";

class Track4ViihdeEngine : public QObject
{
    Q_OBJECT
public:       

    static Track4ViihdeEngine* instance();

    Q_INVOKABLE QString lastErrorMessage();    

    void storeLoginDetails();
    bool loadLoginCredentials();

    Q_INVOKABLE bool isUserLogged();
    Q_INVOKABLE bool isLoginInProgress();
    Q_INVOKABLE void logIn(const QString& userName, const QString& password);

    Q_INVOKABLE bool isEpgReady();    
    Q_INVOKABLE void nextEpgChannel();
    Q_INVOKABLE void previousEpgChannel();
    Q_INVOKABLE void changeChannel(const QString& channel);
    Q_INVOKABLE QString currentChannel();


    Q_INVOKABLE void getChannelList();
    Q_INVOKABLE void getEpg();
    Q_INVOKABLE void getRecordings();
    Q_INVOKABLE void getFolderList(const QString& folderId="", const QString& destination="");
    Q_INVOKABLE void refreshAll();
    Q_INVOKABLE void programDetails(const QString& programId);
    Q_INVOKABLE void recordPogram(const QString& programId);
    Q_INVOKABLE void removeRecording(const QString& recordId);
    Q_INVOKABLE void removeProgram(const QString& programViewId);
    Q_INVOKABLE void playVideo(const QString& streamUrl);



    ProgramListModel* epgModel();
    RecordingListModel* recordingModel();
    RecordingListModel* topTenModel();
    RecordingListModel* recordedShowsModel();
    FolderListModel* folderListModel();
    QStringListModel* channelModel();
    QStringListModel* programDatesModel();
    ProgramDetailsModel* programDetailsModel();

    Q_INVOKABLE QString getEpgDate();
    void newEpgDate(QString date);

signals:
    void userLogged();
    void loginFailed();    
    void epgReady();
    void recordingsReady();
    void error();
    void epgDateChanged();
    void recordedShowReady();
    void programDetailsReady();
    void recordingDone();
    void errorRecord();
    void removed();
    //void channelResult(ResponseCode, QStringList list);

private slots:
    void loginDone(bool loggedIn, bool networkError);
    void epgDone(QHash<QString,QList<Program> >, bool networkError);
    void channelList(QStringList list, bool networkError);
    void recordings(QList<Recording>, bool networkError);
    void topTen(QList<Recording>, bool networkError);
    void folderListing(QPair<QList<Folder>, QList<Recording> >, bool networkError);
    void programInfo(Program, bool);
    void recordReply(bool, bool);
    void removeReply(bool, bool);

private:
    Track4ViihdeEngine();
    ~Track4ViihdeEngine();    
    void fillEpgModel(const QString& channel);
    void setRecordedPrograms();

private:
    static Track4ViihdeEngine* mInstance;
    ElisaNetworkService* mNetworkManager;
    SimpleCrypt* mCrypter;
    QString mErrorMessage;
    bool mUserLogged;
    bool mLogginInProgress;
    QString mUserName;
    QString mPassword;
    QHash<QString, QList<Program> > mEpg;
    ProgramListModel* mEpgModel;
    RecordingListModel* mRecordingModel;
    RecordingListModel* mTopTenModel;
    RecordingListModel* mRecordedShows;
    QStringListModel* mChannelModel;
    QStringListModel* mProgramDateModel;
    ProgramDetailsModel* mProgramDetailsModel;
    FolderListModel* mFoldersModel;
    int mCurrentChannel;
    QString mEpgDate;
    QStringList mNewRecordingIds;

};


#endif
