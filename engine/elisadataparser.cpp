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
#include <QScriptEngine>
#include <QScriptValueIterator>

#include "elisadataparser.h"

ElisaDataParser::ElisaDataParser()
{}

/**
 * Parses the channel listing from elisaviihde.
 */
QStringList ElisaDataParser::parseChannelList(const QByteArray& source)
{
    QStringList results;
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("("+QString(source)+")");        
    QVariant epgData = sc.toVariant();
    if(epgData.canConvert(QVariant::Map)){
        QVariant channelListing = epgData.toMap().value(CHANNELS);
          if(channelListing.canConvert(QVariant::StringList)){
            results = channelListing.toStringList();
          }
    }
    return results;
}

/**
 * Parses the program week listing from elisaviihde. Uses Scriptingengine for it so that 
 * values are converted to QVariant and QVariantMaps to get the json data.
 */
QHash<QString,QList<Program> > ElisaDataParser::parseProgramWeek(const QByteArray& source)
{
    QHash<QString,QList<Program> > results;
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("("+QString(source)+")");    
    QVariant epgData = sc.toVariant();
    if(epgData.canConvert(QVariant::Map)){
        QVariant channelListing = epgData.toMap().value(CHANNELS);
        if(channelListing.canConvert(QVariant::List)){
            foreach(QVariant variant, channelListing.toList()){
                if(variant.canConvert(QVariant::Map)){
                    QMap<QString,QVariant> map = variant.toMap();
                    foreach(QString key, map.keys()){
                        QString channelName = key;      
                        QList<Program> listing;
                        QVariant value = map.value(key);
                        if(value.canConvert(QVariant::List)){
                            QList<QVariant> children = value.toList();
                            foreach(QVariant child, children){
                                if(child.canConvert(QVariant::Map)){
                                    Program program;
                                    parseProgramDetails(child.toMap(), channelName, program);
                                    listing.append(program);
                                }
                            }
                        }                
                        results.insert(channelName, listing);
                    }
                }
            }
        }
    }
    return results;
}

/**
 * Parse folder list and possible 
 */
QPair<QList<Folder>, QList<Recording> > ElisaDataParser::parseFolderList(const QByteArray& source)
{
    QList<Folder> folders;
    QList<Recording> recordings;
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("("+QString(source)+")");    
    QVariant variant = sc.toVariant();
    if(variant.canConvert(QVariant::Map)){
        QVariant dataList = variant.toMap().value(FOLDERS_ROOT);
        if(dataList.canConvert(QVariant::List)){
            foreach(QVariant dataBits, dataList.toList()){
                if(dataBits.canConvert(QVariant::Map)){
                    QMap<QString,QVariant> allData = dataBits.toMap();
                    //folder data
                    QVariant folderData = allData.value("folders");
                    if(folderData.canConvert(QVariant::List)){
                        foreach(QVariant folderData, folderData.toList()){
                            if(folderData.canConvert(QVariant::Map)){
                                Folder folder;
                                parseFolderInfo(folderData.toMap(), folder);
                                folders.append(folder);
                            }
                        }
                    }
                    //recordings                    
                    parseRecordingList(allData.value(RECORDINGS), recordings);
                }
            }
        }
    }
    return QPair<QList<Folder>, QList<Recording> >(folders, recordings);
}

void ElisaDataParser::parseFolderInfo(QMap<QString,QVariant> jsonMap, Folder& folder)
{
    validateProgramMap(jsonMap);
    folder.setId(jsonMap.value(ID).toString());
    folder.setName(jsonMap.value(NAME).toString());    

    if(jsonMap.contains(SIZE)) 
        folder.setSize(jsonMap.value(SIZE).toString());    
}

/**
 * Parses a single program info to the given program reference. Allows existing program 
 * objects to be filled.
 */
void ElisaDataParser::parseProgramInfo(const QByteArray& source, Program& program)
{
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("("+QString(source)+")");    
    QVariant variant = sc.toVariant();
    if(variant.canConvert(QVariant::Map)){
        QMap<QString,QVariant> programMap = variant.toMap();
        parseProgramDetails(programMap, programMap.value(CHANNEL).toString(), program);
    }    
}

QList<Recording> ElisaDataParser::parseRecordings(const QByteArray& source)
{
    return parsePrograms(source, RECORDINGS);
}

QList<Recording> ElisaDataParser::parseTopList(const QByteArray& source)
{
    return parsePrograms(source, PROGRAMS);
}


QList<Recording> ElisaDataParser::parsePrograms(const QByteArray& source, const QString& type)
{
    QList<Recording> results;
    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("("+QString(source)+")");    
    QVariant variant = sc.toVariant();
    if(variant.canConvert(QVariant::Map)){
        QMap<QString,QVariant> recordingMap = variant.toMap();
        QVariant value = recordingMap.value(type);
        parseRecordingList(value, results);
    }
    return results;
}

void ElisaDataParser::parseRecordingList(QVariant value, QList<Recording>& recordingList)
{
    if(value.canConvert(QVariant::List)){
        QList<QVariant> children = value.toList();
        foreach(QVariant child, children){
            if(child.canConvert(QVariant::Map)){
                Recording recording;
                QMap<QString,QVariant> jsonMap = child.toMap();
                parseRecordings(jsonMap,jsonMap.value(CHANNEL).toString(), recording);
                recordingList.append(recording);
            }
        }
    }
}

void ElisaDataParser::parseRecordings(QMap<QString,QVariant> jsonMap, const QString& channel, Recording& recording)
{
    validateProgramMap(jsonMap);
    recording.setId(jsonMap.value(ID).toString());
    recording.setName(jsonMap.value(NAME).toString());
    recording.setChannel(channel);

    if(jsonMap.contains(PROGRAM_ID)) 
        recording.setProgramId(jsonMap.value(PROGRAM_ID).toString());

    if(jsonMap.contains(START_TIME))
        recording.setStartTime(jsonMap.value(START_TIME).toString());

    if(jsonMap.contains(LENGTH)) 
        recording.setLenght(jsonMap.value(LENGTH).toInt());

    if(jsonMap.contains(WILD_CARD)) 
        recording.setWildCard(jsonMap.value(WILD_CARD).toString());

    if(jsonMap.contains(FOLDER_ID)) 
        recording.setFolderId(jsonMap.value(FOLDER_ID).toString());

    if(jsonMap.contains(URL))
        recording.setUrl(jsonMap.value(URL).toString());

}

void ElisaDataParser::parseProgramDetails(QMap<QString,QVariant> jsonMap, const QString& channel, Program& program)
{
    parseRecordings(jsonMap, channel, program);
    if(jsonMap.contains(DESCRIPTION)) 
        program.setDescription(jsonMap.value(DESCRIPTION).toString());

    if(jsonMap.contains(LONG_DESC)) 
        program.setLongDescription(jsonMap.value(LONG_DESC).toString());

    if(jsonMap.contains(END_TIME)) 
        program.setEndTime(jsonMap.value(END_TIME).toString());

    if(jsonMap.contains(STARTED)) 
        program.setStarted(jsonMap.value(STARTED).toBool());

    if(jsonMap.contains(ENDED)) 
        program.setEnded(jsonMap.value(ENDED).toBool());

    if(jsonMap.contains(RECORDED)) 
        program.setRecorded(jsonMap.value(RECORDED).toBool());

    if(jsonMap.contains(READY))
        program.setReady(jsonMap.value(READY).toBool());

    if(jsonMap.contains(IS_WILD)) 
        program.setIsWildCard(jsonMap.value(IS_WILD).toBool());

    if(jsonMap.contains(PROGRAMVIEW_ID)) 
        program.setProgramViewId(jsonMap.value(PROGRAMVIEW_ID).toString());

    if(jsonMap.contains(RECORDING_ID)) 
        program.setRecordId(jsonMap.value(RECORDING_ID).toString());

}

void ElisaDataParser::validateProgramMap(QMap<QString,QVariant> &jsonMap)
{
    if(!jsonMap.contains(ID)) jsonMap.insert(ID,QVariant(""));
    if(!jsonMap.contains(NAME)) jsonMap.insert(NAME,QVariant(""));
}

