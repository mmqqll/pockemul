
#include "imageobject.h"

#include <QtNetwork>
#include <enginioclient.h>
#include <enginioreply.h>

ImageObject::ImageObject(EnginioClient *enginio)
    : m_enginio(enginio)
{}

PMLObject::PMLObject(EnginioClient *enginio)
    : m_enginio(enginio)
{}

void ImageObject::setObject(const QJsonObject &object)
{
    qWarning()<<"setObject";
    m_object = object;
    QString fileId;
    fileId = object.value("file").toObject().value("id").toString();

    if (!fileId.isEmpty()) {
        QJsonObject fileObject;
        fileObject.insert("id", fileId);
//        fileObject.insert("variant", QString("thumbnail"));
        EnginioReply *reply = m_enginio->downloadFile(fileObject);
        connect(reply, SIGNAL(finished(EnginioReply*)), this, SLOT(replyFinished(EnginioReply*)));
    } else {
        // Try to fall back to the local file
        QString localPath = object.value("localPath").toString();
        if (QFile::exists(localPath)) {
            m_image = QImage(localPath);
            emit imageChanged(object.value("id").toString());
        }
    }
}

void PMLObject::setObject(const QJsonObject &object)
{
    qWarning()<<"setObject";
    m_object = object;

    m_fileid = object.value("file").toObject().value("id").toString();

    m_id = object.value("id").toString();
    m_description = object.value("desccription").toString();
    m_name = object.value("name").toString();
    m_version = object.value("version").toDouble();
    m_screenshot.loadFromData(QByteArray::fromBase64(
                                  object.value("screenshot").toString().toLatin1()),"PNG");

    emit PMLChanged(m_object.value("id").toString());

}

void PMLObject::updateFileId(QString id) {
    m_fileid = id;
    QJsonObject fileObject;
    fileObject.insert("id", id);
    m_object.insert("file",fileObject);
    m_object.insert("objectType",QString("files"));
    QJsonDocument jdoc(m_object);
    qWarning()<<"updatefileId:"<<id<<" - "<<jdoc.toJson();
}

void PMLObject::replyFinished(EnginioReply *enginioReply)
{
    QString url = enginioReply->data().value("expiringUrl").toString();
    QNetworkRequest request(url);
    m_reply = m_enginio->networkManager()->get(request);
    m_reply->setParent(this);
    connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void ImageObject::replyFinished(EnginioReply *enginioReply)
{
    QString url = enginioReply->data().value("expiringUrl").toString();
    QNetworkRequest request(url);
    m_reply = m_enginio->networkManager()->get(request);
    m_reply->setParent(this);
    connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

void ImageObject::downloadFinished()
{
    QByteArray imageData = m_reply->readAll();
    m_image.loadFromData(imageData);
    emit imageChanged(m_object.value("id").toString());

    delete m_reply;
    m_reply = 0;
}

void PMLObject::downloadFinished()
{
    m_xmldata = m_reply->readAll();

    qWarning()<<m_xmldata;

//    emit imageChanged(m_object.value("id").toString());

    delete m_reply;
    m_reply = 0;
}


QPixmap ImageObject::thumbnail()
{
    if (m_image.isNull() || !m_thumbnail.size().isNull())
        return m_thumbnail;
    m_thumbnail = QPixmap::fromImage(m_image.scaled(300, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    return m_thumbnail;
}
