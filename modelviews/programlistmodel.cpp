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

#include <QDateTime>
#include <QDebug>

#include "programlistmodel.h"
#include "engine/track4viihdeengine.h"

ProgramListModel::ProgramListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[TimeRole] = "time";
    roles[IdRole] = "id";
    roles[StartDateRole] = "start";
    roles[RecordedRole] = "recorded";
    setRoleNames(roles);    
}

void ProgramListModel::setPrograms(const QList<Program> programs)
{        
    beginRemoveRows(QModelIndex(), 0, mPrograms.count());
    mPrograms.clear();
    endRemoveRows();
    if(programs.count() == 0) return;       
    beginInsertRows(QModelIndex(), 0, programs.count());    
    mPrograms.append(programs); 

    QString dateString = mPrograms.first().startTime().toString("ddd MM.dd");
    Track4ViihdeEngine::instance()->newEpgDate(dateString);

    endInsertRows();    
}

int ProgramListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return mPrograms.count();
}

int ProgramListModel::getFirstIndexOfDate(const QString& dateString)
{
    int index = 0;
    for(int i = 0; i < mPrograms.count(); i++){
        Program program = mPrograms.at(i);
        QDateTime startTime = program.startTime();
        if( dateString == startTime.toString("ddd dd.MM")){
            index  = i;
            break;
        }
    }
    return index;
}


QVariant ProgramListModel::data(const QModelIndex & index, int role) const
{    
    if(index.row() < 0 || index.row() > mPrograms.count()){
        return QVariant();
    }

    Program program = mPrograms.at(index.row());
    if(role == IdRole){
        return program.id();
    }
    else if(role == NameRole){
        return program.name();
    }
    else if(role == RecordedRole){
        return program.toBeRecorded();
    }
    else if (role == DescriptionRole){
        return program.description();
    }    
    else if (role == StartDateRole){
        QString dateString = program.startTime().toString("ddd MM.dd");
        return dateString;
    }
    else if (role == TimeRole){                
        QDateTime startTime = program.startTime();
        Track4ViihdeEngine::instance()->newEpgDate(startTime.toString("ddd dd.MM"));
        QDateTime endTime = program.endTime();
        QString timeString = startTime.toString("ddd hh:mm - ");
        timeString.append(endTime.toString("hh:mm"));        
        return timeString;
    }
    return QVariant();
}

