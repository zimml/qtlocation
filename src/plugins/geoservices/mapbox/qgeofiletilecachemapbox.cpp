// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qgeofiletilecachemapbox.h"
#include <QtLocation/private/qgeotilespec_p.h>
#include <QDir>

QT_BEGIN_NAMESPACE

QGeoFileTileCacheMapbox::QGeoFileTileCacheMapbox(const QList<QGeoMapType> &mapTypes,
                                                 int scaleFactor, const QString &directory,
                                                 QObject *parent)
    :QGeoFileTileCache(directory, parent), m_mapTypes(mapTypes)
{
    m_scaleFactor = qBound(1, scaleFactor, 2);
    for (qsizetype i = 0; i < mapTypes.size(); i++)
        m_mapNameToId.insert(mapTypes[i].name(), i + 1);
}

QGeoFileTileCacheMapbox::~QGeoFileTileCacheMapbox()
{

}

QString QGeoFileTileCacheMapbox::tileSpecToFilename(const QGeoTileSpec &spec, const QString &format,
                                                    const QString &directory) const
{
    QString filename = spec.plugin();
    filename += QLatin1String("-");
    filename += m_mapTypes[spec.mapId()-1].name();
    filename += QLatin1String("-");
    filename += QString::number(spec.zoom());
    filename += QLatin1String("-");
    filename += QString::number(spec.x());
    filename += QLatin1String("-");
    filename += QString::number(spec.y());

    //Append version if real version number to ensure backwards compatibility and eviction of old tiles
    if (spec.version() != -1) {
        filename += QLatin1String("-");
        filename += QString::number(spec.version());
    }

    filename += QLatin1String("-@");
    filename += QString::number(m_scaleFactor);
    filename += QLatin1Char('x');

    filename += QLatin1String(".");
    filename += format;

    QDir dir = QDir(directory);

    return dir.filePath(filename);
}

QGeoTileSpec QGeoFileTileCacheMapbox::filenameToTileSpec(const QString &filename) const
{
    // Split the filename into its parts using the dot separator for map name and
    // extension
    QStringList parts = filename.split('.');

    if (parts.length() != 2) // Expecting two parts: map name and format
        return QGeoTileSpec();

    const QString name = parts.at(0);   // Map name with possible hyphens
    const QString format = parts.at(1); // File extension (e.g., png, jpeg)

    // Now, split the map name by hyphens
    const QStringList fields = name.split('-');

    const qsizetype length = fields.length();

    // Validate length of the fields (length 6 for no version, 7 with version)
    if (length != 6 && length != 7)
        return QGeoTileSpec();

    // Parse the scale factor (last part should contain the @scaleFactor)
    const qsizetype scaleIdx = fields.last().indexOf("@");
    if (scaleIdx < 0 || fields.last().size() <= (scaleIdx + 1))
        return QGeoTileSpec();

    bool ok = false;
    int scaleFactor = fields.last()[scaleIdx + 1].digitValue(); // Assuming scale factor is one digit.
    if (scaleFactor != m_scaleFactor)
        return QGeoTileSpec(); // Scale factor mismatch

    // Collect zoom, x, y, and optionally version
    QList<int> numbers;

    // Extract zoom, x, y from the fields
    for (qsizetype i = 2; i < length - 1; ++i) { // Skipping the scale factor at the end
        ok = false;
        int value = fields.at(i).toInt(&ok);
        if (!ok)
            return QGeoTileSpec();
        numbers.append(value);
    }

    // If the version part is missing, assume it to be -1 (default version)
    if (numbers.length() < 4)
        numbers.append(-1);

    // Return a QGeoTileSpec constructed from parsed data
    return QGeoTileSpec(fields.at(0),
                    m_mapNameToId[fields.at(1)],
                    numbers.at(0),
                    numbers.at(1),
                    numbers.at(2),
                    numbers.at(3));
}

QT_END_NAMESPACE
