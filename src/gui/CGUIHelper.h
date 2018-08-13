/*
 *    Copyright (C) 2018
 *    Albrecht Lohofener (albrechtloh@gmx.de)
 *
 *    This file is based on SDR-J
 *    Copyright (C) 2010, 2011, 2012
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *
 *    This file is part of the welle.io.
 *    Many of the ideas as implemented in welle.io are derived from
 *    other work, made available through the GNU general Public License.
 *    All copyrights of the original authors are recognized.
 *
 *    welle.io is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    welle.io is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with welle.io; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef GUIHELPER_H
#define GUIHELPER_H

#include <QQmlContext>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QtCharts>

#ifndef QT_NO_SYSTEMTRAYICON
    #include <QSystemTrayIcon>
#endif

#include "CMOTImageProvider.h"
#include "dab-constants.h"

#ifdef Q_OS_ANDROID
    #include "rep_CRadioController_replica.h"
#else
    #include "CRadioController.h"
#endif

/*
 *	GThe main gui object. It inherits from
 *	QDialog and the generated form
 */
class CGUIHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap guiData READ guiData NOTIFY guiDataChanged)
    Q_PROPERTY(QVariant stationModel READ stationModel NOTIFY stationModelChanged)
    Q_PROPERTY(float currentGainValue MEMBER m_currentGainValue NOTIFY currentGainValueChanged)
    Q_PROPERTY(QVariant licenses READ licenses CONSTANT)

public:
    static QTranslator* addTranslator(QString Language,
                                      QTranslator *OldTranslator = nullptr);

#ifdef Q_OS_ANDROID
    CGUI(CRadioControllerReplica *RadioController, QObject* parent = NULL);
#else
    CGUIHelper(CRadioController *radioController, QObject* parent = nullptr);
#endif
    ~CGUIHelper();
    Q_INVOKABLE void inputGainChanged(double gain);
    Q_INVOKABLE void registerSpectrumSeries(QAbstractSeries* series);
    Q_INVOKABLE void registerImpulseResonseSeries(QAbstractSeries* series);
    Q_INVOKABLE void registerNullSymbolSeries(QAbstractSeries* series);
    Q_INVOKABLE void registerConstellationSeries(QAbstractSeries* series);
    Q_INVOKABLE void tryHideWindow(void);
    Q_INVOKABLE void updateSpectrum();
    Q_INVOKABLE void updateImpulseResponse();
    Q_INVOKABLE void updateNullSymbol();
    Q_INVOKABLE void updateConstellation();

    QVariantMap guiData() const
    {
        return radioController->guiData();
    }

    QVariant stationModel() const
    {
        return p_stationModel;
    }

    void setNewDebugOutput(QString text);

    CMOTImageProvider* motImage; // ToDo: Must be a getter

private:
#ifdef Q_OS_ANDROID
    CRadioControllerReplica *radioController;
#else
    CRadioController *radioController;
#endif

    QXYSeries* spectrumSeries;
    QVector<QPointF> spectrumSeriesData;

    QXYSeries* impulseResponseSeries;
    QVector<QPointF> impulseResponseSeriesData;

    QXYSeries* nullSymbolSeries;
    QVector<QPointF> nullSymbolSeriesData;

    QXYSeries* constellationSeries;
    QVector<QPointF> constellationSeriesData;

    const QVariantMap licenses();

    QVariant p_stationModel;

    float m_currentGainValue;

#ifndef QT_NO_SYSTEMTRAYICON
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
#endif

public slots:
    void close();

private slots:
#ifdef Q_OS_ANDROID
    void stateChanged(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState);
#endif
    void deviceClosed();
    void motUpdate(QImage motImage);
    void stationsChange(QList<StationElement *> Stations);
    void showErrorMessage(QString Text);
    void showInfoMessage(QString Text);

signals:
    void foundChannelCount(int channelCount);
    void currentGainValueChanged();
    void setSpectrumAxis(qreal Ymax, qreal Xmin, qreal Xmax);
    void setImpulseResponseAxis(qreal Ymax, qreal Xmin, qreal Xmax);
    void setNullSymbolAxis(qreal Ymax, qreal Xmin, qreal Xmax);
    void setConstellationAxis(qreal Xmin, qreal Xmax);
    void guiDataChanged(QVariantMap guiData);
    void stationModelChanged();
    void motChanged(void);
    void newDebugOutput(QString text);

#ifndef QT_NO_SYSTEMTRAYICON
    void minimizeWindow(void);
    void maximizeWindow(void);
    void restoreWindow(void);
#endif
};

#endif // GUIHELPER_H