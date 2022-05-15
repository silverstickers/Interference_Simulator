#include "renderingmaster.h"

RenderingMaster::RenderingMaster(QObject *parent) : QObject(parent)
{

}

RenderingMaster::~RenderingMaster()
{
    stopWorkers = true;
    QThreadPool::globalInstance()->waitForDone();
}

void RenderingMaster::startRender(QImage *image, double xcenter, double ycenter,
                                           double xrange, double yrange, int width, int height,
                                           double angle, int count, double spacing, bool logScale)
{
    stopWorkers = true;
    QThreadPool::globalInstance()->waitForDone();
    stopWorkers = false;
    if (image) {
        if (image->isNull()) {
            m_image = *new QImage(width, height, QImage::Format_RGB32);
            m_image.fill(0);
        }
        else {
            m_image = *image;
        }
    }

    double real = xcenter - xrange / 2;
    double imaginary = ycenter + yrange / 2;

    double realStep = xrange / static_cast<double>(width);
    double imagStep = yrange / static_cast<double>(height);
    std::cout << "start rendering with xcenter: " << xcenter << ", realStep: " << realStep << std::endl;

    int nTasks = height / TASKSIZE;

    for (int i = 0; i < nTasks; i++) {
        RenderingWorker *worker = new RenderingWorker(this, complex(real, imaginary),
                                                        complex(realStep, imagStep), width,
                                                        TASKSIZE, TASKSIZE*i, angle, count, spacing, logScale, &stopWorkers);
        //worker->setAutoDelete(false);
        QThreadPool::globalInstance()->start(worker);
    }
    if (height % TASKSIZE != 0) {
        RenderingWorker *worker = new RenderingWorker(this, complex(real, imaginary), complex(realStep, imagStep),
                                                        width, height % TASKSIZE, height - height % TASKSIZE,
                                                        angle, count, spacing, logScale, &stopWorkers);
        //worker->setAutoDelete(false);
        QThreadPool::globalInstance()->start(worker);
    }
}

void RenderingMaster::handleWorkerResult(QImage partialImage, int yPos)
{
    std::cout << yPos << std::endl;
    QPoint destPos = QPoint(0, yPos);

    QPainter painter(&m_image);
    painter.drawImage(destPos, partialImage);
    painter.end();

    emit imageRendered(m_image);
}
