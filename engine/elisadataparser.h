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


#ifndef ELISADATAPARSER_H
#define ELISADATAPARSER_H

#include <QHash>
#include <QStringList>
#include <QPair>
#include "elisadatamodel.h"


class ElisaDataParser
{
public:
    ElisaDataParser();    

    QStringList parseChannelList(const QByteArray& source);
    QHash<QString,QList<Program> > parseProgramWeek(const QByteArray& source);
    void parseProgramInfo(const QByteArray& source, Program& program);
    QList<Recording> parseRecordings(const QByteArray& source);
    QList<Recording> parseTopList(const QByteArray& source);
    QPair<QList<Folder>, QList<Recording> > parseFolderList(const QByteArray& source);

private:

    QList<Recording> parsePrograms(const QByteArray& source, const QString& type);
    void parseProgramDetails(QMap<QString,QVariant> jsonMap, const QString& channel, Program& program);    
    void validateProgramMap(QMap<QString,QVariant> &jsonMap);    
    void parseRecordings(QMap<QString,QVariant> jsonMap, const QString& channel, Recording& recording);    
    void parseRecordingList(QVariant value, QList<Recording>& recordingList);
    void parseFolderInfo(QMap<QString,QVariant> jsonMap, Folder& folder); 
};

#endif
