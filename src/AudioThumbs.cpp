
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

#include "AudioThumbs.h"

#include <QImage>
#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>

#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/mp4file.h>
#include <taglib/mp4properties.h>
#include <taglib/mp4tag.h>
#include <taglib/mp4coverart.h>
#include <taglib/xiphcomment.h>
#include <taglib/xingheader.h>
#include <taglib/oggflacfile.h>
#include <taglib/oggfile.h>
#include <taglib/vorbisfile.h>

#include <FLAC++/metadata.h>

extern "C"
{
    Q_DECL_EXPORT ThumbCreator *new_creator()
    {
        return new ATCreator;
    }
}


ATCreator::ATCreator()
{
   
}

ATCreator::~ATCreator()
{

}

bool ATCreator::create ( const QString &path, int /*w*/, int /*h*/, QImage &img )
{
    bool bRet = false;

    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(path);

    if (type.name() == "audio/mpeg") {
        TagLib::MPEG::File mp3File(path.toUtf8());
        TagLib::ID3v2::Tag *mp3Tag = mp3File.ID3v2Tag();
        TagLib::ID3v2::FrameList fList = mp3Tag->frameList("APIC");
        TagLib::ID3v2::AttachedPictureFrame *pic;
        pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(fList.front());
        if (!pic->picture().isEmpty()) {
            img.loadFromData((const uchar *) pic->picture().data(),pic->picture().size());
            bRet = true;
        }

    } else if (type.name() == "audio/flac" || type.name() == "audio/x-flac") {
        TagLib::FLAC::File ff(path.toUtf8());
        TagLib::List<TagLib::FLAC::Picture *> coverList;

        if (!ff.pictureList().isEmpty()) {
            coverList.append(ff.pictureList().front());
            TagLib::ByteVector coverData = coverList.front()->data();
            QByteArray data;
            data.setRawData(coverData.data(),coverData.size());
            img.loadFromData(data);
            bRet = true;
            return bRet;
        }

    } else if (type.name() == "audio/mp4") {
        TagLib::MP4::File mp4file(path.toUtf8());
        TagLib::MP4::Tag *tag = mp4file.tag();
        TagLib::MP4::ItemListMap map=tag->itemListMap();
        QByteArray data;

        if (!map.isEmpty()) {
            for (TagLib::MP4::ItemListMap::ConstIterator it = map.begin(); it != map.end(); ++it) {
                TagLib::MP4::CoverArtList coverList=(*it).second.toCoverArtList();
                    if (!coverList.isEmpty()) {
                        TagLib::MP4::CoverArt cover=coverList[0];

                        TagLib::ByteVector coverData=cover.data();
                        data.setRawData(coverData.data(),coverData.size());
                        img.loadFromData(data);

                        bRet = true;
                        return bRet;
                  }
              }
         }
    } else if (type.name() == "audio/x-vorbis+ogg" || type.name() == "audio/ogg") {
	// this part from Coquillo audio tag editor
        TagLib::Ogg::Vorbis::File file(path.toUtf8());
        TagLib::Ogg::XiphComment *tag = file.tag();

        if (tag->contains("METADATA_BLOCK_PICTURE")) {
            TagLib::StringList blocks = tag->fieldListMap()["METADATA_BLOCK_PICTURE"];

            for (uint i = 0; i < blocks.size(); i++) {
                QByteArray data = QByteArray::fromBase64(blocks[i].toCString());
                QDataStream s(&data, QIODevice::ReadOnly);

                int type;
                uint mimelen;
                int descrlen;
                int datalen;

                int w;
                int h;
                int c;
                int ic;

                char * mime;
                char * descr;
                char * pic;

                s >> type;
                s >> mimelen;

                mime = new char[mimelen+1];
                s.readRawData(mime, mimelen);

                mime[mimelen] = 0;

                s >> descrlen;

                descr = new char[descrlen+1];
                s.readRawData(descr, descrlen);

                descr[descrlen] = 0;

                s >> w >> h >> c >> ic >> datalen;

                if (!datalen)
                  return false;

                pic = new char[datalen];
                s.readRawData(pic, datalen);
                img = QImage::fromData(QByteArray(pic, datalen));

                bRet = true;
            }
        }
    }
    
    return bRet;
}

ThumbCreator::Flags ATCreator::flags() const
{
    return (Flags)(None);
}
