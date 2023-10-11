// Copyright (C) 2017 Mapbox, Inc.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR
// GPL-2.0-only OR GPL-3.0-only

#ifndef QMAPBOXAPICOMMON_H
#define QMAPBOXAPICOMMON_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtPositioning/QGeoLocation>

QT_BEGIN_NAMESPACE

static const QString mapboxTilesApiPath =
    QStringLiteral("https://api.mapbox.com/styles/v1/mapbox/");

// https://www.mapbox.com/api-documentation/#geocoding
static const QString mapboxGeocodingApiPath =
    QStringLiteral("https://api.mapbox.com/geocoding/v5/mapbox.places/");
static const QString mapboxGeocodingEnterpriseApiPath = QStringLiteral(
    "https://api.mapbox.com/geocoding/v5/mapbox.places-permanent/");

// https://www.mapbox.com/api-documentation/#directions
static const QString mapboxDirectionsApiPath =
    QStringLiteral("https://api.mapbox.com/directions/v5/mapbox/");

static const QByteArray mapboxDefaultUserAgent =
    QByteArrayLiteral("Qt Location based application");

static const qreal mapboxDefaultRadius = 50000;

class QMapboxCommon {
public:
  static QString mapboxNameForCategory(const QString &category);
  static QGeoLocation parseGeoLocation(const QJsonObject &response);
};

QT_END_NAMESPACE

#endif // QMAPBOXAPICOMMON_H
