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

#include "programdetailsmodel.h"

ProgramDetailsModel::ProgramDetailsModel(Program program, QObject* parent)
    :QObject(parent)
{
    setProgram(program);
}

ProgramDetailsModel::ProgramDetailsModel(QObject* parent)
    :QObject(parent)
{
    setProgram(Program());
}

ProgramDetailsModel::~ProgramDetailsModel()
{}

void ProgramDetailsModel::setProgram(Program program)
{
    mData = program;
}

QString ProgramDetailsModel::id()
{
    return mData.id();
}
QString ProgramDetailsModel::name()
{
    return mData.name();
}

QString ProgramDetailsModel::description()
{
    if(mData.longDescription().isEmpty()){
        return mData.description();
    }
    else{
        return mData.longDescription();
    }
}

QString ProgramDetailsModel::length()
{
    QString min = tr("min");
    QString lenght = "%1 %2";
    return lenght.arg(QString::number(mData.length())).arg(min);
}

QString ProgramDetailsModel::startTime()
{
    QString localPre = tr("at");
    QString timeString = "%1 %2 %3";
    return timeString.arg(mData.startTime().toString("ddd dd.MM.yyyy")).arg(localPre).arg(mData.startTime().toString("hh:mm"));
}

bool ProgramDetailsModel::recorded()
{
    return mData.isRecorded();
}

bool ProgramDetailsModel::ready()
{
    return mData.ready();
}

QString ProgramDetailsModel::recordingId()
{
    return mData.recordId();
}

QString ProgramDetailsModel::programViewId()
{
    return mData.programViewId();
}

QString ProgramDetailsModel::url()
{
    return mData.url();
}
