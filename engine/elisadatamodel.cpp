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

#include <QStringList>
#include <QUrl>
#include <QDebug>

#include "elisadatamodel.h"
#include "utils.h"

/*!
  \class ProgramBase
  \brief ProgramBase Base class for the data in elisa viihde service.
    
  Provides common details in all data formats in elisa viihde.

*/
const char* const DATE_FORMAT = "hh:mm dd.MM.yyyy";
const char* const DATE_FORMAT_FROM = "d.M.yyyy hh:mm:ss";
const char* const DATE_FORMAT_FROM_CUSTOM = "dd.MM.yyyy hh:mm";

QString ProgramBase::id()
{
    return mContent.value(ID, "");    
}

void ProgramBase::setId(QString id)
{
    mContent.insert(ID, id);    
}

QString ProgramBase::name()
{
    return mContent.value(NAME, "");
}

void ProgramBase::setName(QString name)
{
    insertValue(NAME, name);
}

void ProgramBase::insertValue(QString key, QString value)
{
    if(key == START_TIME || key == END_TIME){
        if(value.indexOf(".") == 1){
            value.prepend("0");
        }
    }

    mContent.insert(key, Utils::urlDecode(value));
}

QString ProgramBase::getValue(QString key, QString defaultValue)
{
    return mContent.value(key, defaultValue);
}

QString ProgramBase::toString()
{
    QStringList values;    
    foreach(QString key, mContent.keys()){
        values.append(key + "=>" + mContent.value(key));        
    }
    return values.join(",");
}

QDateTime ProgramBase::parseTime(QString stringValue)
{
    if(stringValue == "") QDateTime();
    QDateTime time = QDateTime::fromString(stringValue, DATE_FORMAT_FROM);
    if(time.isValid()){
        return time;
    }
    else{
        return QDateTime::fromString(stringValue.remove(0,3),DATE_FORMAT_FROM_CUSTOM);
    }
}

Folder::Folder()
{}
    
Folder::Folder(const QString& id, const QString& name, const QString& size)
{
    setId(id);
    setName(name);
    setSize(size);    
}

QString Folder::size()
{
    return getValue(SIZE, "");    
}

void Folder::setSize(QString size)
{
    insertValue(SIZE, size);
}

Recording::Recording()
{}

Recording::Recording(const QString& id, const QString& name, const QString& channel)
{
    setId(id);
    setName(name);
    setChannel(channel);    
}

void Recording::setProgramId(QString programId)
{
    insertValue(PROGRAM_ID, programId);
}

QString Recording::getProgramId()
{
    return getValue(PROGRAM_ID, "");
}

void Recording::setLenght(int lenght)
{
    insertValue(LENGTH, QString::number(lenght));
}

int Recording::length()
{
    return (getValue(LENGTH, "0")).toInt();
}
    
void Recording::setChannel(QString channel)
{
    insertValue(CHANNEL, channel);
}

QString Recording::channel()
{
    return getValue(CHANNEL, "");
}

void Recording::setStartTime(QDateTime startTime)
{
    insertValue(START_TIME, startTime.toString(DATE_FORMAT_FROM));
}

void Recording::setStartTime(QString startTime)
{    
    insertValue(START_TIME, startTime);
}


QDateTime Recording::startTime()
{
    QString stringValue = getValue(START_TIME, "");
    return parseTime(stringValue);
}

QString Recording::startTimeAsString()
{
    return getValue(START_TIME, "");
}

void Recording::setWildCard(QString wildCard)
{
    insertValue(WILD_CARD, wildCard);
}

QString Recording::wildCard()
{
    return getValue(WILD_CARD, "");
}

void Recording::setFolderId(QString folderId)
{
    insertValue(FOLDER_ID, folderId);
}

QString Recording::folderId()
{
    return getValue(FOLDER_ID, "");
}


void Recording::setUrl(const QString& url)
{
    insertValue(URL, url);
}

QString Recording::url()
{
    return getValue(URL, "");
}

Program::Program()
{}

Program::Program(const QString& id, const QString& name, const QString& channel)
    :Recording(id, name, channel)
{
}

void Program::setEndTime(QDateTime endTime)
{
    insertValue(END_TIME, endTime.toString(DATE_FORMAT_FROM));   
}

void Program::setEndTime(QString endTime)
{
    insertValue(END_TIME, endTime);
}

QDateTime Program::endTime()
{
    QString stringValue = getValue(END_TIME, "");
    return parseTime(stringValue);
}

QString Program::endTimeAsString()
{
    return getValue(END_TIME, "");
}

void Program::setDescription(QString description)
{
    insertValue(DESCRIPTION, description);
}

QString Program::description()
{
    return getValue(DESCRIPTION, "");
}
    
void Program::setLongDescription(QString longDescription)
{
    insertValue(LONG_DESC, longDescription);
}
QString Program::longDescription()
{
    return getValue(LONG_DESC, "");
}
    
bool Program::isRecorded()
{
    return Utils::stringToBool(getValue(RECORDED, "false"));
}

void Program::setRecorded(bool wasRecorded)
{
    insertValue(RECORDED, Utils::boolToString(wasRecorded));
}

bool Program::toBeRecorded()
{
    return Utils::stringToBool(getValue(TO_BE_RECORDED, "false"));
}

void Program::setToBeRecorded(bool record)
{
    insertValue(TO_BE_RECORDED, Utils::boolToString(record));
}

bool Program::started()
{
    return Utils::stringToBool(getValue(STARTED, "false"));
}
void Program::setStarted(bool hasStarted)
{
    insertValue(STARTED, Utils::boolToString(hasStarted));
}
bool Program::ended()
{
    return Utils::stringToBool(getValue(ENDED, "false"));
}
void Program::setEnded(bool hasEnded)
{
    insertValue(ENDED, Utils::boolToString(hasEnded));
}
        
bool Program::ready()
{
    return Utils::stringToBool(getValue(READY, "false"));
}

void Program::setReady(bool isReady)
{
    insertValue(READY, Utils::boolToString(isReady));
}

bool Program::isWildCard()
{
    return Utils::stringToBool(getValue(IS_WILD, "false"));
}
void Program::setIsWildCard(bool wildCard)
{
    insertValue(READY, Utils::boolToString(wildCard));
}
    
void Program::setRecordId(QString recordId)
{
    insertValue(RECORDING_ID, recordId);
}

QString Program::recordId()
{
    return getValue(RECORDING_ID, "");
}

void Program::setProgramViewId(QString programViewId)
{
    insertValue(PROGRAMVIEW_ID, programViewId);
}

QString Program::programViewId()
{
    return getValue(PROGRAMVIEW_ID, "");
}

