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
 *****************************************************************************************************************/

#include <QDebug>

#include "folderlistmodel.h"

FolderListModel::FolderListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[IdRole] = "id";
    setRoleNames(roles);
    QString defaultName = tr("Default");
    mDefaultFolder = Folder("",defaultName,"");
    mCurrentFolder = defaultName;
    mFolders.append(mDefaultFolder);
}

void FolderListModel::setFolders(const QList<Folder> folders)
{    
    beginRemoveRows(QModelIndex(), 0, mFolders.count());
    mFolders.clear();
    endRemoveRows();    
    beginInsertRows(QModelIndex(), 0, folders.count()+1);
    mFolders.append(mDefaultFolder);
    mFolders.append(folders);
    endInsertRows();
}

int FolderListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)   
    return mFolders.count();
}

QVariant FolderListModel::data(const QModelIndex & index, int role) const
{    
    if(index.row() < 0 || index.row() > mFolders.count()){
        return QVariant("");
    }

    Folder folder = mFolders.at(index.row());
    if(role == IdRole){
        return folder.id();
    }
    else if(role == NameRole){           
        return folder.name();
    }
    return QVariant();
}

void FolderListModel::setCurrentFolder(const QString& folderId)
{
    foreach(Folder folder, mFolders){
        if(folder.id() == folderId){
            setCurrentFolderName(folder.name());
            break;
        }
    }
}

void FolderListModel::setCurrentFolderName(const QString& folder)
{
    mCurrentFolder = folder;
}



QString FolderListModel::currentFolder()
{
    return mCurrentFolder;
}
