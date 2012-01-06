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

#include "recordinglistmodel.h"

const char* const MINUTES = "min";

RecordingListModel::RecordingListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[LengthRole] = "length";
    roles[TimeRole] = "time";
    roles[ChannelRole] = "channel";
    roles[IdRole] = "id";
    setRoleNames(roles);    
}

void RecordingListModel::setRecordings(const QList<Recording> recordings)
{
    beginRemoveRows(QModelIndex(), 0, mRecordings.count());
    mRecordings.clear();
    endRemoveRows();
    if(recordings.count() == 0) return;
    beginInsertRows(QModelIndex(), 0, recordings.count());
    mRecordings.append(recordings);
    endInsertRows();
}

int RecordingListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return mRecordings.count();
}

QVariant RecordingListModel::data(const QModelIndex & index, int role) const
{    
    if(index.row() < 0 || index.row() > mRecordings.count()){
        return QVariant();
    }    
    Recording recording = mRecordings.at(index.row());
    if(role == NameRole){
        return recording.name();
    }
    else if (role == IdRole){
        return recording.getProgramId();
    }
    else if (role == LengthRole){
        return tr(LENGTH) + " " + QString::number(recording.length()) + " " + MINUTES;
    }
    else if (role == ChannelRole){
        return recording.channel();
    }
    else if (role == TimeRole){
        QDateTime startTime = recording.startTime();
        return startTime.toString("dd.MM hh:mm");
    }
    return QVariant();
}

