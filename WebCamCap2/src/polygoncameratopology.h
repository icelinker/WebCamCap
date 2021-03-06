#ifndef POLYGONCAMERATOPOLOGY_H
#define POLYGONCAMERATOPOLOGY_H

#include "icameratopology.h"

#include "pointchecker.h"

#include <QWaitCondition>
#include <QTime>

class TopologyEdge
{
public:
    TopologyEdge(){}
    TopologyEdge(ICamera *cam1, ICamera *cam2)
    {
        m_camera1 = cam1;
        m_camera2 = cam2;
    }

    ICamera *m_camera1 = nullptr;
    ICamera *m_camera2 = nullptr;
};

class PolygonCameraTopology : public ICameraTopology
{
    Q_OBJECT

    bool m_record = false;

    size_t m_turnedOnCamerasCounter = 0;
    int m_resultsCounter = 0;

    QVector<QThread*> m_cameraThreads;
    QVector<TopologyEdge> m_topology;

    static QMap<ICamera*, QVector<Line>> m_resultLines;

    QWaitCondition * m_waitCondition = nullptr;

    QTime m_frameTimer;

    PointChecker m_pointChecker;

public:

    PolygonCameraTopology(QVector3D roomDims, double maxError, QObject *parent = 0);
    virtual ~PolygonCameraTopology();

    virtual QVariantMap toVariantMap() override;
    virtual bool fromVariantMap(QVariantMap varMap) override;

    virtual void addCameras(QVector<ICamera *> cameras) override;
    virtual void removeCamera(ICamera *camera) override;

    virtual void record(bool start) override;

    static QVector<QVector3D> intersection(TopologyEdge edge);

signals:
    void startRecording();
    void stopRecording();
    void startRecording2D();

private:
    void setNumberOfPoints(int numberOfPoints) override;

    void resolveTopologyEdges();

    void intersections();

    QVector<QVector3D> mergePoints(size_t targetNumberOfPoints, const QVector<QVector<QVector3D>> &points);
    void normaliseCoords(QVector<Marker> &points, QVector3D roomSize);

private slots:
    void handleCameraSettingsChange(CameraSettings::CameraSettingsType type);
    void handleCameraResults(QVector<Line> lines);
    void record2D();

};

#endif // POLYGONCAMERATOPOLOGY_H
