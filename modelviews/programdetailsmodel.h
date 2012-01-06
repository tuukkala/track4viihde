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

#ifndef PROGRAMDETAILSMODEL_H
#define PROGRAMDETAILSMODEL_H

#include "../engine/elisadatamodel.h"

class ProgramDetailsModel : public QObject
{
    Q_OBJECT
public:
    ProgramDetailsModel(Program program, QObject* parent=0);
    ProgramDetailsModel(QObject* parent=0);
    ~ProgramDetailsModel();

    void setProgram(Program program);

    Q_INVOKABLE QString id();
    Q_INVOKABLE QString name();
    Q_INVOKABLE QString description();
    Q_INVOKABLE QString length();
    Q_INVOKABLE QString startTime();
    Q_INVOKABLE bool recorded();
    Q_INVOKABLE QString programViewId();
    Q_INVOKABLE QString recordingId();
    Q_INVOKABLE QString url();
    Q_INVOKABLE bool ready();


private:
    Program mData;
};
#endif // PROGRAMDETAILSMODEL_H
