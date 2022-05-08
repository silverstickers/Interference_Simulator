#ifndef RENDERINGMASTER_H
#define RENDERINGMASTER_H

#include <QObject>
#include <QImage>
#include <QPainter>
#include <QMutex>
#include <QThreadPool>

#include "renderingworker.h"

Q_DECLARE_METATYPE(QImage*);

class RenderingMaster : public QObject
{
    Q_OBJECT
public:
    explicit RenderingMaster(QObject *parent = nullptr);

    ~RenderingMaster();

    typedef std::complex<double> complex;

signals:
    void imageRendered(const QImage &image);

public slots:
    void startRender(QImage *image, double xcenter, double ycenter,
                             double xrange, double yrange, int width, int height,
                             double angle, int count, double spacing);
    void handleWorkerResult(QImage partialImage, int yPos);

private:
    QImage m_image;

    const static int TASKSIZE = 1000;
    volatile bool stopWorkers = false;
};

#endif // RENDERINGMASTER_H
