
/***************************************************************************
 *   This file is part of the AudioThumbs.                                 *
 *   Copyright (C) 2009 Vytautas Mickus <vmickus@gmail.com>                *
 *   Copyright (C) 2011 Raizner Evgeniy <razrfalcon@gmail.com>             *
 *   Copyright (C) 2014 eplightning <me2.legion@gmail.com>                 *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it  under the terms of the GNU Lesser General Public License version  *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,            *
 *   MA  02110-1301  USA                                                   *
 ***************************************************************************/

#ifndef _AUDIO_THUMBS_H_
#define _AUDIO_THUMBS_H_

#include <QObject>
#include <kio/thumbcreator.h>

class ATCreator : public QObject, public ThumbCreator
{
    Q_OBJECT
public:
    ATCreator();
    virtual ~ATCreator();
    virtual bool create(const QString &path, int w, int h, QImage &img);
    virtual Flags flags() const;
private:
};

#endif // _AUDIO_THUMBS_H_
