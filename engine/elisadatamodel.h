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
#ifndef ELISADATAMODEL_H
#define ELISADATAMODEL_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QHash>

const char* const ID = "id";
const char* const NAME = "name";
const char* const CHANNEL = "channel";
const char* const CHANNELS = "channels";
const char* const RECORDINGS = "recordings";
const char* const PROGRAMS = "programs";
const char* const DESCRIPTION = "short_text";
const char* const LONG_DESC = "description";
const char* const LENGTH = "length";
const char* const START_TIME = "start_time";
const char* const END_TIME = "end_time";
const char* const STARTED = "has_started";
const char* const ENDED = "has_ended";
const char* const RECORDED = "recorded";
const char* const READY = "ready";
const char* const IS_WILD = "is_wildcard";
const char* const PROGRAMVIEW_ID = "programviewid";
const char* const RECORDING_ID = "recordingid";
const char* const SIZE = "size";
const char* const WILD_CARD = "wild_card";
const char* const PROGRAM_ID = "program_id";
const char* const FOLDER_ID = "folder_id";
const char* const FOLDERS_ROOT = "ready_data";
const char* const TO_BE_RECORDED = "to_be_recorded";
const char* const URL = "url";


class Recording;

class ProgramBase
{
public:        
    virtual ~ProgramBase(){}

    QString id();
    void setId(QString id);
    
    QString name();
    void setName(QString name);
    
    QString toString();    
    void insertValue(QString key, QString value);
    QString getValue(QString key, QString defaultValue);
    
private:
    QHash<QString, QString> mContent;

 };


class Folder : public ProgramBase
{    
public:
    Folder();
    Folder(const QString& id, const QString& name, const QString& size="");

    QString size();
    void setSize(QString size);    
};

class Recording : public ProgramBase
{    
public:
    Recording();
    Recording(const QString& id, const QString& name, const QString& channel);

    void setLenght(int lenght);
    int length();

    void setProgramId(QString programId);
    QString getProgramId();

    void setChannel(QString channel);
    QString channel();
    
    void setFolderId(QString folderId);
    QString folderId();

    void setStartTime(QDateTime startTime);
    void setStartTime(QString startTime);
    QDateTime startTime();
    QString startTimeAsString();
    
    void setWildCard(QString wildCard);
    QString wildCard();

    void setUrl(const QString& url);
    QString url();

};

class Program : public Recording
{    
public:
    Program();
    Program(const QString& id, const QString& name, const QString& channel);

    void setEndTime(QDateTime endTime);
    void setEndTime(QString endTime);
    QDateTime endTime();
    QString endTimeAsString();

    void setDescription(QString description);
    QString description();
    
    void setLongDescription(QString longDescription);
    QString longDescription();
    
    bool isRecorded();
    void setRecorded(bool wasRecorded);
    
    bool started();
    void setStarted(bool hasStarted);

    bool ended();
    void setEnded(bool hasEnded);

    bool toBeRecorded();
    void setToBeRecorded(bool record);

    bool ready();
    void setReady(bool isReady);        

    bool isWildCard();
    void setIsWildCard(bool wildCard);
    
    void setRecordId(QString recordId);
    QString recordId();

    void setProgramViewId(QString programViewId);
    QString programViewId();
};

#endif
