#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QMutex>
#include "renderingmaster.h"

class Controller : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QImage image READ image NOTIFY imageRendered)

public:
    explicit Controller(QObject *parent = nullptr);

    ~Controller();

    typedef std::complex<double> complex;

    QImage image();

    Q_INVOKABLE void startCalculation();

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();
    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void moveUp();
    Q_INVOKABLE void moveDown();

    Q_INVOKABLE void angleSlider(double angle);
    Q_INVOKABLE void numberSlider(int number);
    Q_INVOKABLE void spacingSlider(double spacing);

public slots:
    void handleNewImage(const QImage &image);

signals:
    void imageRendered();
    void calculateRender(QImage *image, double xcenter, double ycenter,
                             double xrange, double yrange, int width, int height,
                             double angle, int count, double spacing);

private:
    QMutex mutex;
    RenderingMaster master;
    QThread masterThread;
    QImage m_image;
    quint16 m_height = 250, m_width = 375;
    double m_xcenter = static_cast<double>(0);
    double m_ycenter = static_cast<double>(0);
    double m_xrange = 3, m_yrange = 2;
    double m_zoom = 1;

    double m_angle = 0;
    int m_count = 5;
    double m_spacing = 0.005;

    const int ZOOMFACTOR = 2;
    const int TRANSLATIONFRACTION = 7;

};


#endif // CONTROLLER_H
